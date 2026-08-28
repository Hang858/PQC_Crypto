#ifndef _NTT_ADAPTER_H_
#define _NTT_ADAPTER_H_

#include "ntt_drv.h"
#include "ntt_ll_drv.h"
#include "ntt_reg.h"
#include "polarlac_params.h"

#define LAC_Q 			8380417
#define NTT_N 			POLARLAC_PARAM_DIM_N

#define OP_ntt256_init(omega, count, q, inv_q, inv_n, k)	ntt_init(POLARLAC_PARAM_NTT_OMEGA, NTT_N, q,     \
    POLARLAC_PARAM_NTT_HW_CONFIG)
#define OP_ntt256(a_out, a_in, omega_index)             ntt(a_in, a_out, NTT_N)
#define OP_intt256(a_out, a_in, omega_index)            intt(a_in, a_out, NTT_N)
#define OP_cwm(c_out, a_in, b_in, n, q, k)              pwm(a_in, b_in, c_out, NTT_N, k)
//#define OP_poly_add(c_out, a_in, b_in)                  poly_add(a_in, b_in, c_out, NTT_N)
//#define OP_poly_sub(c_out, a_in, b_in)                  poly_sub(a_in, b_in, c_out, NTT_N)

#endif // _NTT_ADAPTER_H_
