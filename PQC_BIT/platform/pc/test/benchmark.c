/* Same driver for the original reference and adapted module. Timing includes
 * DRNG calls, all rejected signing attempts, malloc/free and erasure. */
#define _POSIX_C_SOURCE 200809L
#include "SIG_AlgorithmInstance.h"
#include "params.h"
#ifndef BIT_BENCH_PRODUCTION
#include "drng.h"
#endif
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef BIT_BENCH_BASELINE
DRNG_ctx drng_algorithm;
#endif
#define KEYS 32
#define SIGNATURES 16
#define REQUIRE(c) do { if (!(c)) { fprintf(stderr, "benchmark failure at %d\n", __LINE__); return 1; } } while (0)
static uint64_t now(void)
{
    struct timespec t;
    if (clock_gettime(CLOCK_MONOTONIC, &t) != 0) abort();
    return (uint64_t)t.tv_sec * UINT64_C(1000000000) + (uint64_t)t.tv_nsec;
}
static size_t message(uint8_t *out, unsigned key, unsigned n)
{
    static const size_t lens[] = {0,1,32,64,135,136,137,512,64,64,64,64,64,64,64,64};
    size_t len = lens[n];
    for (size_t i = 0; i < len; ++i) out[i] = (uint8_t)(13 * i + 17 * n + 37 * key);
    return len;
}
static int verify_file(const char *path, const char *resign_path)
{
    FILE *f = fopen(path, "rb");
    uint8_t pk[BIT_PUBLICKEYBYTES], sk[BIT_SECRETKEYBYTES], sig[BIT_SIGNBYTES], msg[512];
    FILE *output = resign_path ? fopen(resign_path, "wb") : NULL;
    REQUIRE(f != NULL && (resign_path == NULL || output != NULL));
    for (unsigned k = 0; k < KEYS; ++k) {
        REQUIRE(fread(pk, 1, sizeof(pk), f) == sizeof(pk));
        REQUIRE(fread(sk, 1, sizeof(sk), f) == sizeof(sk));
        if (output) {
#ifndef BIT_BENCH_PRODUCTION
            unsigned char seed[64];
            for (size_t i = 0; i < sizeof(seed); ++i) seed[i] = (uint8_t)(i + 31 * k + 0xa5);
            REQUIRE(init_random_number(&drng_algorithm, seed, sizeof(seed)) == 0);
#endif
            REQUIRE(fwrite(pk, 1, sizeof(pk), output) == sizeof(pk));
            REQUIRE(fwrite(sk, 1, sizeof(sk), output) == sizeof(sk));
        }
        for (unsigned j = 0; j < SIGNATURES; ++j) {
            size_t len = message(msg, k, j);
            REQUIRE(fread(sig, 1, sizeof(sig), f) == sizeof(sig));
            REQUIRE(sig_verify(pk, sizeof(pk), sig, sizeof(sig), msg, len) == 0);
            if (output) {
                unsigned long long zl = 0;
                REQUIRE(sig_sign(sk, sizeof(sk), msg, len, sig, &zl) == 0 && zl == sizeof(sig));
                REQUIRE(sig_verify(pk, sizeof(pk), sig, zl, msg, len) == 0);
                REQUIRE(fwrite(sig, 1, sizeof(sig), output) == sizeof(sig));
            }
        }
    }
    REQUIRE(fgetc(f) == EOF && fclose(f) == 0);
    if (output) REQUIRE(fclose(output) == 0);
    puts(output ? "PASS: cross signing with imported secret keys, 512 signatures" : "PASS: cross verification of 512 signatures");
    return 0;
}
int main(int argc, char **argv)
{
    if (argc == 3 && strcmp(argv[1], "--verify") == 0) return verify_file(argv[2], NULL);
    if (argc == 4 && strcmp(argv[1], "--resign") == 0) return verify_file(argv[2], argv[3]);
    FILE *f = argc == 3 && strcmp(argv[1], "--emit") == 0 ? fopen(argv[2], "wb") : NULL;
    if (argc > 1) REQUIRE(f != NULL);
    uint8_t pk[BIT_PUBLICKEYBYTES], sk[BIT_SECRETKEYBYTES], sig[BIT_SIGNBYTES], msg[512], seed[64];
    unsigned long long pl, sl, zl;
    uint64_t kt = 0, st = 0, vt = 0, t;
    for (unsigned k = 0; k < KEYS; ++k) {
        for (size_t i = 0; i < sizeof(seed); ++i) seed[i] = (uint8_t)(i + 31 * k + 0xa5);
#ifndef BIT_BENCH_PRODUCTION
        REQUIRE(init_random_number(&drng_algorithm, seed, sizeof(seed)) == 0);
#else
        (void)seed;
#endif
        t = now();
        int rc = sig_keygen(pk, &pl, sk, &sl);
        kt += now() - t;
        REQUIRE(rc == 0 && pl == sizeof(pk) && sl == sizeof(sk));
        if (f) {
            REQUIRE(fwrite(pk, 1, sizeof(pk), f) == sizeof(pk));
            REQUIRE(fwrite(sk, 1, sizeof(sk), f) == sizeof(sk));
        }
        for (unsigned j = 0; j < SIGNATURES; ++j) {
            size_t len = message(msg, k, j);
            t = now();
            rc = sig_sign(sk, sl, msg, len, sig, &zl);
            st += now() - t;
            REQUIRE(rc == 0 && zl == sizeof(sig));
            t = now();
            rc = sig_verify(pk, pl, sig, zl, msg, len);
            vt += now() - t;
            REQUIRE(rc == 0);
            if (f) REQUIRE(fwrite(sig, 1, sizeof(sig), f) == sizeof(sig));
        }
    }
    if (f) REQUIRE(fclose(f) == 0);
    printf("{\"keygen_ns\":%.3f,\"sign_ns\":%.3f,\"verify_ns\":%.3f}\n",
           (double)kt / KEYS, (double)st / (KEYS * SIGNATURES), (double)vt / (KEYS * SIGNATURES));
    return 0;
}
