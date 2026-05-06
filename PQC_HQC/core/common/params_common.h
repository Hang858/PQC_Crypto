#ifndef PQC_HQC_PARAMS_COMMON_H
#define PQC_HQC_PARAMS_COMMON_H

#include "api.h"

#define CEIL_DIVIDE(a, b) (((a) / (b)) + ((a) % (b) == 0 ? 0 : 1))
#define BITMASK(a, size)  ((1UL << ((a) % (size))) - 1)

#define SECRET_KEY_BYTES            CRYPTO_SECRETKEYBYTES
#define PUBLIC_KEY_BYTES            CRYPTO_PUBLICKEYBYTES
#define SHARED_SECRET_BYTES         CRYPTO_BYTES
#define CIPHERTEXT_BYTES            CRYPTO_CIPHERTEXTBYTES

#define VEC_N_SIZE_BYTES            CEIL_DIVIDE(PARAM_N, 8)
#define VEC_K_SIZE_BYTES            PARAM_K
#define VEC_N1_SIZE_BYTES           PARAM_N1
#define VEC_N1N2_SIZE_BYTES         CEIL_DIVIDE(PARAM_N1N2, 8)

#define VEC_N_SIZE_64               CEIL_DIVIDE(PARAM_N, 64)
#define VEC_N1_SIZE_64              CEIL_DIVIDE(PARAM_N1, 8)
#define VEC_N1N2_SIZE_64            CEIL_DIVIDE(PARAM_N1N2, 64)

#endif
