#include "kem.h"
#include "pke.h"
#include "param.h"
#include "fips202.h"
#include "util.h"
#include "random.h"
#include <string.h>
#include <stdlib.h>
int scloud_kemkeygen(uint8_t *pk, uint8_t *sk)
{
	uint8_t z[32];
	randombytes(z, 32);
	scloudplus_pkekeygen(pk, sk);
	memcpy(sk + scloudplus_pke_sk, pk, scloudplus_pk);
	scloudplus_H(sk + scloudplus_pke_sk + scloudplus_pk, pk, scloudplus_pk);
	memcpy(sk + scloudplus_kem_sk - 32, z, 32);
	return 0;
}
int scloud_kemencaps(uint8_t *ctx, uint8_t *ss, const uint8_t *pk)
{
	uint8_t *kc = (uint8_t *)malloc(sizeof(uint8_t) * (scloudplus_ctx + 32));
	uint8_t m[scloudplus_ss + 32], rk[64];
	if (kc == NULL) {
		return -1;
	}
	randombytes(m, scloudplus_ss);
	scloudplus_H(m + scloudplus_ss, pk, scloudplus_pk);
	scloudplus_G(rk, m, scloudplus_ss + 32);
	scloudplus_pkeenc(pk, m, rk, ctx);
	memcpy(kc, rk + 32, 32);
	memcpy(kc + 32, ctx, scloudplus_ctx);
	scloudplus_K(ss, scloudplus_ss, kc, scloudplus_ctx + 32);
	free(kc);
	return 0;
}
int scloud_kemdecaps(uint8_t *ss, const uint8_t *ctx, const uint8_t *sk)
{
	uint8_t m[scloudplus_ss + 32], rk[64];
	uint8_t *ctx1 = (uint8_t *)malloc(sizeof(uint8_t) * (scloudplus_ctx + 32));
	if (ctx1 == NULL) {
		return -1;
	}
	scloudplus_pkedec((uint8_t *)sk, (uint8_t *)ctx, m);
	memcpy(m + scloudplus_ss, sk + scloudplus_pke_sk + scloudplus_pk, 32);
	scloudplus_G(rk, m, scloudplus_ss + 32);
	scloudplus_pkeenc((uint8_t *)sk + scloudplus_pke_sk, m, rk, ctx1 + 32);
	int8_t bl = scloudplus_verify(ctx, ctx1 + 32, scloudplus_ctx);
	memcpy(ctx1 + 32, ctx, scloudplus_ctx);
	scloudplus_cmov(ctx1, rk + 32, sk + scloudplus_kem_sk - 32, 32, bl);
	scloudplus_K(ss, scloudplus_ss, ctx1, scloudplus_ctx + 32);
	free(ctx1);
	return 0;
}
