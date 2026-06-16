#include <munit.h>
#include <stdio.h>

#include "api.h"
#include "hqc_params.h"
#include "munit_utils.h"
#include "parameters.h"

extern MunitTest kem_tests[];
extern MunitTest pke_tests[];

static MunitSuite nested_suites[] = {
    MUNIT_LEAF_ONCE("kem", kem_tests),
    MUNIT_LEAF_ONCE("pke", pke_tests),
    MUNIT_SUITE_END
};

static MunitSuite main_suite = MUNIT_TOP_SUITE("api", nested_suites);

int main(int argc, char *const argv[]) {
    int ret = 0;
    hqc_level_t levels[] = { HQC_1, HQC_3, HQC_5 };

    for (int i = 0; i < 3; i++) {
        HQC_select_level(levels[i]);
        const hqc_params_t *p = HQC_active_params();

        printf("----\n");
        printf("  %s  N=%d  Sec=%d bits\n", p->algname, p->n, p->security_bits);
        printf("----\n\n");

        int r = munit_suite_main(&main_suite, NULL, argc, argv);
        if (r != 0) {
            ret = r;
        }
    }

    return ret;
}
