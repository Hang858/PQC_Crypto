#ifndef PQC_SCLOUDPLUS_PARAMS_H
#define PQC_SCLOUDPLUS_PARAMS_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    SCLOUDPLUS_128 = 0,
    SCLOUDPLUS_192 = 1,
    SCLOUDPLUS_256 = 2
} scloudplus_level_t;

typedef struct {
    const char *algname;
    size_t publickeybytes;
    size_t secretkeybytes;
    size_t ciphertextbytes;
    size_t bytes;
    uint16_t level_bits;
    uint16_t ss;
    uint16_t m;
    uint16_t n;
    uint8_t mbar;
    uint8_t nbar;
    uint8_t logq;
    uint8_t logq1;
    uint8_t logq2;
    uint16_t h1;
    uint16_t h2;
    uint8_t eta1;
    uint8_t eta2;
    uint8_t mu;
    uint8_t tau;
    uint8_t subm;
    uint16_t block_number;
    uint8_t block_size;
    uint16_t block_rowlen;
    size_t c1;
    size_t c2;
    size_t pke_secretkeybytes;
} scloudplus_params_t;

const scloudplus_params_t *SCLOUDPLUS_get_params(scloudplus_level_t level);

#endif
