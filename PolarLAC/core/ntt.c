// Copyright (c) 2025 Ying Liu
// Affiliation: Key Laboratory of Cyberspace Security Defense, Institute of Information Engineering, CAS
//              School of Cyber Security, University of Chinese Academy of Sciences

// This file implements the C-language version of 512-length NTT and NTT-based multiplication for PolarLAC128.

#include <stdint.h>
#include "ntt.h"
#include "polarlac_param.h"
#include "operator_interface.h"
#include "memops.h"
#include "malloc.h"


extern int32_t g_ntt_omega[MAX_NTT_OMEGA_COUNT * 256];
extern int32_t g_ntt_count;      // Stores the length of inverse twiddle factor array
extern int32_t g_ntt_q;          // Stores the modulus
extern int32_t g_ntt_inv_q;
extern int32_t g_ntt_inv_n;
extern int32_t g_ntt_k;  

static int32_t MontgomeryReduce(int64_t a, int32_t q, int32_t inv_q)  
{
    int64_t t;
    int64_t m,r;

    m = a * inv_q;
    m=m&(Beta-1);  // m mod β
    t = m * q;
    r = a - t;
    r >>= 32; // r in thr range of (-Q, Q)
    r = r + ((r >> 31) & q); // Ensure r is in the range of (0, Q)
    return (int32_t)r;
}

#ifndef NTT_HARDWARE_IMPL
// This NTT function is a naive implementation of the Number Theoretic Transform (NTT) algorithm, 
// and we do not use it. It is provided for reference of the theorem of NTT algorithm.
// In-place NTT Algorithm for LAC128 with 512 length and 18433 as modulus
// Input: Coefficient vctor
// Output: NTT vector
int32_t NTT(int32_t *a)
{
	const polarlac_params_t *p = polarlac_current_params();
	int32_t t,m,i,j,s,e;
    int32_t U=0,V=0,S;
    int32_t *a_mid = my_malloc(sizeof(int32_t) * p->dim_n);
    if (a_mid == NULL) {
        return -1;
    }
    t=512; 
    for(i=0;i<512;i++)
    {
        a_mid[i]=a[i];
    }
    // Compute first layer
    m = 1;
    t = (t >> 1);
    for (i = 0; i < m; i++)
    {
        s = (i * t) << 1;
        e = s + t;
        // S = omega1[0];
        S = POLARLAC_PARAM_NTT_OMEGA[0];
        for (j = s; j < e; j++)
        {

            U = a_mid[j];
            V = a_mid[j + t];

            V = MontgomeryReduce((int64_t)V * (int64_t)S, POLARLAC_PARAM_NTTQ, POLARLAC_PARAM_NTT_INV_Q);

            a_mid[j] = U + V - POLARLAC_PARAM_NTTQ;
            a_mid[j + t] = U - V;
        }
    }
    // Call operator
    OP_ntt256(a, a_mid, 0);
    OP_ntt256(a+256, a_mid+256, 1);  

    my_free(a_mid);
    return 0;
}

//The computation process of the INTT is analogous to that of the NTT, 
//and the two exhibit a symmetric relationship.
int32_t INTT(int32_t *a)
{
	const polarlac_params_t *p = polarlac_current_params();
	int32_t t,m,i,j,s,e,h;
    int32_t U=0,V=0,S;
    OP_intt256(a, a, 2);
    OP_intt256(a+256, a+256, 3);

    // final level with 512 points
    t=256;
    m = 2;
    s = 0;
    h = m >> 1;
    for (i = 0; i < h; i++)
    {
        e = s + t;
        // S = omega1[0];
        S = POLARLAC_PARAM_NTT_OMEGA[512];
            for (j = s; j < e; j++)
            {
                U = a[j];
                V = a[j + t];

                // Update the first element of the butterfly
                //  a[j] = U + V ;
                a[j] = (U + V - POLARLAC_PARAM_NTTQ);
                a[j] = a[j] + ((a[j] >> 31) & POLARLAC_PARAM_NTTQ); // Ensure the result is in the range (0, Q)

                // Update the second element of the butterfly using Montgomery reduction
                a[j + t] = MontgomeryReduce((int64_t)(U - V) * (int64_t)S, POLARLAC_PARAM_NTTQ, POLARLAC_PARAM_NTT_INV_Q);
            }
            s = s + 2 * t;
    }

    for(i=0;i<512;i++)
    {
        a[i] = MontgomeryReduce((int64_t)a[i]* (int64_t)INVERSE_TWO_BETA, POLARLAC_PARAM_NTTQ, POLARLAC_PARAM_NTT_INV_Q); // Final Montgomery reduction to ensure standard representation
    }

    return 0;
}
#endif


int32_t poly_mul_ntt(const uint32_t  *a, const uint32_t *s, uint32_t *b)
{
    const polarlac_params_t *p = polarlac_current_params();
    int32_t i;
    int32_t *a_buf = my_malloc(sizeof(int32_t) * p->dim_n);
    int32_t *s_buf = my_malloc(sizeof(int32_t) * p->dim_n);
    int32_t *b_buf = my_malloc(sizeof(int32_t) * p->dim_n);
    if (a_buf == NULL || s_buf == NULL || b_buf == NULL) {
        my_free(a_buf);
        my_free(s_buf);
        my_free(b_buf);
        return -1;
    }

    for(i=0;i < p->dim_n;i++)
    {
        a_buf[i]=a[i];
        s_buf[i]=s[i];
    }

     //NTT form
    NTT(a_buf);
    NTT(s_buf);

     //point mul
	 OP_cwm(b_buf, a_buf, s_buf, p->dim_n, POLARLAC_PARAM_NTTQ, POLARLAC_PARAM_NTT_K); // k=0 means no remaining layers

     //INTT to recover the coefficient form
    INTT(b_buf);

     //send back the results
     for(i=0;i < p->dim_n;i++)
     {
         b[i]=b_buf[i];
     }
     my_free(a_buf);
     my_free(s_buf);
     my_free(b_buf);

     return 0;
}
