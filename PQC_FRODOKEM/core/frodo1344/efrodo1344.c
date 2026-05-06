#include "level.h"
#include "../common/fips202.h"

#define shake shake256

uint16_t CDF_TABLE[7] = {9142, 23462, 30338, 32361, 32725, 32765, 32767};
uint16_t CDF_TABLE_LEN = 7;
