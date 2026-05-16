/*
 * Runtime-level NIST API wrapper for the Falcon FPU implementation.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "api.h"
#include "inner.h"

#define NONCELEN 40
#define TEMPALLOC

void randombytes_init(unsigned char *entropy_input,
    unsigned char *personalization_string,
    int security_strength);
int randombytes(unsigned char *x, unsigned long long xlen);

int crypto_sign_keypair(falcon_level_t level, unsigned char *pk, unsigned char *sk) {
    const falcon_params_t *params = Falcon_get_params(level);
    TEMPALLOC union {
        uint8_t b[FALCON_KEYGEN_TEMP_10];
        uint64_t dummy_u64;
        fpr dummy_fpr;
    } tmp;
    TEMPALLOC int8_t f[FALCON_MAX_N], g[FALCON_MAX_N], F[FALCON_MAX_N];
    TEMPALLOC uint16_t h[FALCON_MAX_N];
    TEMPALLOC unsigned char seed[48];
    TEMPALLOC inner_shake256_context rng;
    size_t u, v;
    unsigned savcw;

    if (params == NULL) {
        return -1;
    }

    savcw = set_fpu_cw(2);
    randombytes(seed, sizeof seed);
    inner_shake256_init(&rng);
    inner_shake256_inject(&rng, seed, sizeof seed);
    inner_shake256_flip(&rng);
    Zf(keygen)(&rng, f, g, F, NULL, h, params->logn, tmp.b);
    set_fpu_cw(savcw);

    sk[0] = (unsigned char)(0x50 + params->logn);
    u = 1;
    v = Zf(trim_i8_encode)(sk + u, params->secretkeybytes - u,
        f, params->logn, Zf(max_fg_bits)[params->logn]);
    if (v == 0) {
        return -1;
    }
    u += v;
    v = Zf(trim_i8_encode)(sk + u, params->secretkeybytes - u,
        g, params->logn, Zf(max_fg_bits)[params->logn]);
    if (v == 0) {
        return -1;
    }
    u += v;
    v = Zf(trim_i8_encode)(sk + u, params->secretkeybytes - u,
        F, params->logn, Zf(max_FG_bits)[params->logn]);
    if (v == 0) {
        return -1;
    }
    u += v;
    if (u != params->secretkeybytes) {
        return -1;
    }

    pk[0] = (unsigned char)(0x00 + params->logn);
    v = Zf(modq_encode)(pk + 1, params->publickeybytes - 1, h, params->logn);
    if (v != params->publickeybytes - 1) {
        return -1;
    }

    return 0;
}

int crypto_sign(falcon_level_t level, unsigned char *sm, unsigned long long *smlen,
    const unsigned char *m, unsigned long long mlen, const unsigned char *sk) {
    const falcon_params_t *params = Falcon_get_params(level);
    TEMPALLOC union {
        uint8_t b[72 * FALCON_MAX_N];
        uint64_t dummy_u64;
        fpr dummy_fpr;
    } tmp;
    TEMPALLOC int8_t f[FALCON_MAX_N], g[FALCON_MAX_N], F[FALCON_MAX_N], G[FALCON_MAX_N];
    TEMPALLOC union {
        int16_t sig[FALCON_MAX_N];
        uint16_t hm[FALCON_MAX_N];
    } r;
    TEMPALLOC unsigned char seed[48], nonce[NONCELEN];
    TEMPALLOC unsigned char esig[FALCON_MAX_BYTES - 2 - NONCELEN];
    TEMPALLOC inner_shake256_context sc;
    size_t u, v, sig_len;
    unsigned savcw;

    if (params == NULL) {
        return -1;
    }

    if (sk[0] != (unsigned char)(0x50 + params->logn)) {
        return -1;
    }
    u = 1;
    v = Zf(trim_i8_decode)(f, params->logn, Zf(max_fg_bits)[params->logn],
        sk + u, params->secretkeybytes - u);
    if (v == 0) {
        return -1;
    }
    u += v;
    v = Zf(trim_i8_decode)(g, params->logn, Zf(max_fg_bits)[params->logn],
        sk + u, params->secretkeybytes - u);
    if (v == 0) {
        return -1;
    }
    u += v;
    v = Zf(trim_i8_decode)(F, params->logn, Zf(max_FG_bits)[params->logn],
        sk + u, params->secretkeybytes - u);
    if (v == 0) {
        return -1;
    }
    u += v;
    if (u != params->secretkeybytes) {
        return -1;
    }
    if (!Zf(complete_private)(G, f, g, F, params->logn, tmp.b)) {
        return -1;
    }

    randombytes(nonce, sizeof nonce);

    inner_shake256_init(&sc);
    inner_shake256_inject(&sc, nonce, sizeof nonce);
    inner_shake256_inject(&sc, m, mlen);
    inner_shake256_flip(&sc);
    Zf(hash_to_point_vartime)(&sc, r.hm, params->logn);

    randombytes(seed, sizeof seed);
    inner_shake256_init(&sc);
    inner_shake256_inject(&sc, seed, sizeof seed);
    inner_shake256_flip(&sc);

    savcw = set_fpu_cw(2);
    Zf(sign_dyn)(r.sig, &sc, f, g, F, G, r.hm, params->logn, tmp.b);
    set_fpu_cw(savcw);

    esig[0] = (unsigned char)(0x20 + params->logn);
    sig_len = Zf(comp_encode)(esig + 1, params->bytes - 2 - NONCELEN - 1,
        r.sig, params->logn);
    if (sig_len == 0) {
        return -1;
    }
    sig_len++;
    memmove(sm + 2 + NONCELEN, m, mlen);
    sm[0] = (unsigned char)(sig_len >> 8);
    sm[1] = (unsigned char)sig_len;
    memcpy(sm + 2, nonce, sizeof nonce);
    memcpy(sm + 2 + NONCELEN + mlen, esig, sig_len);
    *smlen = 2 + NONCELEN + mlen + sig_len;
    return 0;
}

int crypto_sign_open(falcon_level_t level, unsigned char *m, unsigned long long *mlen,
    const unsigned char *sm, unsigned long long smlen, const unsigned char *pk) {
    const falcon_params_t *params = Falcon_get_params(level);
    TEMPALLOC union {
        uint8_t b[2 * FALCON_MAX_N];
        uint64_t dummy_u64;
        fpr dummy_fpr;
    } tmp;
    const unsigned char *esig;
    TEMPALLOC uint16_t h[FALCON_MAX_N], hm[FALCON_MAX_N];
    TEMPALLOC int16_t sig[FALCON_MAX_N];
    TEMPALLOC inner_shake256_context sc;
    size_t sig_len, msg_len;

    if (params == NULL) {
        return -1;
    }

    if (pk[0] != (unsigned char)(0x00 + params->logn)) {
        return -1;
    }
    if (Zf(modq_decode)(h, params->logn, pk + 1, params->publickeybytes - 1)
        != params->publickeybytes - 1)
    {
        return -1;
    }
    Zf(to_ntt_monty)(h, params->logn);

    if (smlen < 2 + NONCELEN) {
        return -1;
    }
    sig_len = ((size_t)sm[0] << 8) | (size_t)sm[1];
    if (sig_len > (smlen - 2 - NONCELEN)) {
        return -1;
    }
    msg_len = smlen - 2 - NONCELEN - sig_len;

    esig = sm + 2 + NONCELEN + msg_len;
    if (sig_len < 1 || esig[0] != (unsigned char)(0x20 + params->logn)) {
        return -1;
    }
    if (Zf(comp_decode)(sig, params->logn,
        esig + 1, sig_len - 1) != sig_len - 1)
    {
        return -1;
    }

    inner_shake256_init(&sc);
    inner_shake256_inject(&sc, sm + 2, NONCELEN + msg_len);
    inner_shake256_flip(&sc);
    Zf(hash_to_point_vartime)(&sc, hm, params->logn);

    if (!Zf(verify_raw)(hm, sig, h, params->logn, tmp.b)) {
        return -1;
    }

    memmove(m, sm + 2 + NONCELEN, msg_len);
    *mlen = msg_len;
    return 0;
}
