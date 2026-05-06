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
    size_t secretkeybytes;
    size_t publickeybytes;
    size_t bytes;
    size_t ciphertextbytes;
    uint16_t l;
    uint16_t ss;
    uint16_t m;
    uint16_t n;
    uint16_t mbar;
    uint16_t nbar;
    uint16_t logq;
    uint16_t logq1;
    uint16_t logq2;
    uint16_t h1;
    uint16_t h2;
    uint16_t eta1;
    uint16_t eta2;
    uint16_t mu;
    uint16_t tau;
    uint16_t subm;
    uint16_t block_number;
    uint16_t block_size;
    uint16_t block_rowlen;
    uint16_t c1;
    uint16_t c2;
    uint16_t ctx;
    uint16_t pk;
    uint16_t pke_sk;
    uint16_t kem_sk;
    uint64_t m2;
    uint64_t m3;
    uint64_t m4;
    uint64_t m5;
    uint64_t n2;
    uint64_t n3;
    uint64_t n4;
    uint64_t n5;
    uint16_t mnin;
    uint16_t mnout;
} scloudplus_params_t;

#define SCLOUDPLUS_MAX_SECRET_KEY_BYTES 21904u
#define SCLOUDPLUS_MAX_PUBLIC_KEY_BYTES 18760u
#define SCLOUDPLUS_MAX_SHARED_SECRET_BYTES 32u
#define SCLOUDPLUS_MAX_CIPHERTEXT_BYTES 16916u

extern const scloudplus_params_t *g_scloudplus_params;

const scloudplus_params_t *SCLOUDPLUS_get_params(scloudplus_level_t level);
int SCLOUDPLUS_select_level(scloudplus_level_t level);

int SCLOUDPLUS_crypto_kem_keypair(scloudplus_level_t level, uint8_t *pk, uint8_t *sk);
int SCLOUDPLUS_crypto_kem_enc(scloudplus_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int SCLOUDPLUS_crypto_kem_dec(scloudplus_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
