/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: matrix arithmetic functions used by the KEM
*********************************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "frodo_macrify.h"
#include "operator_interface.h"
#include "fips202.h"


static int frodo_op_mul_add(uint16_t acc[8][8], uint16_t x[8][8], uint16_t y[8][8]) {
    uint16_t z[8][8];

    if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x, (const uint16_t (*)[8])y,
                          (uint16_t)PARAMS_Q) != OP_SUCCESS) {
        return 0;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            acc[i][j] = (uint16_t)(acc[i][j] + z[i][j]);
        }
    }
    return 1;
}

int frodo_mul_add_as_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A) 
{ // Generate-and-multiply: generate matrix A (N x N) row-wise, multiply by s on the right.
  // Inputs: s, e (N x N_BAR)
  // Output: out = A*s + e (N x N_BAR)
    int i, j;
    int16_t *A = calloc((size_t)PARAMS_N * PARAMS_N, sizeof(int16_t));
    if (A == NULL) {
        return 0;
    }
       
    // Matrix A generation using SHAKE128, done per 16*N-bit row.
    uint8_t seed_A_separated[2 + BYTES_SEED_A];
    memcpy(&seed_A_separated[2], seed_A, BYTES_SEED_A);
    for (i = 0; i < PARAMS_N; i++) {
        uint16_t row = UINT16_TO_LE((uint16_t)i);

        memcpy(seed_A_separated, &row, sizeof row);
        shake128((unsigned char*)(A + i*PARAMS_N), (unsigned long long)(2*PARAMS_N), seed_A_separated, 2 + BYTES_SEED_A);
    }

    for (i = 0; i < PARAMS_N * PARAMS_N; i++) {
        A[i] = LE_TO_UINT16(A[i]);
    }
    memcpy(out, e, PARAMS_NBAR * PARAMS_N * sizeof(uint16_t));  

    for (i = 0; i < PARAMS_N; i += 8) {                         // Matrix multiplication-addition A*s + e
        uint16_t acc[8][8];
        uint16_t x[8][8];
        uint16_t y[8][8];

        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                acc[r][c] = out[(i + r)*PARAMS_NBAR + c];
            }
        }

        for (j = 0; j < PARAMS_N; j += 8) {
            for (int r = 0; r < 8; r++) {
                for (int c = 0; c < 8; c++) {
                    x[r][c] = (uint16_t)A[(i + r)*PARAMS_N + (j + c)];
                    y[r][c] = s[c*PARAMS_N + (j + r)];
                }
            }
            if (!frodo_op_mul_add(acc, x, y)) {
                free(A);
                return 0;
            }
        }

        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                out[(i + r)*PARAMS_NBAR + c] = acc[r][c];
            }
        }
    }
    free(A);
    return 1;
}


int frodo_mul_add_sa_plus_e(uint16_t *out, const uint16_t *s, uint16_t *e, const uint8_t *seed_A) 
{ // Generate-and-multiply: generate matrix A (N x N) column-wise, multiply by s' on the left.
  // Inputs: s', e' (N_BAR x N)
  // Output: out = s'*A + e' (N_BAR x N)
    int i, j;
    int16_t *A = calloc((size_t)PARAMS_N * PARAMS_N, sizeof(int16_t));
    if (A == NULL) {
        return 0;
    }
    
    // Matrix A generation using SHAKE128, done per 16*N-bit row.
    uint8_t seed_A_separated[2 + BYTES_SEED_A];
    memcpy(&seed_A_separated[2], seed_A, BYTES_SEED_A);
    for (i = 0; i < PARAMS_N; i++) {
        uint16_t row = UINT16_TO_LE((uint16_t)i);

        memcpy(seed_A_separated, &row, sizeof row);
        shake128((unsigned char*)(A + i*PARAMS_N), (unsigned long long)(2*PARAMS_N), seed_A_separated, 2 + BYTES_SEED_A);
    }

    for (i = 0; i < PARAMS_N * PARAMS_N; i++) {
        A[i] = LE_TO_UINT16(A[i]);
    }
    memcpy(out, e, PARAMS_NBAR * PARAMS_N * sizeof(uint16_t));

    for (i = 0; i < PARAMS_N; i += 8) {                         // Matrix multiplication-addition s'*A + e
        uint16_t acc[8][8];
        uint16_t x[8][8];
        uint16_t y[8][8];

        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                acc[r][c] = out[r*PARAMS_N + (i + c)];
            }
        }

        for (j = 0; j < PARAMS_N; j += 8) {
            for (int r = 0; r < 8; r++) {
                for (int c = 0; c < 8; c++) {
                    x[r][c] = s[r*PARAMS_N + (j + c)];
                    y[r][c] = (uint16_t)A[(j + r)*PARAMS_N + (i + c)];
                }
            }
            if (!frodo_op_mul_add(acc, x, y)) {
                free(A);
                return 0;
            }
        }

        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                out[r*PARAMS_N + (i + c)] = acc[r][c];
            }
        }
    }
    free(A);
    return 1;
}


void frodo_mul_bs(uint16_t *out, const uint16_t *b, const uint16_t *s) 
{ // Multiply by s on the right
  // Inputs: b (N_BAR x N), s (N x N_BAR)
  // Output: out = b*s (N_BAR x N_BAR)
    int block, i, j, k;
    uint16_t x[8][8];
    uint16_t y[8][8];
    uint16_t z[8][8];
    uint16_t q = (uint16_t)PARAMS_Q;

    memset(out, 0, PARAMS_NBAR * PARAMS_NBAR * sizeof(uint16_t));
    for (block = 0; block < PARAMS_N; block += 8) {
        for (i = 0; i < 8; i++) {
            for (k = 0; k < 8; k++) {
                x[i][k] = b[i * PARAMS_N + block + k];
                y[k][i] = s[i * PARAMS_N + block + k];
            }
        }
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


void frodo_mul_add_sb_plus_e(uint16_t *out, const uint16_t *b, const uint16_t *s, const uint16_t *e) 
{ // Multiply by s on the left
  // Inputs: b (N x N_BAR), s (N_BAR x N), e (N_BAR x N_BAR)
  // Output: out = s*b + e (N_BAR x N_BAR)
    int block, i, j, k;
    uint16_t x[8][8];
    uint16_t y[8][8];
    uint16_t z[8][8];
    uint16_t q = (uint16_t)PARAMS_Q;

    memcpy(out, e, PARAMS_NBAR * PARAMS_NBAR * sizeof(uint16_t));
    for (block = 0; block < PARAMS_N; block += 8) {
        for (i = 0; i < 8; i++) {
            for (k = 0; k < 8; k++) {
                x[i][k] = s[i * PARAMS_N + block + k];
                y[k][i] = b[(block + k) * PARAMS_NBAR + i];
            }
        }
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
