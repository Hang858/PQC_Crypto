#include <stdio.h>
#include "malloc.h"
#include "api.h"
#include "smartchip_sdk_soc.h"

volatile unsigned char g_result640[64] ;
volatile unsigned char g_result976[64] ;
volatile unsigned char g_result1344[64] ;

__attribute__((optimize("O0")))
static int run_one(frodokem_level_t level,unsigned char * presult) {
    const frodokem_params_t *params = FRODOKEM_get_params(level);
    unsigned char *pk = NULL;
    unsigned char *sk = NULL;
    unsigned char *ct = NULL;
    unsigned char ss1[FRODOKEM_MAX_SHARED_SECRET_BYTES] = {0};
    unsigned char ss2[FRODOKEM_MAX_SHARED_SECRET_BYTES] = {0};
    int ret = 1;

//    unsigned int start1, end1, time1;
//    unsigned int start2, end2, time2;
//    unsigned int start3, end3, time3;

    unsigned int  time1,time2,time3;
    uint64_t start1, end1;
    uint64_t start2, end2;
    uint64_t start3, end3;

    unsigned int total_time = 0;
	unsigned char  result[64]={0};

    if (params == NULL) {
        return 1;
    }

    pk = calloc(params->publickeybytes, 1);
    sk = calloc(params->secretkeybytes, 1);
    ct = calloc(params->ciphertextbytes, 1);
    if (pk == NULL || sk == NULL || ct == NULL) {
        goto cleanup;
    }

    //printf("Testing %s\n", params->algname);

    start1 = __get_rv_cycle();
    if (FRODOKEM_crypto_kem_keypair(level, pk, sk) != 0) {
        goto cleanup;
    }
    end1 = __get_rv_cycle();
	time1 = (end1 - start1) / (20 * 1000); // ms

	start2 = __get_rv_cycle();
    if (FRODOKEM_crypto_kem_enc(level, ct, ss1, pk) != 0) {
        goto cleanup;
    }
    end2 = __get_rv_cycle();
	time2 = (end2 - start2) / (20 * 1000); // ms

	start3 = __get_rv_cycle();
    if (FRODOKEM_crypto_kem_dec(level, ss2, ct, sk) != 0) {
        goto cleanup;
    }
    end3 = __get_rv_cycle();
	time3 = (end3 - start3) / (20 * 1000); // ms

	sprintf(result,"%u ms   %u ms   %u ms",time1,time2,time3);
	memcpy(presult,result,strlen(result));

	total_time = time1+time2+time3;

    ret = memcmp(ss1, ss2, params->bytes) != 0;

cleanup:
    free(pk);
    free(sk);
    free(ct);
    return ret;
}

int main(void) {

	uint64_t  start, end;
	unsigned int time;

	start = __get_rv_cycle();
    if (run_one(FRODOKEM_640,g_result640) != 0) {
        return 1;
    }
    end = __get_rv_cycle();
    time = (end - start) / (20 * 1000); // ms
    //printf("FRODOKEM_640 time is %llu \n", time);

    start = __get_rv_cycle();
    if (run_one(FRODOKEM_976,g_result976) != 0) {
        return 1;
    }
    end = __get_rv_cycle();
	time = (end - start) / (20 * 1000); // ms
	printf("FRODOKEM_976 time is %llu \n", time);

//	start = __get_rv_cycle();
//    if (run_one(FRODOKEM_1344,g_result1344) != 0) {
//        return 1;
//    }
//    end = __get_rv_cycle();
//	time = (end - start) / (20 * 1000); // ms
	//printf("FRODOKEM_1344 time is %llu \n", time);

	//printf("All FrodoKEM ref tests passed.\n");
    return 0;
}
