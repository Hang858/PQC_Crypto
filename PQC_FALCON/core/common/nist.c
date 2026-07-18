/*
 * Runtime-level NIST API wrapper for the Falcon FPU implementation.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "inner.h"

#define NONCELEN 40

/*
 * The NIST API is used on targets with a small task stack.  In particular,
 * Falcon-1024 signing needs a 72*N-byte temporary buffer, which is much too
 * large for such a stack.  Keep only scalar state and SHAKE contexts on the
 * stack; all polynomial and temporary work areas are allocated from the heap.
 */
static size_t
falcon_keygen_tmp_size(unsigned logn)
{
    switch (logn) {
    case 9:
        return FALCON_KEYGEN_TEMP_9;
    case 10:
        return FALCON_KEYGEN_TEMP_10;
    default:
        return 0;
    }
}

static void
falcon_free_sensitive(void *ptr, size_t len)
{
    if (ptr != NULL) {
        volatile uint8_t *p = ptr;

        /* Do not let the compiler elide the clearing before free(). */
        while (len -- > 0) {
            *p ++ = 0;
        }
        free(ptr);
    }
}

void randombytes_init(unsigned char *entropy_input,
    unsigned char *personalization_string,
    int security_strength);
int randombytes(unsigned char *x, unsigned long long xlen);

int crypto_sign_keypair(falcon_level_t level, unsigned char *pk, unsigned char *sk) {
    const falcon_params_t *params = Falcon_get_params(level);
    uint8_t *tmp = NULL;
    int8_t *fgF = NULL;
    uint16_t *h = NULL;
    unsigned char seed[48];
    inner_shake256_context rng;
    size_t n, tmp_len;
    size_t u, v;
    unsigned savcw;
    int ret = -1;

    if (params == NULL) {
        return -1;
    }
    n = (size_t)1 << params->logn;
    tmp_len = falcon_keygen_tmp_size(params->logn);
    if (tmp_len == 0) {
        return -1;
    }
    tmp = malloc(tmp_len);
    fgF = malloc(3 * n);
    h = malloc(n * sizeof *h);
    if (tmp == NULL || fgF == NULL || h == NULL) {
        goto cleanup;
    }

    savcw = set_fpu_cw(2);
    randombytes(seed, sizeof seed);
    inner_shake256_init(&rng);
    inner_shake256_inject(&rng, seed, sizeof seed);
    inner_shake256_flip(&rng);
    Zf(keygen)(&rng, fgF, fgF + n, fgF + (2 * n), NULL, h,
        params->logn, tmp);
    set_fpu_cw(savcw);

    sk[0] = (unsigned char)(0x50 + params->logn);
    u = 1;
    v = Zf(trim_i8_encode)(sk + u, params->secretkeybytes - u,
        fgF, params->logn, Zf(max_fg_bits)[params->logn]);
    if (v == 0) {
        goto cleanup;
    }
    u += v;
    v = Zf(trim_i8_encode)(sk + u, params->secretkeybytes - u,
        fgF + n, params->logn, Zf(max_fg_bits)[params->logn]);
    if (v == 0) {
        goto cleanup;
    }
    u += v;
    v = Zf(trim_i8_encode)(sk + u, params->secretkeybytes - u,
        fgF + (2 * n), params->logn, Zf(max_FG_bits)[params->logn]);
    if (v == 0) {
        goto cleanup;
    }
    u += v;
    if (u != params->secretkeybytes) {
        goto cleanup;
    }

    pk[0] = (unsigned char)(0x00 + params->logn);
    v = Zf(modq_encode)(pk + 1, params->publickeybytes - 1, h, params->logn);
    if (v != params->publickeybytes - 1) {
        goto cleanup;
    }

    ret = 0;
cleanup:
    falcon_free_sensitive(tmp, tmp_len);
    falcon_free_sensitive(fgF, 3 * n);
    falcon_free_sensitive(h, n * sizeof *h);
    return ret;
}

int crypto_sign(falcon_level_t level, unsigned char *sm, unsigned long long *smlen,
    const unsigned char *m, unsigned long long mlen, const unsigned char *sk) {
    const falcon_params_t *params = Falcon_get_params(level);
    uint8_t *work = NULL;
    uint8_t *tmp;
    int8_t *fgFG;
    int16_t *sig = NULL;
    uint16_t *hm;
    unsigned char *esig;
    unsigned char seed[48], nonce[NONCELEN];
    inner_shake256_context sc;
    sampler_context *spc;
    size_t n, tmp_len, work_len;
    size_t u, v, sig_len;
    unsigned savcw;
    int ret = -1;

    if (params == NULL) {
        return -1;
    }

    if (sk[0] != (unsigned char)(0x50 + params->logn)) {
        return -1;
    }
    n = (size_t)1 << params->logn;
    tmp_len = 72 * n;
    work_len = tmp_len + (n * sizeof *sig) + sizeof *spc;
    work = malloc(work_len);
    if (work == NULL) {
        goto cleanup;
    }
    tmp = work;
    sig = (int16_t *)(void *)(work + tmp_len);
    spc = (sampler_context *)(void *)(work + tmp_len + (n * sizeof *sig));
    /* The final 4*N bytes are unused while complete_private() runs. */
    fgFG = (int8_t *)(tmp + tmp_len - (4 * n));
    hm = (uint16_t *)(void *)sig;
    u = 1;
    v = Zf(trim_i8_decode)(fgFG, params->logn, Zf(max_fg_bits)[params->logn],
        sk + u, params->secretkeybytes - u);
    if (v == 0) {
        goto cleanup;
    }
    u += v;
    v = Zf(trim_i8_decode)(fgFG + n, params->logn, Zf(max_fg_bits)[params->logn],
        sk + u, params->secretkeybytes - u);
    if (v == 0) {
        goto cleanup;
    }
    u += v;
    v = Zf(trim_i8_decode)(fgFG + (2 * n), params->logn,
        Zf(max_FG_bits)[params->logn],
        sk + u, params->secretkeybytes - u);
    if (v == 0) {
        goto cleanup;
    }
    u += v;
    if (u != params->secretkeybytes) {
        goto cleanup;
    }
    if (!Zf(complete_private)(fgFG + (3 * n), fgFG, fgFG + n,
        fgFG + (2 * n), params->logn, tmp)) {
        goto cleanup;
    }

    randombytes(nonce, sizeof nonce);

    inner_shake256_init(&sc);
    inner_shake256_inject(&sc, nonce, sizeof nonce);
    inner_shake256_inject(&sc, m, mlen);
    inner_shake256_flip(&sc);
    Zf(hash_to_point_vartime)(&sc, hm, params->logn);

    randombytes(seed, sizeof seed);
    inner_shake256_init(&sc);
    inner_shake256_inject(&sc, seed, sizeof seed);
    inner_shake256_flip(&sc);

    savcw = set_fpu_cw(2);
    if (!Zf(sign_dyn)(sig, &sc, fgFG, fgFG + n, fgFG + (2 * n),
        fgFG + (3 * n), hm, sk, params->secretkeybytes, params->logn, tmp, spc)) {
        set_fpu_cw(savcw);
        goto cleanup;
    }
    set_fpu_cw(savcw);

    memmove(sm + 2 + NONCELEN, m, mlen);
    esig = sm + 2 + NONCELEN + mlen;
    esig[0] = (unsigned char)(0x20 + params->logn);
    sig_len = Zf(comp_encode)(esig + 1, params->bytes - 2 - NONCELEN - 1,
        sig, params->logn);
    if (sig_len == 0) {
        goto cleanup;
    }
    sig_len++;
    sm[0] = (unsigned char)(sig_len >> 8);
    sm[1] = (unsigned char)sig_len;
    memcpy(sm + 2, nonce, sizeof nonce);
    *smlen = 2 + NONCELEN + mlen + sig_len;
    ret = 0;
cleanup:
    falcon_free_sensitive(work, work_len);
    return ret;
}

int crypto_sign_open(falcon_level_t level, unsigned char *m, unsigned long long *mlen,
    const unsigned char *sm, unsigned long long smlen, const unsigned char *pk) {
    const falcon_params_t *params = Falcon_get_params(level);
    uint8_t *tmp = NULL;
    const unsigned char *esig;
    uint16_t *hhm = NULL;
    int16_t *sig = NULL;
    inner_shake256_context sc;
    size_t n;
    size_t sig_len, msg_len;
    int ret = -1;

    if (params == NULL) {
        return -1;
    }

    if (pk[0] != (unsigned char)(0x00 + params->logn)) {
        return -1;
    }
    n = (size_t)1 << params->logn;
    tmp = malloc(2 * n);
    hhm = malloc(2 * n * sizeof *hhm);
    sig = malloc(n * sizeof *sig);
    if (tmp == NULL || hhm == NULL || sig == NULL) {
        goto cleanup;
    }
    if (Zf(modq_decode)(hhm, params->logn, pk + 1, params->publickeybytes - 1)
        != params->publickeybytes - 1)
    {
        goto cleanup;
    }
    Zf(to_ntt_monty)(hhm, params->logn);

    if (smlen < 2 + NONCELEN) {
        goto cleanup;
    }
    sig_len = ((size_t)sm[0] << 8) | (size_t)sm[1];
    if (sig_len > (smlen - 2 - NONCELEN)) {
        goto cleanup;
    }
    msg_len = smlen - 2 - NONCELEN - sig_len;

    esig = sm + 2 + NONCELEN + msg_len;
    if (sig_len < 1 || esig[0] != (unsigned char)(0x20 + params->logn)) {
        goto cleanup;
    }
    if (Zf(comp_decode)(sig, params->logn,
        esig + 1, sig_len - 1) != sig_len - 1)
    {
        goto cleanup;
    }

    inner_shake256_init(&sc);
    inner_shake256_inject(&sc, sm + 2, NONCELEN + msg_len);
    inner_shake256_flip(&sc);
    Zf(hash_to_point_vartime)(&sc, hhm + n, params->logn);

    if (!Zf(verify_raw)(hhm + n, sig, hhm, params->logn, tmp)) {
        goto cleanup;
    }

    memmove(m, sm + 2 + NONCELEN, msg_len);
    *mlen = msg_len;
    ret = 0;
cleanup:
    falcon_free_sensitive(tmp, 2 * n);
    falcon_free_sensitive(hhm, 2 * n * sizeof *hhm);
    falcon_free_sensitive(sig, n * sizeof *sig);
    return ret;
}
