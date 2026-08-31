#ifndef PQC_FRODOKEM_PARAMS_H
#define PQC_FRODOKEM_PARAMS_H

#include <stddef.h>
#include <stdint.h>

#include "api.h"

#define FRODOKEM_MAX_SECRET_KEY_BYTES 43088u
#define FRODOKEM_MAX_PUBLIC_KEY_BYTES 21520u
#define FRODOKEM_MAX_SHARED_SECRET_BYTES 32u
#define FRODOKEM_MAX_CIPHERTEXT_BYTES 21696u
#define FRODOKEM_MAX_N 1344u
#define FRODOKEM_NBAR 8u
#define FRODOKEM_BYTES_SEED_A 16u

extern const frodokem_params_t *g_frodokem_params;

const frodokem_params_t *FRODOKEM_get_params(frodokem_level_t level);
int FRODOKEM_select_level(frodokem_level_t level);

int crypto_kem_keypair_impl(uint8_t *pk, uint8_t *sk);
int crypto_kem_enc_impl(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int crypto_kem_dec_impl(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
