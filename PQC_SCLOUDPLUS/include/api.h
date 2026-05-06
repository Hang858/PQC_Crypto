#ifndef PQC_SCLOUDPLUS_API_H
#define PQC_SCLOUDPLUS_API_H

#include <stdint.h>
#include "scloudplus_params.h"

int SCLOUDPLUS_crypto_kem_keypair(scloudplus_level_t level, uint8_t *pk, uint8_t *sk);
int SCLOUDPLUS_crypto_kem_enc(scloudplus_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int SCLOUDPLUS_crypto_kem_dec(scloudplus_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
