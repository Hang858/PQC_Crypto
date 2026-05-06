#ifndef PQC_FRODOKEM_PARAMETERS_H
#define PQC_FRODOKEM_PARAMETERS_H

#include "frodokem_params.h"

#define PARAMS_N (g_frodokem_params->n)
#define PARAMS_NBAR (g_frodokem_params->nbar)
#define PARAMS_LOGQ (g_frodokem_params->logq)
#define PARAMS_Q (g_frodokem_params->q)
#define PARAMS_EXTRACTED_BITS (g_frodokem_params->extracted_bits)
#define PARAMS_STRIPE_STEP (g_frodokem_params->stripe_step)
#define PARAMS_PARALLEL 4

#define BYTES_SEED_A (g_frodokem_params->bytes_seed_a)
#define BYTES_MU (g_frodokem_params->bytes_mu)
#define BYTES_SALT (g_frodokem_params->bytes_salt)
#define BYTES_SEED_SE (g_frodokem_params->bytes_seed_se)
#define BYTES_PKHASH (g_frodokem_params->bytes_pkhash)

#define CRYPTO_SECRETKEYBYTES (g_frodokem_params->secretkeybytes)
#define CRYPTO_PUBLICKEYBYTES (g_frodokem_params->publickeybytes)
#define CRYPTO_BYTES (g_frodokem_params->bytes)
#define CRYPTO_CIPHERTEXTBYTES (g_frodokem_params->ciphertextbytes)
#define CRYPTO_ALGNAME (g_frodokem_params->algname)

#define CDF_TABLE (g_frodokem_params->cdf_table)
#define CDF_TABLE_LEN (g_frodokem_params->cdf_table_len)

#define shake frodokem_shake

void frodokem_shake(uint8_t *output, size_t outlen, const uint8_t *input, size_t inlen);

#endif
