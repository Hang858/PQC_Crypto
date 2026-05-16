#include <munit.h>
#include <string.h>
#include <stdlib.h>

#include "api.h"
#include "runtime_params.h"
#include "hqc.h"
#include "munit_utils.h"

static void run_pke_api(void) {
    size_t k_bytes = PARAM_K;
    unsigned char seed[32] = {0};
    unsigned char theta[32] = {0};
    unsigned char *m1 = calloc(k_bytes, 1);
    unsigned char *m2 = calloc(k_bytes, 1);
    unsigned char *ek_pke = calloc(PUBLIC_KEY_BYTES, 1);
    unsigned char *dk_pke = calloc(32, 1);
    ciphertext_pke_t *c_pke = calloc(1, sizeof(*c_pke));

    munit_rand_memory(32, seed);
    munit_rand_memory(32, theta);
    munit_rand_memory((uint32_t)k_bytes, m1);

    hqc_pke_keygen(ek_pke, dk_pke, seed);
    hqc_pke_encrypt(c_pke, ek_pke, (uint64_t *)m1, theta);
    munit_assert_int(hqc_pke_decrypt((uint64_t *)m2, dk_pke, c_pke), ==, 0);

    munit_assert_memory_equal(k_bytes, m1, m2);

    free(m1);
    free(m2);
    free(ek_pke);
    free(dk_pke);
    free(c_pke);
}

static MunitResult test_pke_api(const MunitParameter params[], void *user_data) {
    (void)params;
    (void)user_data;

    run_pke_api();
    return MUNIT_OK;
}

MunitTest pke_tests[] = {MUNIT_TEST_ENTRY("pke_api", test_pke_api), MUNIT_TEST_END};
