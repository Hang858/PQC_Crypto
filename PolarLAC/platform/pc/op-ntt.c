// Copyright (c) 2025 Liu Ying
// Key Laboratory of Cyberspace Security Defense, Institute of Information Engineering, CAS
// School of Cyber Security, University of Chinese Academy of Sciences

#include <string.h>
#include <stdio.h>
#include "polarlac_param.h"
#include "operator_interface.h"

// NTT global variables
int32_t g_ntt_omega[MAX_NTT_OMEGA_COUNT * 256] = {0};
int32_t g_ntt_count = 0;
int32_t g_ntt_q = 0;
int32_t g_ntt_inv_q = 0;
int32_t g_ntt_inv_n = 0;
int32_t g_ntt_k = 0;

/**
 * @brief Initialize NTT parameters
 * @param omega  Twiddle factors array
 * @param count  Number of twiddle factor sets
 * @param q      Prime modulus
 * @param inv_q  Inverse of q mod 2^32
 * @param inv_n  Inverse of 256 mod q
 * @param k      NTT type (0, 1, 2, 3)
 * @return       0: success; -1: error
 */
int32_t OP_ntt256_init(const int32_t *omega, int32_t count, int32_t q, int32_t inv_q, int32_t inv_n, int32_t k)
{
    if (omega == NULL || count <= 0 || count > MAX_NTT_OMEGA_COUNT || k < 0 || k > 3)
        return OP_FAILURE;
    memcpy(g_ntt_omega, omega, 256 * count * sizeof(int32_t));
    g_ntt_count = count;
    g_ntt_q = q;
    g_ntt_inv_n = inv_n;
    g_ntt_inv_q = inv_q;
    g_ntt_k = k;
    return OP_SUCCESS;
}

// Montgomery reduction: a * Beta^(-1) mod q, where Beta = 2^32
static int32_t MontgomeryReduce(int64_t a, int32_t q, int32_t inv_q)
{
    int64_t t;
    int64_t m, r;
    int64_t beta = 1ULL << 32;
    m = a * inv_q;
    m = m & (beta - 1);
    t = m * q;
    r = a - t;
    r >>= 32;
    r = r + ((r >> 31) & q);
    return (int32_t)r;
}

/**
 * @brief Number Theoretic Transform (NTT)
 * @param a_out       Output array (256 x 32-bit)
 * @param a_in        Input array (256 x 32-bit)
 * @param omega_index Index of twiddle factors to use
 * @return            0: success; -1: error
 */
int32_t OP_ntt256(int32_t a_out[256], const int32_t a_in[256], int32_t omega_index)
{
    if (a_in == NULL)
    {
        return -1;
    }

    int32_t t, m, i, j, s, e;
    int32_t U = 0, V = 0, S;
    t = 256;
    int32_t end_level = 256 >> g_ntt_k;
    int32_t start_index = omega_index << (8 - g_ntt_k);
    for (i = 0; i < 256; i++)
        a_out[i] = a_in[i];
    for (m = 1; m < end_level; m <<= 1)
    {
        t = (t >> 1);
        for (i = 0; i < m; i++)
        {
            s = (i * t) << 1;
            e = s + t;
            S = g_ntt_omega[start_index + m + i];
            for (j = s; j < e; j++)
            {
                U = a_out[j];
                V = a_out[j + t];

                V = MontgomeryReduce((int64_t)V * (int64_t)S, g_ntt_q, g_ntt_inv_q);

                a_out[j] = U + V - g_ntt_q;
                a_out[j] = a_out[j] + ((a_out[j] >> 31) & g_ntt_q);

                a_out[j + t] = U - V;
                a_out[j + t] = a_out[j + t] + ((a_out[j + t] >> 31) & g_ntt_q);
            }
        }
    }

    return 0;
}

/**
 * @brief Inverse Number Theoretic Transform (INTT)
 * @param a_out       Output array (256 x 32-bit)
 * @param a_in        Input array (256 x 32-bit)
 * @param omega_index Index of twiddle factors to use
 * @return            0: success; -1: error
 */
int32_t OP_intt256(int32_t a_out[256], const int32_t a_in[256], int32_t omega_index)
{
    if (a_in == NULL)
    {
        return -1;
    }

    int32_t t, m, i, j, s, e, h;
    int32_t U = 0, V = 0, S;

    for (i = 0; i < 256; i++)
        a_out[i] = a_in[i];

    t = 1 << g_ntt_k;
    int32_t begin_level = 256 >> g_ntt_k;
    int32_t start_index = omega_index * (256 >> g_ntt_k);

    for (m = begin_level; m > 1; m >>= 1)
    {
        s = 0;
        h = m >> 1;
        for (i = 0; i < h; i++)
        {
            e = s + t;
            S = g_ntt_omega[start_index + h + i];
            for (j = s; j < e; j++)
            {
                U = a_out[j];
                V = a_out[j + t];

                a_out[j] = (U + V - g_ntt_q);
                a_out[j] = a_out[j] + ((a_out[j] >> 31) & g_ntt_q);

                a_out[j + t] = MontgomeryReduce((int64_t)(U - V) * (int64_t)S, g_ntt_q, g_ntt_inv_q);
            }
            s = s + 2 * t;
        }
        t *= 2;
    }

    for (i = 0; i < 256; i++)
    {
        a_out[i] = MontgomeryReduce((int64_t)a_out[i] * (int64_t)g_ntt_inv_n, g_ntt_q, g_ntt_inv_q);
    }

    return 0;
}

/**
 * @brief Coefficient-Wise Multiplication (CWM)
 * @param c_out  Output array (n x 32-bit)
 * @param a_in   Input array A (n x 32-bit)
 * @param b_in   Input array B (n x 32-bit)
 * @param n      Length (max 1024)
 * @param q      Prime modulus (< 31-bit)
 * @param k      Type parameter (0, 1 supported)
 * @return       0: success; -1: error
 */
int32_t OP_cwm(int32_t *c_out, const int32_t *a_in, const int32_t *b_in, int32_t n, int32_t q, int32_t k)
{
    int32_t i, j;
    if (k == 0)
        for (i = 0; i < n; i++)
        {
            c_out[i] = MontgomeryReduce((int64_t)a_in[i] * (int64_t)b_in[i], q, g_ntt_inv_q);
        }
    else if (k == 1)
        for (i = 0, j = 0; i < n - 3; i += 4, j++)
        {
            const int64_t r = g_ntt_omega[(n >> 2) + j];

            int64_t a0 = a_in[i];
            int64_t a1 = a_in[i + 1];
            int64_t s0 = b_in[i];
            int64_t s1 = b_in[i + 1];

            int64_t t00 = a0 * s0;
            int64_t t11 = a1 * s1;
            int64_t t11r = MontgomeryReduce(t11, q, g_ntt_inv_q) * r;

            c_out[i] = MontgomeryReduce(t00 + t11r, q, g_ntt_inv_q);

            int64_t t01 = a0 * s1;
            int64_t t10 = a1 * s0;

            c_out[i + 1] = MontgomeryReduce(t01 + t10, q, g_ntt_inv_q);

            int64_t a2 = a_in[i + 2];
            int64_t a3 = a_in[i + 3];
            int64_t s2 = b_in[i + 2];
            int64_t s3 = b_in[i + 3];

            int64_t u22 = a2 * s2;
            int64_t u33 = a3 * s3;
            int64_t u33r = MontgomeryReduce(u33, q, g_ntt_inv_q) * r;

            c_out[i + 2] = MontgomeryReduce(u22 - u33r, q, g_ntt_inv_q);

            int64_t u23 = a2 * s3;
            int64_t u32 = a3 * s2;

            c_out[i + 3] = MontgomeryReduce(u23 + u32, q, g_ntt_inv_q);
        }
    else
        return -1;
    return 0;
}
