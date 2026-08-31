#ifndef PQC_FRODOKEM_API_H
#define PQC_FRODOKEM_API_H

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

int FRODOKEM_gen_keypair(frodokem_level_t level, uint8_t *pk, uint8_t *sk);
int FRODOKEM_enc(frodokem_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int FRODOKEM_dec(frodokem_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
