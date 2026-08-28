// Original copyright: LAC v3

// Modified by:
// Copyright (c) 2025 Ziyao Liu, Ying Liu
// Affiliation: Key Laboratory of Cyberspace Security Defense, Institute of Information Engineering, CAS
//              School of Cyber Security, University of Chinese Academy of Sciences


#include "api.h"
#include "rand.h"
#include "ecc.h"
#include "bin-lwe.h"
#include "ntt.h"
#include "polarlac_param.h"
#include "fips202.h"
#include "operator_interface.h"
#include "memops.h"
#include "malloc.h"

#define RATIO 126 // Q/2

#ifdef ENABLE_POLAR_CYCLE_RECORD
#define MAX_RECORDS 100000 // number of tests
uint64_t polar_enc_cycle[MAX_RECORDS];
uint32_t polar_enc_index = 0;
uint64_t polar_dec_cycle[MAX_RECORDS];
uint32_t polar_dec_index = 0;

#ifndef POLARLAC_CYCLE_COUNTER
#define POLARLAC_CYCLE_COUNTER() ((uint64_t)0)
#endif
#endif


static int32_t encode_to_e2(unsigned char *e2, const unsigned char *m, uint64_t mlen, int32_t *c2_len)
{
	const polarlac_params_t *p = polarlac_current_params();
	int32_t i;
	int8_t message;
	int32_t vec_bound;
	unsigned char *p_code;

	/* polar encoding */
    uint8_t *u = my_malloc(p->code_len * 8); // source sequence(each element stores 1 bits)
	uint8_t *code = my_malloc(p->code_len); // codeword sequence(each element stores 8 bits)
	if (u == NULL || code == NULL) {
		my_free(u);
		my_free(code);
		return -1;
	}
    memset(u, 0, p->code_len * 8);
	memset(code, 0, p->code_len);

	// fill the message m into the source sequence
	int32_t info_cnt = 0;
    for (int32_t i = 0; i < p->code_len; i++)
    {
        for(int32_t j = 0; j < 8; j++)
        {
            if(ecc_info_nodes[8*i+j] == 1)
            {
				u[8*i+j] = (uint8_t)(m[info_cnt/8] >> (info_cnt%8)) & 0x01;
                info_cnt++;
            }
        }
    }

#ifdef ENABLE_POLAR_CYCLE_RECORD
	uint64_t polar_enc_start = POLARLAC_CYCLE_COUNTER();
#endif
	polar_ecc_enc(u);
#ifdef ENABLE_POLAR_CYCLE_RECORD
	uint64_t polar_enc_end = POLARLAC_CYCLE_COUNTER();
	if (polar_enc_index < MAX_RECORDS) {
        polar_enc_cycle[polar_enc_index] = polar_enc_end - polar_enc_start;
		polar_enc_index++;
    }
#endif

	// each element stores 1 binary value -> each element stores 8 binary values
	for (int32_t i = 0; i < p->code_len; i++)
	{
    	for (int32_t j = 0; j < 8; j++) {
        	code[i] |= (u[i * 8 + j] << j);
    	}
	}

	p_code = (unsigned char *)code;


	//compute the length of c2
	*c2_len=p->code_len*8; // the code length of the ecc
	vec_bound=*c2_len;
	//compute code*q/2+e2
	for(i=0;i<vec_bound;i++)
	{
		//RATIO=q/2. add code*q/2 to e2
		message=RATIO*((p_code[i/8]>>(i%8))&1);
		e2[i]=e2[i]+message;
	}

	my_free(u);
	my_free(code);
	return 0;
}
/* END FOR POLAR */


//key generation
int32_t crypto_encrypt_keypair( unsigned char *pk, unsigned char *sk)
{
	//check parameter
	if(pk==NULL || sk==NULL)
	{
		return -1;
	}
	kg(pk,sk);
	
	return 0;
}

//key generation with seed
int32_t kg_seed(unsigned char *pk, unsigned char *sk, unsigned char *seed)
{
	const polarlac_params_t *p = polarlac_current_params();
	unsigned char *seeds = my_malloc(2 * p->seed_len);
	unsigned char *a = my_malloc(p->dim_n);
	unsigned char *e = my_malloc(p->dim_n);
	unsigned char h_pk[HASHLEN];
	//check pointer
	if(pk==NULL || sk==NULL)
	{
		my_free(seeds);
		my_free(a);
		my_free(e);
		return -1;
	}
	if (seeds == NULL || a == NULL || e == NULL) {
		my_free(seeds);
		my_free(a);
		my_free(e);
		return -1;
	}

	//generate two seeds for a,sk,e
	// sha3_512(seeds, seed, SEED_LEN);
	OP_hash(7, 0, 64, seed, p->seed_len, 0, seeds);
	memcpy(pk,seeds,p->seed_len);

	//generate a
	gen_a(a,seeds);//print_bytes(sk,CRYPTO_SECRETKEYBYTES);

	keccak_state state;
    // shake256_absorb_once(&state, seeds + SEED_LEN, SEED_LEN);
	OP_hash_init(3, &state.s, 200+8);
    OP_hash_absorb(3, &state.s, 200+8, seed, p->seed_len);
	//generate  sk,e
	gen_e(sk,&state);
	gen_e(e,&state);
	//compute pk=a*sk+e
	poly_aff(a,sk,e,pk + p->seed_len, p->dim_n);
	//copy pk=as+e to the second part of sk, now sk=s|pk
	memcpy(sk + p->sk_part_len, pk, p->pk_len);
	// sha3_256(h_pk,pk,PK_LEN);
	OP_hash(5, 0, 32, pk, p->pk_len, 0, h_pk);
	memcpy(sk + p->sk_part_len + p->pk_len, h_pk, HASHLEN);

	my_free(seeds);
	my_free(a);
	my_free(e);
	return 0;
}

//key generation
int32_t kg(unsigned char *pk, unsigned char *sk)
{
	const polarlac_params_t *p = polarlac_current_params();
	unsigned char *seed = my_malloc(p->seed_len);

	//generate seed
	// random_bytes(seed,SEED_LEN);
	OP_trng(seed, p->seed_len);
	//key generation with seed
	kg_seed(pk,sk,seed);

	my_free(seed);
	return 0;
}

//************************************ encryption ************************************//
int32_t crypto_encrypt( unsigned char *c, uint64_t *clen, const unsigned char *m, uint64_t mlen, const unsigned char *pk, unsigned char *seeds)
{
	const polarlac_params_t *p = polarlac_current_params();
	//check parameter
	if(c==NULL || m==NULL || pk==NULL)
	{
		return -1;
	}
	if(mlen > p->message_len)
	{
		return -1;
	}

	//call pke encryption function
	original_pke_enc(pk,m, mlen,c,clen, seeds);

	return 0;
}

// encryption
int32_t original_pke_enc(const unsigned char *pk, const unsigned char *m, uint64_t mlen, unsigned char *c, uint64_t *clen, unsigned char *seeds)
{
	original_pke_enc_seed(pk,m,mlen,c,clen,seeds);	

	return 0;
}

// encryption with seed
int32_t original_pke_enc_seed(const unsigned char *pk, const unsigned char *m, uint64_t mlen, unsigned char *c, uint64_t *clen, unsigned char *seed)
{
	const polarlac_params_t *p = polarlac_current_params();
	unsigned char *r = NULL;
	unsigned char *e1 = NULL;
	unsigned char *e2 = NULL;
	unsigned char *c2 = NULL;
	unsigned char *a = NULL;
	unsigned char *c1 = NULL;

	int32_t c2_len;

	//check parameter
	if(pk==NULL || m==NULL || c==NULL )
	{
		return -1;
	}
	if(mlen > p->message_len)
	{
		return -1;
	}
	r = my_malloc(p->dim_n);
	e1 = my_malloc(p->dim_n);
	e2 = my_malloc(p->dim_n);
	c2 = my_malloc(p->c2_vec_num);
	a = my_malloc(p->dim_n);
	c1 = my_malloc(p->dim_n);
	if (r == NULL || e1 == NULL || e2 == NULL || c2 == NULL || a == NULL || c1 == NULL) {
		my_free(r);
		my_free(e1);
		my_free(e2);
		my_free(c2);
		my_free(a);
		my_free(c1);
		return -1;
	}

	//generate  a from seed in the first part of pk
	gen_a(a,pk);
	keccak_state state;
    // shake256_absorb_once(&state, seed, SEED_LEN);
	OP_hash_init(3, &state.s, 200+8);
    OP_hash_absorb(3, &state.s, 200+8, seed, p->seed_len);
	//generate random vector r
	gen_e(r,&state);
	//generate error vector e1
	gen_e(e1,&state);
	//generate error vector e2
	gen_e(e2,&state);

	/* FOR POLAR */
	//encode message to e2
	if (encode_to_e2(e2,m,mlen,&c2_len) != 0) {
		my_free(r);
		my_free(e1);
		my_free(e2);
		my_free(c2);
		my_free(a);
		my_free(c1);
		return -1;
	}
	/* END FOR POLAR */

	if (polarlac_get_level() == POLARLAC_LEVEL_LIGHT) {
	//generate c1: c1=a*r+e1
	poly_aff(a,r,e1,c1,p->dim_n);
	//compress c1
	poly_compress_c1_1bit(c1,c,p->dim_n);

	//generate c2: c2=b*r+e2+m*[q/2]
	poly_aff(pk + p->seed_len,r,e2,c2,c2_len);
	//compress c2
	poly_compress_c2_4bit(c2,c + p->dim_n*7/8,c2_len);
	*clen = p->dim_n*7/8 + c2_len/2;
	} else if (polarlac_get_level() == POLARLAC_LEVEL_128) {
	//generate c1: c1=a*r+e1
	poly_aff(a,r,e1,c1,p->dim_n);
	//compress c1
	poly_compress_c1_1bit(c1,c,p->dim_n);

	//generate c2: c2=b*r+e2+m*[q/2]
	poly_aff(pk + p->seed_len,r,e2,c2,c2_len);
	//compress c2
	poly_compress_c2_5bit(c2,c + p->dim_n*7/8,c2_len);
	*clen = p->dim_n*7/8 + c2_len*3/8;
	} else {
	//generate c1: c1=a*r+e1
	poly_aff(a,r,e1,c,p->dim_n);

	//generate c2: c2=b*r+e2+m*[q/2]
	poly_aff(pk + p->seed_len,r,e2,c2,c2_len);
	//compress c2
	poly_compress_c2_4bit(c2,c + p->dim_n,c2_len);
	*clen = p->dim_n + c2_len/2;
	}

	my_free(r);
	my_free(e1);
	my_free(e2);
	my_free(c2);
	my_free(a);
	my_free(c1);
	return 0;
}


//************************************ decryption ************************************//
int32_t crypto_encrypt_open(unsigned char *m, uint64_t *mlen,const unsigned char *c, uint64_t clen,const unsigned char *sk)
{
	//check parameter
	if(sk==NULL || m==NULL || c==NULL || mlen==NULL)
	{
		return -1;
	}
	
	//call pke decryption function
	original_pke_dec(sk,c,clen,m,mlen);

	return 0;
}

// decrypt
int32_t original_pke_dec(const unsigned char *sk, const unsigned char *c,uint64_t clen, unsigned char *m, uint64_t *mlen)
{
	//check parameter
	if(sk==NULL || m==NULL || c==NULL)
	{
		return -1;
	}

	const polarlac_params_t *p = polarlac_current_params();
	unsigned char *out = my_malloc(p->dim_n);
	unsigned char *c2 = my_malloc(p->c2_vec_num);
	unsigned char *c1 = NULL;
	unsigned char *m_buf = my_malloc(p->message_len);
	int64_t *llr = my_malloc(sizeof(int64_t) * p->code_len * 8); // log-likelihood ratio of the received signal

	int32_t temp;
	int32_t half = 126; // Q/2
	int32_t half_2 = 63; // half/2
	uint8_t *m_cap = my_malloc(p->message_len * 8); // estimated message(each element stores 1 bit)
	int32_t c2_len;

	c1 = my_malloc(p->dim_n);
	if (out == NULL || c2 == NULL || llr == NULL || c1 == NULL || m_buf == NULL || m_cap == NULL) {
		my_free(out);
		my_free(c2);
		my_free(c1);
		my_free(llr);
		my_free(m_buf);
		my_free(m_cap);
		return -1;
	}

	if (polarlac_get_level() == POLARLAC_LEVEL_LIGHT) {
	c2_len=(clen - p->dim_n*7/8) * 2;

	//c1 decompress
	poly_decompress_c1_1bit(c, c1, p->dim_n);
	//c2 decompress
	poly_decompress_c2_4bit(c + p->dim_n*7/8, c2, c2_len);
	//c1*sk
	poly_mul(c1, sk, out, c2_len);
	} else if (polarlac_get_level() == POLARLAC_LEVEL_128) {
	c2_len=(clen - p->dim_n*7/8) / 3 * 8;

	//c1 decompress
	poly_decompress_c1_1bit(c, c1, p->dim_n);
	//c2 decompress
	poly_decompress_c2_5bit(c + p->dim_n*7/8, c2, c2_len);
	//c1*sk
	poly_mul(c1, sk, out, c2_len);
	} else {
	c2_len=(clen - p->dim_n) * 2;

	//c2 decompress
	poly_decompress_c2_4bit(c + p->dim_n, c2, c2_len);
	//c1*sk
	poly_mul(c, sk, out, c2_len);
	}

	/* FOR POLAR */
	*mlen = p->data_len;
	// compute llr
	for(int32_t i = 0; i < c2_len; i++)
	{
		// compute m*q/2+e in [0,250]
		temp = (c2[i]-out[i]+Q)%Q;
		// m*q/2+e in [-63,187]=[-63,125]+[126,187]
		temp = temp - half_2;
		// m*q/2+e in [-125,125]
		if(temp >= half)
		{
			temp = temp - Q; // [126,187]——>[-125,-64]
		}
		llr[i] = llr_table[temp+(Q-1)/2]; // 0 is modulated to -q/4, and 1 is modulated to q/4
	}

#ifdef ENABLE_POLAR_CYCLE_RECORD
	uint64_t polar_dec_start = POLARLAC_CYCLE_COUNTER();
#endif
	//polar decode to recover m
	polar_ecc_dec(m_cap, llr);
#ifdef ENABLE_POLAR_CYCLE_RECORD
	uint64_t polar_dec_end = POLARLAC_CYCLE_COUNTER();
	if (polar_dec_index < MAX_RECORDS) {
        polar_dec_cycle[polar_dec_index] = polar_dec_end - polar_dec_start;
		polar_dec_index++;
    }
#endif

	// each element stores 1 binary value -> each element stores 8 binary values
	memset(m_buf, 0, p->message_len);
	for (int32_t i = 0; i < p->message_len; i++)
    {
        for(int32_t j = 0; j < 8; j++)
        {
            m_buf[i] |= (m_cap[8*i+j] << j);
        }
    }

	memcpy(m, m_buf, *mlen);
	/* END FOR POLAR */

	my_free(out);
	my_free(c2);
	my_free(c1);
	my_free(llr);
	my_free(m_buf);
	my_free(m_cap);
	return 0;
}

