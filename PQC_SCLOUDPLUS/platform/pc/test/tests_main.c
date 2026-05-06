#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "api.h"

static int run_level(scloudplus_level_t level) {
    const scloudplus_params_t *params = SCLOUDPLUS_get_params(level);
    uint8_t *pk = malloc(params->publickeybytes);
    uint8_t *sk = malloc(params->secretkeybytes);
    uint8_t *ct = malloc(params->ciphertextbytes);
    uint8_t *ss1 = malloc(params->bytes);
    uint8_t *ss2 = malloc(params->bytes);
    int ok = 1;

    if (!pk || !sk || !ct || !ss1 || !ss2) {
        ok = 0;
        goto out;
    }

    for (int count = 0; count < 10 && ok; count++) {
        if (SCLOUDPLUS_crypto_kem_keypair(level, pk, sk) != 0) ok = 0;
        if (ok && SCLOUDPLUS_crypto_kem_enc(level, ct, ss1, pk) != 0) ok = 0;
        if (ok && SCLOUDPLUS_crypto_kem_dec(level, ss2, ct, sk) != 0) ok = 0;
        if (ok && memcmp(ss1, ss2, params->bytes) != 0) ok = 0;
    }

out:
    free(pk);
    free(sk);
    free(ct);
    free(ss1);
    free(ss2);
    return ok;
}

int main(void) {
    const scloudplus_level_t levels[] = {SCLOUDPLUS_128, SCLOUDPLUS_192, SCLOUDPLUS_256};
    for (size_t i = 0; i < sizeof(levels) / sizeof(levels[0]); ++i) {
        const scloudplus_params_t *params = SCLOUDPLUS_get_params(levels[i]);
        printf("Testing %s\n", params->algname);
        if (!run_level(levels[i])) {
            fprintf(stderr, "Failure at %s\n", params->algname);
            return EXIT_FAILURE;
        }
    }
    printf("All SCLOUD+ KEM tests passed.\n");
    return EXIT_SUCCESS;
}
