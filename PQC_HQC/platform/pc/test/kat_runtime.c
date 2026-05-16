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

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <unused.req> <output.rsp>\n", argv[0]);
        return 2;
    }

    const hqc_params_t *params = HQC_get_params(HQC_1);
    FILE *rsp = fopen(argv[2], "w");
    (void)argv[1];
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
        if (crypto_kem_keypair(pk, sk) != 0 || crypto_kem_enc(ct, ss, pk) != 0 ||
            crypto_kem_dec(ss_dec, ct, sk) != 0 || memcmp(ss, ss_dec, params->bytes) != 0) {
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
