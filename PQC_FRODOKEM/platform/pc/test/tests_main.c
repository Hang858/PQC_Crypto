#include <stdio.h>
#include <string.h>
#include "api.h"

static int run_one(frodokem_level_t level)
{
    const frodokem_params_t *params = FRODOKEM_get_params(level);
    unsigned char pk[43088];
    unsigned char sk[43088];
    unsigned char ct[21632];
    unsigned char ss1[32];
    unsigned char ss2[32];

    if (params == NULL) {
        return 1;
    }

    memset(pk, 0, sizeof(pk));
    memset(sk, 0, sizeof(sk));
    memset(ct, 0, sizeof(ct));
    memset(ss1, 0, sizeof(ss1));
    memset(ss2, 0, sizeof(ss2));

    printf("Testing %s\n", params->name);
    if (FRODOKEM_crypto_kem_keypair_enc(level, ct, ss1, pk, sk) != 0) {
        return 1;
    }
    if (FRODOKEM_crypto_kem_dec(level, ss2, ct, sk) != 0) {
        return 1;
    }

    return memcmp(ss1, ss2, params->shared_secret_bytes) != 0;
}

int main(void)
{
    if (run_one(FRODOKEM_640) != 0) {
        return 1;
    }
    if (run_one(FRODOKEM_976) != 0) {
        return 1;
    }
    if (run_one(FRODOKEM_1344) != 0) {
        return 1;
    }

    printf("All eFrodoKEM tests passed.\n");
    return 0;
}
