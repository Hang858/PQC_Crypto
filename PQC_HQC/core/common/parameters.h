#ifndef PQC_HQC_PARAMETERS_H
#define PQC_HQC_PARAMETERS_H

#include "hqc_params.h"

#define CEIL_DIVIDE(a, b) (((a) / (b)) + ((a) % (b) == 0 ? 0 : 1))
#define BITMASK(a, size) ((uint64_t)((1ULL << ((a) % (size))) - 1ULL))

/* HQC-5 constants (compile-time maximum values, shared across all levels) */
#define PARAM_M         8
#define PARAM_GF_POLY   0x11D
#define PARAM_GF_MUL_ORDER 255

#define SEED_BYTES      32
#define SALT_BYTES      16
#ifndef HQC_SEED_BYTES
#define HQC_SEED_BYTES  SEED_BYTES
#endif
#ifndef HQC_SALT_BYTES
#define HQC_SALT_BYTES  SALT_BYTES
#endif
#ifndef HQC_SHARED_SECRET_BYTES
#define HQC_SHARED_SECRET_BYTES HQC_MAX_BYTES
#endif

/* Sizes — compile-time maxima for stack/heap allocation */
#define HQC_MAX_VEC_N_SIZE_BYTES      CEIL_DIVIDE(HQC_MAX_N, 8)
#define HQC_MAX_VEC_N1N2_SIZE_BYTES   CEIL_DIVIDE(HQC_MAX_N1N2, 8)
#define HQC_MAX_VEC_N_SIZE_64         CEIL_DIVIDE(HQC_MAX_N, 64)
#define HQC_MAX_VEC_N1_SIZE_64        CEIL_DIVIDE(HQC_MAX_N1, 8)
#define HQC_MAX_VEC_N1N2_SIZE_64      CEIL_DIVIDE(HQC_MAX_N1N2, 64)

/* Run-time sizes (use these in algorithm code) */
#define VEC_K_SIZE_BYTES          HQC_active_params()->k
#define VEC_N1_SIZE_BYTES         HQC_active_params()->n1
#define VEC_N_SIZE_BYTES          CEIL_DIVIDE(HQC_active_params()->n, 8)
#define VEC_N1N2_SIZE_BYTES       CEIL_DIVIDE(HQC_active_params()->n1n2, 8)
#define VEC_N_SIZE_64             CEIL_DIVIDE(HQC_active_params()->n, 64)
#define VEC_N1_SIZE_64            CEIL_DIVIDE(HQC_active_params()->n1, 8)
#define VEC_N1N2_SIZE_64          CEIL_DIVIDE(HQC_active_params()->n1n2, 64)

#define PUBLIC_KEY_BYTES          HQC_active_params()->publickeybytes
#define SECRET_KEY_BYTES          HQC_active_params()->secretkeybytes
#define SHARED_SECRET_BYTES       HQC_active_params()->bytes
#define CIPHERTEXT_BYTES          HQC_active_params()->ciphertextbytes

#define HQC_MAX_PUBLIC_KEY_BYTES      HQC_MAX_PUBLICKEYBYTES
#define HQC_MAX_SECRET_KEY_BYTES      HQC_MAX_SECRETKEYBYTES
#define HQC_MAX_CIPHERTEXT_BYTES      HQC_MAX_CIPHERTEXTBYTES

/* Run-time parameter access */
const hqc_params_t *HQC_active_params(void);

/* Convenience macros (access via active params) */
#define PARAM_N             (HQC_active_params()->n)
#define PARAM_N1            (HQC_active_params()->n1)
#define PARAM_N2            (HQC_active_params()->n2)
#define PARAM_N1N2          (HQC_active_params()->n1n2)
#define PARAM_OMEGA         (HQC_active_params()->omega)
#define PARAM_OMEGA_E       (HQC_active_params()->omega_e)
#define PARAM_OMEGA_R       (HQC_active_params()->omega_r)
#define PARAM_SECURITY      (HQC_active_params()->security_bits)
#define PARAM_SECURITY_BYTES (HQC_active_params()->security_bytes)
#define PARAM_DELTA         (HQC_active_params()->delta)
#define PARAM_K             (HQC_active_params()->k)
#define PARAM_G             (HQC_active_params()->g)
#define PARAM_FFT           (HQC_active_params()->fft)
#define PARAM_DFR_EXP       (HQC_active_params()->security_bits)

/* Compile-time maxima for struct definitions and stack allocations */
#define HQC_MAX_VEC_N_SIZE_BYTES      CEIL_DIVIDE(HQC_MAX_N, 8)
#define HQC_MAX_VEC_N1N2_SIZE_BYTES   CEIL_DIVIDE(HQC_MAX_N1N2, 8)
#define HQC_MAX_VEC_N_SIZE_64         CEIL_DIVIDE(HQC_MAX_N, 64)
#define HQC_MAX_VEC_N1_SIZE_64        CEIL_DIVIDE(HQC_MAX_N1, 8)
#define HQC_MAX_VEC_N1N2_SIZE_64      CEIL_DIVIDE(HQC_MAX_N1N2, 64)
#define HQC_MAX_PUBLIC_KEY_BYTES      HQC_MAX_PUBLICKEYBYTES
#define HQC_MAX_SECRET_KEY_BYTES      HQC_MAX_SECRETKEYBYTES
#define HQC_MAX_CIPHERTEXT_BYTES      HQC_MAX_CIPHERTEXTBYTES

#endif
