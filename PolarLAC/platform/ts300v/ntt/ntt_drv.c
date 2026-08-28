#include <stdint.h>
#include <stddef.h>
#include "ntt_reg.h"
#include "ntt_drv.h"
#include "ntt_ll_drv.h"
#include "ntt_adapter.h"

//int32_t poly_add(void const *opa, void const *opb, void *res, uint32_t length)
//{
//    int32_t ret = ntt_set_opa(opa, NTT_OPA_ADDR, length);
//    ret |= ntt_set_opb(opb, NTT_OPB_ADDR, length);
//    ret |= ntt_set_res_addr(NTT_RES_ADDR);
//    ret |= ntt_op(NTTMR_MODE_POLY_ADD);
//    if (ret != NTT_SUCCESS) {
//        return ret; // Error in setting up the operation
//    }
//    ret |= ntt_read_res((void *)res, NTT_RES_ADDR, length);
//
//    return ret;
//}
//
//int32_t poly_sub(void const *opa, void const *opb, void *res, uint32_t length)
//{
//    int32_t ret = ntt_set_opa(opa, NTT_OPA_ADDR, length);
//    ret |= ntt_set_opb(opb, NTT_OPB_ADDR, length);
//    ret |= ntt_set_res_addr(NTT_RES_ADDR);
//    ret |= ntt_op(NTTMR_MODE_POLY_SUB);
//    if (ret != NTT_SUCCESS) {
//        return ret; // Error in setting up the operation
//    }
//    ret |= ntt_read_res((void *)res, NTT_RES_ADDR, length);
//
//    return ret;
//}

int32_t ntt(void const *indata, void *res, uint32_t length)
{
	int32_t *temp = indata;
	for (int32_t i = 0; i < length; ++i) {
		temp[i] = temp[i] % LAC_Q;
		if(temp[i] < 0) {
		   temp[i] += LAC_Q;
		}
	}
    int32_t ret = ntt_set_opa(indata, NTT_OPA_ADDR, length);
    ret |= ntt_set_res_addr(NTT_RES_ADDR);
    ret |= ntt_op(NTTMR_MODE_NTT);
    if (ret != NTT_SUCCESS) {
        return ret; // Error in setting up the operation
    }
    ret |= ntt_read_res(res, NTT_RES_ADDR, length);

    return ret;
}

int32_t intt(void const *indata, void *res, uint32_t length)
{
	int32_t *temp = indata;
	for (int32_t i = 0; i < length; ++i) {
		temp[i] = temp[i] % LAC_Q;
		if(temp[i] < 0) {
		   temp[i] += LAC_Q;
		}
	}
    int32_t ret = ntt_set_opa(indata, NTT_OPA_ADDR, length);
    ret |= ntt_set_res_addr(NTT_RES_ADDR);
    ret |= ntt_op(NTTMR_MODE_INTT);
    if (ret != NTT_SUCCESS) {
        return ret; // Error in setting up the operation
    }
    ret |= ntt_read_res(res, NTT_RES_ADDR, length);

    return ret;
}

int32_t pwm(void const *opa, void const *opb, void *res, uint32_t length, uint32_t l) {
    int32_t ret = ntt_set_opa(opa, NTT_OPA_ADDR, length);
    ret |= ntt_set_opb(opb, NTT_OPB_ADDR, length);
    ret |= ntt_set_res_addr(NTT_RES_ADDR);
    if (l == 0) {
        ret |= ntt_op(NTTMR_MODE_PWM8);
    } else if (l == 1) {
        ret |= ntt_op(NTTMR_MODE_PWM7);
    } else {
        return NTT_ERROR; // Invalid parameter for l
    }
    if (ret != NTT_SUCCESS) {
        return ret; // Error in setting up the operation
    }
    ret |= ntt_read_res(res, NTT_RES_ADDR, length);

    return ret;
}
