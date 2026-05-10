#include "api.h"
#include "scloudplus_params.h"
#include "kem.h"

static const scloudplus_params_t scloudplus_params_table[] = {
    {
        "SCLOUDPLUS-128",
        8480,
        7216,
        16,
        5456,
        128,
        16,
        600,
        600,
        8,
        8,
        12,
        9,
        7,
        150,
        150,
        7,
        7,
        64,
        3,
        2,
        75,
        4,
        300,
        5400,
        56,
        5456,
        7216,
        1200,
        8480,
        360000ULL,
        216000000ULL,
        0,
        0,
        360000ULL,
        216000000ULL,
        0,
        0,
        679,
        582,
    },
    {
        "SCLOUDPLUS-192",
        13008,
        11152,
        24,
        10832,
        192,
        24,
        928,
        896,
        8,
        8,
        12,
        12,
        10,
        224,
        232,
        2,
        1,
        96,
        4,
        2,
        112,
        4,
        448,
        10752,
        80,
        10832,
        11152,
        1792,
        13008,
        861184ULL,
        799178752ULL,
        0,
        0,
        802816ULL,
        719323136ULL,
        0,
        0,
        671,
        488,
    },
    {
        "SCLOUDPLUS-256",
        21904,
        18760,
        32,
        16916,
        256,
        32,
        1136,
        1120,
        12,
        11,
        12,
        10,
        7,
        280,
        284,
        3,
        2,
        64,
        3,
        4,
        140,
        4,
        560,
        16800,
        116,
        16916,
        18760,
        3080,
        21904,
        1290496ULL,
        1466003456ULL,
        1665379926016ULL,
        1891871595954176ULL,
        1254400ULL,
        1404928000ULL,
        1573519360000ULL,
        1762341683200000ULL,
        680,
        530,
    },
};

const scloudplus_params_t *g_scloudplus_params = &scloudplus_params_table[SCLOUDPLUS_128];

const scloudplus_params_t *SCLOUDPLUS_get_params(scloudplus_level_t level) {
    if (level < SCLOUDPLUS_128 || level > SCLOUDPLUS_256) {
        return 0;
    }
    return &scloudplus_params_table[level];
}

int SCLOUDPLUS_select_level(scloudplus_level_t level) {
    const scloudplus_params_t *params = SCLOUDPLUS_get_params(level);
    if (params == 0) {
        return -1;
    }
    g_scloudplus_params = params;
    return 0;
}

static int crypto_kem_keypair_impl(uint8_t *pk, uint8_t *sk) {
    return scloud_kemkeygen(pk, sk);
}

static int crypto_kem_enc_impl(uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    return scloud_kemencaps((uint8_t *)pk, ct, ss);
}

static int crypto_kem_dec_impl(uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    return scloud_kemdecaps((uint8_t *)sk, (uint8_t *)ct, ss);
}

int crypto_kem_keypair(scloudplus_level_t level, uint8_t *pk, uint8_t *sk) {
    if (SCLOUDPLUS_select_level(level) != 0) {
        return -1;
    }
    return crypto_kem_keypair_impl(pk, sk);
}

int crypto_kem_enc(scloudplus_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    if (SCLOUDPLUS_select_level(level) != 0) {
        return -1;
    }
    return crypto_kem_enc_impl(ct, ss, pk);
}

int crypto_kem_dec(scloudplus_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    if (SCLOUDPLUS_select_level(level) != 0) {
        return -1;
    }
    return crypto_kem_dec_impl(ss, ct, sk);
}

int SCLOUDPLUS_crypto_kem_keypair(scloudplus_level_t level, uint8_t *pk, uint8_t *sk) {
    return crypto_kem_keypair(level, pk, sk);
}

int SCLOUDPLUS_crypto_kem_enc(scloudplus_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    return crypto_kem_enc(level, ct, ss, pk);
}

int SCLOUDPLUS_crypto_kem_dec(scloudplus_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    return crypto_kem_dec(level, ss, ct, sk);
}
