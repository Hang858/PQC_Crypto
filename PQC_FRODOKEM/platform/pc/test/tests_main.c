#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"

static int run_one(frodokem_level_t level) {
    const frodokem_params_t *params = FRODOKEM_get_params(level);
    unsigned char *pk = NULL;
    unsigned char *sk = NULL;
    unsigned char *ct = NULL;
    unsigned char ss1[FRODOKEM_MAX_SHARED_SECRET_BYTES] = {0};
    unsigned char ss2[FRODOKEM_MAX_SHARED_SECRET_BYTES] = {0};
    int ret = 1;

    if (params == NULL) {
        return 1;
    }

    pk = calloc(params->publickeybytes, 1);
    sk = calloc(params->secretkeybytes, 1);
    ct = calloc(params->ciphertextbytes, 1);
    if (pk == NULL || sk == NULL || ct == NULL) {
        goto cleanup;
    }

    printf("Testing %s\n", params->algname);
    if (FRODOKEM_crypto_kem_keypair(level, pk, sk) != 0) {
        goto cleanup;
    }
    if (FRODOKEM_crypto_kem_enc(level, ct, ss1, pk) != 0) {
        goto cleanup;
    }
    if (FRODOKEM_crypto_kem_dec(level, ss2, ct, sk) != 0) {
        goto cleanup;
    }
    ret = memcmp(ss1, ss2, params->bytes) != 0;

cleanup:
    free(pk);
    free(sk);
    free(ct);
    return ret;
}

int main(void) {
    if (run_one(FRODOKEM_640) != 0) {
        return 1;
    }
    if (run_one(FRODOKEM_976) != 0) {
        return 1;
    }
    if (run_one(FRODOKEM_1344) != 0) {
        return 1;
    }

    printf("All FrodoKEM ref tests passed.\n");
    return 0;
}
