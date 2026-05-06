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
    uint32_t vec_n1n2_size_bytes;
    uint16_t vec_n_size_64;
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
} hqc_params_t;

const hqc_params_t *HQC_get_params(hqc_level_t level);

#endif
