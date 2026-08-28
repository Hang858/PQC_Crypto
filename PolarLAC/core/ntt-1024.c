// Copyright (c) 2025 Ying Liu
// Affiliation: Key Laboratory of Cyberspace Security Defense, Institute of Information Engineering, CAS
//              School of Cyber Security, University of Chinese Academy of Sciences


// This file defines the C implementation of the 1024-length NTT and NTT-based multiplication code for PolarLAC256.
// Since the method used here is exactly the same as in the case of length 512, 
// we do not provide redundant comments in this file. For details, please refer to ntt.c.

#include <stdint.h>
#include "polarlac_param.h"
#include "operator_interface.h"
#include "memops.h"
#include "malloc.h"
#include "ntt-1024.h"

extern int32_t g_ntt_omega[MAX_NTT_OMEGA_COUNT * 256];
extern int32_t g_ntt_count;      // Stores the length of inverse twiddle factor array
extern int32_t g_ntt_q;          // Stores the modulus
extern int32_t g_ntt_inv_q;
extern int32_t g_ntt_inv_n;
extern int32_t g_ntt_k; 

//Testbook Multiplication for correctness test
void NormalMul_unsign_251(const uint16_t  *a, const uint16_t *s, uint16_t *b)  // Normal multiplication version 2
{
    const polarlac_params_t *p = polarlac_current_params();
    int32_t i,j;
    uint32_t *m = my_malloc((2 * p->dim_n - 1) * sizeof(uint32_t));
    uint16_t *s2 = my_malloc(sizeof(uint16_t) * p->dim_n);
    if (m == NULL || s2 == NULL) {
        my_free(m);
        my_free(s2);
        return;
    }
    for(i=0;i < p->dim_n;i++)
	{
		if(s[i]>=q_half)
			s2[i]=neg_one ; // -1 was transformed into 255 in uint8, so we recover it to 250 with mod 251
		else
			s2[i] = s[i] ;
	}
    for(i=0;i < p->dim_n;i++)
    {
        for(j=0;j < p->dim_n;j++)
        {
            m[i+j]+=((uint32_t)a[i]*(uint32_t)s2[j])%Q;
            m[i+j]=m[i+j]%Q;
        }
    }

    for(i=0;i < p->dim_n - 1;i++)
        b[i]=(m[i]+Q-m[i + p->dim_n])%Q;
    b[p->dim_n - 1]=m[p->dim_n - 1]%Q;
    my_free(m);
    my_free(s2);

}


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
int32_t NTT_1024(int32_t *a)
{

    const polarlac_params_t *p = polarlac_current_params();
    int32_t t,m,i,j,s,e;
    int32_t U=0,V=0,S;

    int32_t *a_mid = my_malloc(sizeof(int32_t) * p->dim_n);
    if (a_mid == NULL) {
        return -1;
    }

    t=1024; 
    for(i=0;i<1024;i++)
    {
        a_mid[i]=a[i];
    }
    
    // Perform 2 layers of 1024-point NTT
    	//level1
    m=1;
        t=(t>>1);
        for(i=0;i<m;i++)
        {
            s=(i*t)<<1;
            e=s+t;
            // S=omega1[0]; // Store roots of first two layers at first position of small array
            S = POLARLAC_PARAM_NTT_OMEGA[0];
            for(j=s;j<e;j++)
            {
                U = a_mid[j];
                V = a_mid[j + t];

                V = MontgomeryReduce((int64_t)V * (int64_t)S, POLARLAC_PARAM_NTTQ, POLARLAC_PARAM_NTT_INV_Q);

                a_mid[j] = U + V - POLARLAC_PARAM_NTTQ;
                a_mid[j + t] = U - V;
            }
        }
        
        //level 2
    m=2;
    t=(t>>1);
        i=0;
        s=(i*t)<<1;
            e=s+t;
            // S=omega2[0]; // Store roots of first two layers at first position of small array
            S = POLARLAC_PARAM_NTT_OMEGA[256];
            for(j=s;j<e;j++)
            {
                U = a_mid[j];
                V = a_mid[j + t];

                V = MontgomeryReduce((int64_t)V * (int64_t)S, POLARLAC_PARAM_NTTQ, POLARLAC_PARAM_NTT_INV_Q);

                a_mid[j] = U + V - POLARLAC_PARAM_NTTQ;
                a_mid[j + t] = U - V;
            }
        i=1;
        s=(i*t)<<1;
            e=s+t;
            // S=omega3[0]; // Store roots of first two layers at first position of small array
            S = POLARLAC_PARAM_NTT_OMEGA[512];
            for(j=s;j<e;j++)
            {
                 U = a_mid[j];
                 V = a_mid[j + t];

                V = MontgomeryReduce((int64_t)V * (int64_t)S, POLARLAC_PARAM_NTTQ, POLARLAC_PARAM_NTT_INV_Q);

                a_mid[j] = U + V - POLARLAC_PARAM_NTTQ;
                a_mid[j + t] = U - V;
            }

    OP_ntt256(a, a_mid, 0);
    OP_ntt256(a+256, a_mid+256, 1); 
    OP_ntt256(a+512, a_mid+512, 2); 
    OP_ntt256(a+768, a_mid+768, 3); 
    
    my_free(a_mid);
    return 0;
}

int32_t INTT_1024(int32_t *a)
{
    const polarlac_params_t *p = polarlac_current_params();
    int32_t t,m,i,j,s,e,h;
    int32_t U=0,V=0,S;

    OP_intt256(a, a, 4);
    OP_intt256(a+256, a+256, 5);
    OP_intt256(a+512, a+512, 6);
    OP_intt256(a+768, a+768, 7);
    


    // final 2 levels with 1024 points
    t=256;
    
    // level2
    m=4;
        s=0;
        h=m>>1;
        i=0;
        
            e=s+t;
            // S = omega2[0];
            S = POLARLAC_PARAM_NTT_OMEGA[1024+256];
            for (j = s; j < e; j++)
            {
                U = a[j];
                V = a[j + t];

                // Update the first element of the butterfly
                a[j] = (U + V - POLARLAC_PARAM_NTTQ);
                a[j] = a[j] + ((a[j] >> 31) & POLARLAC_PARAM_NTTQ); // Ensure the result is in the range (0, Q)

                // Update the second element of the butterfly using Montgomery reduction
                a[j + t] = MontgomeryReduce((int64_t)(U - V) * (int64_t)S, POLARLAC_PARAM_NTTQ, POLARLAC_PARAM_NTT_INV_Q);
            }
            s = s + 2 * t;
        i=1;
            e=s+t;
            // S = omega3[0];
            S = POLARLAC_PARAM_NTT_OMEGA[1024+512];
            for (j = s; j < e; j++)
            {
                U = a[j];
                V = a[j + t];

                // Update the first element of the butterfly
                a[j] = (U + V - POLARLAC_PARAM_NTTQ);
                a[j] = a[j] + ((a[j] >> 31) & POLARLAC_PARAM_NTTQ); // Ensure the result is in the range (0, Q)

                // Update the second element of the butterfly using Montgomery reduction
                a[j + t] = MontgomeryReduce((int64_t)(U - V) * (int64_t)S, POLARLAC_PARAM_NTTQ, POLARLAC_PARAM_NTT_INV_Q);
            }
            s = s + 2 * t;
        t*=2;
	
    // level1
    m=2;
        s=0;
        h=m>>1;
        for(i=0;i<h;i++)
        {
            e=s+t;
            // S = omega1[0];
            S = POLARLAC_PARAM_NTT_OMEGA[1024];
            for (j = s; j < e; j++)
            {
                U = a[j];
                V = a[j + t];

                // Update the first element of the butterfly
                a[j] = (U + V - POLARLAC_PARAM_NTTQ);
                a[j] = a[j] + ((a[j] >> 31) & POLARLAC_PARAM_NTTQ); // Ensure the result is in the range (0, Q)

                // Update the second element of the butterfly using Montgomery reduction
                a[j + t] = MontgomeryReduce((int64_t)(U - V) * (int64_t)S, POLARLAC_PARAM_NTTQ, POLARLAC_PARAM_NTT_INV_Q);
            }
            s = s + 2 * t;
        }
        t*=2;

    for(i=0;i<1024;i++)
    {
        a[i] = MontgomeryReduce((int64_t)a[i]* (int64_t)INVERSE_FOUR_BETA, POLARLAC_PARAM_NTTQ, POLARLAC_PARAM_NTT_INV_Q); // Final Montgomery reduction to ensure standard representation
    }

    return 0;
}
#endif


int32_t poly_mul_ntt_1024(const uint32_t  *a, const uint32_t *s, uint32_t *b)
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
    NTT_1024(a_buf);
    NTT_1024(s_buf);

     //point mul
     OP_cwm(b_buf, a_buf, s_buf, p->dim_n, POLARLAC_PARAM_NTTQ, left_level); // k=0 means no remaining layers


     //INTT form
    INTT_1024(b_buf);

     //mod Q
     for(i=0;i < p->dim_n;i++)
     {
         b[i]=b_buf[i];
     }
     my_free(a_buf);
     my_free(s_buf);
     my_free(b_buf);

     return 0;
}
