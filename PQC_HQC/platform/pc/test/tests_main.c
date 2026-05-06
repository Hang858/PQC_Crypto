#include <munit.h>
#include <stdio.h>

#include "api.h"
#include "hqc_params.h"
#include "munit_utils.h"

extern MunitTest kem_tests[];
extern MunitTest pke_tests[];

static MunitSuite nested_suites[] = {
    MUNIT_LEAF_ONCE("kem", kem_tests),
    MUNIT_LEAF_ONCE("pke", pke_tests),
    MUNIT_SUITE_END
};

static MunitSuite main_suite = MUNIT_TOP_SUITE("api", nested_suites);

int main(int argc, char *const argv[]) {
    const hqc_params_t *p1 = HQC_get_params(HQC_1);
    const hqc_params_t *p3 = HQC_get_params(HQC_3);
    const hqc_params_t *p5 = HQC_get_params(HQC_5);

    printf("----\n");
    printf("  %s  N=%u  Sec=%u bits\n", p1->algname, p1->n, p1->security_bits);
    printf("  %s  N=%u  Sec=%u bits\n", p3->algname, p3->n, p3->security_bits);
    printf("  %s  N=%u  Sec=%u bits\n", p5->algname, p5->n, p5->security_bits);
    printf("----\n\n");

    return munit_suite_main(&main_suite, NULL, argc, argv);
}
