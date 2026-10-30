/* Copyright (c) 2026 Hang Zhang. OP adaptation, normal 128-bit module. */
#include "symmetric.h"
#include "endian.h"
#include <limits.h>
#include <string.h>

void bit_secure_zero(void *ptr, size_t len)
{
#if defined(__GNUC__) || defined(__clang__)
    memset(ptr, 0, len);
    __asm__ __volatile__("" : : "r"(ptr) : "memory");
#else
    volatile unsigned char *p = (volatile unsigned char *)ptr;
    while (len--) *p++ = 0;
#endif
}

static int absorb(uint8_t alg, uint64_t *state, const uint8_t *in, size_t len)
{
    if (in == NULL && len != 0) return -1;
    while (len) {
        int n = len > INT_MAX ? INT_MAX : (int)len;
        if (OP_hash_absorb(alg, state, OP_STATE_SIZE_SHA3, in, n) != OP_SUCCESS) return -1;
        in += n; len -= (size_t)n;
    }
    return 0;
}

static int squeeze(uint8_t alg, uint64_t *state, uint8_t *out, size_t len)
{
    if (out == NULL && len != 0) return -1;
    while (len) {
        int n = len > INT_MAX ? INT_MAX : (int)len;
        if (OP_hash_squeeze(alg, state, OP_STATE_SIZE_SHA3, out, n) != OP_SUCCESS) return -1;
        out += n; len -= (size_t)n;
    }
    return 0;
}

static int once(uint8_t alg, uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen)
{
    if ((out == NULL && outlen != 0) || (in == NULL && inlen != 0)) return -1;
    /* OP_hash requires non-NULL input even for an empty input. */
    if (in == NULL) in = (const uint8_t *)"";
    if (inlen <= INT_MAX && outlen <= INT_MAX && outlen != 0)
        return OP_hash(alg, OP_MODE_NORMAL, (int)outlen, in, (int)inlen, 0, out) == OP_SUCCESS ? 0 : -1;
    uint64_t state[OP_STATE_SIZE_SHA3 / sizeof(uint64_t)];
    int rc = -1;
    if (OP_hash_init(alg, state, sizeof(state)) == OP_SUCCESS &&
        absorb(alg, state, in, inlen) == 0 && squeeze(alg, state, out, outlen) == 0) rc = 0;
    bit_secure_zero(state, sizeof(state));
    return rc;
}

int bit_hash(uint8_t out[BIT_TRBYTES], const uint8_t *in, size_t len)
{ return once(OP_ALG_SHA3_256, out, BIT_TRBYTES, in, len); }

int bit_hash_message(uint8_t out[BIT_MESSAGEBYTES], const uint8_t *in0, size_t len0,
                     const uint8_t *in1, size_t len1)
{
    uint64_t state[OP_STATE_SIZE_SHA3 / sizeof(uint64_t)];
    int rc = -1;
    if (out != NULL && OP_hash_init(OP_ALG_SHA3_256, state, sizeof(state)) == OP_SUCCESS &&
        absorb(OP_ALG_SHA3_256, state, in0, len0) == 0 &&
        absorb(OP_ALG_SHA3_256, state, in1, len1) == 0 &&
        squeeze(OP_ALG_SHA3_256, state, out, BIT_MESSAGEBYTES) == 0) rc = 0;
    bit_secure_zero(state, sizeof(state));
    return rc;
}

int bit_xof256(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen)
{ return once(OP_ALG_SHAKE256, out, outlen, in, inlen); }

int bit_xof256_nonce(uint8_t *out, size_t outlen, const uint8_t *seed, size_t seedlen, uint16_t nonce)
{
    uint8_t ext[BIT_SEEDBYTES + 2];
    if (seed == NULL || seedlen != BIT_SEEDBYTES) return -1;
    memcpy(ext, seed, seedlen); store16_le(ext + seedlen, nonce);
    int rc = bit_xof256(out, outlen, ext, sizeof(ext));
    bit_secure_zero(ext, sizeof(ext));
    return rc;
}

int bit_xof256_3(uint8_t *out, size_t outlen, const uint8_t *in0, size_t len0,
                  const uint8_t *in1, size_t len1, const uint8_t *in2, size_t len2)
{
    uint64_t state[OP_STATE_SIZE_SHA3 / sizeof(uint64_t)];
    int rc = -1;
    if (len0 != BIT_SEEDBYTES || len1 != BIT_SEEDBYTES || len2 != BIT_MESSAGEBYTES) return -1;
    if (OP_hash_init(OP_ALG_SHAKE256, state, sizeof(state)) == OP_SUCCESS &&
        absorb(OP_ALG_SHAKE256, state, in0, len0) == 0 &&
        absorb(OP_ALG_SHAKE256, state, in1, len1) == 0 &&
        absorb(OP_ALG_SHAKE256, state, in2, len2) == 0 &&
        squeeze(OP_ALG_SHAKE256, state, out, outlen) == 0) rc = 0;
    bit_secure_zero(state, sizeof(state));
    return rc;
}

static void init(bit_xof256_state *s, uint8_t alg, const uint8_t *in, size_t len)
{
    memset(s, 0, sizeof(*s));
    s->error = OP_hash_init(alg, s->state, sizeof(s->state)) != OP_SUCCESS;
    if (!s->error) s->error = absorb(alg, s->state, in, len) != 0;
}
void bit_xof256_init(bit_xof256_state *s, const uint8_t *seed, size_t len, uint16_t nonce)
{
    /* The seed lengths used by normal signing are fixed; no VLA or concatenation. */
    uint8_t n[2]; store16_le(n, nonce);
    init(s, OP_ALG_SHAKE256, seed, len);
    if (!s->error) s->error = absorb(OP_ALG_SHAKE256, s->state, n, sizeof(n)) != 0;
    s->nonce = nonce;
}
int bit_xof256_squeeze(bit_xof256_state *s, uint8_t *out, size_t len)
{
    if (!s->error) s->error = squeeze(OP_ALG_SHAKE256, s->state, out, len) != 0;
    return s->error ? -1 : 0;
}
void bit_xof256_zeroize(bit_xof256_state *s) { bit_secure_zero(s, sizeof(*s)); }
void bit_xof256_ctx_init(bit_xof256_ctx *s, const uint8_t *in, size_t len)
{ init(s, OP_ALG_SHAKE256, in, len); }
int bit_xof256_ctx_squeezeblocks(bit_xof256_ctx *s, uint8_t *out, size_t n)
{
    if (n > SIZE_MAX / BIT_XOF256_RATE) { s->error = 1; return -1; }
    return bit_xof256_squeeze(s, out, n * BIT_XOF256_RATE);
}
void bit_xof256_ctx_zeroize(bit_xof256_ctx *s) { bit_xof256_zeroize(s); }
void bit_xof128_init(bit_xof128_state *s, const uint8_t *in, size_t len)
{ init(s, OP_ALG_SHAKE128, in, len); }
int bit_xof128_squeeze(bit_xof128_state *s, uint8_t *out, size_t len)
{
    if (!s->error) s->error = squeeze(OP_ALG_SHAKE128, s->state, out, len) != 0;
    return s->error ? -1 : 0;
}
void bit_xof128_zeroize(bit_xof128_state *s) { bit_xof256_zeroize(s); }
