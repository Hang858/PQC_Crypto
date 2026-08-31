#include <stdio.h>
#include "malloc.h"
#include "memops.h"
#include "api.h"
#include "frodokem_params.h"
#include "smartchip_sdk_soc.h"

#define USE_UART_MAIN		0

#if USE_UART_MAIN == 1

#include "uart_test_interface.h"
#include "uart.h"

int main(void)
{
	UART_Process();
	return 0;
}
#else

#include "uart.h"
unsigned char g_result640[64] ;
unsigned char g_result976[64] ;
unsigned char g_result1344[64] ;

unsigned int  time1,time2,time3;
uint64_t start1, end1;
uint64_t start2, end2;
uint64_t start3, end3;
int32_t max_heap_usage0, max_heap_usage1, max_heap_usage2, max_heap_usage3;

static int run_one(frodokem_level_t level,unsigned char * presult) {
    const frodokem_params_t *params = FRODOKEM_get_params(level);
    unsigned char *pk = NULL;
    unsigned char *sk = NULL;
    unsigned char *ct = NULL;
    unsigned char ss1[32] = {0};
    unsigned char ss2[32] = {0};
    int ret = 1;

//    unsigned int start1, end1, time1;
//    unsigned int start2, end2, time2;
//    unsigned int start3, end3, time3;


    unsigned int total_time = 0;
	unsigned char  result[64]={0};

    if (params == NULL) {
        return 1;
    }

    reset_max_heap_usage();
    pk = (unsigned char *)malloc(params->publickeybytes);
    sk = (unsigned char *)malloc(params->secretkeybytes);
    ct = (unsigned char *)malloc(params->ciphertextbytes);
    if (pk == NULL || sk == NULL || ct == NULL) {
        goto cleanup;
    }
    max_heap_usage0 = get_max_heap_usage();

    //printf("Testing %s\n", params->algname);

    reset_max_heap_usage();
    start1 = __get_rv_cycle();
    if (FRODOKEM_gen_keypair(level, pk, sk) != 0) {
        goto cleanup;
    }
    end1 = __get_rv_cycle();
	time1 = (end1 - start1) / (20 * 1000); // ms
    max_heap_usage1 = get_max_heap_usage();

    reset_max_heap_usage();
	start2 = __get_rv_cycle();
    if (FRODOKEM_enc(level, ct, ss1, pk) != 0) {
        goto cleanup;
    }
    end2 = __get_rv_cycle();
	time2 = (end2 - start2) / (20 * 1000); // ms
    max_heap_usage2 = get_max_heap_usage();

    reset_max_heap_usage();
	start3 = __get_rv_cycle();
    if (FRODOKEM_dec(level, ss2, ct, sk) != 0) {
        goto cleanup;
    }
    end3 = __get_rv_cycle();
	time3 = (end3 - start3) / (20 * 1000); // ms
    max_heap_usage3 = get_max_heap_usage();

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
	UART_Init(38400);

	start = __get_rv_cycle();
    if (run_one(FRODOKEM_640, g_result640) != 0) {
        return 1;
    }
    end = __get_rv_cycle();
    time = (end - start) / (20 * 1000); // ms
    //printf("FRODOKEM_640 time is %llu \n", time);

//    start = __get_rv_cycle();
//    if (run_one(FRODOKEM_976,g_result976) != 0) {
//        return 1;
//    }
//    end = __get_rv_cycle();
//	time = (end - start) / (20 * 1000); // ms
//	printf("FRODOKEM_976 time is %llu \n", time);

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
#endif
