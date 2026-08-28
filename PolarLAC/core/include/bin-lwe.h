#include <stdint.h>
#include "fips202.h"

#define AES256_BLOCKLEN 64// buffer size for gen_a
#define REJ_BLOCKLEN ((8*POLARLAC_PARAM_DIM_N/8*(1<<8)/Q + AES256_BLOCKLEN) / AES256_BLOCKLEN)
// #define REJ_BLOCKLEN 9

//generate the public parameter a from seed
int32_t gen_a(unsigned char *a,  const unsigned char *seed);
//generate the small random vector for secret and error
int32_t gen_e(unsigned char *e, keccak_state *state);

// poly_mul  b=[as]
int32_t poly_mul(const unsigned char *a, const unsigned char *s, unsigned char *b, uint32_t  vec_num);
// poly_aff  b=as+e 
int32_t poly_aff(const unsigned char *a, const  unsigned char *s, unsigned char *e, unsigned char *b, uint32_t vec_num);
// Compression: c1 discards 1-bit
int32_t poly_compress_c1_1bit(const unsigned char *in,  unsigned char *out, const uint32_t vec_num);
int32_t poly_decompress_c1_1bit(const unsigned char *in,  unsigned char *out, const uint32_t vec_num);
// Compression: c2 discards 4-bit
int32_t poly_compress_c2_4bit(const unsigned char *in,  unsigned char *out, const uint32_t vec_num);
int32_t poly_decompress_c2_4bit(const unsigned char *in,  unsigned char *out, const uint32_t vec_num);
// Compression: c2 discards 5-bit
int32_t poly_compress_c2_5bit(const unsigned char *in,  unsigned char *out, const uint32_t vec_num);
int32_t poly_decompress_c2_5bit(const unsigned char *in,  unsigned char *out, const uint32_t vec_num);
