#ifndef SCLOUDPLUS_KAT_OPERATOR_INTERFACE_H
#define SCLOUDPLUS_KAT_OPERATOR_INTERFACE_H
#include <stdint.h>
#define OP_ALG_SHAKE128 4
#define OP_ALG_SHAKE256 3
#define OP_MODE_NORMAL 0
int OP_hash(uint8_t alg, uint8_t mode, int n, const void *input, int input_len, uint8_t link_count, void *output);
#endif
