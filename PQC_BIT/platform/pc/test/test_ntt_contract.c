/* Independent transform/convolution checks plus every recorded OP failure. */
#include "ntt.h"
#include "operator_interface.h"
#include "api.h"
#include "drng.h"
#include "memory_probe.h"
#include <stdio.h>
#include <string.h>
void ref_ntt_forward(int16_t *);
void ref_ntt_inverse(int16_t *);
void ref_ntt_basemul_raw(int16_t *, const int16_t *, const int16_t *);
void ref_ntt_montgomery_lift(int16_t *);
static unsigned calls, fail_at;
int real_OP_ntt256_init(const int32_t *, int32_t, int32_t, int32_t, int32_t, int32_t);
int real_OP_ntt256(int32_t *, const int32_t *, int);
int real_OP_intt256(int32_t *, const int32_t *, int);
int real_OP_cwm(int32_t *, const int32_t *, const int32_t *, int32_t, int32_t, int32_t);
static int fail(void) { return ++calls == fail_at; }
int OP_ntt256_init(const int32_t *w, int32_t c, int32_t q, int32_t iq, int32_t in, int32_t k)
{ return fail() ? -1 : real_OP_ntt256_init(w, c, q, iq, in, k); }
int OP_ntt256(int32_t *out, const int32_t *in, int index)
{ return fail() ? -1 : real_OP_ntt256(out, in, index); }
int OP_intt256(int32_t *out, const int32_t *in, int index)
{ return fail() ? -1 : real_OP_intt256(out, in, index); }
int OP_cwm(int32_t *out, const int32_t *a, const int32_t *b, int32_t n, int32_t q, int32_t k)
{ return fail() ? -1 : real_OP_cwm(out, a, b, n, q, k); }
#define REQUIRE(c) do { if (!(c)) { fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#c); return 1; } } while (0)
static uint32_t state = 1234567;
static uint32_t next(void) { state ^= state << 13; state ^= state >> 17; state ^= state << 5; return state; }
static int16_t mod(int64_t x) { x %= BIT_Q; return (int16_t)(x < 0 ? x + BIT_Q : x); }
static int primitives(void)
{
    for (unsigned trial = 0; trial < 48; ++trial) {
        int16_t a[BIT_N], b[BIT_N], ah[BIT_N], bh[BIT_N], expected[BIT_N], product[BIT_N], refraw[BIT_N];
        int64_t convolution[BIT_N] = {0};
        for (int i = 0; i < BIT_N; ++i) {
            a[i] = trial == 0 ? 0 : trial == 1 ? (i == 0) : trial == 2 ? (i == 255) : trial == 3 ? BIT_Q - 1 : (int16_t)(next() % BIT_Q);
            b[i] = trial < 3 ? (int16_t)(i * 17 % BIT_Q) : trial == 3 ? BIT_Q - 1 : (int16_t)(next() % BIT_Q);
        }
        /* Arbitrary frequency input, including all q-1, reaches the maximum
         * unreduced inverse sum and exercises the uint32 normalization bound. */
        memcpy(product, a, sizeof(a)); memcpy(expected, a, sizeof(a));
        ref_ntt_inverse(expected);
        REQUIRE(ntt_inverse(product) == 0 && memcmp(product, expected, sizeof(product)) == 0);
        memcpy(ah, a, sizeof(a)); memcpy(bh, b, sizeof(b)); memcpy(expected, a, sizeof(a));
        ref_ntt_forward(expected);
        unsigned before = calls;
        REQUIRE(ntt_forward(ah) == 0 && calls >= before + 2);
        REQUIRE(memcmp(ah, expected, sizeof(ah)) == 0);
        REQUIRE(ntt_forward(bh) == 0);
        ref_ntt_basemul_raw(refraw, ah, bh);
        REQUIRE(ntt_basemul_raw(product, ah, bh) == 0);
        for (int i = 0; i < BIT_N; ++i)
            REQUIRE(mod((int64_t)product[i] * 5569) == mod((int64_t)refraw[i] * 4088));
        REQUIRE(ntt_montgomery_lift(product) == 0);
        REQUIRE(ntt_inverse(product) == 0);
        for (int i = 0; i < BIT_N; ++i)
            for (int j = 0; j < BIT_N; ++j) {
                int index = i + j;
                int64_t value = (int64_t)a[i] * b[j];
                if (index >= BIT_N) convolution[index - BIT_N] -= value;
                else convolution[index] += value;
            }
        for (int i = 0; i < BIT_N; ++i) REQUIRE(product[i] == mod(convolution[i]));
        ref_ntt_montgomery_lift(refraw); ref_ntt_inverse(refraw);
        REQUIRE(memcmp(product, refraw, sizeof(product)) == 0);
        REQUIRE(ntt_inverse(ah) == 0 && memcmp(ah, a, sizeof(a)) == 0);
        REQUIRE(probe_live() == 0 && probe_dirty() == 0);
    }
    /* Exhaust the full signed accumulator range used by the three-term dot
     * products and subtraction before lifting out of R32^-1. */
    for (int base = -BIT_Q; base < 3 * BIT_Q; base += BIT_N) {
        int16_t values[BIT_N], expected[BIT_N];
        for (int i = 0; i < BIT_N; ++i) {
            values[i] = (int16_t)(base + i < 3 * BIT_Q ? base + i : 0);
            expected[i] = mod((int64_t)values[i] * 5569);
        }
        REQUIRE(ntt_montgomery_lift(values) == 0);
        REQUIRE(memcmp(values, expected, sizeof(values)) == 0);
    }
    int16_t a[BIT_N] = {1}, b[BIT_N] = {1}, out[BIT_N];
    for (unsigned f = 1; f <= 2; ++f) {
        calls = 0; fail_at = f; REQUIRE(ntt_forward(a) != 0);
        calls = 0; fail_at = f; REQUIRE(ntt_inverse(a) != 0);
        calls = 0; fail_at = f; REQUIRE(ntt_basemul_raw(out, a, b) != 0);
        REQUIRE(probe_live() == 0 && probe_dirty() == 0);
    }
    calls = fail_at = 0;
    puts("PASS: 48 original transform comparisons, R16/R32 mapping, round trips and schoolbook x^256+1 products");
    return 0;
}
static int fault_sweep(void)
{
    unsigned char pk[BIT_PUBLICKEYBYTES], sk[BIT_SECRETKEYBYTES], sig[BIT_SIGNBYTES], seed[64] = {7}, msg[64] = {3};
    unsigned long long pl = 0, sl = 0, zl = 0;
    REQUIRE(init_random_number(&drng_algorithm, seed, sizeof(seed)) == 0);
    REQUIRE(sig_keygen(pk, &pl, sk, &sl) == 0);
    REQUIRE(sig_sign(sk, sl, msg, sizeof(msg), sig, &zl) == 0);
    for (unsigned op = 0; op < 3; ++op) {
        DRNG_ctx snapshot = drng_algorithm;
        calls = fail_at = 0; probe_reset(0);
        int rc = op == 0 ? sig_keygen(pk, &pl, sk, &sl) : op == 1 ? sig_sign(sk, sl, msg, sizeof(msg), sig, &zl) : sig_verify(pk, pl, sig, zl, msg, sizeof(msg));
        REQUIRE(rc == 0 && probe_live() == 0 && probe_dirty() == 0);
        unsigned count = calls;
        for (unsigned f = 1; f <= count; ++f) {
            drng_algorithm = snapshot; calls = 0; fail_at = f; probe_reset(0);
            rc = op == 0 ? sig_keygen(pk, &pl, sk, &sl) : op == 1 ? sig_sign(sk, BIT_SECRETKEYBYTES, msg, sizeof(msg), sig, &zl) : sig_verify(pk, BIT_PUBLICKEYBYTES, sig, BIT_SIGNBYTES, msg, sizeof(msg));
            REQUIRE(rc != 0 && calls == f && probe_live() == 0 && probe_dirty() == 0);
            if (op == 0) REQUIRE(pl == 0 && sl == 0);
            if (op == 1) REQUIRE(zl == 0);
        }
        drng_algorithm = snapshot; calls = fail_at = 0;
        if (op == 0) REQUIRE(sig_keygen(pk, &pl, sk, &sl) == 0);
        if (op == 1) REQUIRE(sig_sign(sk, sl, msg, sizeof(msg), sig, &zl) == 0);
        printf("PASS: operation %u rejects all %u recorded NTT init/forward/inverse/CWM failures\n", op, count);
    }
    return 0;
}
int main(void)
{
    probe_reset(0);
    REQUIRE(primitives() == 0);
    REQUIRE(fault_sweep() == 0);
    return 0;
}
