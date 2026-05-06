#include "api.h"

int hqc1_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int hqc1_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int hqc1_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

int hqc3_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int hqc3_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int hqc3_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

int hqc5_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int hqc5_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int hqc5_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

static const hqc_params_t HQC_PARAM_TABLE[] = {
    {"HQC-1", 2321, 2241, 32, 4433, 32, 16, 17669, 46, 384, 17664, 2209, 2208, 277, 276, 66, 75, 75, 16, 15, 16, 31, 4, 128},
    {"HQC-3", 4602, 4514, 32, 8978, 32, 16, 35851, 56, 640, 35840, 4482, 4480, 561, 560, 100, 114, 114, 24, 16, 24, 33, 5, 192},
    {"HQC-5", 7333, 7237, 32, 14421, 32, 16, 57637, 90, 640, 57600, 7205, 7200, 901, 900, 131, 149, 149, 32, 29, 32, 59, 5, 256},
};

const hqc_params_t *HQC_get_params(hqc_level_t level) {
    if ((unsigned)level > (unsigned)HQC_5) {
        return NULL;
    }
    return &HQC_PARAM_TABLE[level];
}

int HQC_crypto_kem_keypair(hqc_level_t level, uint8_t *pk, uint8_t *sk) {
    switch (level) {
        case HQC_1: return hqc1_crypto_kem_keypair(pk, sk);
        case HQC_3: return hqc3_crypto_kem_keypair(pk, sk);
        case HQC_5: return hqc5_crypto_kem_keypair(pk, sk);
        default: return -1;
    }
}

int HQC_crypto_kem_enc(hqc_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    switch (level) {
        case HQC_1: return hqc1_crypto_kem_enc(ct, ss, pk);
        case HQC_3: return hqc3_crypto_kem_enc(ct, ss, pk);
        case HQC_5: return hqc5_crypto_kem_enc(ct, ss, pk);
        default: return -1;
    }
}

int HQC_crypto_kem_dec(hqc_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    switch (level) {
        case HQC_1: return hqc1_crypto_kem_dec(ss, ct, sk);
        case HQC_3: return hqc3_crypto_kem_dec(ss, ct, sk);
        case HQC_5: return hqc5_crypto_kem_dec(ss, ct, sk);
        default: return -1;
    }
}
