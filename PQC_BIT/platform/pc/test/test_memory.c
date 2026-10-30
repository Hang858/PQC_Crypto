#include "api.h"
#include "memory_probe.h"
#include "operator_interface.h"
#include <stdio.h>
#include <string.h>
#define REQUIRE(c) do { if (!(c)) { fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#c); return 1; } } while (0)
static unsigned char pk[BIT_PUBLICKEYBYTES], sk[BIT_SECRETKEYBYTES], sig[BIT_SIGNBYTES];
int main(void)
{
    unsigned char msg[64] = {1};
    unsigned long long pl = 0, sl = 0, zl = 0;
    size_t nalloc[3];
    for (unsigned operation = 0; operation < 3; ++operation) {
        probe_reset(0);
        int rc = operation == 0 ? sig_keygen(pk, &pl, sk, &sl) : operation == 1 ?
            sig_sign(sk, sl, msg, sizeof(msg), sig, &zl) : sig_verify(pk, pl, sig, zl, msg, sizeof(msg));
        REQUIRE(rc == 0 && probe_live() == 0 && probe_dirty() == 0);
        size_t total = probe_peak() + 3072 + OP_NTT_WORK_RAM_BYTES +
            sizeof(pk) + sizeof(sk) + sizeof(sig) + 128 + 256;
        printf("operation=%u heap=%zu total_with_3072_stack_128_allocator_256_libc=%zu allocations=%zu\n",
               operation, probe_peak(), total, probe_calls());
        printf("host_allocator_usable_peak_with_probe_header=%zu (host size classes, not embedded malloc metadata)\n", probe_host_peak());
        /* Check the host allocator's size-class rounding as well as payloads. */
        REQUIRE(total - probe_peak() + probe_host_peak() < 40000);
        REQUIRE(total < 40000);
        nalloc[operation] = probe_calls();
    }
    /* Every real allocation must fail cleanly. These operations must use heap
     * because the separate stack check forbids their old >10 KB stack frames. */
    for (unsigned operation = 0; operation < 3; ++operation) {
        REQUIRE(nalloc[operation] == 1);
        for (size_t fail = 1; fail <= nalloc[operation]; ++fail) {
            probe_reset(fail);
            int rc;
            if (operation == 0) {
                pl = sl = 99; rc = sig_keygen(pk, &pl, sk, &sl);
                REQUIRE(rc != 0 && pl == 0 && sl == 0);
            } else if (operation == 1) {
                zl = 99; rc = sig_sign(sk, BIT_SECRETKEYBYTES, msg, sizeof(msg), sig, &zl);
                REQUIRE(rc != 0 && zl == 0);
            } else {
                rc = sig_verify(pk, BIT_PUBLICKEYBYTES, sig, BIT_SIGNBYTES, msg, sizeof(msg));
                REQUIRE(rc != 0);
            }
            REQUIRE(probe_live() == 0 && probe_dirty() == 0);
        }
    }
    puts("PASS: RAM budget, allocation failures, zeroization and no leaks");
    return 0;
}
