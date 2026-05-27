/*
 * Floating-point operations.
 *
 * ==========================(LICENSE BEGIN)============================
 *
 * Copyright (c) 2017-2019  Falcon Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ===========================(LICENSE END)=============================
 *
 * @author   Thomas Pornin <thomas.pornin@nccgroup.com>
 */
#include "operator_interface.h"

typedef struct { op_fpr_t v; } fpr;

static inline fpr FPR(op_fpr_t v) { fpr x; x.v = v; return x; }
static inline fpr fpr_of(int64_t i) { return FPR((op_fpr_t)i); }

static const fpr fpr_q = { (op_fpr_t)12289.0 };
static const fpr fpr_inverse_of_q = { (op_fpr_t)(1.0 / 12289.0) };
static const fpr fpr_inv_2sqrsigma0 = { (op_fpr_t).150865048875372721532312163019 };
static const fpr fpr_inv_sigma[] = {
	{ (op_fpr_t)0.0 }, { (op_fpr_t)0.0069054793295940891952143765991630516 },
	{ (op_fpr_t)0.0068102267767177975961393730687908629 },
	{ (op_fpr_t)0.0067188101910722710707826117910434131 },
	{ (op_fpr_t)0.0065883354370073665545865037227681924 },
	{ (op_fpr_t)0.0064651781207602900738053897763485516 },
	{ (op_fpr_t)0.0063486788828078995327741182928037856 },
	{ (op_fpr_t)0.0062382586529084374473367528433697537 },
	{ (op_fpr_t)0.0061334065020930261548984001431770281 },
	{ (op_fpr_t)0.0060336696681577241031668062510953022 },
	{ (op_fpr_t)0.0059386453095331159950250124336477482 }
};
static const fpr fpr_sigma_min[] = {
	{ (op_fpr_t)0.0 }, { (op_fpr_t)1.1165085072329102588881898380334015 },
	{ (op_fpr_t)1.1321247692325272405718031785357108 },
	{ (op_fpr_t)1.1475285353733668684571123112513188 },
	{ (op_fpr_t)1.1702540788534828939713084716509250 },
	{ (op_fpr_t)1.1925466358390344011122170489094133 },
	{ (op_fpr_t)1.2144300507766139921088487776957699 },
	{ (op_fpr_t)1.2359260567719808790104525941706723 },
	{ (op_fpr_t)1.2570545284063214162779743112075080 },
	{ (op_fpr_t)1.2778336969128335860256340575729042 },
	{ (op_fpr_t)1.2982803343442918539708792538826807 }
};
static const fpr fpr_log2 = { (op_fpr_t)0.69314718055994530941723212146 };
static const fpr fpr_inv_log2 = { (op_fpr_t)1.4426950408889634073599246810 };
static const fpr fpr_bnorm_max = { (op_fpr_t)16822.4121 };
static const fpr fpr_zero = { (op_fpr_t)0.0 };
static const fpr fpr_one = { (op_fpr_t)1.0 };
static const fpr fpr_two = { (op_fpr_t)2.0 };
static const fpr fpr_onehalf = { (op_fpr_t)0.5 };
static const fpr fpr_invsqrt2 = { (op_fpr_t)0.707106781186547524400844362105 };
static const fpr fpr_invsqrt8 = { (op_fpr_t)0.353553390593273762200422181052 };
static const fpr fpr_ptwo31 = { (op_fpr_t)2147483648.0 };
static const fpr fpr_ptwo31m1 = { (op_fpr_t)2147483647.0 };
static const fpr fpr_mtwo31m1 = { (op_fpr_t)-2147483647.0 };
static const fpr fpr_ptwo63m1 = { (op_fpr_t)9223372036854775807.0 };
static const fpr fpr_mtwo63m1 = { (op_fpr_t)-9223372036854775807.0 };
static const fpr fpr_ptwo63 = { (op_fpr_t)9223372036854775808.0 };

static inline int64_t
fpr_rint(fpr x)
{
	int64_t sx, tx, rp, rn, m; uint32_t ub;
	{ op_fpr_t t; OP_fpr_sub(x.v, (op_fpr_t)1.0, &t); sx = (int64_t)t; }
	tx = (int64_t)x.v;
	{ op_fpr_t t; OP_fpr_add(x.v, (op_fpr_t)4503599627370496.0, &t); rp = (int64_t)t - 4503599627370496; }
	{ op_fpr_t t; OP_fpr_sub(x.v, (op_fpr_t)4503599627370496.0, &t); rn = (int64_t)t + 4503599627370496; }
	m = sx >> 63; rn &= m; rp &= ~m;
	ub = (uint32_t)((uint64_t)tx >> 52);
	m = -(int64_t)((((ub + 1) & 0xFFF) - 2) >> 31);
	rp &= m; rn &= m; tx &= ~m;
	return tx | rn | rp;
}

static inline int64_t
fpr_floor(fpr x)
{
	int64_t r; r = (int64_t)x.v;
	{ uint8_t z; OP_fpr_lt(x.v, (op_fpr_t)r, &z); return r - (int64_t)z; }
}

static inline int64_t fpr_trunc(fpr x) { return (int64_t)x.v; }

static inline fpr fpr_add(fpr x, fpr y) { op_fpr_t z; OP_fpr_add(x.v, y.v, &z); return FPR(z); }
static inline fpr fpr_sub(fpr x, fpr y) { op_fpr_t z; OP_fpr_sub(x.v, y.v, &z); return FPR(z); }
static inline fpr fpr_neg(fpr x) { op_fpr_t z; OP_fpr_sub((op_fpr_t)-0.0, x.v, &z); return FPR(z); }
static inline fpr fpr_half(fpr x) { op_fpr_t z; OP_fpr_mul(x.v, fpr_onehalf.v, &z); return FPR(z); }
static inline fpr fpr_double(fpr x) { op_fpr_t z; OP_fpr_add(x.v, x.v, &z); return FPR(z); }
static inline fpr fpr_mul(fpr x, fpr y) { op_fpr_t z; OP_fpr_mul(x.v, y.v, &z); return FPR(z); }
static inline fpr fpr_sqr(fpr x) { return fpr_mul(x, x); }
static inline fpr fpr_inv(fpr x) { op_fpr_t z; OP_fpr_div(fpr_one.v, x.v, &z); return FPR(z); }
static inline fpr fpr_div(fpr x, fpr y) { op_fpr_t z; OP_fpr_div(x.v, y.v, &z); return FPR(z); }
static inline fpr fpr_sqrt(fpr x) { op_fpr_t z; OP_fpr_sqrt(x.v, &z); return FPR(z); }

static inline int fpr_lt(fpr x, fpr y) { uint8_t z; OP_fpr_lt(x.v, y.v, &z); return (int)z; }

static inline uint64_t
fpr_expm_p63(fpr x, fpr ccs)
{
	{ op_fpr_t t, y;
	  OP_fpr_mul((op_fpr_t)0.000000002073772366009083061987, x.v, &y);
	  OP_fpr_sub((op_fpr_t)0.000000025299506379442070029551, y, &y);
	  OP_fpr_mul(y, x.v, &y);
	  OP_fpr_sub((op_fpr_t)0.000000275607356160477811864927, y, &y);
	  OP_fpr_mul(y, x.v, &y);
	  OP_fpr_sub((op_fpr_t)0.000002755586350219122514855659, y, &y);
	  OP_fpr_mul(y, x.v, &y);
	  OP_fpr_sub((op_fpr_t)0.000024801566833585381209939524, y, &y);
	  OP_fpr_mul(y, x.v, &y);
	  OP_fpr_sub((op_fpr_t)0.000198412739277311890541063977, y, &y);
	  OP_fpr_mul(y, x.v, &y);
	  OP_fpr_sub((op_fpr_t)0.001388888894063186997887560103, y, &y);
	  OP_fpr_mul(y, x.v, &y);
	  OP_fpr_sub((op_fpr_t)0.008333333327800835146903501993, y, &y);
	  OP_fpr_mul(y, x.v, &y);
	  OP_fpr_sub((op_fpr_t)0.041666666666110491190622155955, y, &y);
	  OP_fpr_mul(y, x.v, &y);
	  OP_fpr_sub((op_fpr_t)0.166666666666984014666397229121, y, &y);
	  OP_fpr_mul(y, x.v, &y);
	  OP_fpr_sub((op_fpr_t)0.500000000000019206858326015208, y, &y);
	  OP_fpr_mul(y, x.v, &y);
	  OP_fpr_sub((op_fpr_t)0.999999999999994892974086724280, y, &y);
	  OP_fpr_mul(y, x.v, &y);
	  OP_fpr_sub((op_fpr_t)1.000000000000000000000000000000, y, &y);
	  OP_fpr_mul(y, ccs.v, &y);
	  OP_fpr_mul(y, fpr_ptwo63.v, &t);
	  return (uint64_t)t; }
}

#define fpr_gm_tab   Zf(fpr_gm_tab)
extern const fpr fpr_gm_tab[];
#define fpr_p2_tab   Zf(fpr_p2_tab)
extern const fpr fpr_p2_tab[];
