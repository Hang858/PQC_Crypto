#ifndef PQC_FRODOKEM_PARAMS_H
#define PQC_FRODOKEM_PARAMS_H

#include <stdint.h>

typedef enum {
    FRODOKEM_640 = 0,
    FRODOKEM_976 = 1,
    FRODOKEM_1344 = 2
} frodokem_level_t;

typedef struct {
    const char *name;
    uint32_t public_key_bytes;
    uint32_t secret_key_bytes;
    uint32_t ciphertext_bytes;
    uint32_t shared_secret_bytes;
    uint16_t n;
    uint16_t nbar;
    uint16_t logq;
    uint16_t extracted_bits;
    uint16_t stripe_step;
    uint16_t parallel;
    uint16_t bytes_seed_a;
    uint16_t bytes_mu;
    uint16_t bytes_pkhash;
} frodokem_params_t;

const frodokem_params_t *FRODOKEM_get_params(frodokem_level_t level);

#endif
