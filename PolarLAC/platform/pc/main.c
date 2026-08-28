
#include "polarlac.h"
#include "operator_interface.h"
#include "rand.h"
#include "api.h"
#include "polarlac_params.h"
#include "my_log.h"
#include "malloc.h"
#include <string.h>

#define CTESTS 1
uint32_t loop = 1;

static void print_memory_stats(const char *label)
{
	LOG_D("\n--- Memory Stats (%s) ---\n", label);
	LOG_D("  Total heap size:      %u bytes\n", HEAP_SIZE);
	LOG_D("  Current allocated:   %u bytes\n", get_total_allocated_memory());
	LOG_D("  Current free:        %u bytes\n", get_total_free_memory());
	LOG_D("  Max heap usage:      %u bytes\n", get_max_heap_usage());
	LOG_D("  Memory blocks:       %u\n", get_total_block_count());
	LOG_D("  Heap utilization:    %.1f%%\n", 
		  (float)get_total_allocated_memory() / HEAP_SIZE * 100);
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
			crypto_encrypt_open(k2, &mlen, c, clen, sk);
			
			if (memcmp(k1, k2, mlen) != 0)
			{
				error_num++;
				error_bit = error_bit_num(k1, k2, mlen);
				sum += error_bit;
				if (error_bit > 0)
				{
					LOG_D("error bit num:");
					LOG_D("\n");
				}
			}
			free(seed);
		}
		LOG_D("test %d error block:", j + 1);
		LOG_D(" error bit:");
		LOG_D("\n");

	}
	sum_bits = CTESTS * loop*CRYPTO_BYTES * 8;
	LOG_D("total error bit:");
	LOG_D("/");
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
		LOG_D("\n");
	}
	LOG_D("\n");

	free(pk);
	free(sk);
	free(c);
	return error_num;
}

int main(void)
{
	polarlac_level_t level = POLARLAC_LEVEL_256;
	if (polarlac_set_level(level) != 0) {
		return -1;
	}
	const polarlac_params_t *params = polarlac_current_params();
	if (params == NULL) {
		return -1;
	}
	
	reset_max_heap_usage();
	print_memory_stats("Before Test");
	
	int32_t ret = test_pke_correctness();
	if (ret != 0) {
		return ret;
	}
	ret = test_kem_fo_correctness();
	if (ret != 0) {
		return ret;
	}

	print_memory_stats("After Test");
	LOG_D("\n============================================\n");
	LOG_D("  Maximum Heap Usage:   %u bytes (%.1f%% of %uKB)\n", 
		  get_max_heap_usage(), 
		  (float)get_max_heap_usage() / HEAP_SIZE * 100,
		  HEAP_SIZE / 1024);
	LOG_D("============================================\n");

	return 0;
}
