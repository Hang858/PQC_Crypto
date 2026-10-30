/* OP errors must reach callers; streams must match the independent reference. */
#include <stdio.h>
#include <string.h>
#include "symmetric.h"
#include "operator_interface.h"
#include "fips202.h"
#include "drng.h"
#include "api.h"
#include "memory_probe.h"

static unsigned calls, fail_at;
static int trng_failure;
int OP_trng(void *out, int len)
{
    /* TEST ONLY: replay identical traces when injecting a hash failure. */
    return trng_failure ? -1 : get_random_number(&drng_algorithm, out,
                                                 (unsigned long long)len * 8ULL);
}
int real_OP_hash(uint8_t, uint8_t, int, const void *, int, uint8_t, void *);
int real_OP_hash_init(uint8_t, void *, int);
int real_OP_hash_absorb(uint8_t, void *, int, const void *, int);
int real_OP_hash_squeeze(uint8_t, void *, int, void *, int);
static int fail(void) { return ++calls == fail_at; }
int OP_hash(uint8_t a, uint8_t m, int n, const void *in, int len, uint8_t c, void *out)
{ return fail() ? -1 : real_OP_hash(a, m, n, in, len, c, out); }
int OP_hash_init(uint8_t a, void *s, int n)
{ return fail() ? -1 : real_OP_hash_init(a, s, n); }
int OP_hash_absorb(uint8_t a, void *s, int n, const void *in, int len)
{ return fail() ? -1 : real_OP_hash_absorb(a, s, n, in, len); }
int OP_hash_squeeze(uint8_t a, void *s, int n, void *out, int len)
{ return fail() ? -1 : real_OP_hash_squeeze(a, s, n, out, len); }
#define REQUIRE(c) do { if (!(c)) { fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #c); return 1; } } while (0)

static int test_api_errors(void)
{
    uint8_t pk[BIT_PUBLICKEYBYTES], sk[BIT_SECRETKEYBYTES], sig[BIT_SIGNBYTES];
    uint8_t seed[64] = {17}, msg[64] = {23};
    unsigned long long pl = 99, sl = 99, zl = 99;
    REQUIRE(sig_keygen(NULL, &pl, sk, &sl) != 0 && pl == 0 && sl == 0);
    REQUIRE(sig_sign(sk, 0, msg, sizeof(msg), sig, &zl) != 0 && zl == 0);
    calls = fail_at = 0;
    REQUIRE(init_random_number(&drng_algorithm, seed, sizeof(seed)) == 0);
    REQUIRE(sig_keygen(pk, &pl, sk, &sl) == 0);
    REQUIRE(sig_sign(sk, sl, msg, sizeof(msg), sig, &zl) == 0);
    for (unsigned op = 0; op < 3; ++op) {
        DRNG_ctx snapshot = drng_algorithm;
        calls = fail_at = 0; probe_reset(0);
        int rc = op == 0 ? sig_keygen(pk, &pl, sk, &sl) : op == 1 ?
            sig_sign(sk, sl, msg, sizeof(msg), sig, &zl) :
            sig_verify(pk, pl, sig, zl, msg, sizeof(msg));
        REQUIRE(rc == 0 && probe_live() == 0 && probe_dirty() == 0);
        unsigned total = calls;
        for (unsigned f = 1; f <= total; ++f) {
            drng_algorithm = snapshot;
            calls = 0; fail_at = f; probe_reset(0);
            rc = op == 0 ? sig_keygen(pk, &pl, sk, &sl) : op == 1 ?
                sig_sign(sk, BIT_SECRETKEYBYTES, msg, sizeof(msg), sig, &zl) :
                sig_verify(pk, BIT_PUBLICKEYBYTES, sig, BIT_SIGNBYTES, msg, sizeof(msg));
            REQUIRE(rc != 0 && calls >= f && probe_live() == 0 && probe_dirty() == 0);
            if (op == 0) REQUIRE(pl == 0 && sl == 0);
            if (op == 1) REQUIRE(zl == 0);
        }
        /* Restore successful artifacts for the next operation's fault sweep. */
        calls = fail_at = 0; drng_algorithm = snapshot;
        if (op == 0) REQUIRE(sig_keygen(pk, &pl, sk, &sl) == 0);
        if (op == 1) REQUIRE(sig_sign(sk, sl, msg, sizeof(msg), sig, &zl) == 0);
        printf("PASS: operation %u rejects each of %u injected OP failures\n", op, total);
    }
    calls = fail_at = 0; trng_failure = 1;
    REQUIRE(bit_random_init() != 0);
    REQUIRE(sig_keygen(pk, &pl, sk, &sl) != 0 && pl == 0 && sl == 0);
    trng_failure = 0;
    REQUIRE(bit_random_init() == 0);
    REQUIRE(sig_keygen(pk, &pl, sk, &sl) == 0);
    REQUIRE(sig_sign(sk, sl, msg, sizeof(msg), sig, &zl) == 0);
    REQUIRE(sig_verify(pk, pl, sig, zl, msg, sizeof(msg)) == 0);
    return 0;
}

int main(void)
{
    uint8_t seed[BIT_CHALLENGEBYTES], out[1024], expected[1024];
    for (size_t i = 0; i < sizeof(seed); ++i) seed[i] = (uint8_t)(i * 7 + 3);
    calls = 0; fail_at = 1;
    REQUIRE(bit_hash(out, seed, sizeof(seed)) != 0);
    calls = 0; fail_at = 0;
    REQUIRE(bit_hash(out, seed, sizeof(seed)) == 0);
    sha3_256(expected, seed, sizeof(seed));
    REQUIRE(memcmp(out, expected, 32) == 0);
    for (unsigned f = 1; f <= 4; ++f) {
        calls = 0; fail_at = f;
        REQUIRE(bit_hash_message(out, seed, 11, seed + 11, sizeof(seed) - 11) != 0);
    }
    calls = 0; fail_at = 0;
    REQUIRE(bit_hash_message(out, seed, 11, seed + 11, sizeof(seed) - 11) == 0);
    REQUIRE(memcmp(out, expected, 32) == 0);
    {
        const size_t chunks[] = {0, 1, 135, 0, 137, 13, 168, 256, 214, 100};
        bit_xof128_state s128;
        bit_xof256_state s256;
        size_t pos = 0;
        shake128(expected, sizeof(expected), seed, sizeof(seed));
        bit_xof128_init(&s128, seed, sizeof(seed));
        for (size_t i = 0; i < sizeof(chunks)/sizeof(chunks[0]); ++i) {
            REQUIRE(bit_xof128_squeeze(&s128, out + pos, chunks[i]) == 0);
            pos += chunks[i];
        }
        REQUIRE(pos == sizeof(out) && memcmp(out, expected, pos) == 0);
        bit_xof128_zeroize(&s128);
        shake256(expected, sizeof(expected), seed, sizeof(seed));
        /* Generic SHAKE256 includes the little endian nonce. */
        uint8_t ext[BIT_SEEDBYTES + 2];
        memcpy(ext, seed, BIT_SEEDBYTES); ext[BIT_SEEDBYTES] = 0x34; ext[BIT_SEEDBYTES + 1] = 0x12;
        shake256(expected, sizeof(expected), ext, sizeof(ext));
        bit_xof256_init(&s256, seed, BIT_SEEDBYTES, 0x1234);
        pos = 0;
        for (size_t i = 0; i < sizeof(chunks)/sizeof(chunks[0]); ++i) {
            REQUIRE(bit_xof256_squeeze(&s256, out + pos, chunks[i]) == 0);
            pos += chunks[i];
        }
        REQUIRE(memcmp(out, expected, pos) == 0);
        bit_xof256_zeroize(&s256);
        calls = 0; fail_at = 1;
        bit_xof128_init(&s128, seed, sizeof(seed));
        fail_at = 0;
        REQUIRE(bit_xof128_squeeze(&s128, out, 1) != 0);
        bit_xof128_zeroize(&s128);
        calls = 0; fail_at = 0;
        bit_xof256_init(&s256, seed, BIT_SEEDBYTES, 0);
        fail_at = calls + 1;
        REQUIRE(bit_xof256_squeeze(&s256, out, 1) != 0);
        fail_at = 0;
        REQUIRE(bit_xof256_squeeze(&s256, out, 1) != 0);
        bit_xof256_zeroize(&s256);
    }
    {
        DRNG_ctx drng;
        calls = 0; fail_at = 1;
        REQUIRE(init_random_number(&drng, seed, sizeof(seed)) != 0);
        calls = 0; fail_at = 0;
        REQUIRE(init_random_number(&drng, seed, sizeof(seed)) == 0);
        calls = 0; fail_at = 1;
        REQUIRE(get_random_number(&drng, out, 128) != 0);
    }
    REQUIRE(test_api_errors() == 0);
    puts("PASS: OP failure propagation, SM3 DRNG and split SHAKE streams");
    return 0;
}
