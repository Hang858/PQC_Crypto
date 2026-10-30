/**
 * 更新时间： 12-18
 * @file operator_interface.h
 * @brief 硬件算子抽象接口汇总，用于软件团队适配算法工作。
 */

#ifndef HARDWARE_DRIVER_H
#define HARDWARE_DRIVER_H

#include <stdint.h>
#include <stddef.h>

#define OP_SUCCESS 0
#define OP_FAILURE -1

#define OP_ALG_SHA256   1
#define OP_ALG_SM3      2
#define OP_ALG_SHAKE128 4
#define OP_ALG_SHAKE256 3
#define OP_ALG_SHA3_256 5
#define OP_ALG_SHA3_384 6
#define OP_ALG_SHA3_512 7

#define OP_MODE_NORMAL 0
#define OP_MODE_LMS 1
#define OP_MODE_SPHINCS 2

#define OP_STATE_SIZE_SM3 104
#define OP_STATE_SIZE_SHA3 208

#define MAX_NTT_OMEGA_COUNT 8
/* This module's software backend retains one pointer to the caller's static
 * read-only twiddle table. Board driver RAM must be budgeted separately. */
#define OP_NTT_WORK_RAM_BYTES (sizeof(void *))
/* BiT contract: N=256, q=7681, full eight layers (k=0), count=2.
 * omega is forward[256] then inverse[256], each multiplied by R=2^32 mod q.
 * inv_q is q^-1 mod 2^32 (bit pattern 2340676097), inv_n is
 * 256^-1 * R mod q = 1912, not the unscaled inverse 7651.
 * Transform inputs/outputs are canonical [0,q); indices are forward=0,
 * inverse=1. CWM k=0 returns a[i]*b[i]*R^-1 mod q, for canonical inputs.
 * The adapter supplies disjoint int32 buffers; only scalar conversion back
 * from R^-1 accumulators uses a public constant multiplication on the CPU. */
int OP_ntt256_init(const int32_t *omega, int32_t count, int32_t q,
                   int32_t inv_q, int32_t inv_n, int32_t k);
int OP_ntt256(int32_t out[256], const int32_t in[256], int omega_index);
int OP_intt256(int32_t out[256], const int32_t in[256], int omega_index);
int OP_cwm(int32_t *out, const int32_t *a, const int32_t *b,
           int32_t n, int32_t q, int32_t k);

typedef double op_fpr_t;

int OP_hash(uint8_t alg, uint8_t mode, int n, const void *input, int input_len, uint8_t link_count, void *output);
int OP_hash_init(uint8_t alg, void *s, int s_len);
int OP_hash_absorb(uint8_t alg, void *s, int s_len, const void *input, int input_len);
int OP_hash_squeeze(uint8_t alg, void *s, int s_len, void *output, int output_len);
int OP_trng(void *buffer, int size);
int OP_matrix_mul_8x8(uint16_t z_out[8][8], const uint16_t x_in[8][8], const uint16_t y_in[8][8], uint16_t q);
int OP_fpr_add(op_fpr_t a, op_fpr_t b, op_fpr_t *out);
int OP_fpr_sub(op_fpr_t a, op_fpr_t b, op_fpr_t *out);
int OP_fpr_mul(op_fpr_t a, op_fpr_t b, op_fpr_t *out);
int OP_fpr_div(op_fpr_t a, op_fpr_t b, op_fpr_t *out);
int OP_fpr_sqrt(op_fpr_t a, op_fpr_t *out);
int OP_fpr_lt(op_fpr_t a, op_fpr_t b, uint8_t *out);

#endif
