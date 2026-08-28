#include "api.h"
#include "polarlac_params.h"

int32_t polarlac_crypto_encrypt_keypair(polarlac_level_t level,
                                        unsigned char *pk, unsigned char *sk)
{
    int32_t ret = polarlac_set_level(level);
    if (ret != 0) {
        return ret;
    }

    return crypto_encrypt_keypair(pk, sk);
}

int32_t polarlac_crypto_encrypt(polarlac_level_t level, unsigned char *c,
                                uint64_t *clen, const unsigned char *m,
                                uint64_t mlen, const unsigned char *pk,
                                unsigned char *seeds)
{
    int32_t ret = polarlac_set_level(level);
    if (ret != 0) {
        return ret;
    }

    return crypto_encrypt(c, clen, m, mlen, pk, seeds);
}

int32_t polarlac_crypto_encrypt_open(polarlac_level_t level, unsigned char *m,
                                     uint64_t *mlen, const unsigned char *c,
                                     uint64_t clen, const unsigned char *sk)
{
    int32_t ret = polarlac_set_level(level);
    if (ret != 0) {
        return ret;
    }

    return crypto_encrypt_open(m, mlen, c, clen, sk);
}

int32_t polarlac_crypto_kem_keypair(polarlac_level_t level,
                                    unsigned char *pk, unsigned char *sk)
{
    int32_t ret = polarlac_set_level(level);
    if (ret != 0) {
        return ret;
    }

    return crypto_kem_keypair(pk, sk);
}

int32_t polarlac_crypto_kem_enc(polarlac_level_t level, unsigned char *ct,
                                unsigned char *ss, const unsigned char *pk)
{
    int32_t ret = polarlac_set_level(level);
    if (ret != 0) {
        return ret;
    }

    return crypto_kem_enc(ct, ss, pk);
}

int32_t polarlac_crypto_kem_dec(polarlac_level_t level, unsigned char *ss,
                                const unsigned char *ct,
                                const unsigned char *sk)
{
    int32_t ret = polarlac_set_level(level);
    if (ret != 0) {
        return ret;
    }

    return crypto_kem_dec(ss, ct, sk);
}
