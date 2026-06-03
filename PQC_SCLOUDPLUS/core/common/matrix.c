#include "matrix.h"
#include "param.h"
#include "operator_interface.h"
#include <string.h>

static void load_block_8x8(uint16_t dst[8][8], const uint16_t *src,
						   int row, int col, int rows, int cols)
{
	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			if (row + r < rows && col + c < cols)
			{
				dst[r][c] = src[(row + r) * cols + col + c];
			}
			else
			{
				dst[r][c] = 0;
			}
		}
	}
}

static void load_transposed_block_8x8(uint16_t dst[8][8], const uint16_t *src,
									  int row, int col, int rows, int cols)
{
	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			if (row + c < rows && col + r < cols)
			{
				dst[r][c] = src[(row + c) * cols + col + r];
			}
			else
			{
				dst[r][c] = 0;
			}
		}
	}
}

static void add_block_8x8(uint16_t *dst, const uint16_t src[8][8],
						  int row, int col, int rows, int cols)
{
	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			if (row + r < rows && col + c < cols)
			{
				dst[(row + r) * cols + col + c] =
					(uint16_t)(dst[(row + r) * cols + col + c] + src[r][c]);
			}
		}
	}
}

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
	for (int i = 0; i < SCLOUDPLUS_MBAR; i += 8)
	{
		for (int j = 0; j < SCLOUDPLUS_NBAR; j += 8)
		{
			for (int k = 0; k < SCLOUDPLUS_N; k += 8)
			{
				load_block_8x8(x, C, i, k, SCLOUDPLUS_MBAR, SCLOUDPLUS_N);
				load_transposed_block_8x8(y, S, j, k, SCLOUDPLUS_NBAR, SCLOUDPLUS_N);
				if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x, (const uint16_t (*)[8])y, 0) != OP_SUCCESS) {
					return;
				}
				add_block_8x8(out, (const uint16_t (*)[8])z, i, j,
							  SCLOUDPLUS_MBAR, SCLOUDPLUS_NBAR);
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
	if (out != E) {
		memcpy(out, E, SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR * 2);
	}
	for (int i = 0; i < SCLOUDPLUS_MBAR; i += 8)
	{
		for (int j = 0; j < SCLOUDPLUS_NBAR; j += 8)
		{
			for (int k = 0; k < SCLOUDPLUS_M; k += 8)
			{
				load_block_8x8(x, S, i, k, SCLOUDPLUS_MBAR, SCLOUDPLUS_M);
				load_block_8x8(y, B, k, j, SCLOUDPLUS_M, SCLOUDPLUS_NBAR);
				if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x, (const uint16_t (*)[8])y, 0) != OP_SUCCESS) {
					return;
				}
				add_block_8x8(out, (const uint16_t (*)[8])z, i, j,
							  SCLOUDPLUS_MBAR, SCLOUDPLUS_NBAR);
			}
		}
	}
}
