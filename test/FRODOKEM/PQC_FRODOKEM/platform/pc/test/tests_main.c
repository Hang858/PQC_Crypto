#include <stdio.h>
#include "malloc.h"
#include <string.h>
#include "api.h"
#include "frodokem_params.h"
//#include "smartchip_sdk_soc.h"

static int run_one(frodokem_level_t level) {
    const frodokem_params_t *params = FRODOKEM_get_params(level);
    unsigned char pk[params->publickeybytes];
    unsigned char sk[params->secretkeybytes];
    unsigned char ct[params->ciphertextbytes];
    unsigned char ss1[32] = {0};
    unsigned char ss2[32] = {0};
    int ret = 1;

    if (params == NULL) {
        return 1;
    }

    printf("Testing %s\n", params->algname);
    reset_max_heap_usage();
    if (FRODOKEM_gen_keypair(level, pk, sk) != 0) {
        goto cleanup;
    }

    //printf("max_heap_usage is %u \n", get_max_heap_usage());
    reset_max_heap_usage();
    if (FRODOKEM_enc(level, ct, ss1, pk) != 0) {
        goto cleanup;
    }
    //printf("max_heap_usage is %u \n", get_max_heap_usage());
    reset_max_heap_usage();
    if (FRODOKEM_dec(level, ss2, ct, sk) != 0) {
        goto cleanup;
    }
    //printf("max_heap_usage is %u \n", get_max_heap_usage());
    ret = memcmp(ss1, ss2, params->bytes) != 0;

cleanup:
    free(pk);
    free(sk);
    free(ct);
    return ret;
}

int main(void) {

  //  uint64_t start, end, time;
  //  start = __get_rv_cycle();
    if (run_one(FRODOKEM_1344) != 0) {
        return 1;
    }
 //   end = __get_rv_cycle();
 //   time = (end - start) / (20 * 1000); // ms

 //   printf("FRODOKEM_640 time is %ull \n", time);

    // reset_max_heap_usage();
    // if (run_one(FRODOKEM_976) != 0) {
    //     return 1;
    // }

    // printf("FRODOKEM_976 max_heap_usage is %u \n", get_max_heap_usage());

    // reset_max_heap_usage();
    // if (run_one(FRODOKEM_1344) != 0) {
    //     return 1;
    // }

    // printf("FRODOKEM_1344 max_heap_usage is %u \n", get_max_heap_usage());
    // reset_max_heap_usage();

    printf("All FrodoKEM ref tests passed.\n");
    return 0;
}
