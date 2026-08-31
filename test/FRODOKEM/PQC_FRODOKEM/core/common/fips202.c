// Implementation from PQClean project

/* Based on the public domain implementation in
 * crypto_hash/keccakc512/simple/ from http://bench.cr.yp.to/supercop.html
 * by Ronny Van Keer
 * and the public domain "TweetFips202" implementation
 * from https://twitter.com/tweetfips202
 * by Gilles Van Assche, Daniel J. Bernstein, and Peter Schwabe */

#include <stddef.h>
#include <stdint.h>
#include "fips202.h"
// 需要修改为 #include "operator_interface.h" 的统一接口
#include "operator_interface.h"

#define NROUNDS        24
#define ROL(a, offset) (((a) << (offset)) ^ ((a) >> (64 - (offset))))

void shake128_inc_init(shake128incctx *state) {
    OP_hash_init(OP_ALG_SHAKE128, (void*)state->ctx, 200 + 8);
}

void shake128_inc_absorb(shake128incctx *state, const uint8_t *input, size_t inlen) {
    OP_hash_absorb(OP_ALG_SHAKE128, (void*)state->ctx, 200+8, (void*)input, (int)inlen);
}

void shake128_inc_finalize(shake128incctx *state) {
    (void)state;
}

void shake128_inc_squeeze(uint8_t *output, size_t outlen, shake128incctx *state) {
    OP_hash_squeeze(OP_ALG_SHAKE128, (void*)state->ctx, 200+8, (void*)output, outlen);
}

void shake256_inc_init(shake256incctx *state) {
    OP_hash_init(OP_ALG_SHAKE256, (void*)state->ctx, 200+8);
}

void shake256_inc_absorb(shake256incctx *state, const uint8_t *input, size_t inlen) {
    OP_hash_absorb(OP_ALG_SHAKE256, (void*)state->ctx, 200+8, (void*)input, (int)inlen);
}

void shake256_inc_finalize(shake256incctx *state) {
    (void)state;
}

void shake256_inc_squeeze(uint8_t *output, size_t outlen, shake256incctx *state) {
    OP_hash_squeeze(OP_ALG_SHAKE256, (void*)state->ctx, 200+8, (void*)output, outlen);
}

/*************************************************
 * Name:        shake128_absorb
 *
 * Description: Absorb step of the SHAKE128 XOF.
 *              non-incremental, starts by zeroeing the state.
 *
 * Arguments:   - uint64_t *s: pointer to (uninitialized) output Keccak state
 *              - const uint8_t *input: pointer to input to be absorbed
 *                                            into s
 *              - size_t inlen: length of input in bytes
 **************************************************/
void shake128_absorb(shake128ctx *state, const uint8_t *input, size_t inlen) {

    shake128_inc_init((shake128incctx*)state);
    shake128_inc_absorb((shake128incctx*)state, input, inlen);
    shake128_inc_finalize((shake128incctx*)state);
}

/*************************************************
 * Name:        shake128_squeezeblocks
 *
 * Description: Squeeze step of SHAKE128 XOF. Squeezes full blocks of
 *              SHAKE128_RATE bytes each. Modifies the state. Can be called
 *              multiple times to keep squeezing, i.e., is incremental.
 *
 * Arguments:   - uint8_t *output: pointer to output blocks
 *              - size_t nblocks: number of blocks to be squeezed
 *                                            (written to output)
 *              - shake128ctx *state: pointer to input/output Keccak state
 **************************************************/
void shake128_squeezeblocks(uint8_t *output, size_t nblocks, shake128ctx *state) {
    size_t outlen = nblocks * SHAKE128_RATE;
    shake128_inc_squeeze(output, outlen, (shake128incctx*)state);
}

/*************************************************
 * Name:        shake256_absorb
 *
 * Description: Absorb step of the SHAKE256 XOF.
 *              non-incremental, starts by zeroeing the state.
 *
 * Arguments:   - shake256ctx *state: pointer to (uninitialized) output Keccak state
 *              - const uint8_t *input: pointer to input to be absorbed
 *                                            into s
 *              - size_t inlen: length of input in bytes
 **************************************************/
void shake256_absorb(shake256ctx *state, const uint8_t *input, size_t inlen) {
    shake256_inc_init((shake256incctx*)state);
    shake256_inc_absorb((shake256incctx*)state, input, inlen);
    shake256_inc_finalize((shake256incctx*)state);
}

/*************************************************
 * Name:        shake256_squeezeblocks
 *
 * Description: Squeeze step of SHAKE256 XOF. Squeezes full blocks of
 *              SHAKE256_RATE bytes each. Modifies the state. Can be called
 *              multiple times to keep squeezing, i.e., is incremental.
 *
 * Arguments:   - uint8_t *output: pointer to output blocks
 *              - size_t nblocks: number of blocks to be squeezed
 *                                (written to output)
 *              - shake256ctx *state: pointer to input/output Keccak state
 **************************************************/

void shake256_squeezeblocks(uint8_t *output, size_t nblocks, shake256ctx *state) {
    shake256_inc_squeeze(output, nblocks * SHAKE256_RATE, (shake256incctx*)state);
}

/*************************************************
 * Name:        shake128
 *
 * Description: SHAKE128 XOF with non-incremental API
 *
 * Arguments:   - uint8_t *output: pointer to output
 *              - size_t outlen: requested output length in bytes
 *              - const uint8_t *input: pointer to input
 *              - size_t inlen: length of input in bytes
 **************************************************/

void shake128(uint8_t *output, size_t outlen, const uint8_t *input, size_t inlen) {
    OP_hash(OP_ALG_SHAKE128, OP_MODE_NORMAL, (int)outlen, (void*)input, (int)inlen, 0, output);
}

/*************************************************
 * Name:        shake256
 *
 * Description: SHAKE256 XOF with non-incremental API
 *
 * Arguments:   - uint8_t *output: pointer to output
 *              - size_t outlen: requested output length in bytes
 *              - const uint8_t *input: pointer to input
 *              - size_t inlen: length of input in bytes
 **************************************************/

void shake256(uint8_t *output, size_t outlen, const uint8_t *input, size_t inlen) {
    OP_hash(OP_ALG_SHAKE256, OP_MODE_NORMAL, (int)outlen, (void*)input, (int)inlen, 0, output);
}

void sha3_256_inc_init(sha3_256incctx *state) {
    OP_hash_init(OP_ALG_SHA3_256, (void*)state->ctx, 200+8);
}

void sha3_256_inc_absorb(sha3_256incctx *state, const uint8_t *input, size_t inlen) {
    OP_hash_absorb(OP_ALG_SHA3_256, (void*)state->ctx, 200+8, (void*)input, inlen);
}

void sha3_256_inc_finalize(uint8_t *output, sha3_256incctx *state) {
    OP_hash_squeeze(OP_ALG_SHA3_256, (void*)state->ctx, 200+8, output, 32);
}

/*************************************************
 * Name:        sha3_256
 *
 * Description: SHA3-256 with non-incremental API
 *
 * Arguments:   - uint8_t *output:      pointer to output
 *              - const uint8_t *input: pointer to input
 *              - size_t inlen:   length of input in bytes
 **************************************************/
void sha3_256(uint8_t *output, const uint8_t *input, size_t inlen) {
    OP_hash(OP_ALG_SHA3_256, OP_MODE_NORMAL, 32, (void*)input, (int)inlen, 0, output);
}

void sha3_384_inc_init(sha3_384incctx *state) {
    OP_hash_init(OP_ALG_SHA3_384, (void*)state->ctx, 200+8);
}

void sha3_384_inc_absorb(sha3_384incctx *state, const uint8_t *input, size_t inlen) {
    OP_hash_absorb(OP_ALG_SHA3_384, (void*)state->ctx, 200+8, (void*)input, inlen);
}

void sha3_384_inc_finalize(uint8_t *output, sha3_384incctx *state) {
    OP_hash_squeeze(OP_ALG_SHA3_384, (void*)state->ctx, 200+8, output, 48);
}

/*************************************************
 * Name:        sha3_384
 *
 * Description: SHA3-256 with non-incremental API
 *
 * Arguments:   - uint8_t *output:      pointer to output
 *              - const uint8_t *input: pointer to input
 *              - size_t inlen:   length of input in bytes
 **************************************************/
void sha3_384(uint8_t *output, const uint8_t *input, size_t inlen) {
    OP_hash(OP_ALG_SHA3_384, OP_MODE_NORMAL, 48, (void*)input, (int)inlen, 0, output);
}

void sha3_512_inc_init(sha3_512incctx *state) {
    OP_hash_init(OP_ALG_SHA3_512, (void*)state->ctx, 200+8);
}

void sha3_512_inc_absorb(sha3_512incctx *state, const uint8_t *input, size_t inlen) {
    OP_hash_absorb(OP_ALG_SHA3_512, (void*)state->ctx, 200+8, (void*)input, inlen);
}

void sha3_512_inc_finalize(uint8_t *output, sha3_512incctx *state) {
    OP_hash_squeeze(OP_ALG_SHA3_512, (void*)state->ctx, 200+8, output, 64);
}

/*************************************************
 * Name:        sha3_512
 *
 * Description: SHA3-512 with non-incremental API
 *
 * Arguments:   - uint8_t *output:      pointer to output
 *              - const uint8_t *input: pointer to input
 *              - size_t inlen:   length of input in bytes
 **************************************************/
void sha3_512(uint8_t *output, const uint8_t *input, size_t inlen) {
    OP_hash(OP_ALG_SHA3_512, OP_MODE_NORMAL, 64, (void*)input, (int)inlen, 0, output);
}
