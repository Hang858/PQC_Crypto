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

int OP_hash(uint8_t alg, uint8_t mode, int n, const void *input, int input_len, uint8_t link_count, void *output);
int OP_hash_init(uint8_t alg, void *s, int s_len);
int OP_hash_absorb(uint8_t alg, void *s, int s_len, const void *input, int input_len);
int OP_hash_squeeze(uint8_t alg, void *s, int s_len, void *output, int output_len);
int OP_trng(void *buffer, int size);
int OP_matrix_mul_8x8(uint16_t z_out[8][8], const uint16_t x_in[8][8], const uint16_t y_in[8][8], uint16_t q);

#endif
