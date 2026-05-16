#include <math.h>
#include <stdint.h>

#if defined __GNUC__ && defined __SSE2_MATH__
#include <emmintrin.h>
#endif

#include "operator_interface.h"

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

#if defined __GNUC__ && defined __SSE2_MATH__
	*out = _mm_cvtsd_f64(_mm_sqrt_pd(_mm_set1_pd(a)));
#elif defined __GNUC__ && defined __i386__
	__asm__ __volatile__ (
		"fldl   %0\n\t"
		"fsqrt\n\t"
		"fstpl  %0\n\t"
		: "+m" (a) : : );
	*out = a;
#elif defined __PPC__ && defined __GNUC__
#if defined __clang__
	__asm__ ( "fsqrt  %0, %1" : "=f" (*out) : "f" (a) : );
#else
	__asm__ ( "fsqrt  %0, %1" : "=d" (*out) : "d" (a) : );
#endif
#elif (defined __ARM_FP && ((__ARM_FP & 0x08) == 0x08)) \
	|| (!defined __ARM_FP && defined __ARM_VFPV2__)
#if defined __aarch64__ && __aarch64__
	__asm__ ( "fsqrt   %d0, %d0" : "+w" (a) : : );
#else
	__asm__ ( "fsqrtd  %P0, %P0" : "+w" (a) : : );
#endif
	*out = a;
#else
	*out = sqrt(a);
#endif
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
