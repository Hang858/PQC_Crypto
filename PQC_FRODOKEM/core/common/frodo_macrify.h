/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: header for internal functions
*********************************************************************************************/

#ifndef _FRODO_MACRIFY_H_
#define _FRODO_MACRIFY_H_

#include <stddef.h>
#include <stdint.h>
#include "config.h"
#include "parameters.h"

// SHA3/SHAKE incremental operator state size, in uint64 words (208 bytes: 25-word
// Keccak state + 1 bookkeeping word). Matches OP_STATE_SIZE_SHA3 in the hash operator.
#define FRODO_SHA3_STATE_U64 26


void frodo_pack(unsigned char *out, const size_t outlen, const uint16_t *in, const size_t inlen, const unsigned char lsb);
void frodo_unpack(uint16_t *out, const size_t outlen, const unsigned char *in, const size_t inlen, const unsigned char lsb);
void frodo_sample_n(uint16_t *s, const size_t n);
int8_t ct_verify(const uint16_t *a, const uint16_t *b, size_t len);
int8_t ct_verify_packed(const uint16_t *b, const uint8_t *packed, size_t nelem, unsigned int logq);
void ct_select(uint8_t *r, const uint8_t *a, const uint8_t *b, size_t len, int8_t selector);
void clear_bytes(uint8_t *mem, size_t n);

int frodo_mul_add_as_plus_e(uint16_t *b, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
int frodo_mul_add_sa_plus_e(uint16_t *b, const uint16_t *s, const uint8_t *seed_A);
void frodo_mul_add_sb_plus_e(uint16_t *out, const uint8_t *b_packed, const uint16_t *s, const uint16_t *e);
void frodo_mul_bs(uint16_t *out, const uint16_t *b, const uint16_t *s);

void frodo_add(uint16_t *out, const uint16_t *a, const uint16_t *b);
void frodo_sub(uint16_t *out, const uint16_t *a, const uint16_t *b);
void frodo_key_encode(uint16_t *out, const uint16_t *in);
void frodo_key_decode(uint16_t *out, const uint16_t *in);

// Low-memory column-tiled variant: computes one 8-column tile of (s¡ÁA + e).
// s:        N_BAR ¡Á N matrix (full, native endian)
// seed_A:   seed for A matrix generation (column-block by column-block)
// col_block: starting column index (0, 8, 16, ¡­)
// out_tile: accumulator (N_BAR ¡Á 8), initialized from e_tile by caller
// hash_state: the caller-owned, single active SHAKE128 state
// discard: caller-owned scratch used only for skipped SHAKE output
int frodo_mul_add_sa_tile(uint16_t out_tile[8][8],
                           const uint16_t *s,
                           const uint8_t *seed_A,
                           int col_block,
                           uint64_t hash_state[FRODO_SHA3_STATE_U64],
                           uint8_t *discard,
                           size_t discard_len);

// Low-memory keypair helpers:
// 1) consume the post-S SHAKE stream and store sampled E packed in pk_b;
// 2) compute B = A*S + E by unpacking each packed-E row block from pk_b,
//    generating A rows with a single SHAKE128 state at a time, and packing
//    the final B row block back into pk_b.
int frodo_pack_e_from_state(uint8_t *pk_b,
                            uint64_t e_st[FRODO_SHA3_STATE_U64],
                            uint8_t shake_alg);
int frodo_mul_add_as_plus_packed_e_from_sk(uint8_t *pk_b,
                                           const uint8_t *sk_S,
                                           const uint8_t *seed_A);

#endif
