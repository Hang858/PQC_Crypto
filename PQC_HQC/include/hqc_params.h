#ifndef PQC_HQC_PARAMS_H
#define PQC_HQC_PARAMS_H

#define HQC_SEED_BYTES 32u
#define HQC_SALT_BYTES 16u
#define HQC_SHARED_SECRET_BYTES 32u

typedef enum {
    HQC_1 = 0
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
    unsigned int security_bits;
} hqc_params_t;

const hqc_params_t *HQC_get_params(hqc_level_t level);
int HQC_select_level(hqc_level_t level);

int HQC_crypto_kem_keypair(hqc_level_t level, unsigned char *pk, unsigned char *sk);
int HQC_crypto_kem_enc(hqc_level_t level, unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int HQC_crypto_kem_dec(hqc_level_t level, unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#endif
