#define FALCON_PREFIX falcon_inner

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inner.h"

typedef union {
	double d;
	uint64_t u;
} d64;

static uint64_t rng_state = 0x9e3779b97f4a7c15ULL;

static uint64_t
xorshift64(void)
{
	uint64_t x = rng_state;

	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	rng_state = x;
	return x;
}

static double
make_finite_double(void)
{
	d64 x;

	x.u = xorshift64();
	x.u &= 0x7FFFFFFFFFFFFFFFULL;
	if (((x.u >> 52) & 0x7FFu) == 0x7FFu) {
		x.u ^= 0x0010000000000000ULL;
	}
	return x.d;
}

static uint64_t
as_u64(double x)
{
	d64 v;

	v.d = x;
	return v.u;
}

static fpr
mk(double x)
{
	fpr v;

	v.v = x;
	return v;
}

static double
ref_rint(double x)
{
	int64_t sx, tx, rp, rn, m;
	uint32_t ub;

	sx = (int64_t)(x - 1.0);
	tx = (int64_t)x;
	rp = (int64_t)(x + 4503599627370496.0) - 4503599627370496LL;
	rn = (int64_t)(x - 4503599627370496.0) + 4503599627370496LL;
	m = sx >> 63;
	rn &= m;
	rp &= ~m;
	ub = (uint32_t)((uint64_t)tx >> 52);
	m = -(int64_t)((((ub + 1) & 0xFFF) - 2) >> 31);
	rp &= m;
	rn &= m;
	tx &= ~m;
	return (double)(tx | rn | rp);
}

static double
ref_floor(double x)
{
	int64_t r = (int64_t)x;

	return (double)(r - (x < (double)r));
}

static double
ref_trunc(double x)
{
	return (double)(int64_t)x;
}

static uint64_t
ref_expm_p63(double x, double ccs)
{
	double y;

	y = 0.000000002073772366009083061987;
	y = 0.000000025299506379442070029551 - y * x;
	y = 0.000000275607356160477811864927 - y * x;
	y = 0.000002755586350219122514855659 - y * x;
	y = 0.000024801566833585381209939524 - y * x;
	y = 0.000198412739277311890541063977 - y * x;
	y = 0.001388888894063186997887560103 - y * x;
	y = 0.008333333327800835146903501993 - y * x;
	y = 0.041666666666110491190622155955 - y * x;
	y = 0.166666666666984014666397229121 - y * x;
	y = 0.500000000000019206858326015208 - y * x;
	y = 0.999999999999994892974086724280 - y * x;
	y = 1.000000000000000000000000000000 - y * x;
	y *= ccs;
	return (uint64_t)(y * 9223372036854775808.0);
}

static double
ref_sqrt(double x)
{
	double y;
	int i;

	if (x <= 0.0) {
		return 0.0;
	}
	y = x;
	for (i = 0; i < 40; i++) {
		y = 0.5 * (y + x / y);
	}
	return y;
}

static int
check_i64(const char *name, int64_t got, int64_t ref, double a)
{
	if (got != ref) {
		fprintf(stderr, "FAIL %s\n", name);
		fprintf(stderr, "  a=%a\n", a);
		fprintf(stderr, "  got=%lld ref=%lld\n",
			(long long)got, (long long)ref);
		return 1;
	}
	return 0;
}

static int
check_double(const char *name, double got, double ref, double a, double b)
{
	if (as_u64(got) != as_u64(ref)) {
		fprintf(stderr, "FAIL %s\n", name);
		fprintf(stderr, "  a=%a b=%a\n", a, b);
		fprintf(stderr, "  got=%a ref=%a\n", got, ref);
		fprintf(stderr, "  got_bits=%016llx ref_bits=%016llx\n",
			(unsigned long long)as_u64(got),
			(unsigned long long)as_u64(ref));
		return 1;
	}
	return 0;
}

static int
test_basic_ops(void)
{
	size_t i;

	for (i = 0; i < 200000; i++) {
		double a = make_finite_double();
		double b = make_finite_double();
		double c;
		double ref;

		c = fpr_add(mk(a), mk(b)).v;
		ref = a + b;
		if (check_double("add", c, ref, a, b)) return 1;

		c = fpr_sub(mk(a), mk(b)).v;
		ref = a - b;
		if (check_double("sub", c, ref, a, b)) return 1;

		c = fpr_neg(mk(a)).v;
		ref = -a;
		if (check_double("neg", c, ref, a, 0.0)) return 1;

		c = fpr_half(mk(a)).v;
		ref = a * 0.5;
		if (check_double("half", c, ref, a, 0.0)) return 1;

		c = fpr_double(mk(a)).v;
		ref = a + a;
		if (check_double("double", c, ref, a, 0.0)) return 1;

		c = fpr_mul(mk(a), mk(b)).v;
		ref = a * b;
		if (check_double("mul", c, ref, a, b)) return 1;

		if (b == 0.0) {
			b = 1.0;
		}
		c = fpr_div(mk(a), mk(b)).v;
		ref = a / b;
		if (check_double("div", c, ref, a, b)) return 1;

		c = fpr_inv(mk(b)).v;
		ref = 1.0 / b;
		if (check_double("inv", c, ref, b, 0.0)) return 1;

		c = fpr_sqr(mk(a)).v;
		ref = a * a;
		if (check_double("sqr", c, ref, a, 0.0)) return 1;

		{
			int got = fpr_lt(mk(a), mk(b));
			int refi = (a < b);
			if (got != refi) {
				fprintf(stderr, "FAIL lt\n  a=%a b=%a got=%d ref=%d\n",
					a, b, got, refi);
				return 1;
			}
		}
	}
	return 0;
}

static int
test_rounding(void)
{
	static const double edge[] = {
		0.0, -0.0, 0.25, -0.25, 0.5, -0.5, 1.0, -1.0,
		4503599627370496.0, -4503599627370496.0,
		4503599627370495.5, -4503599627370495.5,
		9223372036854775807.0, -9223372036854775807.0
	};
	size_t i;

	for (i = 0; i < sizeof edge / sizeof edge[0]; i++) {
		double x = edge[i];
		if (check_i64("rint(edge)", fpr_rint(mk(x)),
			(int64_t)ref_rint(x), x)) return 1;
		if (check_i64("floor(edge)", fpr_floor(mk(x)),
			(int64_t)ref_floor(x), x)) return 1;
		if (check_i64("trunc(edge)", fpr_trunc(mk(x)),
			(int64_t)ref_trunc(x), x)) return 1;
	}

	for (i = 0; i < 200000; i++) {
		double x = make_finite_double();

		if (check_i64("rint", fpr_rint(mk(x)), (int64_t)ref_rint(x), x))
			return 1;
		if (check_i64("floor", fpr_floor(mk(x)), (int64_t)ref_floor(x), x))
			return 1;
		if (check_i64("trunc", fpr_trunc(mk(x)), (int64_t)ref_trunc(x), x))
			return 1;
	}
	return 0;
}

static int
test_expm(void)
{
	size_t i;
	const double log2_val = 0.69314718055994530941723212146;

	for (i = 0; i < 200000; i++) {
		double x = (double)(xorshift64() & 0x3FF) / 1024.0 * log2_val;
		double ccs = 0.5 + (double)(xorshift64() & 0x3FF) / 1024.0;
		uint64_t got = fpr_expm_p63(mk(x), mk(ccs));
		uint64_t ref = ref_expm_p63(x, ccs);

		if (got != ref) {
			fprintf(stderr, "FAIL expm_p63\n");
			fprintf(stderr, "  x=%a ccs=%a\n", x, ccs);
			fprintf(stderr, "  got=%016llx ref=%016llx\n",
				(unsigned long long)got, (unsigned long long)ref);
			return 1;
		}
	}
	return 0;
}

static int
test_sqrt(void)
{
	static const double edge[] = {
		0.0, 1.0, 2.0, 4.0, 0.25, 1e-308, 1e-200,
		3.0, 16.0, 12345.6789, 4503599627370496.0
	};
	size_t i;

	for (i = 0; i < sizeof edge / sizeof edge[0]; i++) {
		double x = edge[i];
		double got = fpr_sqrt(mk(x)).v;
		double ref = ref_sqrt(x);

		if (check_double("sqrt(edge)", got, ref, x, 0.0)) return 1;
	}
	for (i = 0; i < 100000; i++) {
		double x = (double)(xorshift64() & 0x000FFFFFFFFFFFFFULL);
		double got = fpr_sqrt(mk(x)).v;
		double ref = ref_sqrt(x);

		if (check_double("sqrt", got, ref, x, 0.0)) return 1;
	}
	return 0;
}

int
main(void)
{
	if (test_basic_ops() != 0) return 1;
	if (test_rounding() != 0) return 1;
	if (test_expm() != 0) return 1;
	if (test_sqrt() != 0) return 1;
	puts("FPR compare passed");
	return 0;
}
