#ifndef __SHA3_INTERFACE_H__
#define __SHA3_INTERFACE_H__

// typedef enum {
//     OP_ALG_SHAKE256 = 3,  
//     OP_ALG_SHAKE128 = 4,  
//     OP_ALG_SHA3_256 = 5,
//     OP_ALG_SHA3_384 = 6,
//     OP_ALG_SHA3_512 = 7,
//     OP_ALG_SHA3_224 = 8
// } op_alg_type_t;
#include "operator_interface.h"

typedef struct {
    uint8_t buf[200];    
    uint8_t pos;
    uint8_t IS_FINALIZED;
} sha3_ctx_t;

#define SEC_LVL_1           (0U << 8)
#define SEC_LVL_3           (1U << 8)
#define SEC_LVL_5           (2U << 8)
#define SEC_LVL_MASK        (3U << 8)  

uint32_t shake256_hash_chain(uint8_t level, void* input, uint32_t input_len, uint8_t* output, uint32_t output_len, uint8_t link_count);
uint32_t shake256_prf(uint8_t level, void* input, uint32_t input_len, uint8_t* output, uint32_t output_len);
uint32_t shake256_Tlen(uint8_t level, void* input, uint32_t input_len, uint8_t* output, uint32_t output_len);


uint32_t sha3_hash(uint8_t alg, uint8_t *input, uint32_t input_len, uint8_t *output, uint32_t output_len);
uint32_t sha3_absorb(uint8_t alg, void *s, uint32_t s_len, const uint8_t *input, uint32_t input_len);
uint32_t sha3_squeeze(uint8_t alg, void *s, uint32_t s_len, uint8_t *output, uint32_t output_len);
uint32_t sha3_init(uint8_t alg, void *s, uint32_t s_len);

// 0 = success
// Positive = error code
#define SHA3_OK               0x55AAAA55    // Success
#define SHA3_ERR_NULL_PTR     0xAA55A501    // Null pointer error
#define SHA3_ERR_INVALID_LEN  0xAA55A502    // Invalid length
#define SHA3_ERR_MALLOC_FAIL  0xAA55A503    // Memory allocation failed
#define SHA3_ERR_LINK_COUNT   0xAA55A504    // Invalid link count
#define SHA3_ERR_ARG          0xAA55A505    // ARG ERR
#define SHA3_ERR_SRDONE       0xAA55A506    // ERR_SRDONE
#endif

