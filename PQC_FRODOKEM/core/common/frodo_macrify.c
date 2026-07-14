/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: matrix arithmetic functions used by the KEM
*********************************************************************************************/
#include "frodo_macrify.h"
#include "operator_interface.h"
#include "fips202.h"
#include "malloc.h"
#include "memops.h"


int frodo_mul_add_as_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A)
{ // Generate-and-multiply: generate matrix A (N x N) row-wise, multiply by s on the right.
  // Inputs: s, e (N x N_BAR)
  // Output: out = A*s + e (N x N_BAR)
  // Low-memory: A is streamed one 8x8 tile at a time using 8 incremental SHAKE states
  // (one per row of the current row-block), so no full A row buffer is held.
    int i, j;

    for (i = 0; i < (PARAMS_N*PARAMS_NBAR); i += 2) {
        *((uint32_t*)&out[i]) = *((uint32_t*)&e[i]);
    }

    uint8_t seed_A_separated[2 + BYTES_SEED_A];
    uint16_t* seed_A_origin = (uint16_t*)&seed_A_separated;
    memcpy(&seed_A_separated[2], seed_A, BYTES_SEED_A);

    // Eight SHAKE states occupy 1,664 B. Keep this workspace off the
    // embedded stack and reuse it for every row block in this invocation.
    uint64_t (*a_state)[FRODO_SHA3_STATE_U64] = malloc(8 * sizeof(*a_state));
    if (a_state == NULL) {
        return 0;
    }

    for (i = 0; i < PARAMS_N; i += 8) {
        for (int r = 0; r < 8; r++) {
            seed_A_origin[0] = UINT16_TO_LE(i + r);
            OP_hash_init(OP_ALG_SHAKE128, a_state[r], (int)sizeof(a_state[r]));
            OP_hash_absorb(OP_ALG_SHAKE128, a_state[r], (int)sizeof(a_state[r]), seed_A_separated, 2 + BYTES_SEED_A);
        }

        uint16_t acc[8][8];
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < PARAMS_NBAR; c++) {
                acc[r][c] = out[(i + r)*PARAMS_NBAR + c];
            }
        }

        for (j = 0; j < PARAMS_N; j += 8) {
            uint16_t x[8][8], y[8][8];
            for (int r = 0; r < 8; r++) {
                uint8_t abuf[16];  // 8 uint16 of row (i+r), columns j..j+7
                OP_hash_squeeze(OP_ALG_SHAKE128, a_state[r], (int)sizeof(a_state[r]), abuf, (int)sizeof(abuf));
                for (int c = 0; c < 8; c++) {
                    x[r][c] = (uint16_t)(abuf[2*c] | ((uint16_t)abuf[2*c + 1] << 8));
                    y[r][c] = s[c*PARAMS_N + (j + r)];
                }
            }

            uint16_t z[8][8];
            if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x, (const uint16_t (*)[8])y, (uint16_t)PARAMS_Q) != OP_SUCCESS) {
                clear_bytes((uint8_t *)a_state, 8 * sizeof(*a_state));
                free(a_state);
                return 0;
            }
            for (int r = 0; r < 8; r++) {
                for (int c = 0; c < 8; c++) {
                    acc[r][c] = (uint16_t)(acc[r][c] + z[r][c]);
                }
            }
        }

        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < PARAMS_NBAR; c++) {
                out[(i + r)*PARAMS_NBAR + c] = acc[r][c];
            }
        }
    }

    clear_bytes((uint8_t *)a_state, 8 * sizeof(*a_state));
    free(a_state);
    return 1;
}


int frodo_mul_add_sa_plus_e(uint16_t *out, const uint16_t *s, const uint8_t *seed_A)
{ // Generate-and-multiply: generate matrix A (N x N) column-wise, multiply by s' on the left.
  // Inputs: s' (N_BAR x N); out must already contain the error e' (N_BAR x N)
  // Output: out = s'*A + e' (N_BAR x N), accumulated in place
    int i, q_block;

    uint8_t seed_A_separated[2 + BYTES_SEED_A];
    uint16_t* seed_A_origin = (uint16_t*)&seed_A_separated;
    memcpy(&seed_A_separated[2], seed_A, BYTES_SEED_A);

    // Eight SHAKE states occupy 1,664 B. Keep this workspace off the
    // embedded stack and reuse it for every row block in this invocation.
    uint64_t (*a_state)[FRODO_SHA3_STATE_U64] = malloc(8 * sizeof(*a_state));
    if (a_state == NULL) {
        return 0;
    }

    for (i = 0; i < PARAMS_N; i += 8) {
        for (int r = 0; r < 8; r++) {
            seed_A_origin[0] = UINT16_TO_LE(i + r);
            OP_hash_init(OP_ALG_SHAKE128, a_state[r], (int)sizeof(a_state[r]));
            OP_hash_absorb(OP_ALG_SHAKE128, a_state[r], (int)sizeof(a_state[r]), seed_A_separated, 2 + BYTES_SEED_A);
        }

        uint16_t x[8][8];
        for (int r = 0; r < PARAMS_NBAR; r++) {
            for (int c = 0; c < 8; c++) {
                x[r][c] = s[r*PARAMS_N + (i + c)];
            }
        }

        for (q_block = 0; q_block < PARAMS_N; q_block += 8) {
            uint16_t y[8][8];
            for (int r = 0; r < 8; r++) {
                uint8_t abuf[16];  // 8 uint16 of row (i+r), columns q_block..q_block+7
                OP_hash_squeeze(OP_ALG_SHAKE128, a_state[r], (int)sizeof(a_state[r]), abuf, (int)sizeof(abuf));
                for (int c = 0; c < 8; c++) {
                    y[r][c] = (uint16_t)(abuf[2*c] | ((uint16_t)abuf[2*c + 1] << 8));
                }
            }

            uint16_t z[8][8];
            if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x, (const uint16_t (*)[8])y, (uint16_t)PARAMS_Q) != OP_SUCCESS) {
                clear_bytes((uint8_t *)a_state, 8 * sizeof(*a_state));
                free(a_state);
                return 0;
            }
            for (int r = 0; r < PARAMS_NBAR; r++) {
                for (int c = 0; c < 8; c++) {
                    out[r*PARAMS_N + (q_block + c)] = (uint16_t)(out[r*PARAMS_N + (q_block + c)] + z[r][c]);
                }
            }
        }
    }

    clear_bytes((uint8_t *)a_state, 8 * sizeof(*a_state));
    free(a_state);
    return 1;
}


void frodo_mul_bs(uint16_t *out, const uint16_t *b, const uint16_t *s)
{ // Multiply by s on the right
  // Inputs: b (N_BAR x N), s (N x N_BAR)
  // Output: out = b*s (N_BAR x N_BAR)
    int block, i, j, k;
    uint16_t q = (uint16_t)PARAMS_Q;

    memset(out, 0, PARAMS_NBAR * PARAMS_NBAR * sizeof(uint16_t));
    for (block = 0; block < PARAMS_N; block += 8) {
        uint16_t x[8][8], y[8][8];
        for (i = 0; i < 8; i++) {
            for (k = 0; k < 8; k++) {
                x[i][k] = b[i * PARAMS_N + block + k];
                y[k][i] = s[i * PARAMS_N + block + k];
            }
        }

        uint16_t z[8][8];
        if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x, (const uint16_t (*)[8])y, q) != OP_SUCCESS) {
            return;
        }
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                out[i * PARAMS_NBAR + j] = (uint16_t)(out[i * PARAMS_NBAR + j] + z[i][j]);
            }
        }
    }
    for (i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++) {
        out[i] = (uint16_t)(out[i] & ((1 << PARAMS_LOGQ) - 1));
    }
}


void frodo_mul_add_sb_plus_e(uint16_t *out, const uint8_t *b_packed, const uint16_t *s, const uint16_t *e)
{ // Multiply by s on the left
  // Inputs: b_packed = packed B (N x N_BAR), s (N_BAR x N), e (N_BAR x N_BAR)
  // Output: out = s*b + e (N_BAR x N_BAR)
  // Low-memory: B is unpacked one 8-row tile at a time from its packed form (each 8-row tile
  // is exactly NBAR*LOGQ bytes, byte-aligned), so the full unpacked B is never materialized.
    int block, i, j, k;
    uint16_t q = (uint16_t)PARAMS_Q;
    const size_t row_bytes = (size_t)PARAMS_NBAR * PARAMS_LOGQ / 8;  // bytes per row of B

    memcpy(out, e, PARAMS_NBAR * PARAMS_NBAR * sizeof(uint16_t));
    for (block = 0; block < PARAMS_N; block += 8) {
        uint16_t btile[8 * PARAMS_NBAR];  // 8 rows x N_BAR of B
        frodo_unpack(btile, 8 * PARAMS_NBAR, b_packed + (size_t)block * row_bytes,
                     8 * row_bytes, PARAMS_LOGQ);

        uint16_t x[8][8], y[8][8];
        for (i = 0; i < 8; i++) {
            for (k = 0; k < 8; k++) {
                x[i][k] = s[i * PARAMS_N + block + k];
                y[k][i] = btile[k * PARAMS_NBAR + i];
            }
        }

        uint16_t z[8][8];
        if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x, (const uint16_t (*)[8])y, q) != OP_SUCCESS) {
            return;
        }
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                out[i * PARAMS_NBAR + j] = (uint16_t)(out[i * PARAMS_NBAR + j] + z[i][j]);
            }
        }
    }
    for (i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++) {
        out[i] = (uint16_t)(out[i] & ((1 << PARAMS_LOGQ) - 1));
    }
}


void frodo_add(uint16_t *out, const uint16_t *a, const uint16_t *b)
{ // Add a and b
  // Inputs: a, b (N_BAR x N_BAR)
  // Output: c = a + b

    for (int i = 0; i < (PARAMS_NBAR*PARAMS_NBAR); i++) {
        out[i] = (a[i] + b[i]) & ((1<<PARAMS_LOGQ)-1);
    }
}


void frodo_sub(uint16_t *out, const uint16_t *a, const uint16_t *b)
{ // Subtract a and b
  // Inputs: a, b (N_BAR x N_BAR)
  // Output: c = a - b

    for (int i = 0; i < (PARAMS_NBAR*PARAMS_NBAR); i++) {
        out[i] = (a[i] - b[i]) & ((1<<PARAMS_LOGQ)-1);
    }
}


void frodo_key_encode(uint16_t *out, const uint16_t *in)
{ // Encoding
    unsigned int i, j, npieces_word = 8;
    unsigned int nwords = (PARAMS_NBAR*PARAMS_NBAR)/8;
    uint64_t temp, mask = ((uint64_t)1 << PARAMS_EXTRACTED_BITS) - 1;
    uint16_t* pos = out;

    for (i = 0; i < nwords; i++) {
        temp = 0;
        for(j = 0; j < PARAMS_EXTRACTED_BITS; j++)
            temp |= ((uint64_t)((uint8_t*)in)[i*PARAMS_EXTRACTED_BITS + j]) << (8*j);
        for (j = 0; j < npieces_word; j++) {
            *pos = (uint16_t)((temp & mask) << (PARAMS_LOGQ - PARAMS_EXTRACTED_BITS));
            temp >>= PARAMS_EXTRACTED_BITS;
            pos++;
        }
    }
}


void frodo_key_decode(uint16_t *out, const uint16_t *in)
{ // Decoding
    unsigned int i, j, index = 0, npieces_word = 8;
    unsigned int nwords = (PARAMS_NBAR * PARAMS_NBAR) / 8;
    uint16_t temp, maskex=((uint16_t)1 << PARAMS_EXTRACTED_BITS) -1, maskq =((uint16_t)1 << PARAMS_LOGQ) -1;
    uint8_t  *pos = (uint8_t*)out;
    uint64_t templong;

    for (i = 0; i < nwords; i++) {
        templong = 0;
        for (j = 0; j < npieces_word; j++) {  // temp = floor(in*2^{-11}+0.5)
            temp = ((in[index] & maskq) + (1 << (PARAMS_LOGQ - PARAMS_EXTRACTED_BITS - 1))) >> (PARAMS_LOGQ - PARAMS_EXTRACTED_BITS);
            templong |= ((uint64_t)(temp & maskex)) << (PARAMS_EXTRACTED_BITS * j);
            index++;
        }
	for(j = 0; j < PARAMS_EXTRACTED_BITS; j++)
	    pos[i*PARAMS_EXTRACTED_BITS + j] = (templong >> (8*j)) & 0xFF;
    }
}


int frodo_mul_add_sa_tile(uint16_t out_tile[8][8],
                           const uint16_t *s,
                           const uint16_t e_tile[8][8],
                           const uint8_t *seed_A,
                           int col_block)
{ // Low-memory column-tiled variant: compute one 8-column tile of s x A + e.
  // s:      N_BAR x N  (full, native endian)
  // e_tile: N_BAR x 8  (pre-sampled error for these columns)
  // out_tile: accumulator, caller must initialize from e_tile before calling.
    int i_block;

    uint8_t seed_A_separated[2 + BYTES_SEED_A];
    memcpy(&seed_A_separated[2], seed_A, BYTES_SEED_A);
    uint16_t *seed_A_origin = (uint16_t *)&seed_A_separated;

    uint64_t (*a_state)[FRODO_SHA3_STATE_U64] = malloc(8 * sizeof(*a_state));
    if (a_state == NULL) {
        return 0;
    }

    for (i_block = 0; i_block < PARAMS_N; i_block += 8) {
        // Initialize 8 SHAKE128 states for A rows i_block .. i_block+7
        for (int r = 0; r < 8; r++) {
            seed_A_origin[0] = UINT16_TO_LE((uint16_t)(i_block + r));
            OP_hash_init(OP_ALG_SHAKE128, a_state[r], (int)sizeof(a_state[r]));
            OP_hash_absorb(OP_ALG_SHAKE128, a_state[r], (int)sizeof(a_state[r]),
                           seed_A_separated, 2 + BYTES_SEED_A);
            // Skip preceding column blocks by squeezing & discarding
            uint8_t discard[16];
            for (int skip = 0; skip < col_block; skip += 8) {
                OP_hash_squeeze(OP_ALG_SHAKE128, a_state[r], (int)sizeof(a_state[r]),
                                discard, (int)sizeof(discard));
            }
        }

        // Read s[i_block] -- 8 consecutive columns of s (N_BAR rows x 8 cols)
        uint16_t x[8][8];
        for (int r = 0; r < PARAMS_NBAR; r++) {
            for (int c = 0; c < 8; c++) {
                x[r][c] = s[r * PARAMS_N + (i_block + c)];
            }
        }

        // Generate A[i_block][col_block] -- one 8x8 tile
        uint16_t y[8][8];
        for (int r = 0; r < 8; r++) {
            uint8_t abuf[16];
            OP_hash_squeeze(OP_ALG_SHAKE128, a_state[r], (int)sizeof(a_state[r]),
                            abuf, (int)sizeof(abuf));
            for (int c = 0; c < 8; c++) {
                y[r][c] = (uint16_t)(abuf[2 * c] | ((uint16_t)abuf[2 * c + 1] << 8));
            }
        }

        // Multiply and accumulate
        uint16_t z[8][8];
        if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x,
                              (const uint16_t (*)[8])y,
                              (uint16_t)PARAMS_Q) != OP_SUCCESS) {
            clear_bytes((uint8_t *)a_state, 8 * sizeof(*a_state));
            free(a_state);
            return 0;
        }
        for (int r = 0; r < PARAMS_NBAR; r++) {
            for (int c = 0; c < 8; c++) {
                out_tile[r][c] = (uint16_t)(out_tile[r][c] + z[r][c]);
            }
        }
    }

    clear_bytes((uint8_t *)a_state, 8 * sizeof(*a_state));
    free(a_state);
    return 1;
}


int frodo_mul_add_as_plus_e_from_sk(uint8_t *pk_b,
                                    const uint8_t *sk_S,
                                    uint64_t e_st[FRODO_SHA3_STATE_U64],
                                    uint8_t shake_alg,
                                    const uint8_t *seed_A)
{ // Low-memory keypair variant: B = A x S + E, row-block by row-block.
  // S is read from sk_S (N x N_BAR stored column-major, LE uint16).
  // E is squeezed incrementally from e_st (SHAKE state already past the S squeeze).
  // Each 8-row block of B is packed into pk_b immediately.
    int i_block, j_block;
    const size_t row_bytes = (size_t)PARAMS_NBAR * PARAMS_LOGQ / 8;

    uint8_t seed_A_separated[2 + BYTES_SEED_A];
    memcpy(&seed_A_separated[2], seed_A, BYTES_SEED_A);
    uint16_t *seed_A_origin = (uint16_t *)&seed_A_separated;

    // Eight SHAKE states occupy 1,664 B. Allocate them once and reuse them
    // for every row block instead of putting them in this call frame.
    uint64_t (*a_state)[FRODO_SHA3_STATE_U64] = malloc(8 * sizeof(*a_state));
    if (a_state == NULL) {
        return 0;
    }

    for (i_block = 0; i_block < PARAMS_N; i_block += 8) {
        // Squeeze E[i_block] (8 rows x N_BAR cols) from the incremental SHAKE state
        uint16_t e_tile[64];
        OP_hash_squeeze(shake_alg, e_st, (int)(FRODO_SHA3_STATE_U64 * sizeof(uint64_t)),
                        (uint8_t *)e_tile, (int)(8 * PARAMS_NBAR * sizeof(uint16_t)));
        for (int k = 0; k < 8 * PARAMS_NBAR; k++) {
            e_tile[k] = LE_TO_UINT16(e_tile[k]);
        }
        frodo_sample_n(e_tile, (size_t)(8 * PARAMS_NBAR));

        // Initialize B_acc[8][NBAR] from e_tile
        uint16_t B_acc[8][8];
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < PARAMS_NBAR; c++) {
                B_acc[r][c] = e_tile[r * PARAMS_NBAR + c];
            }
        }

        // Initialize 8 SHAKE128 states for A rows i_block .. i_block+7
        for (int r = 0; r < 8; r++) {
            seed_A_origin[0] = UINT16_TO_LE((uint16_t)(i_block + r));
            OP_hash_init(OP_ALG_SHAKE128, a_state[r], (int)sizeof(a_state[r]));
            OP_hash_absorb(OP_ALG_SHAKE128, a_state[r], (int)sizeof(a_state[r]),
                           seed_A_separated, 2 + BYTES_SEED_A);
        }

        // Multiply A[i_block] x S for all column blocks of A
        for (j_block = 0; j_block < PARAMS_N; j_block += 8) {
            // Read S[j_block] from sk_S -- S is stored column-major in LE
            uint16_t S_tile[8][8];
            for (int r = 0; r < 8; r++) {
                for (int c = 0; c < PARAMS_NBAR; c++) {
                    size_t idx = (size_t)c * PARAMS_N + (size_t)(j_block + r);
                    S_tile[r][c] = LE_TO_UINT16(((const uint16_t *)sk_S)[idx]);
                }
            }

            // Generate A[i_block][j_block] from SHAKE states
            uint16_t x_tile[8][8];
            for (int r = 0; r < 8; r++) {
                uint8_t abuf[16];
                OP_hash_squeeze(OP_ALG_SHAKE128, a_state[r], (int)sizeof(a_state[r]),
                                abuf, (int)sizeof(abuf));
                for (int c = 0; c < 8; c++) {
                    x_tile[r][c] = (uint16_t)(abuf[2 * c] | ((uint16_t)abuf[2 * c + 1] << 8));
                }
            }

            // Compute 8x8 matrix multiplication: B_acc += A_tile x S_tile
            uint16_t z[8][8];
            if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x_tile,
                                  (const uint16_t (*)[8])S_tile,
                                  (uint16_t)PARAMS_Q) != OP_SUCCESS) {
                clear_bytes((uint8_t *)a_state, 8 * sizeof(*a_state));
                free(a_state);
                return 0;
            }
            for (int r = 0; r < 8; r++) {
                for (int c = 0; c < PARAMS_NBAR; c++) {
                    B_acc[r][c] = (uint16_t)(B_acc[r][c] + z[r][c]);
                }
            }
        }

        // Pack this 8-row block of B into pk_b
        frodo_pack(pk_b + (size_t)i_block * row_bytes,
                   row_bytes * 8,
                   (const uint16_t *)B_acc, 8 * PARAMS_NBAR, PARAMS_LOGQ);
    }

    clear_bytes((uint8_t *)a_state, 8 * sizeof(*a_state));
    free(a_state);
    return 1;
}
