#include "operator_interface.h"

static void
mul_u64(uint64_t a, uint64_t b, uint64_t *hi, uint64_t *lo)
{
	uint64_t a0, a1, b0, b1;
	uint64_t p0, p1, p2, p3;
	uint64_t t, carry, out_lo, out_hi;

	a0 = a & 0xFFFFFFFFu;
	a1 = a >> 32;
	b0 = b & 0xFFFFFFFFu;
	b1 = b >> 32;

	p0 = a0 * b0;
	p1 = a0 * b1;
	p2 = a1 * b0;
	p3 = a1 * b1;

	t = p1 + p2;
	carry = (t < p1);
	out_lo = p0 + ((t & 0xFFFFFFFFu) << 32);
	carry += (out_lo < p0);
	out_hi = p3 + (t >> 32) + carry;

	*hi = out_hi;
	*lo = out_lo;
}

static int
square_leq(uint64_t x, uint64_t n_hi, uint64_t n_lo)
{
	uint64_t sq_hi, sq_lo;

	mul_u64(x, x, &sq_hi, &sq_lo);
	return (sq_hi < n_hi) || (sq_hi == n_hi && sq_lo <= n_lo);
}

static op_fpr_t
op_fpr_sqrt_impl(op_fpr_t a)
{
	union {
		op_fpr_t d;
		uint64_t u;
	} x, y;
	uint64_t frac;
	uint64_t mant;
	uint64_t lo;
	uint64_t hi;
	uint64_t s;
	uint64_t q;
	uint64_t n_hi;
	uint64_t n_lo;
	uint64_t sq_hi;
	uint64_t sq_lo;
	int e;
	int odd;

	x.d = a;
	if (a <= (op_fpr_t)0.0) {
		return (op_fpr_t)0.0;
	}

	frac = x.u & 0x000FFFFFFFFFFFFFu;
	e = (int)((x.u >> 52) & 0x7FFu);
	if (e == 0x7FF) {
		return a;
	}
	if (e == 0) {
		e = -1022;
		mant = frac;
		while ((mant & 0x0010000000000000u) == 0) {
			mant <<= 1;
			e --;
		}
	} else {
		e -= 1023;
		mant = 0x0010000000000000u | frac;
	}

	odd = e & 1;
	e >>= 1;

	/*
	 * The output significand is round(sqrt(mant * 2^(52 + odd))).
	 * This uses only 64-bit arithmetic and exact limb-wise comparisons.
	 */
	if (odd == 0) {
		n_lo = mant << 52;
		n_hi = mant >> 12;
	} else {
		n_lo = mant << 53;
		n_hi = mant >> 11;
	}
	lo = 0x0010000000000000u;
	hi = 0x0020000000000000u;
	while (lo + 1 < hi) {
		uint64_t mid;

		mid = lo + ((hi - lo) >> 1);
		if (square_leq(mid, n_hi, n_lo)) {
			lo = mid;
		} else {
			hi = mid;
		}
	}
	s = lo;
	mul_u64(s, s, &sq_hi, &sq_lo);
	{
		uint64_t sum_lo, sum_hi, inc;

		sum_lo = sq_lo + s;
		sum_hi = sq_hi + (sum_lo < sq_lo);
		inc = 0;
		if (n_hi > sum_hi || (n_hi == sum_hi && n_lo > sum_lo)) {
			inc = 1;
		}
		q = s + inc;
	}

	if (q == 0x0020000000000000u) {
		q = 0x0010000000000000u;
		e ++;
	}
	y.u = (uint64_t)(e + 1023) << 52;
	y.u |= q & 0x000FFFFFFFFFFFFFu;
	return y.d;
}

int
OP_fpr_add(op_fpr_t a, op_fpr_t b, op_fpr_t *out)
{
	if (out == 0) {
		return OP_FAILURE;
	}
	*out = a + b;
	return OP_SUCCESS;
}

int
OP_fpr_sub(op_fpr_t a, op_fpr_t b, op_fpr_t *out)
{
	if (out == 0) {
		return OP_FAILURE;
	}
	*out = a - b;
	return OP_SUCCESS;
}

int
OP_fpr_mul(op_fpr_t a, op_fpr_t b, op_fpr_t *out)
{
	if (out == 0) {
		return OP_FAILURE;
	}
	*out = a * b;
	return OP_SUCCESS;
}

int
OP_fpr_div(op_fpr_t a, op_fpr_t b, op_fpr_t *out)
{
	if (out == 0) {
		return OP_FAILURE;
	}
	*out = a / b;
	return OP_SUCCESS;
}

int
OP_fpr_sqrt(op_fpr_t a, op_fpr_t *out)
{
	if (out == 0) {
		return OP_FAILURE;
	}
	*out = op_fpr_sqrt_impl(a);
	return OP_SUCCESS;
}

int
OP_fpr_lt(op_fpr_t a, op_fpr_t b, uint8_t *out)
{
	if (out == 0) {
		return OP_FAILURE;
	}
	*out = (uint8_t)(a < b);
	return OP_SUCCESS;
}
