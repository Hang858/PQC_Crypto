/* BiT-128 scalar software OP backend: q=7681, full NTT, R=2^32.
 * The caller owns a static read-only 512-word table; no copied global table. */
#include "operator_interface.h"
#include <stdint.h>

static const int32_t *ntt_omega;
#define NTT_Q 7681
#define NTT_INV_Q UINT32_C(2340676097)

/* Low 32 bits of a and m*q are equal, so subtracting their high halves gives
 * the exact Montgomery quotient without a 64-bit subtraction. */
static inline int32_t montgomery(int64_t a)
{
    uint32_t m = (uint32_t)a * NTT_INV_Q;
    int32_t r = (int32_t)(a >> 32) - (int32_t)(((uint64_t)m * NTT_Q) >> 32);
    r += (r >> 31) & NTT_Q;
    r += (r >> 31) & NTT_Q;
    return r;
}
/* Forward/CWM products and the final inverse scale fit uint32_t. Their high
 * half is zero; this keeps all corrections in 32-bit SIMD lanes. */
static inline int32_t montgomery_u32(uint32_t a)
{
    uint32_t m = a * NTT_INV_Q;
    int32_t r = -(int32_t)(((uint64_t)m * NTT_Q) >> 32);
    return r + ((r >> 31) & NTT_Q);
}
_Static_assert(UINT64_C(9) * NTT_Q * NTT_Q < (UINT64_C(1) << 32), "forward product bound");
_Static_assert(UINT64_C(256) * NTT_Q * 1912 < (UINT64_C(1) << 32), "inverse scale bound");
static inline int32_t canonical(int32_t a)
{
    int32_t t = (int32_t)(((int64_t)a * 559168) >> 32);
    a -= t * NTT_Q;
    a += (a >> 31) & NTT_Q;
    a -= NTT_Q;
    return a + ((a >> 31) & NTT_Q);
}
int OP_ntt256_init(const int32_t *omega, int32_t count, int32_t q,
                   int32_t inv_q, int32_t inv_n, int32_t k)
{
    ntt_omega = NULL;
    if (omega == NULL || count != 2 || q != NTT_Q || (uint32_t)inv_q != NTT_INV_Q ||
        inv_n != 1912 || k != 0) return OP_FAILURE;
    ntt_omega = omega;
    return OP_SUCCESS;
}
int OP_ntt256(int32_t out[256], const int32_t in[256], int index)
{
    if (ntt_omega == NULL || out == NULL || in == NULL || index != 0) return OP_FAILURE;
    for (int i = 0; i < 256; ++i) out[i] = in[i];
    int t = 256;
    /* Each forward stage adds at most q to the nonnegative bound. After all
     * eight stages values are <9q and fit int32_t. Canonicalize once at end. */
    for (int m = 1; m < 256; m <<= 1) {
        t >>= 1;
        for (int i = 0; i < m; ++i) {
            int32_t w = ntt_omega[m + i];
            int start = 2 * i * t;
            for (int j = start; j < start + t; ++j) {
                int32_t u = out[j], v = montgomery_u32((uint32_t)out[j + t] * (uint32_t)w);
                out[j] = u + v;
                out[j + t] = u - v + NTT_Q;
            }
        }
    }
    for (int i = 0; i < 256; ++i) out[i] = canonical(out[i]);
    return OP_SUCCESS;
}
int OP_intt256(int32_t out[256], const int32_t in[256], int index)
{
    if (ntt_omega == NULL || out == NULL || in == NULL || index != 1) return OP_FAILURE;
    for (int i = 0; i < 256; ++i) out[i] = in[i];
    int t = 1;
    /* Unreduced sums grow at most to 256q; differences times a root remain
     * below 256q^2 in magnitude, well inside the Montgomery bound above. */
    for (int m = 256; m > 1; m >>= 1) {
        int h = m >> 1;
        for (int i = 0; i < h; ++i) {
            int32_t w = ntt_omega[256 + h + i];
            int start = 2 * i * t;
            for (int j = start; j < start + t; ++j) {
                int32_t u = out[j], v = out[j + t];
                out[j] = u + v;
                out[j + t] = montgomery((int64_t)(u - v) * w);
            }
        }
        t <<= 1;
    }
    for (int i = 0; i < 256; ++i) out[i] = montgomery_u32((uint32_t)out[i] * 1912U);
    return OP_SUCCESS;
}
int OP_cwm(int32_t *out, const int32_t *a, const int32_t *b, int32_t n, int32_t q, int32_t k)
{
    if (ntt_omega == NULL || out == NULL || a == NULL || b == NULL ||
        n != 256 || q != NTT_Q || k != 0) return OP_FAILURE;
    for (int i = 0; i < 256; ++i) out[i] = montgomery_u32((uint32_t)a[i] * (uint32_t)b[i]);
    return OP_SUCCESS;
}
