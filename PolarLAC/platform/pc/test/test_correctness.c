#include "my_log.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "api.h"
#include "rand.h"
#include "bin-lwe.h"
#include "ntt.h"
#include "ntt-1024.h"
#include "fips202.h"


#define CTESTS 1
uint64_t loop=1;

static void print_uint64(uint64_t num)
{
	if(num>=10)
		print_uint64(num/10);
	LOG_D("%u",(uint32_t)(num%10));
}


int64_t error_bit_num(unsigned char *k1, unsigned char *k2, int32_t num)
{
	int32_t i;
	int64_t sum=0;
	unsigned char temp;
	for(i=0;i<num;i++)
	{
		temp=k1[i]^k2[i];
		if(temp>0)
		{
			sum+=(temp&0x1);
			sum+=((temp>>1)&0x1);
			sum+=((temp>>2)&0x1);
			sum+=((temp>>3)&0x1);
			sum+=((temp>>4)&0x1);
			sum+=((temp>>5)&0x1);
			sum+=((temp>>6)&0x1);
			sum+=((temp>>7)&0x1);
		}
	}
	
	return sum;
}
int32_t print_error_bit(unsigned char *k1, unsigned char *k2, int32_t num)
{
	int32_t i,sum=0;
	unsigned char temp;
	LOG_D("\nerror bit:\n");
	for(i=0;i<num;i++)
	{
		temp=k1[i]^k2[i];
		LOG_D("%d%d%d%d%d%d%d%d",temp&0x1,((temp>>1)&0x1),((temp>>2)&0x1),((temp>>3)&0x1),((temp>>4)&0x1),((temp>>5)&0x1),((temp>>6)&0x1),((temp>>7)&0x1));
	}
	LOG_D("\n");
	
	return sum;
}

//print bytes
int32_t print_bytes(unsigned char *buf, int32_t len)
{
	int32_t i;
	for(i=0;i<len;i++)
	{
		LOG_D("%d ",buf[i]);
	}
	LOG_D("\n");
	
	return 0;
}


//test correctness of pke
int32_t test_pke_correctness()
{
	LOG_D("\n =====================================  test pke  ========================================== \n");

	unsigned char *pk = malloc(CRYPTO_PUBLICKEYBYTES);
	unsigned char *sk = malloc(CRYPTO_SECRETKEYBYTES);
	unsigned char *k1 = malloc(CRYPTO_BYTES);
	unsigned char *k2 = malloc(CRYPTO_BYTES);
	unsigned char *c = malloc(CRYPTO_CIPHERTEXTBYTES);
	int32_t i, j;
	int64_t  error_bit, sum = 0;
	int64_t  error_num = 0, sum_bits;
	uint64_t mlen=polarlac_current_params()->message_len,clen=CRYPTO_CIPHERTEXTBYTES;
	if (pk == NULL || sk == NULL || k1 == NULL || k2 == NULL || c == NULL) {
		free(pk);
		free(sk);
		free(k1);
		free(k2);
		free(c);
		return -1;
	}

	LOG_D("correctness test of pke:\n");
	for (j = 0; j < loop; j++)
	{
		crypto_encrypt_keypair(pk,sk);
		random_bytes(k1, CRYPTO_BYTES);
		for (i = 0; i < CTESTS; i++)
		{
			unsigned char *seed = malloc(polarlac_current_params()->seed_len);
			if (seed == NULL) {
				free(pk);
				free(sk);
				free(k1);
				free(k2);
				free(c);
				return -1;
			}
			//generate seed
			random_bytes(seed, polarlac_current_params()->seed_len);
			crypto_encrypt(c, &clen, k1, mlen, pk, seed);
			// LOG_D("c_byts=%lld\n",c_byts);
			crypto_encrypt_open(k2, &mlen, c, clen, sk);
			
			if (memcmp(k1, k2, mlen) != 0)
			{
				error_num++;
				error_bit = error_bit_num(k1, k2, mlen);
				sum += error_bit;
				if (error_bit > 0)
				{
					LOG_D("error bit num:");
					print_uint64(error_bit);
					LOG_D("\n");
					print_error_bit(k1, k2, mlen);
				}
			}
			free(seed);
		}
		LOG_D("test %d error block:", j + 1);
		print_uint64(error_num);
		LOG_D(" error bit:");
		print_uint64(sum);
		LOG_D("\n");

	}
	sum_bits = CTESTS * loop*CRYPTO_BYTES * 8;
	LOG_D("total error bit:");
	print_uint64(sum);
	LOG_D("/");
	print_uint64(sum_bits);
	LOG_D("\n\n");

	// LOG_D("pke test over\n\n");
	free(pk);
	free(sk);
	free(k1);
	free(k2);
	free(c);
	return error_num;
}


//test kem fo correctness
int32_t test_kem_fo_correctness()
{
	unsigned char *pk = malloc(CRYPTO_PUBLICKEYBYTES);
	unsigned char *sk = malloc(CRYPTO_SECRETKEYBYTES);
	unsigned char k1[CRYPTO_SESSION],k2[CRYPTO_SESSION];
	unsigned char *c = malloc(CRYPTO_CIPHERTEXTBYTES);
	int32_t i,j;
	int64_t  error_num=0;
	if (pk == NULL || sk == NULL || c == NULL) {
		free(pk);
		free(sk);
		free(c);
		return -1;
	}
	
	LOG_D("correctness test of kem_fo:\n");
	for(j=0;j<loop;j++)
	{
		crypto_kem_keypair(pk,sk);
		random_bytes(k1,CRYPTO_SESSION);
		for(i=0;i<CTESTS;i++)
		{
			crypto_kem_enc(c,k1,pk);
			crypto_kem_dec(k2,c,sk);
			
			if(memcmp(k1,k2,CRYPTO_SESSION)!=0)
			{
				error_num++;
			}
			
		}
		LOG_D("test %d error block:",j+1);
		print_uint64(error_num);
		LOG_D("\n");
	}
	LOG_D("\n");

	free(pk);
	free(sk);
	free(c);
	return error_num;
}


int32_t test_mul_correctness()
{
	//   test correctness of polymul
	const polarlac_params_t *p = polarlac_current_params();
	unsigned char *ctestn = malloc(p->dim_n);
	uint16_t *ctest = malloc(sizeof(uint16_t) * p->dim_n);
	unsigned char *uinta = malloc(p->dim_n);
	unsigned char *uints = malloc(p->dim_n);
	uint16_t *uinta2 = malloc(sizeof(uint16_t) * p->dim_n);
	uint16_t *uints2 = malloc(sizeof(uint16_t) * p->dim_n);
	unsigned char *seed = malloc(p->seed_len);
	unsigned char *seeds = malloc(3 * p->seed_len);
	if (ctestn == NULL || ctest == NULL || uinta == NULL || uints == NULL || uinta2 == NULL || uints2 == NULL || seed == NULL || seeds == NULL) {
		free(ctestn);
		free(ctest);
		free(uinta);
		free(uints);
		free(uinta2);
		free(uints2);
		free(seed);
		free(seeds);
		return -1;
	}
	random_bytes(seed, p->seed_len);

	gen_a(uinta,seeds);

	keccak_state state;
    // shake256_absorb_once(&state, seeds + SEED_LEN, SEED_LEN);
	OP_hash_init(3, &state.s, 200+8);
    OP_hash_absorb(3, &state.s, 200+8, seed, p->seed_len);
	//generate  sk,e
	gen_e(uints,&state);

	int32_t i;
	for(i=0;i < p->dim_n;i++)
	{
		uinta2[i]=uinta[i];
		uints2[i]=uints[i];
	}
	// LOG_D(" input a: \n"); //
	// for (i = 0; i < DIM_N; i++)
	// {

	// 	LOG_D("%d,", uinta[i]);
	// }
	// LOG_D("\n");
	// LOG_D(" input r: \n"); //
	// int32_t num=0;
	// for (i = 0; i < DIM_N; i++)
	// {
	// 	if(uints[i]==0)
	// 		num++;
	// 	LOG_D("%d,", uints[i]);
	// }
	// LOG_D("\n number of 0 : %d \n",num);


	

	LOG_D("\n =====================================  test poly_mul======================================= \n");
	poly_mul(uinta,uints,ctestn,p->dim_n);
	NormalMul_unsign_251(uinta2,uints2,ctest);

	LOG_D("\n");

	// for(i=0;i<DIM_N;i++)
	// 	ctest[i]=ctest[i]+e1[i];

	for(i=0;i < p->dim_n;i++)
	{
		if(ctestn[i]!=ctest[i])
			break;
	}
	if(i < p->dim_n)
	{
		LOG_D("Lifting Multiplication            : wrong , wrong index is %d, the wrong value is %d, correct valure is %d\n", i, ctest[i], ctestn[i]);
	}
	else
	{
		LOG_D("Lifting Multiplication            : right \n");
	}


	// LOG_D("\n ntt res: \n"); //
	// for (i = 0; i < DIM_N; i++)
	// {

	// 	LOG_D("%d,", ctestn[i]);
	// }
	// LOG_D("\n mul res: \n"); //
	// for (i = 0; i < DIM_N; i++)
	// {

	// 	LOG_D("%d,", ctest[i]);
	// }

	LOG_D("\n =====================================  test poly_aff======================================= \n");
	unsigned char *e = malloc(p->dim_n);
	if (e == NULL) {
		free(ctestn);
		free(ctest);
		free(uinta);
		free(uints);
		free(uinta2);
		free(uints2);
		return -1;
	}
	gen_e(e,&state);

	for(i=0;i < p->dim_n;i++)
	{
		if(e[i]>q_half)
			e[i] = 250;
		else
			e[i]=e[i];
	}
	poly_aff(uinta, uints, e, ctestn, p->dim_n);

	for(i=0;i < p->dim_n;i++)
		ctest[i]=(ctest[i]+e[i] )% Q;

	for(i=0;i < p->dim_n;i++)
	{
		if(ctestn[i]!=ctest[i])
			break;
	}
	if(i < p->dim_n)
	{
		LOG_D("Lifting Multiplication aff            : wrong , wrong index is %d, the wrong value is %d, correct valure is %d\n", i, ctest[i], ctestn[i]);
	}
	else
	{
		LOG_D("Lifting Multiplication aff            : right \n");
	}

	
	free(ctestn);
	free(ctest);
	free(uinta);
	free(uints);
	free(uinta2);
	free(uints2);
	free(e);
	return 0;
}
