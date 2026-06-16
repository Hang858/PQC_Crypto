#include "hqc_params.h"
#include "api.h"

/* RS generator polynomial coefficients for each level */
static const uint16_t RS_POLY_1[] = {
    89, 69, 153, 116, 176, 117, 111, 75, 73, 233, 242, 233, 65, 210, 21, 139,
    103, 173, 67, 118, 105, 210, 174, 110, 74, 69, 228, 82, 255, 181, 1
};

static const uint16_t RS_POLY_3[] = {
    45, 216, 239, 24, 253, 104, 27, 40, 107, 50, 163, 210, 227, 134, 224, 158,
    119, 13, 158, 1, 238, 164, 82, 43, 15, 232, 246, 142, 50, 189, 29, 232, 1
};

static const uint16_t RS_POLY_5[] = {
    49, 167, 49, 39, 200, 121, 124, 91, 240, 63, 148, 71, 150, 123, 87, 101,
    32, 215, 159, 71, 201, 115, 97, 210, 186, 183, 141, 217, 123, 12, 31, 243,
    180, 219, 152, 239, 99, 141, 4, 246, 191, 144, 8, 232, 47, 27, 141, 178,
    130, 64, 124, 47, 39, 188, 216, 48, 199, 187, 1
};

static const hqc_params_t HQC_PARAM_TABLE[] = {
    {   /* HQC-1 (128-bit) */
        "HQC-1", 2321, 2241, 32, 4433,
        17669, 46, 384, 17664, 16, 31, 15, 66, 75, 75, 4, 128, 16,
        RS_POLY_1, 243079ULL, 16767881
    },
    {   /* HQC-3 (192-bit) */
        "HQC-3", 4602, 4514, 32, 8978,
        35851, 56, 640, 35840, 24, 33, 16, 100, 114, 114, 5, 192, 24,
        RS_POLY_3, 119800ULL, 16742417
    },
    {   /* HQC-5 (256-bit) */
        "HQC-5", 7333, 7237, 32, 14421,
        57637, 90, 640, 57600, 32, 59, 29, 131, 149, 149, 5, 256, 32,
        RS_POLY_5, 74517ULL, 16772367
    }
};

static const hqc_params_t *active_params = &HQC_PARAM_TABLE[0];

const hqc_params_t *HQC_get_params(hqc_level_t level) {
    if ((unsigned)level > (unsigned)HQC_5) {
        return 0;
    }
    return &HQC_PARAM_TABLE[level];
}

int HQC_select_level(hqc_level_t level) {
    if ((unsigned)level > (unsigned)HQC_5) {
        return -1;
    }
    active_params = &HQC_PARAM_TABLE[level];
    return 0;
}

const hqc_params_t *HQC_active_params(void) {
    return active_params;
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
