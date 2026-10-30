#ifndef PQC_BIT_API_H
#define PQC_BIT_API_H
#include "params.h"
#include "randombytes.h"
#include "SIG_AlgorithmInstance.h"
#define CRYPTO_ALGNAME "BiT-128"
#define CRYPTO_PUBLICKEYBYTES BIT_PUBLICKEYBYTES
#define CRYPTO_SECRETKEYBYTES BIT_SECRETKEYBYTES
#define CRYPTO_BYTES BIT_SIGNBYTES
/* Normal 128-bit signing only. No precomputation. Signatures are detached.
 * Every random request calls OP_trng directly; there is no software DRNG state.
 * bit_random_init() is an optional availability probe, not a seeding function.
 * All output buffers have the fixed sizes above; messages are never copied.
 * See SIG_AlgorithmInstance.h for sig_keygen/sign/verify and length queries. */
#endif
