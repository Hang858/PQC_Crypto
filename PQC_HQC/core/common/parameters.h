#ifndef PQC_HQC_RUNTIME_PARAMETERS_H
#define PQC_HQC_RUNTIME_PARAMETERS_H

#include "api.h"
#include "hqc_params.h"

#define CEIL_DIVIDE(a, b) (((a) / (b)) + ((a) % (b) == 0 ? 0 : 1))
#define BITMASK(a, size) ((uint64_t)((1ULL << ((a) % (size))) - 1ULL))

#define PARAM_N (g_hqc_params->n)
#define PARAM_N1 (g_hqc_params->n1)
#define PARAM_N2 (g_hqc_params->n2)
#define PARAM_N1N2 (g_hqc_params->n1n2)
#define PARAM_OMEGA (g_hqc_params->omega)
#define PARAM_OMEGA_E (g_hqc_params->omega_e)
#define PARAM_OMEGA_R (g_hqc_params->omega_r)
#define PARAM_SECURITY (g_hqc_params->security_bits)
#define PARAM_SECURITY_BYTES (g_hqc_params->security_bytes)
#define PARAM_DFR_EXP (g_hqc_params->security_bits)

#define SECRET_KEY_BYTES (g_hqc_params->secretkeybytes)
#define PUBLIC_KEY_BYTES (g_hqc_params->publickeybytes)
#define SHARED_SECRET_BYTES (g_hqc_params->bytes)
#define CIPHERTEXT_BYTES (g_hqc_params->ciphertextbytes)

#define VEC_N_SIZE_BYTES (g_hqc_params->vec_n_size_bytes)
#define VEC_K_SIZE_BYTES (g_hqc_params->vec_k_size_bytes)
#define VEC_N1_SIZE_BYTES (g_hqc_params->vec_n1_size_bytes)
#define VEC_N1N2_SIZE_BYTES (g_hqc_params->vec_n1n2_size_bytes)
#define VEC_N_SIZE_64 (g_hqc_params->vec_n_size_64)
#define VEC_N1_SIZE_64 (g_hqc_params->vec_n1_size_64)
#define VEC_N1N2_SIZE_64 (g_hqc_params->vec_n1n2_size_64)

#define PARAM_DELTA (g_hqc_params->delta)
#define PARAM_M 8u
#define PARAM_GF_POLY 0x11Du
#define PARAM_GF_MUL_ORDER 255u
#define PARAM_K (g_hqc_params->k)
#define PARAM_G (g_hqc_params->g)
#define PARAM_FFT (g_hqc_params->fft)

#define SEED_BYTES HQC_SEED_BYTES
#define SALT_BYTES HQC_SALT_BYTES
#define PARAM_N_MU (g_hqc_params->n_mu)
#define UTILS_REJECTION_THRESHOLD (g_hqc_params->rejection_threshold)

#endif
