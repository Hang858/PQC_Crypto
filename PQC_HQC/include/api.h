#ifndef PQC_HQC_API_H
#define PQC_HQC_API_H

#include <stdint.h>
#include "hqc_params.h"

#define CRYPTO_ALGNAME "HQC"
#define CRYPTO_SECRETKEYBYTES HQC_MAX_SECRET_KEY_BYTES
#define CRYPTO_PUBLICKEYBYTES HQC_MAX_PUBLIC_KEY_BYTES
#define CRYPTO_BYTES HQC_SHARED_SECRET_BYTES
#define CRYPTO_CIPHERTEXTBYTES HQC_MAX_CIPHERTEXT_BYTES

int crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
