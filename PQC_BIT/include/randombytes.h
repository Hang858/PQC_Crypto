#ifndef PQC_BIT_RANDOMBYTES_H
#define PQC_BIT_RANDOMBYTES_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
/* Direct OP_trng requests. Zero bytes succeeds without calling the operator.
 * On failure, the entire supplied output buffer is cleared; discard it.
 * No seeding or persistent algorithm state. OP backend controls concurrency. */
int bit_randombytes(void *output, size_t bytes);
/* Optional availability check; keygen/sign do not require an init call. */
int bit_random_init(void);
#ifdef __cplusplus
}
#endif
#endif
