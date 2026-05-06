#include "api.h"

int frodokem640_crypto_kem_keypair_enc(uint8_t *ct, uint8_t *ss, uint8_t *pk, uint8_t *sk);
int frodokem640_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
int frodokem976_crypto_kem_keypair_enc(uint8_t *ct, uint8_t *ss, uint8_t *pk, uint8_t *sk);
int frodokem976_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
int frodokem1344_crypto_kem_keypair_enc(uint8_t *ct, uint8_t *ss, uint8_t *pk, uint8_t *sk);
int frodokem1344_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

static const frodokem_params_t FRODOKEM_PARAM_TABLE[] = {
    {"eFrodoKEM-640", 9616, 19888, 9720, 16, 640, 8, 15, 2, 8, 4, 16, 16, 16},
    {"eFrodoKEM-976", 15632, 31296, 15744, 24, 976, 8, 16, 3, 8, 4, 16, 24, 24},
    {"eFrodoKEM-1344", 21520, 43088, 21632, 32, 1344, 8, 16, 4, 8, 4, 16, 32, 32},
};

const frodokem_params_t *FRODOKEM_get_params(frodokem_level_t level)
{
    if ((unsigned)level > (unsigned)FRODOKEM_1344) {
        return 0;
    }
    return &FRODOKEM_PARAM_TABLE[level];
}

int FRODOKEM_crypto_kem_keypair_enc(frodokem_level_t level, uint8_t *ct, uint8_t *ss, uint8_t *pk, uint8_t *sk)
{
    switch (level) {
    case FRODOKEM_640:
        return frodokem640_crypto_kem_keypair_enc(ct, ss, pk, sk);
    case FRODOKEM_976:
        return frodokem976_crypto_kem_keypair_enc(ct, ss, pk, sk);
    case FRODOKEM_1344:
        return frodokem1344_crypto_kem_keypair_enc(ct, ss, pk, sk);
    default:
        return -1;
    }
}

int FRODOKEM_crypto_kem_dec(frodokem_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk)
{
    switch (level) {
    case FRODOKEM_640:
        return frodokem640_crypto_kem_dec(ss, ct, sk);
    case FRODOKEM_976:
        return frodokem976_crypto_kem_dec(ss, ct, sk);
    case FRODOKEM_1344:
        return frodokem1344_crypto_kem_dec(ss, ct, sk);
    default:
        return -1;
    }
}
