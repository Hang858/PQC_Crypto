/*
 * Copyright (c) 2026 Hang Zhang.
 * State Key Laboratory of Cyberspace Security Defense,
 * Institute of Information Engineering, CAS
 * School of Cyber Security, University of Chinese Academy of Sciences
 */
#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include <stddef.h>
#include <stdint.h>
#include "params.h"


#include "operator_interface.h"
#define BIT_XOF256_RATE 136
#define BIT_XOF128_RATE 168
#define BIT_XOF256_STREAM_BLOCKBYTES 136
/* The driver owns all 208 state bytes; never copy opaque live driver state. */
typedef struct {
    uint64_t state[OP_STATE_SIZE_SHA3 / sizeof(uint64_t)];
    int error;
    uint16_t nonce;
} bit_xof256_state;
typedef bit_xof256_state bit_xof128_state;
typedef bit_xof256_state bit_xof256_ctx;

#ifdef __cplusplus
extern "C" {
#endif

void bit_secure_zero(void *ptr, size_t len);

/* Hash widths follow BIT_TRBYTES and BIT_MESSAGEBYTES. */
#define bit_h256 bit_hash
#define bit_h256_2 bit_hash_message
int bit_hash(uint8_t out[BIT_TRBYTES], const uint8_t *in, size_t inlen);
int bit_hash_message(uint8_t out[BIT_MESSAGEBYTES], const uint8_t *in0, size_t inlen0,
                     const uint8_t *in1, size_t inlen1);


/* Fallible hashes and XOF reads return 0 on success, -1 on failure.
 * Discard partial output and release the stream after a failed read. */
int bit_xof256(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen);

/* Seed must be BIT_SEEDBYTES; returns -1 for an invalid length. */
int bit_xof256_nonce(uint8_t *out, size_t outlen, const uint8_t *seed, size_t seedlen,
                     uint16_t nonce);
/* Inputs must be seed, seed, message lengths; returns -1 otherwise. */
int bit_xof256_3(uint8_t *out, size_t outlen, const uint8_t *in0, size_t inlen0, const uint8_t *in1,
                 size_t inlen1, const uint8_t *in2, size_t inlen2);

/* SHAKE256 stateful stream */
void bit_xof256_init(bit_xof256_state *st, const uint8_t *seed, size_t seedlen, uint16_t nonce);
int bit_xof256_squeeze(bit_xof256_state *st, uint8_t *out, size_t outlen);
void bit_xof256_zeroize(bit_xof256_state *st);

/* SHAKE256 block-level context */
void bit_xof256_ctx_init(bit_xof256_ctx *ctx, const uint8_t *seed, size_t seedlen);
int bit_xof256_ctx_squeezeblocks(bit_xof256_ctx *ctx, uint8_t *out, size_t nblocks);
void bit_xof256_ctx_zeroize(bit_xof256_ctx *ctx);

/* SHAKE128 stateful stream (matrix expansion) */
void bit_xof128_init(bit_xof128_state *st, const uint8_t *in, size_t inlen);
int bit_xof128_squeeze(bit_xof128_state *st, uint8_t *out, size_t outlen);
void bit_xof128_zeroize(bit_xof128_state *st);

#ifdef __cplusplus
}
#endif
#endif
