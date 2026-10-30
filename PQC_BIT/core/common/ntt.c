/* OP adapter: 16-bit persistent polynomials, 32-bit operator scratch. */
#include "ntt.h"
#include "operator_interface.h"
#include "symmetric.h"
#include "ntt_roots32.h"
#include <stdlib.h>

int bit_ntt_init(void)
{
    return OP_ntt256_init(bit_ntt_roots32, 2, 7681, -1954291199, 1912, 0) == OP_SUCCESS ? 0 : -1;
}
static int transform(int16_t *out, const int16_t *in, bit_ntt_workspace *s, int inverse)
{
    for (int i = 0; i < BIT_N; ++i) s->x[i] = in[i];
    int rc = inverse ? OP_intt256(s->z, s->x, 1) : OP_ntt256(s->z, s->x, 0);
    if (rc != OP_SUCCESS) return -1;
    for (int i = 0; i < BIT_N; ++i) out[i] = (int16_t)s->z[i];
    return 0;
}
int ntt_forward_ws(int16_t *out, const int16_t *in, bit_ntt_workspace *s)
{ return transform(out, in, s, 0); }
int ntt_inverse_ws(int16_t *out, const int16_t *in, bit_ntt_workspace *s)
{ return transform(out, in, s, 1); }
int ntt_basemul_raw_ws(int16_t *out, const int16_t *a, const int16_t *b, bit_ntt_workspace *s)
{
    for (int i = 0; i < BIT_N; ++i) { s->x[i] = a[i]; s->y[i] = b[i]; }
    if (OP_cwm(s->z, s->x, s->y, BIT_N, BIT_Q, 0) != OP_SUCCESS) return -1;
    for (int i = 0; i < BIT_N; ++i) out[i] = (int16_t)s->z[i];
    return 0;
}
int ntt_montgomery_lift_ws(int16_t *a, bit_ntt_workspace *s)
{
    (void)s;
    /* Representation conversion, not a polynomial product: multiply the raw
     * CWM accumulator by the public constant R32 mod q. Signed accumulators
     * are in [-q,3q), so the product fits int32_t. No extra OP transfer. */
    for (int i = 0; i < BIT_N; ++i) a[i] = (int16_t)reduce_barrett((int32_t)a[i] * 5569);
    return 0;
}

static int standalone(int kind, int16_t *out, const int16_t *a, const int16_t *b)
{
    bit_ntt_workspace *s = malloc(sizeof(*s));
    if (s == NULL) return -1;
    int rc = -1;
    if (bit_ntt_init() != 0) goto done;
    if (kind < 2) rc = transform(out, a, s, kind);
    else if (kind == 2) rc = ntt_basemul_raw_ws(out, a, b, s);
    else if (kind == 3) {
        for (int i = 0; i < BIT_N; ++i) { s->x[i] = a[i]; s->y[i] = b[i]; }
        if (OP_cwm(s->z, s->x, s->y, BIT_N, BIT_Q, 0) != OP_SUCCESS) goto done;
        for (int i = 0; i < BIT_N; ++i) {
            int32_t sum = (int32_t)out[i] + s->z[i] - BIT_Q;
            out[i] = (int16_t)(sum + ((sum >> 31) & BIT_Q));
        }
        rc = 0;
    } else rc = ntt_montgomery_lift_ws(out, s);
 done:
    bit_secure_zero(s, sizeof(*s)); free(s);
    return rc;
}
int ntt_forward(int16_t a[BIT_N]) { return standalone(0, a, a, NULL); }
int ntt_inverse(int16_t a[BIT_N]) { return standalone(1, a, a, NULL); }
int ntt_basemul_raw(int16_t *out, const int16_t *a, const int16_t *b) { return standalone(2, out, a, b); }
int ntt_basemul_acc_raw(int16_t *out, const int16_t *a, const int16_t *b) { return standalone(3, out, a, b); }
int ntt_montgomery_lift(int16_t a[BIT_N]) { return standalone(4, a, NULL, NULL); }
