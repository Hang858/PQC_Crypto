/* TEST ONLY: SM3 Hash_DRBG compatible with the ICCS reference DRNG.
 * Original template copyright ICCS; adaptation routes every hash through OP. */
#include "drng.h"
#include "symmetric.h"
#include "operator_interface.h"
#include <limits.h>
#include <stdint.h>
#include <string.h>

DRNG_ctx drng_algorithm;

static void inc(unsigned char *v, size_t n)
{
    while (n) { if (++v[--n]) break; }
}
static int sm3_parts(unsigned char out[32], const unsigned char *a, size_t alen,
                     const unsigned char *b, size_t blen)
{
    uint64_t state[OP_STATE_SIZE_SM3 / sizeof(uint64_t)];
    int rc = -1;
    if (OP_hash_init(OP_ALG_SM3, state, sizeof(state)) != OP_SUCCESS) goto done;
    const unsigned char *parts[2] = {a, b};
    size_t lengths[2] = {alen, blen};
    for (unsigned i = 0; i < 2; ++i) {
        const unsigned char *p = parts[i]; size_t left = lengths[i];
        if (p == NULL && left != 0) goto done;
        while (left) {
            int n = left > INT_MAX ? INT_MAX : (int)left;
            if (OP_hash_absorb(OP_ALG_SM3, state, sizeof(state), p, n) != OP_SUCCESS) goto done;
            p += n; left -= (size_t)n;
        }
    }
    if (OP_hash_squeeze(OP_ALG_SM3, state, sizeof(state), out, 32) == OP_SUCCESS) rc = 0;
 done:
    bit_secure_zero(state, sizeof(state));
    return rc;
}
static int df(unsigned char out[SEEDLEN], const unsigned char *in, size_t len)
{
    unsigned char digest[64];
    unsigned char prefix[5] = {1, 0, 0, 1, 0xb8}; /* 8 * 55 = 440, big endian */
    int rc = -1;
    if (sm3_parts(digest, prefix, sizeof(prefix), in, len) != 0) goto done;
    prefix[0] = 2;
    if (sm3_parts(digest + 32, prefix, sizeof(prefix), in, len) != 0) goto done;
    memcpy(out, digest, SEEDLEN); rc = 0;
 done:
    bit_secure_zero(digest, sizeof(digest));
    return rc;
}
int init_random_number(DRNG_ctx *drng, const unsigned char *seed, unsigned long long len)
{
    unsigned char padded[1 + SEEDLEN];
    int rc = -1;
    if (drng == NULL || (seed == NULL && len != 0) || len > SIZE_MAX) return -1;
    bit_secure_zero(drng, sizeof(*drng));
    if (df(drng->V, seed, (size_t)len) != 0) goto done;
    padded[0] = 0; memcpy(padded + 1, drng->V, SEEDLEN);
    if (df(drng->C, padded, sizeof(padded)) != 0) goto done;
    inc(drng->reseed_counter, SEEDLEN); rc = 0;
 done:
    bit_secure_zero(padded, sizeof(padded));
    if (rc != 0) bit_secure_zero(drng, sizeof(*drng));
    return rc;
}
int get_random_number(DRNG_ctx *drng, unsigned char *out, unsigned long long bits)
{
    unsigned char data[SEEDLEN], digest[32], h[SEEDLEN] = {0};
    unsigned char padded[1 + SEEDLEN];
    DRNG_ctx next;
    int rc = -1;
    unsigned long long bytes = bits / 8 + (bits % 8 != 0), remaining = bytes;
    if (drng == NULL || (out == NULL && bytes != 0) || bytes > SIZE_MAX) return -1;
    next = *drng;
    memcpy(data, next.V, sizeof(data));
    while (remaining) {
        if (sm3_parts(digest, data, sizeof(data), NULL, 0) != 0) goto done;
        size_t n = remaining > 32 ? 32 : (size_t)remaining;
        memcpy(out, digest, n); out += n; remaining -= n; inc(data, sizeof(data));
    }
    if (bytes != 0 && bits % 8 != 0) out[-1] &= (unsigned char)(0xffU << (8 - bits % 8));
    padded[0] = 3; memcpy(padded + 1, next.V, SEEDLEN);
    if (sm3_parts(h + SEEDLEN - 32, padded, sizeof(padded), NULL, 0) != 0) goto done;
    unsigned carry = 0;
    for (size_t i = SEEDLEN; i > 0; --i) {
        unsigned sum = next.V[i-1] + h[i-1] + next.C[i-1] + next.reseed_counter[i-1] + carry;
        next.V[i-1] = (unsigned char)sum; carry = sum >> 8;
    }
    inc(next.reseed_counter, SEEDLEN);
    *drng = next; rc = 0;
 done:
    bit_secure_zero(&next, sizeof(next)); bit_secure_zero(data, sizeof(data));
    bit_secure_zero(digest, sizeof(digest)); bit_secure_zero(h, sizeof(h));
    bit_secure_zero(padded, sizeof(padded));
    return rc;
}
