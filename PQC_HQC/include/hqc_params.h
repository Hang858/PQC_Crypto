#ifndef PQC_HQC_PARAMS_H
#define PQC_HQC_PARAMS_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    HQC_1 = 0,
    HQC_3 = 1,
    HQC_5 = 2
} hqc_level_t;

typedef struct {
    const char *algname;
    size_t secretkeybytes;
    size_t publickeybytes;
    size_t bytes;
    size_t ciphertextbytes;
    uint16_t seed_bytes;
    uint16_t salt_bytes;
    uint32_t n;
    uint32_t n1;
    uint32_t n2;
    uint32_t n1n2;
    uint32_t vec_n_size_bytes;
    uint32_t vec_k_size_bytes;
    uint32_t vec_n1_size_bytes;
    uint32_t vec_n1n2_size_bytes;
    uint16_t vec_n_size_64;
    uint16_t vec_n1_size_64;
    uint16_t vec_n1n2_size_64;
    uint16_t omega;
    uint16_t omega_e;
    uint16_t omega_r;
    uint16_t security_bytes;
    uint16_t delta;
    uint16_t k;
    uint16_t g;
    uint16_t fft;
    uint32_t security_bits;
    uint32_t n_mu;
    uint32_t rejection_threshold;
    const uint16_t *rs_poly;
    const uint16_t *alpha_ij_pow;
    uint16_t alpha_ij_cols;
} hqc_params_t;

#define HQC_SEED_BYTES 32u
#define HQC_SALT_BYTES 16u
#define HQC_SHARED_SECRET_BYTES 32u

#define HQC_MAX_N 57637u
#define HQC_MAX_N1 90u
#define HQC_MAX_N2 640u
#define HQC_MAX_N1N2 57600u
#define HQC_MAX_OMEGA_R 149u
#define HQC_MAX_SECURITY_BYTES 32u
#define HQC_MAX_DELTA 29u
#define HQC_MAX_K 32u
#define HQC_MAX_G 59u
#define HQC_MAX_FFT 5u
#define HQC_MAX_VEC_N_SIZE_BYTES 7205u
#define HQC_MAX_VEC_N1N2_SIZE_BYTES 7200u
#define HQC_MAX_VEC_N_SIZE_64 901u
#define HQC_MAX_VEC_N1_SIZE_64 12u
#define HQC_MAX_VEC_N1N2_SIZE_64 900u
#define HQC_MAX_PUBLIC_KEY_BYTES 7237u
#define HQC_MAX_SECRET_KEY_BYTES 7333u
#define HQC_MAX_CIPHERTEXT_BYTES 14421u

extern const hqc_params_t *g_hqc_params;

const hqc_params_t *HQC_get_params(hqc_level_t level);
int HQC_select_level(hqc_level_t level);

int HQC_crypto_kem_keypair(hqc_level_t level, uint8_t *pk, uint8_t *sk);
int HQC_crypto_kem_enc(hqc_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int HQC_crypto_kem_dec(hqc_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

int crypto_kem_keypair_impl(uint8_t *pk, uint8_t *sk);
int crypto_kem_enc_impl(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int crypto_kem_dec_impl(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
