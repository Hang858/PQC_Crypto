#ifndef PQC_FRODOKEM_API_H
#define PQC_FRODOKEM_API_H

#include <stdint.h>
#include "frodokem_params.h"

int FRODOKEM_crypto_kem_keypair_enc(frodokem_level_t level, uint8_t *ct, uint8_t *ss, uint8_t *pk, uint8_t *sk);
int FRODOKEM_crypto_kem_dec(frodokem_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
