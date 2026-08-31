/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: matrix arithmetic functions used by the KEM
*********************************************************************************************/
#include "frodo_macrify.h"
#include "my_log.h"
#include "operator_interface.h"
#include "fips202.h"
#include "malloc.h"
#include "memops.h"


int frodo_mul_add_as_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A)
{ // Generate-and-multiply: generate matrix A (N x N) row-wise, multiply by s on the right.
  // Inputs: s, e (N x N_BAR)
  // Output: out = A*s + e (N x N_BAR)
    int i, j;

    for (i = 0; i < (PARAMS_N*PARAMS_NBAR); i += 2) {
        *((uint32_t*)&out[i]) = *((uint32_t*)&e[i]);
    }

    uint8_t seed_A_separated[2 + BYTES_SEED_A];
    uint16_t* seed_A_origin = (uint16_t*)&seed_A_separated;
    memcpy(&seed_A_separated[2], seed_A, BYTES_SEED_A);

    uint16_t *a_rows = malloc((size_t)8 * PARAMS_N * sizeof(*a_rows));
    uint64_t *a_state = malloc(FRODO_SHA3_STATE_U64 * sizeof(*a_state));
    if (a_rows == NULL || a_state == NULL) {
        free(a_rows);
        free(a_state);
        return 0;
    }

    for (i = 0; i < PARAMS_N; i += 8) {
        for (int r = 0; r < 8; r++) {
            uint8_t *row_bytes = (uint8_t *)&a_rows[(size_t)r * PARAMS_N];
            seed_A_origin[0] = UINT16_TO_LE(i + r);
            OP_hash_init(OP_ALG_SHAKE128, a_state,
                         (int)(FRODO_SHA3_STATE_U64 * sizeof(*a_state)));
            OP_hash_absorb(OP_ALG_SHAKE128, a_state,
                           (int)(FRODO_SHA3_STATE_U64 * sizeof(*a_state)),
                           seed_A_separated, 2 + BYTES_SEED_A);
            OP_hash_squeeze(OP_ALG_SHAKE128, a_state,
                            (int)(FRODO_SHA3_STATE_U64 * sizeof(*a_state)),
                            row_bytes, (int)(PARAMS_N * sizeof(uint16_t)));
            for (int c = 0; c < PARAMS_N; c++) {
                a_rows[(size_t)r * PARAMS_N + c] =
                    (uint16_t)(row_bytes[2*c] | ((uint16_t)row_bytes[2*c + 1] << 8));
            }
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
                for (int c = 0; c < 8; c++) {
                    x[r][c] = a_rows[(size_t)r * PARAMS_N + (j + c)];
                    y[r][c] = s[c*PARAMS_N + (j + r)];
                }
            }

            uint16_t z[8][8];
            if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x, (const uint16_t (*)[8])y, (uint16_t)PARAMS_Q) != OP_SUCCESS) {
                clear_bytes((uint8_t *)a_rows, (size_t)8 * PARAMS_N * sizeof(*a_rows));
                clear_bytes((uint8_t *)a_state, FRODO_SHA3_STATE_U64 * sizeof(*a_state));
                free(a_rows);
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

    clear_bytes((uint8_t *)a_rows, (size_t)8 * PARAMS_N * sizeof(*a_rows));
    clear_bytes((uint8_t *)a_state, FRODO_SHA3_STATE_U64 * sizeof(*a_state));
    free(a_rows);
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

    uint16_t *a_rows = malloc((size_t)8 * PARAMS_N * sizeof(*a_rows));
    uint64_t *a_state = malloc(FRODO_SHA3_STATE_U64 * sizeof(*a_state));
    if (a_rows == NULL || a_state == NULL) {
        free(a_rows);
        free(a_state);
        return 0;
    }

    for (i = 0; i < PARAMS_N; i += 8) {
        for (int r = 0; r < 8; r++) {
            uint8_t *row_bytes = (uint8_t *)&a_rows[(size_t)r * PARAMS_N];
            seed_A_origin[0] = UINT16_TO_LE(i + r);
            OP_hash_init(OP_ALG_SHAKE128, a_state,
                         (int)(FRODO_SHA3_STATE_U64 * sizeof(*a_state)));
            OP_hash_absorb(OP_ALG_SHAKE128, a_state,
                           (int)(FRODO_SHA3_STATE_U64 * sizeof(*a_state)),
                           seed_A_separated, 2 + BYTES_SEED_A);
            OP_hash_squeeze(OP_ALG_SHAKE128, a_state,
                            (int)(FRODO_SHA3_STATE_U64 * sizeof(*a_state)),
                            row_bytes, (int)(PARAMS_N * sizeof(uint16_t)));
            for (int c = 0; c < PARAMS_N; c++) {
                a_rows[(size_t)r * PARAMS_N + c] =
                    (uint16_t)(row_bytes[2*c] | ((uint16_t)row_bytes[2*c + 1] << 8));
            }
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
                for (int c = 0; c < 8; c++) {
                    y[r][c] = a_rows[(size_t)r * PARAMS_N + (q_block + c)];
                }
//                if(((PARAMS_N-1)==i)&&((PARAMS_N-1)==q_block))
                if((8==i)&&(8==q_block))
                {
                    uint8_t abuf[16];  // 8 uint16 of row (i+r), columns q_block..q_block+7
                    for (int c = 0; c < 8; c++) {
                        abuf[2*c] = (uint8_t)y[r][c];
                        abuf[2*c + 1] = (uint8_t)(y[r][c] >> 8);
                    }
                	LOG_A("en-abuf", abuf, (int)sizeof(abuf));
                }
            }

            uint16_t z[8][8];
            if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x, (const uint16_t (*)[8])y, (uint16_t)PARAMS_Q) != OP_SUCCESS) {
                clear_bytes((uint8_t *)a_rows, (size_t)8 * PARAMS_N * sizeof(*a_rows));
                clear_bytes((uint8_t *)a_state, FRODO_SHA3_STATE_U64 * sizeof(*a_state));
                free(a_rows);
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

    clear_bytes((uint8_t *)a_rows, (size_t)8 * PARAMS_N * sizeof(*a_rows));
    clear_bytes((uint8_t *)a_state, FRODO_SHA3_STATE_U64 * sizeof(*a_state));
    free(a_rows);
    free(a_state);
    return 1;
}


int frodo_mul_add_sa_plus_packed_e(uint8_t *out_packed, const uint16_t *s, const uint8_t *seed_A)
{ // out_packed initially contains packed e'. It is overwritten with packed s'*A + e'.
    int i, q_block;
    const size_t group_bytes = (size_t)PARAMS_NBAR * PARAMS_LOGQ / 8;
    const size_t row_stride = (size_t)(PARAMS_N / 8) * group_bytes;

    uint8_t seed_A_separated[2 + BYTES_SEED_A];
    uint16_t* seed_A_origin = (uint16_t*)&seed_A_separated;
    memcpy(&seed_A_separated[2], seed_A, BYTES_SEED_A);

    uint16_t *a_rows = malloc((size_t)8 * PARAMS_N * sizeof(*a_rows));
    uint64_t *a_state = malloc(FRODO_SHA3_STATE_U64 * sizeof(*a_state));
    if (a_rows == NULL || a_state == NULL) {
        free(a_rows);
        free(a_state);
        return 0;
    }

    for (i = 0; i < PARAMS_N; i += 8) {
        for (int r = 0; r < 8; r++) {
            uint8_t *row_bytes = (uint8_t *)&a_rows[(size_t)r * PARAMS_N];
            seed_A_origin[0] = UINT16_TO_LE(i + r);
            OP_hash_init(OP_ALG_SHAKE128, a_state,
                         (int)(FRODO_SHA3_STATE_U64 * sizeof(*a_state)));
            OP_hash_absorb(OP_ALG_SHAKE128, a_state,
                           (int)(FRODO_SHA3_STATE_U64 * sizeof(*a_state)),
                           seed_A_separated, 2 + BYTES_SEED_A);
            OP_hash_squeeze(OP_ALG_SHAKE128, a_state,
                            (int)(FRODO_SHA3_STATE_U64 * sizeof(*a_state)),
                            row_bytes, (int)(PARAMS_N * sizeof(uint16_t)));
            for (int c = 0; c < PARAMS_N; c++) {
                a_rows[(size_t)r * PARAMS_N + c] =
                    (uint16_t)(row_bytes[2*c] | ((uint16_t)row_bytes[2*c + 1] << 8));
            }
        }

        uint16_t x[8][8];
        for (int r = 0; r < PARAMS_NBAR; r++) {
            for (int c = 0; c < 8; c++) {
                x[r][c] = s[r*PARAMS_N + (i + c)];
            }
        }

        for (q_block = 0; q_block < PARAMS_N; q_block += 8) {
            uint16_t y[8][8];
            uint16_t out_tile[8][8];
            int g = q_block / 8;

            for (int r = 0; r < 8; r++) {
                for (int c = 0; c < 8; c++) {
                    y[r][c] = a_rows[(size_t)r * PARAMS_N + (q_block + c)];
                }
            }

            uint16_t z[8][8];
            if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x,
                                  (const uint16_t (*)[8])y,
                                  (uint16_t)PARAMS_Q) != OP_SUCCESS) {
                clear_bytes((uint8_t *)a_rows, (size_t)8 * PARAMS_N * sizeof(*a_rows));
                clear_bytes((uint8_t *)a_state, FRODO_SHA3_STATE_U64 * sizeof(*a_state));
                free(a_rows);
                free(a_state);
                return 0;
            }

            for (int r = 0; r < PARAMS_NBAR; r++) {
                uint8_t *packed_row = out_packed + (size_t)r * row_stride + (size_t)g * group_bytes;
                frodo_unpack(&out_tile[r][0], PARAMS_NBAR, packed_row, group_bytes, PARAMS_LOGQ);
                for (int c = 0; c < 8; c++) {
                    out_tile[r][c] = (uint16_t)(out_tile[r][c] + z[r][c]);
                }
                frodo_pack(packed_row, group_bytes, &out_tile[r][0], PARAMS_NBAR, PARAMS_LOGQ);
            }
        }
    }

    clear_bytes((uint8_t *)a_rows, (size_t)8 * PARAMS_N * sizeof(*a_rows));
    clear_bytes((uint8_t *)a_state, FRODO_SHA3_STATE_U64 * sizeof(*a_state));
    free(a_rows);
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
                           const uint8_t *seed_A,
                           int col_block,
                           uint64_t hash_state[FRODO_SHA3_STATE_U64],
                           uint8_t *discard,
                           size_t discard_len)
{ // Low-memory column-tiled variant: compute one 8-column tile of s x A + e.
  // s:      N_BAR x N  (full, native endian)
  // out_tile: accumulator, caller must initialize it from e before calling.
  // Only one SHAKE128 state is active. Each A row is generated to completion
  // before the same state workspace is reinitialized for the next row.
    int i_block;

    uint8_t seed_A_separated[2 + BYTES_SEED_A];
    memcpy(&seed_A_separated[2], seed_A, BYTES_SEED_A);
    uint16_t *seed_A_origin = (uint16_t *)&seed_A_separated;

    if (hash_state == NULL || discard == NULL || discard_len == 0) {
        return 0;
    }

    for (i_block = 0; i_block < PARAMS_N; i_block += 8) {
        // Read s[i_block] -- 8 consecutive columns of s (N_BAR rows x 8 cols)
        uint16_t x[8][8];
        for (int r = 0; r < PARAMS_NBAR; r++) {
            for (int c = 0; c < 8; c++) {
                x[r][c] = s[r * PARAMS_N + (i_block + c)];
            }
        }

        // Generate A[i_block][col_block] one row at a time. Reinitializing the
        // sole workspace means no SHAKE state ever has to be saved or restored.
        uint16_t y[8][8];
        for (int r = 0; r < 8; r++) {
            seed_A_origin[0] = UINT16_TO_LE((uint16_t)(i_block + r));
            OP_hash_init(OP_ALG_SHAKE128, hash_state,
                         (int)(FRODO_SHA3_STATE_U64 * sizeof(*hash_state)));
            OP_hash_absorb(OP_ALG_SHAKE128, hash_state,
                           (int)(FRODO_SHA3_STATE_U64 * sizeof(*hash_state)),
                           seed_A_separated, 2 + BYTES_SEED_A);

            size_t skip_bytes = (size_t)col_block * sizeof(uint16_t);
            while (skip_bytes > 0) {
                size_t n = (skip_bytes > discard_len) ? discard_len : skip_bytes;
                OP_hash_squeeze(OP_ALG_SHAKE128, hash_state,
                                (int)(FRODO_SHA3_STATE_U64 * sizeof(*hash_state)),
                                discard, (int)n);
                skip_bytes -= n;
            }
            uint8_t abuf[16];
            OP_hash_squeeze(OP_ALG_SHAKE128, hash_state,
                            (int)(FRODO_SHA3_STATE_U64 * sizeof(*hash_state)),
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
            return 0;
        }
        for (int r = 0; r < PARAMS_NBAR; r++) {
            for (int c = 0; c < 8; c++) {
                out_tile[r][c] = (uint16_t)(out_tile[r][c] + z[r][c]);
            }
        }
    }

    return 1;
}


int frodo_pack_e_from_state(uint8_t *pk_b,
                            uint64_t e_st[FRODO_SHA3_STATE_U64],
                            uint8_t shake_alg)
{ // Consume the post-S SHAKE stream, sample E row-block by row-block,
  // and store E packed in pk_b.  pk_b is later overwritten with final B.
    int i_block;
    const size_t row_bytes = (size_t)PARAMS_NBAR * PARAMS_LOGQ / 8;

    for (i_block = 0; i_block < PARAMS_N; i_block += 8) {
        uint16_t e_tile[64];
        OP_hash_squeeze(shake_alg, e_st, (int)(FRODO_SHA3_STATE_U64 * sizeof(uint64_t)),
                        (uint8_t *)e_tile, (int)(8 * PARAMS_NBAR * sizeof(uint16_t)));
        for (int k = 0; k < 8 * PARAMS_NBAR; k++) {
            e_tile[k] = LE_TO_UINT16(e_tile[k]);
        }
        frodo_sample_n(e_tile, (size_t)(8 * PARAMS_NBAR));

        frodo_pack(pk_b + (size_t)i_block * row_bytes,
                   row_bytes * 8,
                   e_tile, 8 * PARAMS_NBAR, PARAMS_LOGQ);
    }

    return 1;
}


int frodo_mul_add_as_plus_packed_e_from_sk(uint8_t *pk_b,
                                           const uint8_t *sk_S,
                                           const uint8_t *seed_A)
{ // Low-memory keypair variant for hardware SHAKE engines that cannot keep
  // multiple software-visible states alive.  E is already packed in pk_b.
  // A rows are generated one complete row at a time with a single SHAKE128
  // state, then consumed as 8x8 tiles to compute B = A*S + E.
    int i_block, j_block;
    const size_t row_bytes = (size_t)PARAMS_NBAR * PARAMS_LOGQ / 8;

    uint8_t seed_A_separated[2 + BYTES_SEED_A];
    memcpy(&seed_A_separated[2], seed_A, BYTES_SEED_A);
    uint16_t *seed_A_origin = (uint16_t *)&seed_A_separated;

    uint16_t *a_rows = malloc((size_t)8 * PARAMS_N * sizeof(uint16_t));
    uint64_t *a_state = malloc(FRODO_SHA3_STATE_U64 * sizeof(*a_state));
    if (a_rows == NULL || a_state == NULL) {
        free(a_rows);
        free(a_state);
        return 0;
    }

    for (i_block = 0; i_block < PARAMS_N; i_block += 8) {
        uint16_t B_acc[8][8];

        frodo_unpack((uint16_t *)B_acc, 8 * PARAMS_NBAR,
                     pk_b + (size_t)i_block * row_bytes,
                     row_bytes * 8, PARAMS_LOGQ);

        // Generate A rows i_block .. i_block+7.  Complete each row before
        // reinitialising SHAKE128 for the next row; no hash state is paused.
        for (int r = 0; r < 8; r++) {
            uint8_t *row_bytes_out = (uint8_t *)&a_rows[(size_t)r * PARAMS_N];
            seed_A_origin[0] = UINT16_TO_LE((uint16_t)(i_block + r));
            OP_hash_init(OP_ALG_SHAKE128, a_state,
                         (int)(FRODO_SHA3_STATE_U64 * sizeof(*a_state)));
            OP_hash_absorb(OP_ALG_SHAKE128, a_state,
                           (int)(FRODO_SHA3_STATE_U64 * sizeof(*a_state)),
                           seed_A_separated, 2 + BYTES_SEED_A);
            OP_hash_squeeze(OP_ALG_SHAKE128, a_state,
                            (int)(FRODO_SHA3_STATE_U64 * sizeof(*a_state)),
                            row_bytes_out, (int)(PARAMS_N * sizeof(uint16_t)));
            for (int c = 0; c < PARAMS_N; c++) {
                a_rows[(size_t)r * PARAMS_N + c] =
                    (uint16_t)(row_bytes_out[2 * c] |
                               ((uint16_t)row_bytes_out[2 * c + 1] << 8));
            }
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
            // LOG_A("S_tile", (uint8_t *)S_tile, 8 * PARAMS_NBAR * sizeof(uint16_t));

            // Generate A[i_block][j_block] from SHAKE states
            uint16_t x_tile[8][8];
            for (int r = 0; r < 8; r++) {
                for (int c = 0; c < 8; c++) {
                    x_tile[r][c] = a_rows[(size_t)r * PARAMS_N + (j_block + c)];
                }
            }
//            LOG_A("x_tile", (uint8_t *)x_tile, 8 * 8 * sizeof(uint16_t));

            // Compute 8x8 matrix multiplication: B_acc += A_tile x S_tile
            uint16_t z[8][8];
            if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x_tile,
                                  (const uint16_t (*)[8])S_tile,
                                  (uint16_t)PARAMS_Q) != OP_SUCCESS) {
                clear_bytes((uint8_t *)a_rows, (size_t)8 * PARAMS_N * sizeof(uint16_t));
                clear_bytes((uint8_t *)a_state, FRODO_SHA3_STATE_U64 * sizeof(*a_state));
                free(a_rows);
                free(a_state);
                return 0;
            }
            // LOG_A("z", (uint8_t *)z, 8 * 8 * sizeof(uint16_t));
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

    clear_bytes((uint8_t *)a_rows, (size_t)8 * PARAMS_N * sizeof(uint16_t));
    clear_bytes((uint8_t *)a_state, FRODO_SHA3_STATE_U64 * sizeof(*a_state));
    free(a_rows);
    free(a_state);
    return 1;
}
