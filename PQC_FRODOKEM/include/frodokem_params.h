#ifndef PQC_FRODOKEM_PARAMS_H
#define PQC_FRODOKEM_PARAMS_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    FRODOKEM_640 = 0,
    FRODOKEM_976 = 1,
    FRODOKEM_1344 = 2
} frodokem_level_t;

typedef void (*frodokem_shake_fn)(uint8_t *output, size_t outlen, const uint8_t *input, size_t inlen);

typedef struct {
    const char *algname;
    size_t secretkeybytes;
    size_t publickeybytes;
    size_t bytes;
    size_t ciphertextbytes;
    uint16_t n;
    uint16_t nbar;
    uint16_t logq;
    uint32_t q;
    uint16_t extracted_bits;
    uint16_t stripe_step;
    uint16_t bytes_seed_a;
    uint16_t bytes_mu;
    uint16_t bytes_salt;
    uint16_t bytes_seed_se;
    uint16_t bytes_pkhash;
    const uint16_t *cdf_table;
    uint16_t cdf_table_len;
    frodokem_shake_fn shake;
} frodokem_params_t;

#define FRODOKEM_MAX_SECRET_KEY_BYTES 43088u
#define FRODOKEM_MAX_PUBLIC_KEY_BYTES 21520u
#define FRODOKEM_MAX_SHARED_SECRET_BYTES 32u
#define FRODOKEM_MAX_CIPHERTEXT_BYTES 21696u
#define FRODOKEM_MAX_N 1344u
#define FRODOKEM_NBAR 8u
#define FRODOKEM_BYTES_SEED_A 16u

extern const frodokem_params_t *g_frodokem_params;

const frodokem_params_t *FRODOKEM_get_params(frodokem_level_t level);
int FRODOKEM_select_level(frodokem_level_t level);

int FRODOKEM_crypto_kem_keypair(frodokem_level_t level, uint8_t *pk, uint8_t *sk);
int FRODOKEM_crypto_kem_enc(frodokem_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int FRODOKEM_crypto_kem_dec(frodokem_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
int FRODOKEM_crypto_kem_keypair_enc(frodokem_level_t level, uint8_t *ct, uint8_t *ss, uint8_t *pk, uint8_t *sk);

int crypto_kem_keypair_impl(uint8_t *pk, uint8_t *sk);
int crypto_kem_enc_impl(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int crypto_kem_dec_impl(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
