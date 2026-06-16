#ifndef PQC_HQC_API_H
#define PQC_HQC_API_H

#include "hqc_params.h"

#define CRYPTO_ALGNAME "HQC"

#define CRYPTO_SECRETKEYBYTES  HQC_MAX_SECRETKEYBYTES
#define CRYPTO_PUBLICKEYBYTES  HQC_MAX_PUBLICKEYBYTES
#define CRYPTO_BYTES           HQC_MAX_BYTES
#define CRYPTO_CIPHERTEXTBYTES HQC_MAX_CIPHERTEXTBYTES

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#endif
