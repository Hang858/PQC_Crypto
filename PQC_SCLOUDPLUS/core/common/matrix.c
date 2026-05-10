#include "matrix.h"
#include "param.h"
#include "operator_interface.h"
#include <string.h>
void scloudplus_add(uint16_t *in0, uint16_t *in1, int len, uint16_t *out)
{
	for (int i = 0; i < len; i++)
	{
		out[i] = (in0[i] + in1[i]) & 0xFFF;
	}
}
void scloudplus_sub(uint16_t *in0, uint16_t *in1, int len, uint16_t *out)
{
	for (int i = 0; i < len; i++)
	{
		out[i] = (in0[i] - in1[i]) & 0xFFF;
	}
}

void scloudplus_mul_cs(uint16_t *C, uint16_t *S, uint16_t *out)
{
	uint16_t x[8][8];
	uint16_t y[8][8];
	uint16_t z[8][8];
	memset(out, 0, SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR * 2);
	for (int block = 0; block < SCLOUDPLUS_N; block += 8)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int k = 0; k < 8; k++)
			{
				x[i][k] = C[i * SCLOUDPLUS_N + block + k];
				y[k][i] = S[i * SCLOUDPLUS_N + block + k];
			}
		}
		if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x, (const uint16_t (*)[8])y, 0) != OP_SUCCESS) {
			return;
		}
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				out[i * SCLOUDPLUS_NBAR + j] =
					(uint16_t)(out[i * SCLOUDPLUS_NBAR + j] + z[i][j]);
			}
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 8; j < SCLOUDPLUS_NBAR; j++)
		{
			for (int k = 0; k < SCLOUDPLUS_N; k++)
			{
				out[i * SCLOUDPLUS_NBAR + j] +=
					C[i * SCLOUDPLUS_N + k] * (uint16_t)S[j * SCLOUDPLUS_N + k];
			}
		}
	}
	for (int i = 8; i < SCLOUDPLUS_MBAR; i++)
	{
		for (int j = 0; j < SCLOUDPLUS_NBAR; j++)
		{
			for (int k = 0; k < SCLOUDPLUS_N; k++)
			{
				out[i * SCLOUDPLUS_NBAR + j] +=
					C[i * SCLOUDPLUS_N + k] * (uint16_t)S[j * SCLOUDPLUS_N + k];
			}
		}
	}
}
void scloudplus_mul_add_sb_e(const uint16_t *S, const uint16_t *B,
							 const uint16_t *E, uint16_t *out)
{
	uint16_t x[8][8];
	uint16_t y[8][8];
	uint16_t z[8][8];
	memcpy(out, E, SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR * 2);
	for (int block = 0; block < SCLOUDPLUS_M; block += 8)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int k = 0; k < 8; k++)
			{
				x[i][k] = S[i * SCLOUDPLUS_M + block + k];
				y[k][i] = B[(block + k) * SCLOUDPLUS_NBAR + i];
			}
		}
		if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x, (const uint16_t (*)[8])y, 0) != OP_SUCCESS) {
			return;
		}
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				out[i * SCLOUDPLUS_NBAR + j] =
					(uint16_t)(out[i * SCLOUDPLUS_NBAR + j] + z[i][j]);
			}
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 8; j < SCLOUDPLUS_NBAR; j++)
		{
			for (int k = 0; k < SCLOUDPLUS_M; k++)
			{
				out[i * SCLOUDPLUS_NBAR + j] +=
					(uint16_t)S[i * SCLOUDPLUS_M + k] * B[k * SCLOUDPLUS_NBAR + j];
			}
		}
	}
	for (int i = 8; i < SCLOUDPLUS_MBAR; i++)
	{
		for (int j = 0; j < SCLOUDPLUS_NBAR; j++)
		{
			for (int k = 0; k < SCLOUDPLUS_M; k++)
			{
				out[i * SCLOUDPLUS_NBAR + j] +=
					(uint16_t)S[i * SCLOUDPLUS_M + k] * B[k * SCLOUDPLUS_NBAR + j];
			}
		}
	}
}
