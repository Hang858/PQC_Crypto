#include <munit.h>
#include <string.h>
#include <stdlib.h>

#include "hqc_params.h"
#include "munit_utils.h"

void hqc1_hqc_pke_keygen(unsigned char *ek_pke, unsigned char *dk_pke, unsigned char *seed);
void hqc1_hqc_pke_encrypt(void *c_pke, const unsigned char *ek_pke, const uint64_t *m, const unsigned char *theta);
uint8_t hqc1_hqc_pke_decrypt(uint64_t *m, const unsigned char *dk_pke, const void *c_pke);

void hqc3_hqc_pke_keygen(unsigned char *ek_pke, unsigned char *dk_pke, unsigned char *seed);
void hqc3_hqc_pke_encrypt(void *c_pke, const unsigned char *ek_pke, const uint64_t *m, const unsigned char *theta);
uint8_t hqc3_hqc_pke_decrypt(uint64_t *m, const unsigned char *dk_pke, const void *c_pke);

void hqc5_hqc_pke_keygen(unsigned char *ek_pke, unsigned char *dk_pke, unsigned char *seed);
void hqc5_hqc_pke_encrypt(void *c_pke, const unsigned char *ek_pke, const uint64_t *m, const unsigned char *theta);
uint8_t hqc5_hqc_pke_decrypt(uint64_t *m, const unsigned char *dk_pke, const void *c_pke);

static void run_pke_api(hqc_level_t level) {
    const hqc_params_t *p = HQC_get_params(level);
    size_t k_bytes = (level == HQC_1) ? 16 : (level == HQC_3 ? 24 : 32);
    size_t vec_n_bytes = (level == HQC_1) ? ((17669 + 7) / 8) : (level == HQC_3 ? ((35851 + 7) / 8) : ((57637 + 7) / 8));
    size_t vec_n1n2_bytes = (level == HQC_1) ? ((17664 + 7) / 8) : (level == HQC_3 ? ((35840 + 7) / 8) : ((57600 + 7) / 8));
    size_t c_pke_bytes = vec_n_bytes + vec_n1n2_bytes;
    unsigned char seed[32] = {0};
    unsigned char theta[32] = {0};
    unsigned char *m1 = calloc(k_bytes, 1);
    unsigned char *m2 = calloc(k_bytes, 1);
    unsigned char *ek_pke = calloc(p->publickeybytes, 1);
    unsigned char *dk_pke = calloc(32, 1);
    unsigned char *c_pke = calloc(c_pke_bytes, 1);

    munit_rand_memory(32, seed);
    munit_rand_memory(32, theta);
    munit_rand_memory((uint32_t)k_bytes, m1);

    switch (level) {
        case HQC_1:
            hqc1_hqc_pke_keygen(ek_pke, dk_pke, seed);
            hqc1_hqc_pke_encrypt(c_pke, ek_pke, (uint64_t *)m1, theta);
            munit_assert_int(hqc1_hqc_pke_decrypt((uint64_t *)m2, dk_pke, c_pke), ==, 0);
            break;
        case HQC_3:
            hqc3_hqc_pke_keygen(ek_pke, dk_pke, seed);
            hqc3_hqc_pke_encrypt(c_pke, ek_pke, (uint64_t *)m1, theta);
            munit_assert_int(hqc3_hqc_pke_decrypt((uint64_t *)m2, dk_pke, c_pke), ==, 0);
            break;
        case HQC_5:
            hqc5_hqc_pke_keygen(ek_pke, dk_pke, seed);
            hqc5_hqc_pke_encrypt(c_pke, ek_pke, (uint64_t *)m1, theta);
            munit_assert_int(hqc5_hqc_pke_decrypt((uint64_t *)m2, dk_pke, c_pke), ==, 0);
            break;
    }

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

    run_pke_api(HQC_1);
    run_pke_api(HQC_3);
    run_pke_api(HQC_5);
    return MUNIT_OK;
}

MunitTest pke_tests[] = {MUNIT_TEST_ENTRY("pke_api", test_pke_api), MUNIT_TEST_END};
