/* The public key must contain seed_A supplied directly by OP_trng. */
#include "api.h"
#include "operator_interface.h"
#include "memory_probe.h"
#include <stdio.h>
#include <string.h>
static unsigned requests, fail_at;
static int first_size;
int OP_trng(void *out, int n)
{
    if (++requests == 1) first_size = n;
    for (int i = 0; i < n; ++i)
        ((unsigned char *)out)[i] = (unsigned char)(0xa5 + 37 * (requests - 1) + (unsigned)i);
    return requests == fail_at ? OP_FAILURE : OP_SUCCESS;
}
#define REQUIRE(c) do { if (!(c)) { fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#c); return 1; } } while (0)
int main(void)
{
    unsigned char pk[BIT_PUBLICKEYBYTES], sk[BIT_SECRETKEYBYTES], sig[BIT_SIGNBYTES];
    unsigned char msg[64] = {3};
    unsigned long long pl = 0, sl = 0, zl = 0;
    probe_reset(0);
    REQUIRE(sig_keygen(pk, &pl, sk, &sl) == 0);
    REQUIRE(first_size == 2 * BIT_SEEDBYTES);
    for (unsigned i = 0; i < BIT_SEEDBYTES; ++i)
        REQUIRE(pk[i] == (unsigned char)(0xa5 + i));
    unsigned keygen_requests = requests;
    unsigned before = requests;
    REQUIRE(sig_sign(sk, sl, msg, sizeof(msg), sig, &zl) == 0);
    REQUIRE(requests > before);
    REQUIRE(sig_verify(pk, pl, sig, zl, msg, sizeof(msg)) == 0);
    unsigned sign_requests = requests - before;
    /* Replay each actual RNG request, including the post-sampling sign-bit
     * batch request, so later failures cannot hide behind first-call tests. */
    for (unsigned f = 1; f <= sign_requests; ++f) {
        requests = before; fail_at = before + f;
        REQUIRE(sig_sign(sk, sl, msg, sizeof(msg), sig, &zl) != 0 && zl == 0);
        REQUIRE(requests == fail_at && probe_live() == 0 && probe_dirty() == 0);
    }
    for (unsigned f = 1; f <= keygen_requests; ++f) {
        requests = 0; fail_at = f;
        REQUIRE(sig_keygen(pk, &pl, sk, &sl) != 0 && pl == 0 && sl == 0);
        REQUIRE(requests == fail_at && probe_live() == 0 && probe_dirty() == 0);
    }
    unsigned char entropy[32];
    fail_at = requests + 1;
    REQUIRE(bit_randombytes(entropy, sizeof(entropy)) != 0);
    for (unsigned i = 0; i < sizeof(entropy); ++i) REQUIRE(entropy[i] == 0);
    before = requests;
    REQUIRE(bit_randombytes(NULL, 0) == 0 && requests == before);
    REQUIRE(bit_randombytes(NULL, 1) != 0 && requests == before);
    printf("PASS: direct OP_trng seed delivery; faulted all %u keygen / %u sign RNG requests, clean release\n",
           keygen_requests, sign_requests);
    return 0;
}
