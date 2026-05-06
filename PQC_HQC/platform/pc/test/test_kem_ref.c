#define _DEFAULT_SOURCE

#include <munit.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "api.h"
#include "hqc_params.h"
#include "munit_utils.h"

static inline void flip_random_bit(uint8_t *buf, size_t start, size_t len) {
    uint32_t idx = munit_rand_uint32() % (uint32_t)len;
    uint8_t bit = (uint8_t)(1u << (munit_rand_uint32() & 7u));
    buf[start + idx] ^= bit;
}

static void run_kem_roundtrip(hqc_level_t level) {
    const hqc_params_t *params = HQC_get_params(level);
    uint8_t *pk = calloc(params->publickeybytes, 1);
    uint8_t *sk = calloc(params->secretkeybytes, 1);
    uint8_t *ct = calloc(params->ciphertextbytes, 1);
    uint8_t *ss1 = calloc(params->bytes, 1);
    uint8_t *ss2 = calloc(params->bytes, 1);

    munit_assert_not_null(pk);
    munit_assert_not_null(sk);
    munit_assert_not_null(ct);
    munit_assert_not_null(ss1);
    munit_assert_not_null(ss2);

    munit_assert_int(HQC_crypto_kem_keypair(level, pk, sk), ==, 0);
    munit_assert_int(HQC_crypto_kem_enc(level, ct, ss1, pk), ==, 0);
    munit_assert_int(HQC_crypto_kem_dec(level, ss2, ct, sk), ==, 0);
    munit_assert_memory_equal(params->bytes, ss1, ss2);

    free(pk);
    free(sk);
    free(ct);
    free(ss1);
    free(ss2);
}

static MunitResult test_kem_api(const MunitParameter params[], void *user_data) {
    (void)user_data;

    int iterations = 1;
    const char *iter_param = munit_parameters_get(params, "iterations");
    if (iter_param != NULL) {
        int parsed = atoi(iter_param);
        if (parsed > 0) {
            iterations = parsed;
        }
    }

    for (int run = 0; run < iterations; run++) {
        run_kem_roundtrip(HQC_1);
        run_kem_roundtrip(HQC_3);
        run_kem_roundtrip(HQC_5);
    }

    return MUNIT_OK;
}

static void run_ct_corruption(hqc_level_t level) {
    const hqc_params_t *params = HQC_get_params(level);
    uint8_t *pk = calloc(params->publickeybytes, 1);
    uint8_t *sk = calloc(params->secretkeybytes, 1);
    uint8_t *ct = calloc(params->ciphertextbytes, 1);
    uint8_t *mutated = calloc(params->ciphertextbytes, 1);
    uint8_t *ss_enc = calloc(params->bytes, 1);
    uint8_t *ss_dec = calloc(params->bytes, 1);

    size_t vec_n_bytes;
    size_t vec_n1n2_bytes;
    switch (level) {
        case HQC_1:
            vec_n_bytes = (17669 + 7) / 8;
            vec_n1n2_bytes = (17664 + 7) / 8;
            break;
        case HQC_3:
            vec_n_bytes = (35851 + 7) / 8;
            vec_n1n2_bytes = (35840 + 7) / 8;
            break;
        case HQC_5:
            vec_n_bytes = (57637 + 7) / 8;
            vec_n1n2_bytes = (57600 + 7) / 8;
            break;
        default:
            vec_n_bytes = 0;
            vec_n1n2_bytes = 0;
            break;
    }

    munit_assert_int(HQC_crypto_kem_keypair(level, pk, sk), ==, 0);
    munit_assert_int(HQC_crypto_kem_enc(level, ct, ss_enc, pk), ==, 0);

    memcpy(mutated, ct, params->ciphertextbytes);
    flip_random_bit(mutated, 0, vec_n_bytes);
    HQC_crypto_kem_dec(level, ss_dec, mutated, sk);
    munit_assert_memory_not_equal(params->bytes, ss_enc, ss_dec);

    memcpy(mutated, ct, params->ciphertextbytes);
    flip_random_bit(mutated, vec_n_bytes, vec_n1n2_bytes);
    HQC_crypto_kem_dec(level, ss_dec, mutated, sk);
    munit_assert_memory_not_equal(params->bytes, ss_enc, ss_dec);

    memcpy(mutated, ct, params->ciphertextbytes);
    flip_random_bit(mutated, vec_n_bytes + vec_n1n2_bytes, 16);
    HQC_crypto_kem_dec(level, ss_dec, mutated, sk);
    munit_assert_memory_not_equal(params->bytes, ss_enc, ss_dec);

    free(pk);
    free(sk);
    free(ct);
    free(mutated);
    free(ss_enc);
    free(ss_dec);
}

static MunitResult test_kem_ct_corruption(const MunitParameter params[], void *user_data) {
    (void)params;
    (void)user_data;

    run_ct_corruption(HQC_1);
    run_ct_corruption(HQC_3);
    run_ct_corruption(HQC_5);
    return MUNIT_OK;
}

static MunitResult test_kem_sk_corruption(const MunitParameter params[], void *user_data) {
    (void)params;
    (void)user_data;

    for (hqc_level_t level = HQC_1; level <= HQC_5; level++) {
        const hqc_params_t *p = HQC_get_params(level);
        uint8_t *pk = calloc(p->publickeybytes, 1);
        uint8_t *sk = calloc(p->secretkeybytes, 1);
        uint8_t *sk_bad = calloc(p->secretkeybytes, 1);
        uint8_t *ct = calloc(p->ciphertextbytes, 1);
        uint8_t *ss_enc = calloc(p->bytes, 1);
        uint8_t *ss_dec = calloc(p->bytes, 1);
        size_t sigma_len = (level == HQC_1) ? 16 : (level == HQC_3 ? 24 : 32);

        munit_assert_int(HQC_crypto_kem_keypair(level, pk, sk), ==, 0);
        munit_assert_int(HQC_crypto_kem_enc(level, ct, ss_enc, pk), ==, 0);

        memcpy(sk_bad, sk, p->secretkeybytes);
        flip_random_bit(sk_bad, 0, p->publickeybytes);
        HQC_crypto_kem_dec(level, ss_dec, ct, sk_bad);
        munit_assert_memory_not_equal(p->bytes, ss_enc, ss_dec);

        memcpy(sk_bad, sk, p->secretkeybytes);
        flip_random_bit(sk_bad, p->publickeybytes, 32);
        HQC_crypto_kem_dec(level, ss_dec, ct, sk_bad);
        munit_assert_memory_not_equal(p->bytes, ss_enc, ss_dec);

        memcpy(sk_bad, sk, p->secretkeybytes);
        flip_random_bit(sk_bad, p->publickeybytes + 32, sigma_len);
        HQC_crypto_kem_dec(level, ss_dec, ct, sk_bad);
        munit_assert_memory_equal(p->bytes, ss_enc, ss_dec);

        free(pk);
        free(sk);
        free(sk_bad);
        free(ct);
        free(ss_enc);
        free(ss_dec);
    }

    return MUNIT_OK;
}

static char *iteration_values[] = {(char *)"1", NULL};

static MunitParameterEnum kem_api_params[] = {
    {(char *)"iterations", iteration_values},
    {NULL, NULL},
};

MunitTest kem_tests[] = {
    MUNIT_TEST_ENTRY_ITER("kem_api", test_kem_api, kem_api_params),
    MUNIT_TEST_ENTRY("kem_ct_corruption", test_kem_ct_corruption),
    MUNIT_TEST_ENTRY("kem_sk_corruption", test_kem_sk_corruption),
    MUNIT_TEST_END
};
