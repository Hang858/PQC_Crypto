#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "malloc.h"
#include "api.h"

// Host-side model of secret-key NVM.  This is .bss on a PC, not real flash;
// on the target it must be placed in protected non-volatile storage.
static unsigned char flash_sk[FRODOKEM_MAX_SECRET_KEY_BYTES];

// Real embedded memory pattern:
//   Phase 1 (Keypair): heap = pk + sk + internal  ≈ 31.5 KB
//   Phase 2 (Encaps):  heap = pk + ct + internal  ≈ 42.0 KB  (sk in NVM)
//   Phase 3 (Decaps):  heap = sk + ct + internal  ≈ 42.8 KB  (pk freed)
// Peak heap is the maximum of these phases, not their sum.

int main(void) {
    const frodokem_params_t *params = FRODOKEM_get_params(FRODOKEM_640);
    unsigned char *pk, *sk, *ct;
    unsigned char ss1[32], ss2[32];
    int ret = 1;
    size_t keypair_peak = 0;
    size_t enc_peak = 0;
    size_t dec_peak = 0;

    /*=== Phase 1: Keypair ===*/
    reset_max_heap_usage();
    pk = calloc(params->publickeybytes, 1);        //   9,616 B
    sk = calloc(params->secretkeybytes, 1);        //  19,888 B
    if (!pk || !sk) goto cleanup;
    if (FRODOKEM_crypto_kem_keypair(FRODOKEM_640, pk, sk) != 0) goto cleanup;
    keypair_peak = get_max_heap_usage();
    // Save sk to "flash" so we can free heap RAM
    memcpy(flash_sk, sk, params->secretkeybytes);
    free(sk);
    sk = NULL;
    // Heap now: pk only (9.6 KB) — sk is in simulated flash

    /*=== Phase 2: Encaps (sender: encrypt with pk, sk NOT needed) ===*/
    reset_max_heap_usage();
    ct = calloc(params->ciphertextbytes, 1);       //   9,752 B
    if (!ct) goto cleanup;
    if (FRODOKEM_crypto_kem_enc(FRODOKEM_640, ct, ss1, pk) != 0) goto cleanup;
    enc_peak = get_max_heap_usage();
    // Current implementation keeps a full Bp matrix, hence this phase is about 42 KB.
    free(pk);
    pk = NULL;
    // Heap now: ct only (9.8 KB)

    /*=== Phase 3: Decaps (receiver: decrypt with sk+ct, pk NOT needed) ===*/
    reset_max_heap_usage();
    sk = calloc(params->secretkeybytes, 1);        //  19,888 B
    if (!sk) goto cleanup;
    // Restore sk from host-side NVM model.  A target with XIP can instead pass an NVM pointer.
    memcpy(sk, flash_sk, params->secretkeybytes);
    // flash_sk can now be cleared (not needed anymore)
    memset(flash_sk, 0, params->secretkeybytes);

    if (FRODOKEM_crypto_kem_dec(FRODOKEM_640, ss2, ct, sk) != 0) goto cleanup;
    dec_peak = get_max_heap_usage();
    // This phase is about 42.8 KB with the current dynamic SHAKE workspaces.

    /* Verify shared secret match */
    {
        volatile int ok = (memcmp(ss1, ss2, params->bytes) == 0);
        ret = ok ? 0 : 5;
    }

    printf("FrodoKEM-640 lifecycle heap peaks: keypair=%zu B, enc=%zu B, dec=%zu B\n",
           keypair_peak, enc_peak, dec_peak);

cleanup:
    free(pk);
    free(sk);
    free(ct);
    memset(flash_sk, 0, sizeof(flash_sk));
    return ret;
}
