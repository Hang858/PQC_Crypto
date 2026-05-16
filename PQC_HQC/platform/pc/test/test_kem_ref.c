#define _DEFAULT_SOURCE

#include <munit.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "munit_utils.h"
#include "parameters.h"

static inline void flip_random_bit(uint8_t *buf, size_t start, size_t len) {
    uint32_t idx = munit_rand_uint32() % (uint32_t)len;
    uint8_t bit = (uint8_t)(1u << (munit_rand_uint32() & 7u));
    buf[start + idx] ^= bit;
}

static void run_kem_roundtrip(void) {
    uint8_t *pk = calloc(CRYPTO_PUBLICKEYBYTES, 1);
    uint8_t *sk = calloc(CRYPTO_SECRETKEYBYTES, 1);
    uint8_t *ct = calloc(CRYPTO_CIPHERTEXTBYTES, 1);
    uint8_t *ss1 = calloc(CRYPTO_BYTES, 1);
    uint8_t *ss2 = calloc(CRYPTO_BYTES, 1);

    munit_assert_not_null(pk);
    munit_assert_not_null(sk);
    munit_assert_not_null(ct);
    munit_assert_not_null(ss1);
    munit_assert_not_null(ss2);

    munit_assert_int(crypto_kem_keypair(pk, sk), ==, 0);
    munit_assert_int(crypto_kem_enc(ct, ss1, pk), ==, 0);
    munit_assert_int(crypto_kem_dec(ss2, ct, sk), ==, 0);
    munit_assert_memory_equal(CRYPTO_BYTES, ss1, ss2);

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
        run_kem_roundtrip();
    }

    return MUNIT_OK;
}

static void run_ct_corruption(void) {
    uint8_t *pk = calloc(CRYPTO_PUBLICKEYBYTES, 1);
    uint8_t *sk = calloc(CRYPTO_SECRETKEYBYTES, 1);
    uint8_t *ct = calloc(CRYPTO_CIPHERTEXTBYTES, 1);
    uint8_t *mutated = calloc(CRYPTO_CIPHERTEXTBYTES, 1);
    uint8_t *ss_enc = calloc(CRYPTO_BYTES, 1);
    uint8_t *ss_dec = calloc(CRYPTO_BYTES, 1);

    munit_assert_int(crypto_kem_keypair(pk, sk), ==, 0);
    munit_assert_int(crypto_kem_enc(ct, ss_enc, pk), ==, 0);

    memcpy(mutated, ct, CRYPTO_CIPHERTEXTBYTES);
    flip_random_bit(mutated, 0, VEC_N_SIZE_BYTES);
    crypto_kem_dec(ss_dec, mutated, sk);
    munit_assert_memory_not_equal(CRYPTO_BYTES, ss_enc, ss_dec);

    memcpy(mutated, ct, CRYPTO_CIPHERTEXTBYTES);
    flip_random_bit(mutated, VEC_N_SIZE_BYTES, VEC_N1N2_SIZE_BYTES);
    crypto_kem_dec(ss_dec, mutated, sk);
    munit_assert_memory_not_equal(CRYPTO_BYTES, ss_enc, ss_dec);

    memcpy(mutated, ct, CRYPTO_CIPHERTEXTBYTES);
    flip_random_bit(mutated, VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES, SALT_BYTES);
    crypto_kem_dec(ss_dec, mutated, sk);
    munit_assert_memory_not_equal(CRYPTO_BYTES, ss_enc, ss_dec);

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

    run_ct_corruption();
    return MUNIT_OK;
}

static MunitResult test_kem_sk_corruption(const MunitParameter params[], void *user_data) {
    (void)params;
    (void)user_data;

    uint8_t *pk = calloc(CRYPTO_PUBLICKEYBYTES, 1);
    uint8_t *sk = calloc(CRYPTO_SECRETKEYBYTES, 1);
    uint8_t *sk_bad = calloc(CRYPTO_SECRETKEYBYTES, 1);
    uint8_t *ct = calloc(CRYPTO_CIPHERTEXTBYTES, 1);
    uint8_t *ss_enc = calloc(CRYPTO_BYTES, 1);
    uint8_t *ss_dec = calloc(CRYPTO_BYTES, 1);

    munit_assert_int(crypto_kem_keypair(pk, sk), ==, 0);
    munit_assert_int(crypto_kem_enc(ct, ss_enc, pk), ==, 0);

    memcpy(sk_bad, sk, CRYPTO_SECRETKEYBYTES);
    flip_random_bit(sk_bad, 0, CRYPTO_PUBLICKEYBYTES);
    crypto_kem_dec(ss_dec, ct, sk_bad);
    munit_assert_memory_not_equal(CRYPTO_BYTES, ss_enc, ss_dec);

    memcpy(sk_bad, sk, CRYPTO_SECRETKEYBYTES);
    flip_random_bit(sk_bad, CRYPTO_PUBLICKEYBYTES, SEED_BYTES);
    crypto_kem_dec(ss_dec, ct, sk_bad);
    munit_assert_memory_not_equal(CRYPTO_BYTES, ss_enc, ss_dec);

    memcpy(sk_bad, sk, CRYPTO_SECRETKEYBYTES);
    flip_random_bit(sk_bad, CRYPTO_PUBLICKEYBYTES + SEED_BYTES, PARAM_SECURITY_BYTES);
    crypto_kem_dec(ss_dec, ct, sk_bad);
    munit_assert_memory_equal(CRYPTO_BYTES, ss_enc, ss_dec);

    free(pk);
    free(sk);
    free(sk_bad);
    free(ct);
    free(ss_enc);
    free(ss_dec);
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
