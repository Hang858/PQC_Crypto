#include <stdlib.h>
#include <string.h>
#include "api.h"

// Simulates flash storage for the secret key (not on the heap).
// In a real embedded device, sk would be stored in non-volatile flash.
static unsigned char flash_sk[FRODOKEM_MAX_SECRET_KEY_BYTES];

// Real embedded memory pattern:
//   Phase 1 (Keypair): heap = pk + sk + internal  ≈ 29 KB
//   Phase 2 (Encaps):  heap = pk + ct + internal  ≈ 30 KB  (sk in flash, not RAM)
//   Phase 3 (Decaps):  heap = sk + ct + internal  ≈ 40 KB  (pk freed)
// Peak heap = max(29, 30, 40) = 40 KB — well under the 50 KB all-at-once number.

int main(void) {
    const frodokem_params_t *params = FRODOKEM_get_params(FRODOKEM_640);
    unsigned char *pk, *sk, *ct;
    unsigned char ss1[32], ss2[32];
    int ret = 1;

    /*=== Phase 1: Keypair ===*/
    pk = calloc(params->publickeybytes, 1);        //   9,616 B
    sk = calloc(params->secretkeybytes, 1);        //  19,888 B
    if (!pk || !sk) goto cleanup;
    if (FRODOKEM_crypto_kem_keypair(FRODOKEM_640, pk, sk) != 0) goto cleanup;
    // Save sk to "flash" so we can free heap RAM
    memcpy(flash_sk, sk, params->secretkeybytes);
    free(sk);
    sk = NULL;
    // Heap now: pk only (9.6 KB) — sk is in simulated flash

    /*=== Phase 2: Encaps (sender: encrypt with pk, sk NOT needed) ===*/
    ct = calloc(params->ciphertextbytes, 1);       //   9,752 B
    if (!ct) goto cleanup;
    if (FRODOKEM_crypto_kem_enc(FRODOKEM_640, ct, ss1, pk) != 0) goto cleanup;
    // Heap peak: pk(9616) + ct(9752) + Sp(10368) + small(496) ≈ 30 KB
    free(pk);
    pk = NULL;
    // Heap now: ct only (9.8 KB)

    /*=== Phase 3: Decaps (receiver: decrypt with sk+ct, pk NOT needed) ===*/
    sk = calloc(params->secretkeybytes, 1);        //  19,888 B
    if (!sk) goto cleanup;
    // Restore sk from "flash" (simulates loading from non-volatile storage)
    memcpy(sk, flash_sk, params->secretkeybytes);
    // flash_sk can now be cleared (not needed anymore)
    memset(flash_sk, 0, params->secretkeybytes);

    if (FRODOKEM_crypto_kem_dec(FRODOKEM_640, ss2, ct, sk) != 0) goto cleanup;
    // Heap peak: sk(19888) + ct(9752) + Sp(10368) + small(496) ≈ 40 KB

    /* Verify shared secret match */
    {
        volatile int ok = (memcmp(ss1, ss2, params->bytes) == 0);
        ret = ok ? 0 : 5;
    }

cleanup:
    free(pk);
    free(sk);
    free(ct);
    memset(flash_sk, 0, sizeof(flash_sk));
    return ret;
}
