#include "sample.h"
#include "fips202.h"
#include "aes.h"
#include <string.h>
#include <stdlib.h>
#include "config.h"
// This code is based on the implementation of FrodoKEM
void scloudplus_mul_add_as_e(const uint8_t *seedA, const uint16_t *S,
							 const uint16_t *E, uint16_t *B)
{
	memcpy(B, E, 2 * SCLOUDPLUS_M * SCLOUDPLUS_NBAR);
	ALIGN_HEADER(32)
	uint16_t *AROWOUT = (uint16_t *)calloc((size_t)4 * SCLOUDPLUS_N, sizeof(uint16_t));
	ALIGN_HEADER(32)
	uint32_t *AROWIN = (uint32_t *)calloc((size_t)4 * SCLOUDPLUS_BLOCK_ROWLEN, sizeof(uint32_t));
	uint8_t aes_key_schedule[16 * 11];
	if (AROWOUT == NULL || AROWIN == NULL) {
		free(AROWOUT);
		free(AROWIN);
		return;
	}
	AES128_load_schedule(seedA, aes_key_schedule);
	for (int i = 0; i < SCLOUDPLUS_M; i += 4)
	{

		for (int j = 0; j < SCLOUDPLUS_BLOCK_NUMBER; j += 1)
		{
			AROWIN[SCLOUDPLUS_BLOCK_SIZE * j + 0 * SCLOUDPLUS_BLOCK_ROWLEN] =
				i * SCLOUDPLUS_BLOCK_NUMBER + j;
			AROWIN[SCLOUDPLUS_BLOCK_SIZE * j + 1 * SCLOUDPLUS_BLOCK_ROWLEN] =
				(i + 1) * SCLOUDPLUS_BLOCK_NUMBER + j;
			AROWIN[SCLOUDPLUS_BLOCK_SIZE * j + 2 * SCLOUDPLUS_BLOCK_ROWLEN] =
				(i + 2) * SCLOUDPLUS_BLOCK_NUMBER + j;
			AROWIN[SCLOUDPLUS_BLOCK_SIZE * j + 3 * SCLOUDPLUS_BLOCK_ROWLEN] =
				(i + 3) * SCLOUDPLUS_BLOCK_NUMBER + j;
		}
		AES128_CTR_enc_sch((uint8_t *)AROWIN, 4 * SCLOUDPLUS_N * sizeof(uint16_t),
						   aes_key_schedule, (uint8_t *)AROWOUT);

		for (int k = 0; k < SCLOUDPLUS_NBAR; k++)
		{
			uint16_t sum[4] = {0};
			for (int j = 0; j < SCLOUDPLUS_N; j++)
			{
				uint16_t sp = S[k * SCLOUDPLUS_N + j];
				sum[0] += AROWOUT[0 * SCLOUDPLUS_N + j] * sp;
				sum[1] += AROWOUT[1 * SCLOUDPLUS_N + j] * sp;
				sum[2] += AROWOUT[2 * SCLOUDPLUS_N + j] * sp;
				sum[3] += AROWOUT[3 * SCLOUDPLUS_N + j] * sp;
			}
			B[(i + 0) * SCLOUDPLUS_NBAR + k] += sum[0];
			B[(i + 1) * SCLOUDPLUS_NBAR + k] += sum[1];
			B[(i + 2) * SCLOUDPLUS_NBAR + k] += sum[2];
			B[(i + 3) * SCLOUDPLUS_NBAR + k] += sum[3];
		}
	}
	AES128_free_schedule(aes_key_schedule);
	free(AROWOUT);
	free(AROWIN);
}
// This code is based on the implementation of FrodoKEM
void scloudplus_mul_add_sa_e(const uint8_t *seedA, const uint16_t *S,
							 uint16_t *E, uint16_t *C)
{

	ALIGN_HEADER(32)
	uint16_t *AROWOUT = (uint16_t *)calloc((size_t)8 * SCLOUDPLUS_N, sizeof(uint16_t));

	uint8_t aes_key_schedule[16 * 11];

	ALIGN_HEADER(32)
	uint32_t *AROWIN = (uint32_t *)calloc((size_t)8 * SCLOUDPLUS_BLOCK_ROWLEN, sizeof(uint32_t));
	if (AROWOUT == NULL || AROWIN == NULL) {
		free(AROWOUT);
		free(AROWIN);
		return;
	}
	AES128_load_schedule(seedA, aes_key_schedule);

	for (int i = 0; i < SCLOUDPLUS_M; i += 8)
	{
		for (int q = 0; q < 8; q++)
		{
			for (int p = 0; p < SCLOUDPLUS_BLOCK_NUMBER; p += 1)
			{
				AROWIN[q * SCLOUDPLUS_BLOCK_ROWLEN + SCLOUDPLUS_BLOCK_SIZE * p] =
					(i + q) * SCLOUDPLUS_BLOCK_NUMBER + p;
			}
		}
		AES128_CTR_enc_sch((uint8_t *)AROWIN, 8 * SCLOUDPLUS_N * sizeof(uint16_t),
						   aes_key_schedule, (uint8_t *)AROWOUT);

		for (int j = 0; j < SCLOUDPLUS_MBAR; j++)
		{
			uint16_t sum = 0;
			uint16_t sp[8];
			for (int p = 0; p < 8; p++)
			{
				sp[p] = S[j * SCLOUDPLUS_M + i + p];
			}
			for (int q = 0; q < SCLOUDPLUS_N; q++)
			{
				sum = E[j * SCLOUDPLUS_N + q];
				for (int p = 0; p < 8; p++)
				{
					sum += sp[p] * AROWOUT[p * SCLOUDPLUS_N + q];
				}
				E[j * SCLOUDPLUS_N + q] = sum;
			}
		}
	}
	memcpy((unsigned char *)C, (unsigned char *)E,
		   2 * SCLOUDPLUS_MBAR * SCLOUDPLUS_N);
	AES128_free_schedule(aes_key_schedule);
	free(AROWOUT);
	free(AROWIN);
}

static inline uint32_t read3bytestou32(const uint8_t *ptr)
{
	return ((uint32_t)ptr[0] << 0) | ((uint32_t)ptr[1] << 8) |
		   ((uint32_t)ptr[2] << 16);
}
static inline uint32_t read4bytestou32(const uint8_t *ptr)
{
	return ((uint32_t)ptr[0]) | ((uint32_t)ptr[1] << 8) |
		   ((uint32_t)ptr[2] << 16) | ((uint32_t)ptr[3] << 24);
}

static inline uint64_t read7bytestou64(const uint8_t *ptr)
{
	return ((uint64_t)ptr[0] << 0) | ((uint64_t)ptr[1] << 8) |
		   ((uint64_t)ptr[2] << 16) | ((uint64_t)ptr[3] << 24) |
		   ((uint64_t)ptr[4] << 32) | ((uint64_t)ptr[5] << 40) |
		   ((uint64_t)ptr[6] << 48);
}

static inline void cbd1(uint8_t in, uint16_t *out)
{
	uint8_t b, b0, b1;
	b = in;
	for (size_t j = 0; j < 4; j++)
	{
		b0 = b & 1;
		b1 = (b >> 1) & 1;
		*(out + j) = (uint16_t)(b0 - b1);
		b = b >> 2;
	}
}

static inline void cbd2(uint8_t in, uint16_t *out)
{
	uint8_t b = 0;
	b += in & 0x55;
	b += (in >> 1) & 0x55;
	*out = (uint16_t)((b & 0x03) - ((b >> 2) & 0x03));
	*(out + 1) = (uint16_t)(((b >> 4) & 0x03) - ((b >> 6) & 0x03));
}

static inline void cbd3(uint32_t in, uint16_t *out)
{
	uint32_t b = 0;
	b += in & 0x00249249;
	b += (in >> 1) & 0x00249249;
	b += (in >> 2) & 0x00249249;
	for (int i = 0; i < 4; i++)
	{
		out[i] = ((b >> (6 * i)) & 0x07) - ((b >> (6 * i + 3)) & 0x07);
	}
}

static inline void cbd7(uint64_t in, uint16_t *out)
{
	uint64_t b0 = 0;
	b0 += in & 0x2040810204081;
	b0 += (in >> 1) & 0x2040810204081;
	b0 += (in >> 2) & 0x2040810204081;
	b0 += (in >> 3) & 0x2040810204081;
	b0 += (in >> 4) & 0x2040810204081;
	b0 += (in >> 5) & 0x2040810204081;
	b0 += (in >> 6) & 0x2040810204081;
	for (int i = 0; i < 4; i++)
	{
		out[i] = ((b0 >> (14 * i)) & 0x7F) - ((b0 >> (14 * i + 7)) & 0x7F);
	}
}

void scloudplus_sampleeta1(uint8_t *seed, uint16_t *matrixe)
{
	size_t hashlen =
		((SCLOUDPLUS_M * SCLOUDPLUS_NBAR) * (2 * SCLOUDPLUS_ETA1)) >> 3;
	uint8_t *tmp = (uint8_t *)malloc(hashlen * sizeof(uint8_t));
	uint8_t *ptrtmp = tmp;
	uint16_t *ptrmatrix = matrixe;
	memset(matrixe, 0, SCLOUDPLUS_M * SCLOUDPLUS_NBAR * sizeof(uint16_t));
	if (tmp == NULL) {
		return;
	}
	shake256(tmp, hashlen, seed, 32);
if (SCLOUDPLUS_ETA1 == 2) {
	for (size_t i = 0; i < SCLOUDPLUS_M * SCLOUDPLUS_NBAR; i = i + 2)
	{
		cbd2(*ptrtmp, ptrmatrix);
		ptrtmp = ptrtmp + 1;
		ptrmatrix = ptrmatrix + 2;
	}
} else if (SCLOUDPLUS_ETA1 == 3) {
	for (size_t i = 0; i < SCLOUDPLUS_M * SCLOUDPLUS_NBAR; i = i + 4)
	{
		cbd3(read3bytestou32(ptrtmp) & 0xFFFFFF, ptrmatrix);
		ptrtmp = ptrtmp + 3;
		ptrmatrix = ptrmatrix + 4;
	}
} else if (SCLOUDPLUS_ETA1 == 7) {
	for (size_t i = 0; i < SCLOUDPLUS_M * SCLOUDPLUS_NBAR; i = i + 4)
	{
		cbd7(read7bytestou64(ptrtmp) & 0xFFFFFFFFFFFFFF, ptrmatrix);
		ptrtmp = ptrtmp + 7;
		ptrmatrix = ptrmatrix + 4;
	}
}
	free(tmp);
}

void scloudplus_sampleeta2(uint8_t *seed, uint16_t *matrixe1,
						   uint16_t *matrixe2)
{
	size_t hash1len =
		((SCLOUDPLUS_MBAR * SCLOUDPLUS_N) * (2 * SCLOUDPLUS_ETA2)) >> 3;
	size_t hash2len =
		((SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR) * (2 * SCLOUDPLUS_ETA2) + 7) >> 3;
	uint8_t *tmp = (uint8_t *)malloc((hash1len + hash2len) * sizeof(uint8_t));
	uint8_t *ptrtmp1;
	uint8_t *ptrtmp2;
	uint16_t *ptrmatrix1 = matrixe1;
	uint16_t *ptrmatrix2 = matrixe2;
	memset(matrixe1, 0, SCLOUDPLUS_MBAR * SCLOUDPLUS_N * 2);
	memset(matrixe2, 0, SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR * 2);
	if (tmp == NULL) {
		return;
	}
	ptrtmp1 = tmp;
	ptrtmp2 = tmp + hash1len;
	shake256(tmp, hash1len + hash2len, seed, 32);
if (SCLOUDPLUS_ETA2 == 1) {
	for (size_t i = 0; i < SCLOUDPLUS_MBAR * SCLOUDPLUS_N; i = i + 4)
	{
		cbd1(*ptrtmp1, ptrmatrix1);
		ptrtmp1 = ptrtmp1 + 1;
		ptrmatrix1 = ptrmatrix1 + 4;
	}
	for (size_t i = 0; i < SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR; i = i + 4)
	{
		cbd1(*ptrtmp2, ptrmatrix2);
		ptrtmp2 = ptrtmp2 + 1;
		ptrmatrix2 = ptrmatrix2 + 4;
	}
} else if (SCLOUDPLUS_ETA2 == 2) {
	for (size_t i = 0; i < SCLOUDPLUS_MBAR * SCLOUDPLUS_N; i = i + 2)
	{
		cbd2(*ptrtmp1, ptrmatrix1);
		ptrtmp1 = ptrtmp1 + 1;
		ptrmatrix1 = ptrmatrix1 + 2;
	}
	for (size_t i = 0; i < SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR; i = i + 2)
	{
		cbd2(*ptrtmp2, ptrmatrix2);
		ptrtmp2 = ptrtmp2 + 1;
		ptrmatrix2 = ptrmatrix2 + 2;
	}
} else if (SCLOUDPLUS_ETA2 == 7) {
	for (size_t i = 0; i < SCLOUDPLUS_MBAR * SCLOUDPLUS_N; i = i + 4)
	{
		cbd7(read7bytestou64(ptrtmp1) & 0xFFFFFFFFFFFFFF, ptrmatrix1);
		ptrtmp1 = ptrtmp1 + 7;
		ptrmatrix1 = ptrmatrix1 + 4;
	}
	for (size_t i = 0; i < SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR; i = i + 4)
	{
		cbd7(read7bytestou64(ptrtmp2) & 0xFFFFFFFFFFFFFF, ptrmatrix2);
		ptrtmp2 = ptrtmp2 + 7;
		ptrmatrix2 = ptrmatrix2 + 4;
	}
}
	free(tmp);
}

void readu8ton(uint8_t *in, int n, uint16_t *out, int *outlen)
{
	uint8_t *ptrin = in;
	uint16_t *ptrout = out;
	*outlen = 0;
if (SCLOUDPLUS_L == 128) {
	uint32_t tmp;
	for (int i = 0; i < n; i = i + 7)
	{
		tmp = read4bytestou32(ptrin) & 0xFFFFFFF;
		if (tmp < SCLOUDPLUS_N3)
		{
			*ptrout = tmp % SCLOUDPLUS_N;
			*(ptrout + 1) = tmp / SCLOUDPLUS_N % SCLOUDPLUS_N;
			*(ptrout + 2) = tmp / SCLOUDPLUS_N2 % SCLOUDPLUS_N;
			ptrout = ptrout + 3;
			*outlen += 3;
		}
		tmp = (read4bytestou32(ptrin + 3) >> 4) & 0xFFFFFFF;
		if (tmp < SCLOUDPLUS_N3)
		{
			*ptrout = tmp % SCLOUDPLUS_N;
			*(ptrout + 1) = tmp / SCLOUDPLUS_N % SCLOUDPLUS_N;
			*(ptrout + 2) = tmp / SCLOUDPLUS_N2 % SCLOUDPLUS_N;
			ptrout = ptrout + 3;
			*outlen += 3;
		}
		ptrin = ptrin + 7;
	}
} else if (SCLOUDPLUS_L == 192) {
	uint16_t tmp[8] = {0};
	for (int i = 0; i < n; i = i + 11)
	{
		tmp[0] = *(uint16_t *)ptrin & 0x7FF;
		tmp[1] = (*(uint16_t *)(ptrin + 1) >> 3) & 0x7FF;
		tmp[2] = (*(uint32_t *)(ptrin + 2) >> 6) & 0x7FF;
		tmp[3] = (*(uint16_t *)(ptrin + 4) >> 1) & 0x7FF;
		tmp[4] = (*(uint16_t *)(ptrin + 5) >> 4) & 0x7FF;
		tmp[5] = (*(uint32_t *)(ptrin + 6) >> 7) & 0x7FF;
		tmp[6] = (*(uint16_t *)(ptrin + 8) >> 2) & 0x7FF;
		tmp[7] = (*(uint16_t *)(ptrin + 9) >> 5) & 0x7FF;
		for (int j = 0; j < 8; j++)
		{
			if (tmp[j] < SCLOUDPLUS_N)
			{
				*ptrout = tmp[j];
				ptrout = ptrout + 1;
				*outlen += 1;
			}
		}
		ptrin = ptrin + 11;
	}
} else if (SCLOUDPLUS_L == 256) {
	uint64_t A[8] = {0};
	for (int i = 0; i < 13; i++)
	{
		A[0] = *(uint64_t *)ptrin & 0x7FFFFFFFFFFFF;
		A[1] = (*(uint64_t *)(ptrin + 6) >> 3) & 0x7FFFFFFFFFFFF;
		A[2] = (*(uint64_t *)(ptrin + 12) >> 6) & 0x7FFFFFFFFFFFF;
		A[3] = (*(uint64_t *)(ptrin + 19) >> 1) & 0x7FFFFFFFFFFFF;
		A[4] = (*(uint64_t *)(ptrin + 25) >> 4) & 0x7FFFFFFFFFFFF;
		A[5] = (*(uint64_t *)(ptrin + 31) >> 7) & 0x7FFFFFFFFFFFF;
		A[6] = (*(uint64_t *)(ptrin + 38) >> 2) & 0x7FFFFFFFFFFFF;
		A[7] = (*(uint64_t *)(ptrin + 44) >> 5) & 0x7FFFFFFFFFFFF;
		for (int j = 0; j < 8; j++)
		{
			if (A[j] < SCLOUDPLUS_N5)
			{
				*ptrout = A[j] % SCLOUDPLUS_N;
				*(ptrout + 1) = A[j] / SCLOUDPLUS_N % SCLOUDPLUS_N;
				*(ptrout + 2) = A[j] / SCLOUDPLUS_N2 % SCLOUDPLUS_N;
				*(ptrout + 3) = A[j] / SCLOUDPLUS_N3 % SCLOUDPLUS_N;
				*(ptrout + 4) = A[j] / SCLOUDPLUS_N4 % SCLOUDPLUS_N;
				ptrout = ptrout + 5;
				*outlen += 5;
			}
		}
		ptrin = ptrin + 51;
	}
	A[0] = *(uint64_t *)ptrin & 0x7FFFFFFFFFFFF;
	A[1] = (*(uint64_t *)(ptrin + 6) >> 3) & 0x7FFFFFFFFFFFF;
	for (int j = 0; j < 2; j++)
	{
		if (A[j] < SCLOUDPLUS_N5)
		{
			*ptrout = A[j] % SCLOUDPLUS_N;
			*(ptrout + 1) = A[j] / SCLOUDPLUS_N % SCLOUDPLUS_N;
			*(ptrout + 2) = A[j] / SCLOUDPLUS_N2 % SCLOUDPLUS_N;
			*(ptrout + 3) = A[j] / SCLOUDPLUS_N3 % SCLOUDPLUS_N;
			*(ptrout + 4) = A[j] / SCLOUDPLUS_N4 % SCLOUDPLUS_N;
			ptrout = ptrout + 5;
			*outlen += 5;
		}
	}
}
}

void readu8tom(uint8_t *in, int n, uint16_t *out, int *outlen)
{
	uint8_t *ptrin = in;
	uint16_t *ptrout = out;
	*outlen = 0;
if (SCLOUDPLUS_L == 128) {
	uint32_t tmp;
	for (int i = 0; i < n; i = i + 7)
	{
		tmp = read4bytestou32(ptrin) & 0xFFFFFFF;
		if (tmp < SCLOUDPLUS_M3)
		{
			*ptrout = tmp % SCLOUDPLUS_M;
			*(ptrout + 1) = tmp / SCLOUDPLUS_M % SCLOUDPLUS_M;
			*(ptrout + 2) = tmp / SCLOUDPLUS_M2 % SCLOUDPLUS_M;
			ptrout = ptrout + 3;
			*outlen += 3;
		}
		tmp = (read4bytestou32(ptrin + 3) >> 4) & 0xFFFFFFF;
		if (tmp < SCLOUDPLUS_M3)
		{
			*ptrout = tmp % SCLOUDPLUS_M;
			*(ptrout + 1) = tmp / SCLOUDPLUS_M % SCLOUDPLUS_M;
			*(ptrout + 2) = tmp / SCLOUDPLUS_M2 % SCLOUDPLUS_M;
			ptrout = ptrout + 3;
			*outlen += 3;
		}
		ptrin = ptrin + 7;
	}
} else if (SCLOUDPLUS_L == 192) {
	uint16_t tmp[8] = {0};
	for (int i = 0; i < n; i = i + 11)
	{
		tmp[0] = *(uint16_t *)ptrin & 0x7FF;
		tmp[1] = (*(uint16_t *)(ptrin + 1) >> 3) & 0x7FF;
		tmp[2] = (*(uint32_t *)(ptrin + 2) >> 6) & 0x7FF;
		tmp[3] = (*(uint16_t *)(ptrin + 4) >> 1) & 0x7FF;
		tmp[4] = (*(uint16_t *)(ptrin + 5) >> 4) & 0x7FF;
		tmp[5] = (*(uint32_t *)(ptrin + 6) >> 7) & 0x7FF;
		tmp[6] = (*(uint16_t *)(ptrin + 8) >> 2) & 0x7FF;
		tmp[7] = (*(uint16_t *)(ptrin + 9) >> 5) & 0x7FF;
		for (int j = 0; j < 8; j++)
		{
			if (tmp[j] < SCLOUDPLUS_N)
			{
				*ptrout = tmp[j];
				ptrout = ptrout + 1;
				*outlen += 1;
			}
		}
		ptrin = ptrin + 11;
	}
} else if (SCLOUDPLUS_L == 256) {
	uint64_t A[8] = {0};
	for (int i = 0; i < 13; i++)
	{
		A[0] = *(uint64_t *)ptrin & 0x7FFFFFFFFFFFF;
		A[1] = (*(uint64_t *)(ptrin + 6) >> 3) & 0x7FFFFFFFFFFFF;
		A[2] = (*(uint64_t *)(ptrin + 12) >> 6) & 0x7FFFFFFFFFFFF;
		A[3] = (*(uint64_t *)(ptrin + 19) >> 1) & 0x7FFFFFFFFFFFF;
		A[4] = (*(uint64_t *)(ptrin + 25) >> 4) & 0x7FFFFFFFFFFFF;
		A[5] = (*(uint64_t *)(ptrin + 31) >> 7) & 0x7FFFFFFFFFFFF;
		A[6] = (*(uint64_t *)(ptrin + 38) >> 2) & 0x7FFFFFFFFFFFF;
		A[7] = (*(uint64_t *)(ptrin + 44) >> 5) & 0x7FFFFFFFFFFFF;
		for (int j = 0; j < 8; j++)
		{
			if (A[j] < SCLOUDPLUS_M5)
			{
				*ptrout = A[j] % SCLOUDPLUS_M;
				*(ptrout + 1) = A[j] / SCLOUDPLUS_M % SCLOUDPLUS_M;
				*(ptrout + 2) = A[j] / SCLOUDPLUS_M2 % SCLOUDPLUS_M;
				*(ptrout + 3) = A[j] / SCLOUDPLUS_M3 % SCLOUDPLUS_M;
				*(ptrout + 4) = A[j] / SCLOUDPLUS_M4 % SCLOUDPLUS_M;
				ptrout = ptrout + 5;
				*outlen += 5;
			}
		}
		ptrin = ptrin + 51;
	}
	A[0] = *(uint64_t *)ptrin & 0x7FFFFFFFFFFFF;
	A[1] = (*(uint64_t *)(ptrin + 6) >> 3) & 0x7FFFFFFFFFFFF;
	for (int j = 0; j < 2; j++)
	{
		if (A[j] < SCLOUDPLUS_M5)
		{
			*ptrout = A[j] % SCLOUDPLUS_M;
			*(ptrout + 1) = A[j] / SCLOUDPLUS_M % SCLOUDPLUS_M;
			*(ptrout + 2) = A[j] / SCLOUDPLUS_M2 % SCLOUDPLUS_M;
			*(ptrout + 3) = A[j] / SCLOUDPLUS_M3 % SCLOUDPLUS_M;
			*(ptrout + 4) = A[j] / SCLOUDPLUS_M4 % SCLOUDPLUS_M;
			ptrout = ptrout + 5;
			*outlen += 5;
		}
	}
}
}

void scloudplus_samplepsi(uint8_t *seed, uint16_t *matrixs)
{
	memset(matrixs, 0, SCLOUDPLUS_N * SCLOUDPLUS_NBAR * 2);
	uint8_t *hash = (uint8_t *)calloc(680, sizeof(uint8_t));
	uint16_t *tmp = (uint16_t *)calloc(SCLOUDPLUS_MNOUT, sizeof(uint16_t));
	keccak_state state;
	int outlen, condition, k = 0;
	uint16_t location, mask;
	if (hash == NULL || tmp == NULL) {
		free(hash);
		free(tmp);
		return;
	}
	shake256_absorb_once(&state, seed, 32);
	shake256_squeezeblocks(hash, 5, &state);
	readu8ton(hash, SCLOUDPLUS_MNIN, tmp, &outlen);
	for (int i = 0; i < SCLOUDPLUS_NBAR; i++)
	{
		int j = 0;
		while (j < SCLOUDPLUS_H1 * 2)
		{
			if (k == outlen)
			{
				shake256_squeezeblocks(hash, 5, &state);
				readu8ton(hash, SCLOUDPLUS_MNIN, tmp, &outlen);
				k = 0;
			}
			location = tmp[k];
			condition = (matrixs[i * SCLOUDPLUS_N + location] == 0);
			mask = -condition;
			matrixs[i * SCLOUDPLUS_N + location] =
				(matrixs[i * SCLOUDPLUS_N + location] & ~mask) |
				((1 - 2 * (j & 1)) & mask);
			j += condition;
			k++;
		}
	}
	free(hash);
	free(tmp);
}

void scloudplus_samplephi(uint8_t *seed, uint16_t *matrixs)
{
	memset(matrixs, 0, SCLOUDPLUS_MBAR * SCLOUDPLUS_M * 2);
	uint8_t *hash = (uint8_t *)calloc(680, sizeof(uint8_t));
	uint16_t *tmp = (uint16_t *)calloc(SCLOUDPLUS_MNOUT, sizeof(uint16_t));
	keccak_state state;
	int outlen, condition, k = 0;
	uint16_t location, mask;
	if (hash == NULL || tmp == NULL) {
		free(hash);
		free(tmp);
		return;
	}
	shake256_absorb_once(&state, seed, 32);
	shake256_squeezeblocks(hash, 5, &state);
	readu8tom(hash, SCLOUDPLUS_MNIN, tmp, &outlen);
	for (int i = 0; i < SCLOUDPLUS_MBAR; i++)
	{
		int j = 0;
		while (j < SCLOUDPLUS_H2 * 2)
		{
			if (k == outlen)
			{
				shake256_squeezeblocks(hash, 5, &state);
				readu8tom(hash, SCLOUDPLUS_MNIN, tmp, &outlen);
				k = 0;
			}
			location = tmp[k];
			condition = (matrixs[i * SCLOUDPLUS_M + location] == 0);
			mask = -condition;
			matrixs[i * SCLOUDPLUS_M + location] =
				(matrixs[i * SCLOUDPLUS_M + location] & ~mask) |
				((1 - 2 * (j & 1)) & mask);
			j += condition;
			k++;
		}
	}
	free(hash);
	free(tmp);
}
