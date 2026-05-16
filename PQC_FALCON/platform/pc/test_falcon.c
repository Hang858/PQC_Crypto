#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"

static int run_level(falcon_level_t level) {
    const falcon_params_t *params = Falcon_get_params(level);
    const unsigned char msg[] = "falcon runtime test";
    unsigned char *pk;
    unsigned char *sk;
    unsigned char *sm;
    unsigned char *opened;
    unsigned long long smlen = 0;
    unsigned long long opened_len = 0;
    int ret = -1;

    if (params == NULL) {
        return -1;
    }

    pk = calloc(params->publickeybytes, 1);
    sk = calloc(params->secretkeybytes, 1);
    sm = calloc(sizeof(msg) + params->bytes, 1);
    opened = calloc(sizeof(msg) + params->bytes, 1);
    if (pk == NULL || sk == NULL || sm == NULL || opened == NULL) {
        goto cleanup;
    }

    if (crypto_sign_keypair(level, pk, sk) != 0) {
        goto cleanup;
    }
    if (crypto_sign(level, sm, &smlen, msg, sizeof(msg), sk) != 0) {
        goto cleanup;
    }
    if (crypto_sign_open(level, opened, &opened_len, sm, smlen, pk) != 0) {
        goto cleanup;
    }
    if (opened_len != sizeof(msg) || memcmp(opened, msg, sizeof(msg)) != 0) {
        goto cleanup;
    }

    printf("%s ok\n", params->algname);
    ret = 0;

cleanup:
    free(pk);
    free(sk);
    free(sm);
    free(opened);
    return ret;
}

int main(void) {
    if (run_level(FALCON_512) != 0) {
        return 1;
    }
    if (run_level(FALCON_1024) != 0) {
        return 1;
    }
    return 0;
}
