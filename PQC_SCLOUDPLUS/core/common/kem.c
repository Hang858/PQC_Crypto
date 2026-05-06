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
	uint8_t *z = (uint8_t *)malloc(32);
	if (z == NULL) {
		return -1;
	}
	randombytes(z, 32);
	if (scloudplus_pkekeygen(pk, sk) != 0) {
		free(z);
		return -1;
	}
	memcpy(sk + SCLOUDPLUS_PKE_SK, pk, SCLOUDPLUS_PK);
	scloudplus_H(sk + SCLOUDPLUS_PKE_SK + SCLOUDPLUS_PK, pk, SCLOUDPLUS_PK);
	memcpy(sk + SCLOUDPLUS_KEM_SK - 32, z, 32);
	free(z);
	return 0;
}
int scloud_kemencaps(uint8_t *pk, uint8_t *ctx, uint8_t *ss)
{
	uint8_t *kc = (uint8_t *)malloc(sizeof(uint8_t) * (SCLOUDPLUS_CTX + 32));
	uint8_t *m = (uint8_t *)malloc(SCLOUDPLUS_SS + 32);
	uint8_t *rk = (uint8_t *)malloc(64);
	if (kc == NULL || m == NULL || rk == NULL) {
		free(kc);
		free(m);
		free(rk);
		return -1;
	}
	randombytes(m, SCLOUDPLUS_SS);
	scloudplus_H(m + SCLOUDPLUS_SS, pk, SCLOUDPLUS_PK);
	scloudplus_G(rk, m, SCLOUDPLUS_SS + 32);
	if (scloudplus_pkeenc(pk, m, rk, ctx) != 0) {
		free(kc);
		free(m);
		free(rk);
		return -1;
	}
	memcpy(kc, rk + 32, 32);
	memcpy(kc + 32, ctx, SCLOUDPLUS_CTX);
	scloudplus_K(ss, SCLOUDPLUS_SS, kc, SCLOUDPLUS_CTX + 32);
	free(kc);
	free(m);
	free(rk);
	return 0;
}
int scloud_kemdecaps(uint8_t *sk, uint8_t *ctx, uint8_t *ss)
{
	uint8_t *m = (uint8_t *)malloc(SCLOUDPLUS_SS + 32);
	uint8_t *rk = (uint8_t *)malloc(64);
	uint8_t *ctx1 = (uint8_t *)malloc(sizeof(uint8_t) * (SCLOUDPLUS_CTX + 32));
	if (m == NULL || rk == NULL || ctx1 == NULL) {
		free(m);
		free(rk);
		free(ctx1);
		return -1;
	}
	if (scloudplus_pkedec(sk, ctx, m) != 0) {
		free(m);
		free(rk);
		free(ctx1);
		return -1;
	}
	memcpy(m + SCLOUDPLUS_SS, sk + SCLOUDPLUS_PKE_SK + SCLOUDPLUS_PK, 32);
	scloudplus_G(rk, m, SCLOUDPLUS_SS + 32);
	if (scloudplus_pkeenc(sk + SCLOUDPLUS_PKE_SK, m, rk, ctx1 + 32) != 0) {
		free(m);
		free(rk);
		free(ctx1);
		return -1;
	}
	int8_t bl = scloudplus_verify(ctx, ctx1 + 32, SCLOUDPLUS_CTX);
	memcpy(ctx1 + 32, ctx, SCLOUDPLUS_CTX);
	scloudplus_cmov(ctx1, rk + 32, sk + SCLOUDPLUS_KEM_SK - 32, 32, bl);
	scloudplus_K(ss, SCLOUDPLUS_SS, ctx1, SCLOUDPLUS_CTX + 32);
	free(m);
	free(rk);
	free(ctx1);
	return 0;
}
