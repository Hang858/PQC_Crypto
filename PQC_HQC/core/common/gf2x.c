/**
 * @file gf2x.c
 * @brief Low-memory multiplication in GF(2)[X] / (X^n - 1).
 *
 * HQC always calls vect_mul() with a fixed-weight sparse first operand
 * (y or r2). If its support is {p_i}, multiplication is simply
 *
 *     a1 * a2 = XOR_i (X^p_i * a2) mod (X^n - 1).
 *
 * The implementation below evaluates that identity directly. It requires
 * no NTT buffers and no heap allocation. The second operand is public in
 * all current HQC call sites, so iterating over its set bits does not expose
 * secret-dependent information beyond the sparse positions already used by
 * the previous block-skipping implementation.
 */

#include "gf2x.h"

#include <stdint.h>
#include <string.h>

#include "parameters.h"

/** Return the index of the least-significant set bit. x must be non-zero. */
static inline unsigned int trailing_zeroes_u64(uint64_t x) {
    return (unsigned int)__builtin_ctzll(x);
}

/**
 * XOR X^shift * dense into out modulo X^PARAM_N - 1.
 *
 * Only scalar temporaries are used. Bits outside PARAM_N in the final word
 * are ignored, and one subtraction is sufficient because both exponents are
 * strictly smaller than PARAM_N.
 */
static void xor_shifted_dense(uint64_t *out, const uint64_t *dense, uint32_t shift) {
    const size_t words = VEC_N_SIZE_64;
    const uint32_t n = PARAM_N;

    for (size_t word = 0; word < words; ++word) {
        uint64_t bits = dense[word];

        if (word + 1 == words) {
            bits &= BITMASK(PARAM_N, 64);
        }

        while (bits != 0) {
            const unsigned int bit = trailing_zeroes_u64(bits);
            uint32_t position = shift + (uint32_t)(word * 64u) + bit;

            if (position >= n) {
                position -= n;
            }

            out[position >> 6] ^= UINT64_C(1) << (position & 63u);
            bits &= bits - 1;
        }
    }
}

/**
 * Multiply a sparse polynomial by a dense polynomial modulo X^PARAM_N - 1.
 *
 * All HQC call sites pass y or r2 as sparse. No temporary multiplication
 * buffer is needed; out must not alias either input (as in all HQC callers).
 */
void vect_mul(uint64_t *out, const uint64_t *sparse, const uint64_t *dense) {
    const size_t words = VEC_N_SIZE_64;

    memset(out, 0, words * sizeof(uint64_t));

    for (size_t word = 0; word < words; ++word) {
        uint64_t bits = sparse[word];

        if (word + 1 == words) {
            bits &= BITMASK(PARAM_N, 64);
        }

        while (bits != 0) {
            const unsigned int bit = trailing_zeroes_u64(bits);
            const uint32_t position = (uint32_t)(word * 64u) + bit;

            xor_shifted_dense(out, dense, position);
            bits &= bits - 1;
        }
    }

    out[words - 1] &= BITMASK(PARAM_N, 64);
}
