#ifndef PQC_FALCON_API_H
#define PQC_FALCON_API_H

#include "falcon_params.h"

#define CRYPTO_SECRETKEYBYTES FALCON_MAX_SECRETKEYBYTES
#define CRYPTO_PUBLICKEYBYTES FALCON_MAX_PUBLICKEYBYTES
#define CRYPTO_BYTES FALCON_MAX_BYTES
#define CRYPTO_ALGNAME "Falcon"

int crypto_sign_keypair(falcon_level_t level, unsigned char *pk, unsigned char *sk);
int crypto_sign(falcon_level_t level, unsigned char *sm, unsigned long long *smlen,
    const unsigned char *m, unsigned long long mlen, const unsigned char *sk);
int crypto_sign_open(falcon_level_t level, unsigned char *m, unsigned long long *mlen,
    const unsigned char *sm, unsigned long long smlen, const unsigned char *pk);

#endif
