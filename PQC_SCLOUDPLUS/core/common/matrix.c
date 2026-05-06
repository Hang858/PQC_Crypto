#include "matrix.h"
#include "param.h"
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
	memset(out, 0, SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR * 2);
	for (int i = 0; i < SCLOUDPLUS_MBAR; i++)
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
	memcpy(out, E, SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR * 2);
	for (int i = 0; i < SCLOUDPLUS_MBAR; i++)
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