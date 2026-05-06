#include "frodokem_params.h"
#include "api.h"
#include "fips202.h"

static void frodokem_shake128(uint8_t *output, size_t outlen, const uint8_t *input, size_t inlen) {
    shake128(output, outlen, input, inlen);
}

static void frodokem_shake256(uint8_t *output, size_t outlen, const uint8_t *input, size_t inlen) {
    shake256(output, outlen, input, inlen);
}

static const uint16_t cdf_table_640[13] = {
    4643, 13363, 20579, 25843, 29227, 31145, 32103, 32525, 32689, 32745, 32762, 32766, 32767
};

static const uint16_t cdf_table_976[11] = {
    5638, 15915, 23689, 28571, 31116, 32217, 32613, 32731, 32760, 32766, 32767
};

static const uint16_t cdf_table_1344[7] = {
    9142, 23462, 30338, 32361, 32725, 32765, 32767
};

static const frodokem_params_t frodokem_params_table[] = {
    {
        "FrodoKEM-640",
        19888,
        9616,
        16,
        9752,
        640,
        8,
        15,
        1u << 15,
        2,
        8,
        16,
        16,
        32,
        32,
        16,
        cdf_table_640,
        13,
        frodokem_shake128,
    },
    {
        "FrodoKEM-976",
        31296,
        15632,
        24,
        15792,
        976,
        8,
        16,
        1u << 16,
        3,
        8,
        16,
        24,
        48,
        48,
        24,
        cdf_table_976,
        11,
        frodokem_shake256,
    },
    {
        "FrodoKEM-1344",
        43088,
        21520,
        32,
        21696,
        1344,
        8,
        16,
        1u << 16,
        4,
        8,
        16,
        32,
        64,
        64,
        32,
        cdf_table_1344,
        7,
        frodokem_shake256,
    },
};

const frodokem_params_t *g_frodokem_params = &frodokem_params_table[FRODOKEM_640];

const frodokem_params_t *FRODOKEM_get_params(frodokem_level_t level) {
    if (level < FRODOKEM_640 || level > FRODOKEM_1344) {
        return NULL;
    }
    return &frodokem_params_table[level];
}

int FRODOKEM_select_level(frodokem_level_t level) {
    const frodokem_params_t *params = FRODOKEM_get_params(level);
    if (params == NULL) {
        return -1;
    }
    g_frodokem_params = params;
    return 0;
}

void frodokem_shake(uint8_t *output, size_t outlen, const uint8_t *input, size_t inlen) {
    g_frodokem_params->shake(output, outlen, input, inlen);
}

int FRODOKEM_crypto_kem_keypair(frodokem_level_t level, uint8_t *pk, uint8_t *sk) {
    if (FRODOKEM_select_level(level) != 0) {
        return -1;
    }
    return crypto_kem_keypair(pk, sk);
}

int FRODOKEM_crypto_kem_enc(frodokem_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    if (FRODOKEM_select_level(level) != 0) {
        return -1;
    }
    return crypto_kem_enc(ct, ss, pk);
}

int FRODOKEM_crypto_kem_dec(frodokem_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    if (FRODOKEM_select_level(level) != 0) {
        return -1;
    }
    return crypto_kem_dec(ss, ct, sk);
}

int FRODOKEM_crypto_kem_keypair_enc(frodokem_level_t level, uint8_t *ct, uint8_t *ss, uint8_t *pk, uint8_t *sk) {
    if (FRODOKEM_crypto_kem_keypair(level, pk, sk) != 0) {
        return -1;
    }
    return FRODOKEM_crypto_kem_enc(level, ct, ss, pk);
}
