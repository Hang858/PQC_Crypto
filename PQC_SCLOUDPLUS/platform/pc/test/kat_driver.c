#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void kat_random_init(uint64_t seed);

#ifdef SCLOUDPLUS_KAT_MODIFIED
#include "api.h"
#include "scloudplus_params.h"

static int parse_level(const char *arg, scloudplus_level_t *level)
{
    if (strcmp(arg, "128") == 0) {
        *level = SCLOUDPLUS_128;
        return 0;
    }
    if (strcmp(arg, "192") == 0) {
        *level = SCLOUDPLUS_192;
        return 0;
    }
    if (strcmp(arg, "256") == 0) {
        *level = SCLOUDPLUS_256;
        return 0;
    }
    return -1;
}

#else
#include "kem.h"
#include "param.h"
#endif

static void print_hex(const char *label, const uint8_t *buf, size_t len)
{
    printf("%s = ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02X", buf[i]);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    unsigned int level_value;
    size_t pk_len;
    size_t sk_len;
    size_t ct_len;
    size_t ss_len;
#ifdef SCLOUDPLUS_KAT_MODIFIED
    scloudplus_level_t level;
    const scloudplus_params_t *params;

    if (argc != 2 || parse_level(argv[1], &level) != 0) {
        fprintf(stderr, "usage: %s <128|192|256>\n", argv[0]);
        return 2;
    }
    params = SCLOUDPLUS_get_params(level);
    if (params == 0) {
        return 2;
    }
    level_value = params->l;
    pk_len = params->publickeybytes;
    sk_len = params->secretkeybytes;
    ct_len = params->ciphertextbytes;
    ss_len = params->bytes;
#else
    (void)argc;
    (void)argv;
    level_value = scloudplus_l;
    pk_len = scloudplus_pk;
    sk_len = scloudplus_kem_sk;
    ct_len = scloudplus_ctx;
    ss_len = scloudplus_ss;
#endif

    uint8_t *pk = calloc(pk_len, 1);
    uint8_t *sk = calloc(sk_len, 1);
    uint8_t *ct = calloc(ct_len, 1);
    uint8_t *ss = calloc(ss_len, 1);
    uint8_t *ss_dec = calloc(ss_len, 1);

    if (pk == 0 || sk == 0 || ct == 0 || ss == 0 || ss_dec == 0) {
        free(pk);
        free(sk);
        free(ct);
        free(ss);
        free(ss_dec);
        return 2;
    }

    printf("# SCLOUDPLUS-%u differential KAT\n", level_value);
    for (unsigned int count = 0; count < 10; count++) {
        uint64_t seed = 0x53434c4f5544504cULL ^ ((uint64_t)level_value << 32) ^ count;
        memset(pk, 0, pk_len);
        memset(sk, 0, sk_len);
        memset(ct, 0, ct_len);
        memset(ss, 0, ss_len);
        memset(ss_dec, 0, ss_len);

        kat_random_init(seed);
#ifdef SCLOUDPLUS_KAT_MODIFIED
        if (crypto_kem_keypair(level, pk, sk) != 0 ||
            crypto_kem_enc(level, ct, ss, pk) != 0 ||
            crypto_kem_dec(level, ss_dec, ct, sk) != 0) {
            fprintf(stderr, "crypto failure at count %u\n", count);
            return 1;
        }
#else
        scloud_kemkeygen(pk, sk);
        scloud_kemencaps(pk, ct, ss);
        scloud_kemdecaps(sk, ct, ss_dec);
#endif

        printf("count = %u\n", count);
        printf("seed = %016llX\n", (unsigned long long)seed);
        print_hex("pk", pk, pk_len);
        print_hex("sk", sk, sk_len);
        print_hex("ct", ct, ct_len);
        print_hex("ss", ss, ss_len);
        print_hex("ss_dec", ss_dec, ss_len);
        printf("\n");

        if (memcmp(ss, ss_dec, ss_len) != 0) {
            fprintf(stderr, "decapsulation mismatch at count %u\n", count);
            return 1;
        }
    }

    free(pk);
    free(sk);
    free(ct);
    free(ss);
    free(ss_dec);
    return 0;
}
