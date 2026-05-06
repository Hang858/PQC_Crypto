#ifndef PQC_FRODOKEM_API_H
#define PQC_FRODOKEM_API_H

#include <stdint.h>
#include "frodokem_params.h"

#define CRYPTO_ALGNAME "FrodoKEM"
#define CRYPTO_SECRETKEYBYTES FRODOKEM_MAX_SECRET_KEY_BYTES
#define CRYPTO_PUBLICKEYBYTES FRODOKEM_MAX_PUBLIC_KEY_BYTES
#define CRYPTO_BYTES FRODOKEM_MAX_SHARED_SECRET_BYTES
#define CRYPTO_CIPHERTEXTBYTES FRODOKEM_MAX_CIPHERTEXT_BYTES

int crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
