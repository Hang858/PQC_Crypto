#ifndef HQC_API_H
#define HQC_API_H

#include <stddef.h>
#include <stdint.h>
#include "hqc_params.h"

int HQC_crypto_kem_keypair(hqc_level_t level, uint8_t *pk, uint8_t *sk);
int HQC_crypto_kem_enc(hqc_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int HQC_crypto_kem_dec(hqc_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
