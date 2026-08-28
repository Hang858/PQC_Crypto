#include<stdint.h>
#include "operator_interface.h"

#ifndef NTT_HARDWARE_IMPL
int32_t NTT(int32_t *a);
int32_t INTT(int32_t *a);
#else

#define NTT(a)          OP_ntt256(a, a, 0)
#define INTT(a)         OP_intt256(a, a, 0)

#endif

int32_t poly_mul_ntt(const uint32_t  *a, const uint32_t *s, uint32_t *b);
