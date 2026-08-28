#include<stdint.h>
#include "operator_interface.h"

#ifndef NTT_HARDWARE_IMPL

int32_t NTT_1024(int32_t *a);
int32_t INTT_1024(int32_t *a);

#else

#define NTT_1024(a)          OP_ntt256(a, a, 0)
#define INTT_1024(a)         OP_intt256(a, a, 0)

#endif

int32_t poly_mul_ntt_1024(const uint32_t  *a, const uint32_t *s, uint32_t *b);

void NormalMul_unsign_251(const uint16_t  *a, const uint16_t *s, uint16_t *b);
