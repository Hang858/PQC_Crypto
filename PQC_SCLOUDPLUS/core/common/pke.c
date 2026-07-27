#include "pke.h"
#include "param.h"
#include "encode.h"
#include "matrix.h"
#include "sample.h"
#include "fips202.h"
#include "random.h"
#include <stdlib.h>
#include <string.h>

static inline uint32_t
scloudplus_read3bytestou32(const uint8_t *ptr)
{
	return ((uint32_t)ptr[0] << 0) | ((uint32_t)ptr[1] << 8) |
		   ((uint32_t)ptr[2] << 16);
}

static inline uint64_t
scloudplus_read7bytestou64(const uint8_t *ptr)
{
	return ((uint64_t)ptr[0] << 0) | ((uint64_t)ptr[1] << 8) |
		   ((uint64_t)ptr[2] << 16) | ((uint64_t)ptr[3] << 24) |
		   ((uint64_t)ptr[4] << 32) | ((uint64_t)ptr[5] << 40) |
		   ((uint64_t)ptr[6] << 48);
}

static inline void
scloudplus_cbd1(uint8_t in, uint16_t *out)
{
	uint8_t b = in;

	for (size_t j = 0; j < 4; j++) {
		uint8_t b0 = b & 1;
		uint8_t b1 = (b >> 1) & 1;
		out[j] = (uint16_t)(b0 - b1);
		b >>= 2;
	}
}

static inline void
scloudplus_cbd2(uint8_t in, uint16_t *out)
{
	uint8_t b = 0;

	b += in & 0x55;
	b += (in >> 1) & 0x55;
	out[0] = (uint16_t)((b & 0x03) - ((b >> 2) & 0x03));
	out[1] = (uint16_t)(((b >> 4) & 0x03) - ((b >> 6) & 0x03));
}

static inline void
scloudplus_cbd3(uint32_t in, uint16_t *out)
{
	uint32_t b = 0;

	b += in & 0x00249249;
	b += (in >> 1) & 0x00249249;
	b += (in >> 2) & 0x00249249;
	for (int i = 0; i < 4; i++) {
		out[i] = (uint16_t)(((b >> (6 * i)) & 0x07) -
							((b >> (6 * i + 3)) & 0x07));
	}
}

static inline void
scloudplus_cbd7(uint64_t in, uint16_t *out)
{
	uint64_t b0 = 0;

	b0 += in & 0x2040810204081;
	b0 += (in >> 1) & 0x2040810204081;
	b0 += (in >> 2) & 0x2040810204081;
	b0 += (in >> 3) & 0x2040810204081;
	b0 += (in >> 4) & 0x2040810204081;
	b0 += (in >> 5) & 0x2040810204081;
	b0 += (in >> 6) & 0x2040810204081;
	for (int i = 0; i < 4; i++) {
		out[i] = (uint16_t)(((b0 >> (14 * i)) & 0x7F) -
							((b0 >> (14 * i + 7)) & 0x7F));
	}
}

typedef struct {
	keccak_state state;
	uint8_t block[SHAKE256_RATE];
	size_t block_pos;
	int eta;
	uint16_t values[4];
	int value_pos;
	int value_count;
} scloudplus_eta_stream_t;

static void
scloudplus_eta_stream_init(scloudplus_eta_stream_t *stream, uint8_t *seed, int eta)
{
	shake256_absorb_once(&stream->state, seed, 32);
	stream->block_pos = SHAKE256_RATE;
	stream->eta = eta;
	stream->value_pos = 0;
	stream->value_count = 0;
}

static uint8_t
scloudplus_eta_stream_get_byte(scloudplus_eta_stream_t *stream)
{
	if (stream->block_pos == SHAKE256_RATE) {
		shake256_squeezeblocks(stream->block, 1, &stream->state);
		stream->block_pos = 0;
	}
	return stream->block[stream->block_pos++];
}

static uint16_t
scloudplus_eta_stream_next(scloudplus_eta_stream_t *stream)
{
	if (stream->value_pos == stream->value_count) {
		if (stream->eta == 1) {
			scloudplus_cbd1(scloudplus_eta_stream_get_byte(stream), stream->values);
			stream->value_count = 4;
		} else if (stream->eta == 2) {
			scloudplus_cbd2(scloudplus_eta_stream_get_byte(stream), stream->values);
			stream->value_count = 2;
		} else if (stream->eta == 3) {
			uint8_t buf[3];

			for (int i = 0; i < 3; i++) {
				buf[i] = scloudplus_eta_stream_get_byte(stream);
			}
			scloudplus_cbd3(scloudplus_read3bytestou32(buf) & 0xFFFFFF,
							stream->values);
			stream->value_count = 4;
		} else {
			uint8_t buf[7];

			for (int i = 0; i < 7; i++) {
				buf[i] = scloudplus_eta_stream_get_byte(stream);
			}
			scloudplus_cbd7(scloudplus_read7bytestou64(buf) & 0xFFFFFFFFFFFFFF,
							stream->values);
			stream->value_count = 4;
		}
		stream->value_pos = 0;
	}
	return stream->values[stream->value_pos++];
}

static uint16_t
scloudplus_pk_get(const uint8_t *pk, size_t row, size_t col)
{
	size_t idx = row * (size_t)SCLOUDPLUS_NBAR + col;
	const uint8_t *p = pk + (idx >> 1) * 3;

	if ((idx & 1u) == 0) {
		return (uint16_t)(p[0] | (((uint16_t)p[1] & 0x0F) << 8));
	}
	return (uint16_t)(((uint16_t)p[1] >> 4) | ((uint16_t)p[2] << 4));
}

static void
scloudplus_pk_put(uint8_t *pk, size_t row, size_t col, uint16_t v)
{
	size_t idx = row * (size_t)SCLOUDPLUS_NBAR + col;
	uint8_t *p = pk + (idx >> 1) * 3;

	v &= 0x0FFF;
	if ((idx & 1u) == 0) {
		p[0] = (uint8_t)v;
		p[1] = (uint8_t)((p[1] & 0xF0) | (v >> 8));
	} else {
		p[1] = (uint8_t)((p[1] & 0x0F) | ((v & 0x000F) << 4));
		p[2] = (uint8_t)(v >> 4);
	}
}

static void
scloudplus_c1_put_compressed(uint8_t *out, size_t idx, uint16_t v)
{
	size_t inlen = (size_t)SCLOUDPLUS_MBAR * SCLOUDPLUS_N;

	if (SCLOUDPLUS_L == 128) {
		v = (uint16_t)(((((uint32_t)(v & 0x0FFF) << 9) + 2048) >> 12) & 0x01FF);
		out[idx] = (uint8_t)v;
		if ((idx & 7u) == 0) {
			out[inlen + (idx >> 3)] = 0;
		}
		out[inlen + (idx >> 3)] =
			(uint8_t)(out[inlen + (idx >> 3)] | ((v >> 8) << (7 - (idx & 7u))));
	} else if (SCLOUDPLUS_L == 192) {
		uint8_t *p = out + (idx >> 1) * 3;

		v &= 0x0FFF;
		if ((idx & 1u) == 0) {
			p[0] = (uint8_t)v;
			p[1] = (uint8_t)((p[1] & 0xF0) | (v >> 8));
		} else {
			p[1] = (uint8_t)((p[1] & 0x0F) | ((v & 0x000F) << 4));
			p[2] = (uint8_t)(v >> 4);
		}
	} else {
		v = (uint16_t)(((((uint32_t)(v & 0x0FFF) << 10) + 2048) >> 12) & 0x03FF);
		out[idx] = (uint8_t)v;
		if ((idx & 3u) == 0) {
			out[inlen + (idx >> 2)] = 0;
		}
		out[inlen + (idx >> 2)] =
			(uint8_t)(out[inlen + (idx >> 2)] | ((v >> 8) << (6 - 2 * (idx & 3u))));
	}
}

static uint16_t
scloudplus_c1_get_decompressed(const uint8_t *in, size_t idx)
{
	size_t outlen = (size_t)SCLOUDPLUS_MBAR * SCLOUDPLUS_N;
	uint16_t v;

	if (SCLOUDPLUS_L == 128) {
		v = in[idx];
		v = (uint16_t)(v | (((uint16_t)in[outlen + (idx >> 3)] << (1 + (idx & 7u))) & 0x0100));
		return (uint16_t)(((uint32_t)((v & 0x01FF) << 12) + 256) >> 9);
	}
	if (SCLOUDPLUS_L == 192) {
		const uint8_t *p = in + (idx >> 1) * 3;
		if ((idx & 1u) == 0) {
			v = (uint16_t)(p[0] | (((uint16_t)p[1] & 0x0F) << 8));
		} else {
			v = (uint16_t)(((uint16_t)p[1] >> 4) | ((uint16_t)p[2] << 4));
		}
		return v;
	}
	v = in[idx];
	v = (uint16_t)(v | (((uint16_t)in[outlen + (idx >> 2)] << (2 + 2 * (idx & 3u))) & 0x0300));
	return (uint16_t)(((uint32_t)((v & 0x03FF) << 12) + 512) >> 10);
}

static void
scloudplus_gen_a_tile(const uint8_t *seedA, int row, int col,
					  const uint8_t *aes_key_schedule, uint16_t out[8][8])
{
	uint8_t in[8][16];
	uint8_t enc[8][16];

	memset(in, 0, sizeof in);
	for (int r = 0; r < 8; r++) {
		if (row + r < SCLOUDPLUS_M) {
			uint32_t ctr = (uint32_t)((row + r) * SCLOUDPLUS_BLOCK_NUMBER + (col >> 3));
			in[r][0] = (uint8_t)ctr;
			in[r][1] = (uint8_t)(ctr >> 8);
			in[r][2] = (uint8_t)(ctr >> 16);
			in[r][3] = (uint8_t)(ctr >> 24);
		}
	}
	AES128_CTR_enc_sch((const uint8_t *)in, sizeof in, aes_key_schedule, (uint8_t *)enc);
	(void)seedA;
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			if (row + r < SCLOUDPLUS_M && col + c < SCLOUDPLUS_N) {
				out[r][c] = (uint16_t)(enc[r][2 * c] | ((uint16_t)enc[r][2 * c + 1] << 8));
			} else {
				out[r][c] = 0;
			}
		}
	}
}

static void
scloudplus_load_s_transposed(uint16_t dst[8][8], const uint16_t *S,
							 int col, int nbar_col)
{
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			if (col + r < SCLOUDPLUS_N && nbar_col + c < SCLOUDPLUS_NBAR) {
				dst[r][c] = S[(size_t)(nbar_col + c) * SCLOUDPLUS_N + (col + r)];
			} else {
				dst[r][c] = 0;
			}
		}
	}
}

static int
scloudplus_samplephi_row(uint8_t *seed, int target_row, uint16_t *row)
{
	uint8_t *hash = calloc(680, sizeof(uint8_t));
	uint16_t *tmp = calloc(SCLOUDPLUS_MNOUT, sizeof(uint16_t));
	uint16_t *scratch = calloc(SCLOUDPLUS_M, sizeof(uint16_t));
	keccak_state state;
	int outlen, k = 0;

	if (hash == NULL || tmp == NULL || scratch == NULL) {
		free(hash);
		free(tmp);
		free(scratch);
		return -1;
	}
	memset(row, 0, (size_t)SCLOUDPLUS_M * sizeof(uint16_t));
	shake256_absorb_once(&state, seed, 32);
	shake256_squeezeblocks(hash, 5, &state);
	readu8tom(hash, SCLOUDPLUS_MNIN, tmp, &outlen);
	for (int i = 0; i <= target_row; i++) {
		uint16_t *cur = (i == target_row) ? row : scratch;
		int j = 0;
		memset(cur, 0, (size_t)SCLOUDPLUS_M * sizeof(uint16_t));
		while (j < SCLOUDPLUS_H2 * 2) {
			uint16_t location;
			int condition;
			uint16_t mask;

			if (k == outlen) {
				shake256_squeezeblocks(hash, 5, &state);
				readu8tom(hash, SCLOUDPLUS_MNIN, tmp, &outlen);
				k = 0;
			}
			location = tmp[k];
			condition = (cur[location] == 0);
			mask = (uint16_t)-condition;
			cur[location] = (cur[location] & ~mask) |
				((uint16_t)(1 - 2 * (j & 1)) & mask);
			j += condition;
			k++;
		}
	}
	free(hash);
	free(tmp);
	free(scratch);
	return 0;
}
int scloudplus_pkekeygen(uint8_t *pk, uint8_t *sk)
{
	uint16_t *S =
		(uint16_t *)malloc(sizeof(uint16_t) * SCLOUDPLUS_N * SCLOUDPLUS_NBAR);
	uint8_t alpha[32], seed[80];
	uint8_t *seedA = seed;
	uint8_t *r1 = seed + 16;
	uint8_t *r2 = seed + 48;
	scloudplus_eta_stream_t eta_stream;
	uint8_t aes_key_schedule[16 * 11];
	uint16_t x[8][8];
	uint16_t y[8][8];
	uint16_t z[8][8];
	uint16_t B_tile[8][8];
	uint16_t E_block[8][16];

	if (S == NULL) {
		free(S);
		return -1;
	}
	randombytes(alpha, 32);
	scloudplus_F(seed, 80, alpha, 32);
	scloudplus_samplepsi(r1, S);
	scloudplus_eta_stream_init(&eta_stream, r2, SCLOUDPLUS_ETA1);
	memset(pk, 0, SCLOUDPLUS_PK);
	AES128_load_schedule(seedA, aes_key_schedule);
	for (int i = 0; i < SCLOUDPLUS_M; i += 8) {
		memset(E_block, 0, sizeof E_block);
		for (int r = 0; r < 8; r++) {
			if (i + r >= SCLOUDPLUS_M) {
				continue;
			}
			for (int c = 0; c < SCLOUDPLUS_NBAR; c++) {
				E_block[r][c] = scloudplus_eta_stream_next(&eta_stream);
			}
		}
		for (int jbar = 0; jbar < SCLOUDPLUS_NBAR; jbar += 8) {
			for (int r = 0; r < 8; r++) {
				for (int c = 0; c < 8; c++) {
					if (i + r < SCLOUDPLUS_M && jbar + c < SCLOUDPLUS_NBAR) {
						B_tile[r][c] = E_block[r][jbar + c];
					} else {
						B_tile[r][c] = 0;
					}
				}
			}
			for (int k = 0; k < SCLOUDPLUS_N; k += 8) {
				scloudplus_gen_a_tile(seedA, i, k, aes_key_schedule, x);
				scloudplus_load_s_transposed(y, S, k, jbar);
				if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x,
									  (const uint16_t (*)[8])y, 0) != OP_SUCCESS) {
					AES128_free_schedule(aes_key_schedule);
					free(S);
					return -1;
				}
				for (int r = 0; r < 8; r++) {
					for (int c = 0; c < 8; c++) {
						if (i + r < SCLOUDPLUS_M && jbar + c < SCLOUDPLUS_NBAR) {
							B_tile[r][c] = (uint16_t)(B_tile[r][c] + z[r][c]);
						}
					}
				}
			}
			for (int r = 0; r < 8; r++) {
				for (int c = 0; c < 8; c++) {
					if (i + r < SCLOUDPLUS_M && jbar + c < SCLOUDPLUS_NBAR) {
						scloudplus_pk_put(pk, (size_t)i + r,
										  (size_t)jbar + c, B_tile[r][c]);
					}
				}
			}
		}
	}
	AES128_free_schedule(aes_key_schedule);
	memcpy(pk + SCLOUDPLUS_PK - 16, seedA, 16);
	scloudplus_packsk(S, sk);
	free(S);
	return 0;
}

int scloudplus_pkeenc(uint8_t *pk, uint8_t *m, uint8_t *r, uint8_t *ctx)
{
	uint16_t *S_row =
		(uint16_t *)malloc(sizeof(uint16_t) * SCLOUDPLUS_M);
	uint16_t *mu0 =
		(uint16_t *)malloc(sizeof(uint16_t) * SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR);
	uint16_t *C2 =
		(uint16_t *)malloc(sizeof(uint16_t) * SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR);
	uint8_t seed[64];
	uint8_t *seedA = pk + SCLOUDPLUS_PK - 16;
	uint8_t *r1 = seed;
	uint8_t *r2 = seed + 32;
	scloudplus_eta_stream_t eta_stream;
	uint8_t aes_key_schedule[16 * 11];
	uint16_t x[8][8];
	uint16_t y[8][8];
	uint16_t z[8][8];
	uint16_t C_tile[8][8];

	if (S_row == NULL || mu0 == NULL || C2 == NULL) {
		free(S_row);
		free(mu0);
		free(C2);
		return -1;
	}
	scloudplus_F(seed, 64, r, 32);
	scloudplus_eta_stream_init(&eta_stream, r2, SCLOUDPLUS_ETA2);
	scloudplus_msgencode(m, mu0);
	memset(ctx, 0, SCLOUDPLUS_CTX);
	memset(C2, 0, (size_t)SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR * sizeof(uint16_t));
	AES128_load_schedule(seedA, aes_key_schedule);
	for (int row = 0; row < SCLOUDPLUS_MBAR; row++) {
		if (scloudplus_samplephi_row(r1, row, S_row) != 0) {
			AES128_free_schedule(aes_key_schedule);
			free(S_row);
			free(mu0);
			free(C2);
			return -1;
		}
		for (int q = 0; q < SCLOUDPLUS_N; q += 8) {
			memset(C_tile, 0, sizeof C_tile);
			for (int c = 0; c < 8; c++) {
				if (q + c < SCLOUDPLUS_N) {
					C_tile[0][c] = scloudplus_eta_stream_next(&eta_stream);
				}
			}
			for (int i = 0; i < SCLOUDPLUS_M; i += 8) {
				memset(x, 0, sizeof x);
				for (int c = 0; c < 8; c++) {
					if (i + c < SCLOUDPLUS_M) {
						x[0][c] = S_row[i + c];
					}
				}
				scloudplus_gen_a_tile(seedA, i, q, aes_key_schedule, y);
				if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x,
									  (const uint16_t (*)[8])y, 0) != OP_SUCCESS) {
					AES128_free_schedule(aes_key_schedule);
					free(S_row);
					free(mu0);
					free(C2);
					return -1;
				}
				for (int c = 0; c < 8; c++) {
					C_tile[0][c] = (uint16_t)(C_tile[0][c] + z[0][c]);
				}
			}
			for (int c = 0; c < 8; c++) {
				if (q + c < SCLOUDPLUS_N) {
					scloudplus_c1_put_compressed(ctx,
						(size_t)row * SCLOUDPLUS_N + q + c,
						C_tile[0][c]);
				}
			}
		}
	}
	for (int row = 0; row < SCLOUDPLUS_MBAR; row++) {
		if (scloudplus_samplephi_row(r1, row, S_row) != 0) {
			AES128_free_schedule(aes_key_schedule);
			free(S_row);
			free(mu0);
			free(C2);
			return -1;
		}
		for (int c = 0; c < SCLOUDPLUS_NBAR; c++) {
			C2[row * SCLOUDPLUS_NBAR + c] =
				scloudplus_eta_stream_next(&eta_stream);
		}
		for (int i = 0; i < SCLOUDPLUS_M; i += 8) {
			memset(x, 0, sizeof x);
			for (int c = 0; c < 8; c++) {
				if (i + c < SCLOUDPLUS_M) {
					x[0][c] = S_row[i + c];
				}
			}
			for (int jbar = 0; jbar < SCLOUDPLUS_NBAR; jbar += 8) {
				for (int rr = 0; rr < 8; rr++) {
					for (int cc = 0; cc < 8; cc++) {
						y[rr][cc] = (i + rr < SCLOUDPLUS_M &&
									 jbar + cc < SCLOUDPLUS_NBAR) ?
							scloudplus_pk_get(pk, (size_t)i + rr,
											  (size_t)jbar + cc) : 0;
					}
				}
				if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x,
									  (const uint16_t (*)[8])y, 0) != OP_SUCCESS) {
					AES128_free_schedule(aes_key_schedule);
					free(S_row);
					free(mu0);
					free(C2);
					return -1;
				}
				for (int c = 0; c < 8; c++) {
					if (jbar + c < SCLOUDPLUS_NBAR) {
						C2[row * SCLOUDPLUS_NBAR + jbar + c] =
							(uint16_t)(C2[row * SCLOUDPLUS_NBAR + jbar + c] + z[0][c]);
					}
				}
			}
		}
	}
	AES128_free_schedule(aes_key_schedule);
	scloudplus_add(C2, mu0, SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR, C2);
	scloudplus_compressc2(C2, C2);
	scloudplus_packc2(C2, ctx + SCLOUDPLUS_C1);
	free(S_row);
	free(mu0);
	free(C2);
	return 0;
}

int scloudplus_pkedec(uint8_t *sk, uint8_t *ctx, uint8_t *m)
{
	uint16_t *S =
		(uint16_t *)malloc(sizeof(uint16_t) * SCLOUDPLUS_N * SCLOUDPLUS_NBAR);
	uint16_t *C2 =
		(uint16_t *)malloc(sizeof(uint16_t) * SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR);
	uint16_t *D =
		(uint16_t *)malloc(sizeof(uint16_t) * SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR);
	uint16_t x[8][8];
	uint16_t y[8][8];
	uint16_t z[8][8];

	if (S == NULL || C2 == NULL || D == NULL) {
		free(S);
		free(C2);
		free(D);
		return -1;
	}
	scloudplus_unpacksk(sk, S);
	scloudplus_unpackc2(ctx + SCLOUDPLUS_C1, C2);
	scloudplus_decompressc2(C2, C2);
	memset(D, 0, (size_t)SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR * sizeof(uint16_t));
	for (int q = 0; q < SCLOUDPLUS_N; q += 8) {
		for (int rowblk = 0; rowblk < SCLOUDPLUS_MBAR; rowblk += 8) {
			for (int r = 0; r < 8; r++) {
				for (int c = 0; c < 8; c++) {
					x[r][c] = (rowblk + r < SCLOUDPLUS_MBAR &&
							   q + c < SCLOUDPLUS_N) ?
						scloudplus_c1_get_decompressed(ctx,
							(size_t)(rowblk + r) * SCLOUDPLUS_N + q + c) : 0;
				}
			}
			for (int colblk = 0; colblk < SCLOUDPLUS_NBAR; colblk += 8) {
				for (int r = 0; r < 8; r++) {
					for (int c = 0; c < 8; c++) {
						y[r][c] = (q + r < SCLOUDPLUS_N &&
								   colblk + c < SCLOUDPLUS_NBAR) ?
							S[(size_t)(colblk + c) * SCLOUDPLUS_N + q + r] : 0;
					}
				}
				if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x,
									  (const uint16_t (*)[8])y, 0) != OP_SUCCESS) {
					free(S);
					free(C2);
					free(D);
					return -1;
				}
				for (int r = 0; r < 8; r++) {
					for (int c = 0; c < 8; c++) {
						if (rowblk + r < SCLOUDPLUS_MBAR &&
							colblk + c < SCLOUDPLUS_NBAR) {
							D[(rowblk + r) * SCLOUDPLUS_NBAR + colblk + c] =
								(uint16_t)(D[(rowblk + r) * SCLOUDPLUS_NBAR + colblk + c] +
										   z[r][c]);
						}
					}
				}
			}
		}
	}
	scloudplus_sub(C2, D, SCLOUDPLUS_MBAR * SCLOUDPLUS_NBAR, D);
	scloudplus_msgdecode(D, m);
	free(S);
	free(C2);
	free(D);
	return 0;
}
