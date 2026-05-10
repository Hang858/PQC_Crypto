#ifndef PQC_SCLOUDPLUS_API_H
#define PQC_SCLOUDPLUS_API_H

#include <stdint.h>
#include "scloudplus_params.h"

#define CRYPTO_ALGNAME "SCLOUD+"
#define CRYPTO_SECRETKEYBYTES SCLOUDPLUS_MAX_SECRET_KEY_BYTES
#define CRYPTO_PUBLICKEYBYTES SCLOUDPLUS_MAX_PUBLIC_KEY_BYTES
#define CRYPTO_BYTES SCLOUDPLUS_MAX_SHARED_SECRET_BYTES
#define CRYPTO_CIPHERTEXTBYTES SCLOUDPLUS_MAX_CIPHERTEXT_BYTES

int crypto_kem_keypair(scloudplus_level_t level, uint8_t *pk, uint8_t *sk);
int crypto_kem_enc(scloudplus_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int crypto_kem_dec(scloudplus_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
