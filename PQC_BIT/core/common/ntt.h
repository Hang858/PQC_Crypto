/* BiT-128 OP NTT adapter. Private algorithm interface. */
#ifndef BIT_NTT_H
#define BIT_NTT_H
#include <stdint.h>
#include "params.h"
#include "reduce.h"
typedef struct { int32_t x[BIT_N], y[BIT_N], z[BIT_N]; } bit_ntt_workspace;
/* Must be called once per top-level operation before any workspace operation.
 * Full 8-layer NTT, q=7681, R=2^32. Errors return -1, no local fallback. */
int bit_ntt_init(void);
int ntt_forward_ws(int16_t *out, const int16_t *in, bit_ntt_workspace *scratch);
int ntt_inverse_ws(int16_t *out, const int16_t *in, bit_ntt_workspace *scratch);
/* Raw products carry R32^-1; lift restores ordinary NTT-domain residues. */
int ntt_basemul_raw_ws(int16_t *out, const int16_t *a, const int16_t *b, bit_ntt_workspace *scratch);
int ntt_montgomery_lift_ws(int16_t *a, bit_ntt_workspace *scratch);
/* Standalone helpers allocate/clear their own scratch and initialize the OP.
 * Top-level signing/keygen/verify exclusively use the explicit workspace API. */
int ntt_forward(int16_t a[BIT_N]);
int ntt_inverse(int16_t a[BIT_N]);
int ntt_basemul_raw(int16_t *out, const int16_t *a, const int16_t *b);
int ntt_basemul_acc_raw(int16_t *out, const int16_t *a, const int16_t *b);
int ntt_montgomery_lift(int16_t a[BIT_N]);
#endif
