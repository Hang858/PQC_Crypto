#include "operator_interface.h"

static op_fpr_t
op_fpr_sqrt_impl(op_fpr_t a)
{
	union {
		op_fpr_t d;
		uint64_t u;
	} x, y;
	uint64_t bits;
	int exp;
	int shift;
	int odd;
	int i;

	x.d = a;
	if (a <= (op_fpr_t)0.0) {
		return (op_fpr_t)0.0;
	}

	bits = x.u;
	exp = (int)((bits >> 52) & 0x7FFu);
	if (exp == 0) {
		shift = 0;
		while ((bits & 0x7FF0000000000000u) == 0 && shift < 54) {
			bits <<= 1;
			shift ++;
		}
		x.u = bits;
		exp = (int)((bits >> 52) & 0x7FFu) - shift;
	}

	odd = exp & 1;
	exp = (exp >> 1) + 1023;
	y.u = (uint64_t)exp << 52;
	if (odd != 0) {
		y.d *= (op_fpr_t)1.41421356237309504880;
	}

	for (i = 0; i < 8; ++i) {
		y.d = (op_fpr_t)0.5 * (y.d + a / y.d);
	}
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
