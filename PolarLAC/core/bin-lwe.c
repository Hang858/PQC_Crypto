// Original copyright: LAC v3

// Modified by:
// Copyright (c) 2025 Ying Liu, Yu Zhang
// Affiliation: Key Laboratory of Cyberspace Security Defense, Institute of Information Engineering, CAS
//              School of Cyber Security, University of Chinese Academy of Sciences

// Performance and Optimization Features:
// - Compact lifted modulus NTT-based multiplication, implemented in poly_mul and poly_aff functions [Ying Liu]
// - Sparse ternary distribution sampling, implemented in gen_e function [Yu Zhang]


#include "bin-lwe.h"
#include "rand.h"
#include "polarlac_param.h"
#include "ntt.h"
#include "ntt-1024.h"
#include "fips202.h"
#include "operator_interface.h"
#include "memops.h"
#include "malloc.h"

static uint32_t rej_uniform(uint8_t *r, uint32_t len, const uint8_t *buf, uint32_t buflen)
{
  uint32_t ctr, pos;

  ctr = pos = 0;
  while(ctr < len && pos<= buflen) 
  {
    if(buf[pos] < Q)
      r[ctr++] = buf[pos];
	pos++;
  }

  return ctr;
}

static void op_shake256_squeezeblocks(uint8_t *out, size_t nblocks, keccak_state *state)
{
	OP_hash_squeeze(3, state->s, 200+8, out, nblocks * SHAKE256_RATE);
}

// generate the public parameter a from seed
int32_t gen_a(unsigned char *a, const unsigned char *seed)
{
	const polarlac_params_t *p = polarlac_current_params();
	uint32_t ctr, buflen;
	keccak_state state;
	// shake256_absorb_once(&state, seed, SEED_LEN);
	OP_hash_init(3, &state.s, 200+8);
    OP_hash_absorb(3, &state.s, 200+8, seed, p->seed_len);

	buflen = (p->dim_n == 1024) ? 1088 : 544;
	uint8_t *buf = my_malloc(buflen);
	if (buf == NULL) {
		return -1;
	}

	op_shake256_squeezeblocks(buf, buflen / SHAKE256_RATE, &state);
	if (p->dim_n == 1024) {
		OP_hash_squeeze(3, &state.s, 200+8, buf + 408, 44);
	}

	ctr = rej_uniform(a, p->dim_n, buf, buflen);
	while(ctr < p->dim_n)
	{
		// shake256_squeezeblocks(buf, 1, &state);
		op_shake256_squeezeblocks(buf, 1, &state);
		buflen = SHAKE256_RATE;
		ctr += rej_uniform(a + ctr, p->dim_n - ctr, buf, buflen);
	}

	my_free(buf);
	return 0;
}

// The generation of errors with constant time.
// -1 will be stord as 255 with the unsigned char form
int32_t gen_e(unsigned char *e, keccak_state *state)
{
	const polarlac_params_t *p = polarlac_current_params();
	uint8_t flag;
	uint8_t *r = my_malloc(SHAKE256_RATE * 2);
	uint8_t *tmp = my_malloc(p->dim_n * 2);
	uint16_t i, j, t;
	uint16_t mask, norm;
	uint16_t e_1,e_2;
	if (r == NULL || tmp == NULL) {
		my_free(r);
		my_free(tmp);
		return -1;
	}
	memset(e, 0, p->dim_n);
	if (polarlac_get_level() == POLARLAC_LEVEL_LIGHT) {
		op_shake256_squeezeblocks(r, 1, state);
		t = 0;
		for (i = 0; i < 8; i++) {
			for (j = 0; j < p->dim_n_8 * 2; j++) {
				tmp[i * (p->dim_n_8 * 2) + j] = (r[t + j] & 1);
				r[t + j] = (r[t + j] >> 1);
			}
		}
		for (i = 0; i < p->dim_n; i++) {
			tmp[i] = tmp[i] - tmp[i + p->dim_n];
		}

		flag = 1;
		while (flag) {
			OP_hash_squeeze(3, state->s, 200+8, r, 64);
			t = 0;
			for (i = 0; i < 8; i++) {
				for (j = 0; j < p->dim_n_8; j++) {
					tmp[p->dim_n + i * p->dim_n_8 + j] = (r[t + j] & 1);
					r[t + j] = (r[t + j] >> 1);
				}
			}

			flag = 0;
			norm = 0;
			for (i = 0; i < p->dim_n; i++) {
				e[i] = tmp[i] * tmp[i + p->dim_n];
				norm += (e[i] & e[i] & 1);
			}
			mask = (norm < 110);
			flag = (1 & (-mask)) | (flag & (~(-mask)));
			mask = (norm > 146);
			flag = (1 & (-mask)) | (flag & (~(-mask)));
		}
	} else if (polarlac_get_level() == POLARLAC_LEVEL_128) {
		OP_hash_squeeze(3, state->s, 200+8, r, 64);
		t = 0;
		for (i = 0; i < 8; i++) {
			for (j = 0; j < p->dim_n_8; j++) {
				tmp[i * p->dim_n_8 + j] = (r[t + j] & 1);
				r[t + j] = (r[t + j] >> 1);
			}
		}

		flag = 1;
		while (flag) {
			OP_hash_squeeze(3, state->s, 200+8, r, 64);
			t = 0;
			for (i = 0; i < 8; i++) {
				for (j = 0; j < p->dim_n_8; j++) {
					tmp[i * p->dim_n_8 + j + p->dim_n] = (r[t + j] & 1);
					r[t + j] = (r[t + j] >> 1);
				}
			}
			flag = 0;
			norm = 0;
			for (i = 0; i < p->dim_n; i++) {
				e[i] = tmp[i] - tmp[i + p->dim_n];
				norm += (e[i] & e[i] & 1);
			}
			mask = (norm < 248);
			flag = (1 & (-mask)) | (flag & (~(-mask)));
			mask = (norm > 264);
			flag = (1 & (-mask)) | (flag & (~(-mask)));
		}
	} else if (polarlac_get_level() == POLARLAC_LEVEL_256) {
		op_shake256_squeezeblocks(r, 2, state);
		t = 0;
		for (i = 0; i < 8; i++) {
			for (j = 0; j < p->dim_n_8 * 2; j++) {
				tmp[i * (p->dim_n_8 * 2) + j] = (r[t + j] & 1);
				r[t + j] = (r[t + j] >> 1);
			}
		}
		for (i = 0; i < p->dim_n; i++) {
			tmp[i] = tmp[i] - tmp[i + p->dim_n];
		}

		flag = 1;
		while (flag) {
			op_shake256_squeezeblocks(r, 1, state);
			t = 0;
			for (i = 0; i < 8; i++) {
				for (j = 0; j < p->dim_n_8; j++) {
					tmp[i * p->dim_n_8 + j + p->dim_n] = (r[t + j] & 1);
					r[t + j] = (r[t + j] >> 1);
				}
			}
			flag = 0;
			norm = 0;
			for (i = 0; i < p->dim_n; i++) {
				e[i] = tmp[i] * tmp[i + p->dim_n];
				norm += (e[i] & e[i] & 1);
			}
			mask = (norm < 242);
			flag = (1 & (-mask)) | (flag & (~(-mask)));
			mask = (norm > 270);
			flag = (1 & (-mask)) | (flag & (~(-mask)));
		}
	} else {
		my_free(r);
		my_free(tmp);
		return -1;
	}

	for (i = 0; i < p->dim_n; i++) {
		mask = (e[i] > q_half);
		e_1 = neg_one;
		e_2 = e[i];
		e[i] = (e_1 & (-mask)) | (e_2 & (~(-mask)));
	}
	my_free(r);
	my_free(tmp);
	return 0;
}

// b=as using compact lift multiplication with constant time.
int32_t poly_mul(const unsigned char *a, const unsigned char *s, unsigned char *b, uint32_t vec_num)
{
	const polarlac_params_t *p = polarlac_current_params();
	int32_t i;

	uint32_t *a2 = my_malloc(sizeof(uint32_t) * p->dim_n);
	uint32_t *s2 = my_malloc(sizeof(uint32_t) * p->dim_n);
	uint32_t *b2 = my_malloc(sizeof(uint32_t) * p->dim_n);
	uint32_t mask;
	uint32_t a_1, a_2, s_1, s_2, b_1, b_2;
	if (a2 == NULL || s2 == NULL || b2 == NULL) {
		my_free(a2);
		my_free(s2);
		my_free(b2);
		return -1;
	}

	// step 1: map to the lifted ring with NTTQ as the modulus
	for (i = 0; i < p->dim_n; i++)
	{
		mask = (a[i] >= q_half);
		a_1 = a[i] + Q_sub_q;
		a_2 = a[i] + p->ntt_q;
		a2[i] = (a_1 & (-mask)) | (a_2 & (~(-mask)));
	}

	for (i = 0; i < p->dim_n; i++)
	{
		mask = (s[i] >= q_half);
		s_1 = s[i] + Q_sub_q;
		s_2 = s[i] + p->ntt_q;
		s2[i] = (s_1 & (-mask)) | (s_2 & (~(-mask)));
	}

	// step 2: perform ntt-based multiplciation with NTTQ as the modulus
	if (polarlac_get_level() == POLARLAC_LEVEL_256) {
		poly_mul_ntt_1024(a2, s2, b2);
	} else {
		poly_mul_ntt(a2, s2, b2);
	}
	// step 3: map back to the original ring with Q as the modulus
	for (i = 0; i < vec_num; i++)
	{
		mask = (b2[i] < Q_half);
		b_1 = b2[i] + p->ntt_q;
		b_2 = b2[i];
		b2[i] = (b_1 & (-mask)) | (b_2 & (~(-mask)));
		b[i] = (b2[i] + neg_Q_mod_q) % Q;
	}


	my_free(a2);
	my_free(s2);
	my_free(b2);
	return 0;
}

// b=as+e using compact lift multiplication with constant time.
int32_t poly_aff(const unsigned char *a, const unsigned char *s, unsigned char *e, unsigned char *b, uint32_t vec_num)
{
	const polarlac_params_t *p = polarlac_current_params();
	int32_t i;

	uint32_t *a2 = my_malloc(sizeof(uint32_t) * p->dim_n);
	uint32_t *s2 = my_malloc(sizeof(uint32_t) * p->dim_n);
	uint32_t *b2 = my_malloc(sizeof(uint32_t) * p->dim_n);

	// test ntt
	uint32_t mask;
	uint32_t a_1, a_2, s_1, s_2, b_1, b_2;
	if (a2 == NULL || s2 == NULL || b2 == NULL) {
		my_free(a2);
		my_free(s2);
		my_free(b2);
		return -1;
	}
	for (i = 0; i < p->dim_n; i++)
	{
		mask = (a[i] >= q_half);
		a_1 = a[i] + Q_sub_q;
		a_2 = a[i] + p->ntt_q;
		a2[i] = (a_1 & (-mask)) | (a_2 & (~(-mask)));
	}

	for (i = 0; i < p->dim_n; i++)
	{
		mask = (s[i] >= q_half);
		s_1 = s[i] + Q_sub_q;
		s_2 = s[i] + p->ntt_q;
		s2[i] = (s_1 & (-mask)) | (s_2 & (~(-mask)));
	}

	if (polarlac_get_level() == POLARLAC_LEVEL_256) {
		poly_mul_ntt_1024(a2, s2, b2);
	} else {
		poly_mul_ntt(a2, s2, b2);
	}

	for (i = 0; i < vec_num; i++)
	{
		mask = (b2[i] < Q_half);
		b_1 = b2[i] + p->ntt_q;
		b_2 = b2[i];
		b2[i] = (b_1 & (-mask)) | (b_2 & (~(-mask)));
		b2[i] = (b2[i] + neg_Q_mod_q) % Q;
		b[i] = (b2[i] + e[i] + Q) % Q;
	}
	my_free(a2);
	my_free(s2);
	my_free(b2);
	return 0;
}

// Compression: c1 discards 1-bit
int32_t poly_compress_c1_1bit(const unsigned char *in,  unsigned char *out, const uint32_t vec_num)
{
	int32_t i,j,loop;
	loop = vec_num / 8;
    for (i = 0; i < loop; i++) {
        unsigned char buf[8];
        for (j = 0; j < 8; j++) {
            buf[j] = (in[i * 8 + j]) >> 1;
        }

        out[i * 7 + 0] = (buf[1] << 7) | buf[0];
        out[i * 7 + 1] = (buf[2] << 6) | (buf[1] >> 1);
        out[i * 7 + 2] = (buf[3] << 5) | (buf[2] >> 2);
        out[i * 7 + 3] = (buf[4] << 4) | (buf[3] >> 3);
        out[i * 7 + 4] = (buf[5] << 3) | (buf[4] >> 4);
        out[i * 7 + 5] = (buf[6] << 2) | (buf[5] >> 5);
        out[i * 7 + 6] = (buf[7] << 1) | (buf[6] >> 6);
    }
	
	return 0;
}
int32_t poly_decompress_c1_1bit(const unsigned char *in,  unsigned char *out, const uint32_t vec_num)
{
	int32_t i,loop;
	loop = vec_num / 8;
    for (i = 0; i < loop; i++) {
        out[i * 8 + 0] = (in[i * 7 + 0] << 1) + 0b00000001;
		out[i * 8 + 1] = ((in[i * 7 + 1] << 2) | ((in[i * 7 + 0] & 0b10000000) >> 6)) + 0b00000001;
		out[i * 8 + 2] = ((in[i * 7 + 2] << 3) | ((in[i * 7 + 1] & 0b11000000) >> 5)) + 0b00000001;
		out[i * 8 + 3] = ((in[i * 7 + 3] << 4) | ((in[i * 7 + 2] & 0b11100000) >> 4)) + 0b00000001;
		out[i * 8 + 4] = ((in[i * 7 + 4] << 5) | ((in[i * 7 + 3] & 0b11110000) >> 3)) + 0b00000001;
		out[i * 8 + 5] = ((in[i * 7 + 5] << 6) | ((in[i * 7 + 4] & 0b11111000) >> 2)) + 0b00000001;
		out[i * 8 + 6] = ((in[i * 7 + 6] << 7) | ((in[i * 7 + 5] & 0b11111100) >> 1)) + 0b00000001;
		out[i * 8 + 7] = (in[i * 7 + 6] & 0b11111110) + 0b00000001;
    }
	
	return 0;
}

// Compression: c2 discards 4-bit
int32_t poly_compress_c2_4bit(const unsigned char *in,  unsigned char *out, const uint32_t vec_num)
{
	int32_t i,loop;
	loop=vec_num/2;
	for(i=0;i<loop;i++)
	{
		out[i]=(in[i*2])>>4;
		out[i]=out[i]^(in[i*2+1]&0xf0);
	}
	
	return 0;
}
int32_t poly_decompress_c2_4bit(const unsigned char *in,  unsigned char *out, const uint32_t vec_num)
{
	int32_t i,loop;
	loop=vec_num/2;
	for(i=0;i<loop;i++)
	{
		out[i*2]=(in[i]<<4)^0x08;
		out[i*2+1]=(in[i]&0xf0)^0x08;
	}
	
	return 0;
}

// Compression: c2 discards 5-bit
int32_t poly_compress_c2_5bit(const unsigned char *in,  unsigned char *out, const uint32_t vec_num)
{
	int32_t i,j,loop;
	loop = vec_num / 8;
    for (i = 0; i < loop; i++) {
        unsigned char buf[8];
        for (j = 0; j < 8; j++) {
            buf[j] = in[i * 8 + j] >> 5;
        }

        out[i * 3 + 0] = (buf[2] << 6) | (buf[1] << 3) | buf[0];
        out[i * 3 + 1] = (buf[5] << 7) | (buf[4] << 4) | (buf[3] << 1) | (buf[2] >> 2);
        out[i * 3 + 2] = (buf[7] << 5) | (buf[6] << 2) | (buf[5] >> 1);
    }
	
	return 0;
}
int32_t poly_decompress_c2_5bit(const unsigned char *in,  unsigned char *out, const uint32_t vec_num)
{
	int32_t i,loop;
	loop = vec_num / 8;
    for (i = 0; i < loop; i++) {
        out[i * 8 + 0] = (in[i * 3 + 0] << 5) + 0b00010000;
		out[i * 8 + 1] = ((in[i * 3 + 0] & 0b00111000) << 2) + 0b00010000;
		out[i * 8 + 2] = ((in[i * 3 + 1] << 7) | (in[i * 3 + 0] & 0b11000000) >> 1) + 0b00010000;
		out[i * 8 + 3] = ((in[i * 3 + 1] & 0b00001110) << 4) + 0b00010000;
		out[i * 8 + 4] = ((in[i * 3 + 1] & 0b01110000) << 1) + 0b00010000;
		out[i * 8 + 5] = ((in[i * 3 + 2] << 6) | (in[i * 3 + 1] & 0b10000000) >> 2) + 0b00010000;
		out[i * 8 + 6] = ((in[i * 3 + 2] & 0b00011100) << 3) + 0b00010000;
		out[i * 8 + 7] = (in[i * 3 + 2] & 0b11100000) + 0b00010000;
    }
	
	return 0;
}
