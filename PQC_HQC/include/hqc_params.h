#ifndef PQC_HQC_PARAMS_H
#define PQC_HQC_PARAMS_H

#include <stdint.h>

#define HQC_SEED_BYTES 32u
#define HQC_SALT_BYTES 16u
#define HQC_SHARED_SECRET_BYTES 32u

typedef enum {
    HQC_1 = 0,
    HQC_3 = 1,
    HQC_5 = 2
} hqc_level_t;

typedef struct {
    const char *algname;
    unsigned int secretkeybytes;
    unsigned int publickeybytes;
    unsigned int bytes;
    unsigned int ciphertextbytes;
    unsigned int n;
    unsigned int n1;
    unsigned int n2;
    unsigned int n1n2;
    unsigned int k;
    unsigned int g;
    unsigned int delta;
    unsigned int omega;
    unsigned int omega_e;
    unsigned int omega_r;
    unsigned int fft;
    unsigned int security_bits;
    unsigned int security_bytes;
    const uint16_t *rs_poly;
    uint64_t params_n_mu;
    uint32_t rejection_threshold;
} hqc_params_t;

/* Max sizes across all levels (HQC-5 is the largest) */
#define HQC_MAX_SECRETKEYBYTES  7333u
#define HQC_MAX_PUBLICKEYBYTES  7237u
#define HQC_MAX_BYTES           32u
#define HQC_MAX_CIPHERTEXTBYTES 14421u
#define HQC_MAX_N               57637u
#define HQC_MAX_N1              90u
#define HQC_MAX_N2              640u
#define HQC_MAX_N1N2            57600u
#define HQC_MAX_K               32u
#define HQC_MAX_G               59u
#define HQC_MAX_DELTA           29u
#define HQC_MAX_OMEGA_E         149u
#define HQC_MAX_OMEGA_R         149u
#define HQC_MAX_FFT             5u
#define HQC_MAX_SECURITY_BYTES  32u

const hqc_params_t *HQC_get_params(hqc_level_t level);
int HQC_select_level(hqc_level_t level);

int HQC_crypto_kem_keypair(hqc_level_t level, unsigned char *pk, unsigned char *sk);
int HQC_crypto_kem_enc(hqc_level_t level, unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int HQC_crypto_kem_dec(hqc_level_t level, unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#endif
