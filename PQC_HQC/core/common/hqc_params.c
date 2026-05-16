#include "hqc_params.h"
#include "api.h"

static const hqc_params_t HQC_PARAM = {
    CRYPTO_ALGNAME,
    CRYPTO_SECRETKEYBYTES,
    CRYPTO_PUBLICKEYBYTES,
    CRYPTO_BYTES,
    CRYPTO_CIPHERTEXTBYTES,
    17669,
    46,
    384,
    17664,
    16,
    128,
};

const hqc_params_t *HQC_get_params(hqc_level_t level) {
    if (level != HQC_1) {
        return 0;
    }
    return &HQC_PARAM;
}

int HQC_select_level(hqc_level_t level) {
    return level == HQC_1 ? 0 : -1;
}

int HQC_crypto_kem_keypair(hqc_level_t level, unsigned char *pk, unsigned char *sk) {
    if (HQC_select_level(level) != 0) {
        return -1;
    }
    return crypto_kem_keypair(pk, sk);
}

int HQC_crypto_kem_enc(hqc_level_t level, unsigned char *ct, unsigned char *ss, const unsigned char *pk) {
    if (HQC_select_level(level) != 0) {
        return -1;
    }
    return crypto_kem_enc(ct, ss, pk);
}

int HQC_crypto_kem_dec(hqc_level_t level, unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    if (HQC_select_level(level) != 0) {
        return -1;
    }
    return crypto_kem_dec(ss, ct, sk);
}
