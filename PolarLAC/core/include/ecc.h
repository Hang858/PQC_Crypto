#ifndef ECC_H
#define ECC_H

#include <stdint.h>
#include "polarlac_param.h"

#define ecc_info_nodes POLARLAC_PARAM_POLAR_INFO_NODES
#define llr_table POLARLAC_PARAM_LLR_TABLE

int32_t polar_ecc_enc(uint8_t *d);
int32_t polar_ecc_dec(uint8_t *d, const int64_t *c);

#endif
