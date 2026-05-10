#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "hqc_params.h"
#include "symmetric.h"

static void fprint_bstr(FILE *fp, const char *label, const unsigned char *data, size_t len) {
    fprintf(fp, "%s", label);
    for (size_t i = 0; i < len; i++) {
        fprintf(fp, "%02X", data[i]);
    }
    fprintf(fp, "\n");
}

static int parse_level(const char *arg, hqc_level_t *level) {
    if (strcmp(arg, "1") == 0 || strcmp(arg, "hqc-1") == 0) {
        *level = HQC_1;
        return 0;
    }
    if (strcmp(arg, "3") == 0 || strcmp(arg, "hqc-3") == 0) {
        *level = HQC_3;
        return 0;
    }
    if (strcmp(arg, "5") == 0 || strcmp(arg, "hqc-5") == 0) {
        *level = HQC_5;
        return 0;
    }
    return -1;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s <1|3|5> <unused.req> <output.rsp>\n", argv[0]);
        return 2;
    }

    hqc_level_t level;
    if (parse_level(argv[1], &level) != 0 || HQC_select_level(level) != 0) {
        fprintf(stderr, "invalid HQC level: %s\n", argv[1]);
        return 2;
    }

    const hqc_params_t *params = HQC_get_params(level);
    FILE *rsp = fopen(argv[3], "w");
    (void)argv[2];
    if (rsp == NULL) {
        fprintf(stderr, "failed to open KAT output file\n");
        if (rsp != NULL) {
            fclose(rsp);
        }
        return 2;
    }

    unsigned char seed[48] = {0};
    unsigned char *pk = calloc(params->publickeybytes, 1);
    unsigned char *sk = calloc(params->secretkeybytes, 1);
    unsigned char *ct = calloc(params->ciphertextbytes, 1);
    unsigned char ss[HQC_SHARED_SECRET_BYTES] = {0};
    unsigned char ss_dec[HQC_SHARED_SECRET_BYTES] = {0};
    if (pk == NULL || sk == NULL || ct == NULL) {
        fclose(rsp);
        free(pk);
        free(sk);
        free(ct);
        return 2;
    }

    unsigned char entropy_input[48] = {0};
    unsigned char seeds[100][48] = {{0}};
    for (size_t i = 0; i < sizeof(entropy_input); i++) {
        entropy_input[i] = (unsigned char)i;
    }
    prng_init(entropy_input, NULL, 48, 0);
    for (size_t i = 0; i < 100; i++) {
        prng_get_bytes(seeds[i], 48);
    }

    fprintf(rsp, "# %s\n\n", params->algname);
    for (int count = 0; count < 100; count++) {
        memcpy(seed, seeds[count], sizeof(seed));
        fprintf(rsp, "count = %d\n", count);
        fprint_bstr(rsp, "seed = ", seed, sizeof(seed));

        prng_init(seed, NULL, 48, 0);
        if (crypto_kem_keypair(level, pk, sk) != 0 || crypto_kem_enc(level, ct, ss, pk) != 0 ||
            crypto_kem_dec(level, ss_dec, ct, sk) != 0 || memcmp(ss, ss_dec, params->bytes) != 0) {
            fprintf(stderr, "crypto failure at count %d\n", count);
            fclose(rsp);
            free(pk);
            free(sk);
            free(ct);
            return 1;
        }

        fprint_bstr(rsp, "pk = ", pk, params->publickeybytes);
        fprint_bstr(rsp, "sk = ", sk, params->secretkeybytes);
        fprint_bstr(rsp, "ct = ", ct, params->ciphertextbytes);
        fprint_bstr(rsp, "ss = ", ss, params->bytes);
        fprintf(rsp, "\n");
    }

    fclose(rsp);
    free(pk);
    free(sk);
    free(ct);
    return 0;
}
