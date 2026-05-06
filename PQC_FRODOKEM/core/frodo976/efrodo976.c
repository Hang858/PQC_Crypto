#include "level.h"
#include "../common/fips202.h"

#define shake shake256

uint16_t CDF_TABLE[11] = {5638, 15915, 23689, 28571, 31116, 32217, 32613, 32731, 32760, 32766, 32767};
uint16_t CDF_TABLE_LEN = 11;
