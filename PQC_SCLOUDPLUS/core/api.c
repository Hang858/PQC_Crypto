#include "api.h"

int scloudplus128_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int scloudplus128_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int scloudplus128_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

int scloudplus192_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int scloudplus192_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int scloudplus192_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

int scloudplus256_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int scloudplus256_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int scloudplus256_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

static const scloudplus_params_t SCLOUDPLUS_PARAM_TABLE[] = {
    {"SCLOUD+-128", 7216, 8480, 5456, 16, 128, 16, 600, 600, 8, 8, 12, 9, 7, 150, 150, 7, 7, 64, 3, 2, 75, 4, 300, 5400, 56, 1200},
    {"SCLOUD+-192", 11152, 13008, 10832, 24, 192, 24, 928, 896, 8, 8, 12, 12, 10, 224, 232, 2, 1, 96, 4, 2, 112, 4, 448, 10752, 80, 1792},
    {"SCLOUD+-256", 18760, 21904, 16916, 32, 256, 32, 1136, 1120, 12, 11, 12, 10, 7, 280, 284, 3, 2, 64, 3, 4, 140, 4, 560, 16800, 116, 3080},
};

const scloudplus_params_t *SCLOUDPLUS_get_params(scloudplus_level_t level) {
    if ((unsigned)level > (unsigned)SCLOUDPLUS_256) {
        return NULL;
    }
    return &SCLOUDPLUS_PARAM_TABLE[level];
}

int SCLOUDPLUS_crypto_kem_keypair(scloudplus_level_t level, uint8_t *pk, uint8_t *sk) {
    switch (level) {
        case SCLOUDPLUS_128: return scloudplus128_crypto_kem_keypair(pk, sk);
        case SCLOUDPLUS_192: return scloudplus192_crypto_kem_keypair(pk, sk);
        case SCLOUDPLUS_256: return scloudplus256_crypto_kem_keypair(pk, sk);
        default: return -1;
    }
}

int SCLOUDPLUS_crypto_kem_enc(scloudplus_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    switch (level) {
        case SCLOUDPLUS_128: return scloudplus128_crypto_kem_enc(ct, ss, pk);
        case SCLOUDPLUS_192: return scloudplus192_crypto_kem_enc(ct, ss, pk);
        case SCLOUDPLUS_256: return scloudplus256_crypto_kem_enc(ct, ss, pk);
        default: return -1;
    }
}

int SCLOUDPLUS_crypto_kem_dec(scloudplus_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    switch (level) {
        case SCLOUDPLUS_128: return scloudplus128_crypto_kem_dec(ss, ct, sk);
        case SCLOUDPLUS_192: return scloudplus192_crypto_kem_dec(ss, ct, sk);
        case SCLOUDPLUS_256: return scloudplus256_crypto_kem_dec(ss, ct, sk);
        default: return -1;
    }
}
