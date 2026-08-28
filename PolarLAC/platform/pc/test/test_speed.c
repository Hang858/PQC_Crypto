#include "my_log.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "polarlac_time.h"
#include "api.h"
#include "rand.h"
#include "bin-lwe.h"
#include "fips202.h"

#define NTESTS 10000

static void print_avg_time_us(const char *label, clock_t start, clock_t finish)
{
	uint64_t elapsed = (uint64_t)(finish - start);
	uint64_t denom = (uint64_t)POLARLAC_CLOCKS_PER_SEC * (uint64_t)NTESTS;
	uint64_t avg_us_x1000 = (elapsed * 1000000000ULL + denom / 2) / denom;
	LOG_D("%s%llu.%03llu us\n",
		   label,
		   (uint64_t)(avg_us_x1000 / 1000ULL),
		   (uint64_t)(avg_us_x1000 % 1000ULL));
}

//test poly_mul
int32_t test_poly_mul_speed()
{
	const polarlac_params_t *p = polarlac_current_params();
	clock_t start,finish;
	unsigned char *a = malloc(p->dim_n);
	unsigned char *pk = malloc(p->dim_n);
	unsigned char *sk = malloc(p->dim_n);
	unsigned char *seed = malloc(p->seed_len);
	int32_t i;
	if (a == NULL || pk == NULL || sk == NULL || seed == NULL) {
		free(a);
		free(pk);
		free(sk);
		free(seed);
		return -1;
	}

	random_bytes(a, p->dim_n);
	random_bytes(seed, p->seed_len);
	keccak_state state;
    shake256_absorb_once(&state, seed, p->seed_len);
	gen_e(sk,&state);

    start=POLARLAC_CLOCK();
    for(i=0;i<NTESTS;i++)
	{
		poly_mul(a,sk,pk,p->dim_n);
	}
    finish=POLARLAC_CLOCK();
	print_avg_time_us("poly_mul time    :", start, finish);

	free(a);
	free(pk);
	free(sk);
	free(seed);
	return 0;
}

//test pke
int32_t test_pke_speed()
{
	clock_t start,finish;
	unsigned char *pk = malloc(CRYPTO_PUBLICKEYBYTES);
	unsigned char *sk = malloc(CRYPTO_SECRETKEYBYTES);
	unsigned char *k1 = malloc(CRYPTO_BYTES);
	unsigned char *k2 = malloc(CRYPTO_BYTES);
	unsigned char *c = malloc(CRYPTO_CIPHERTEXTBYTES);
	int32_t i;
	uint64_t mlen=CRYPTO_BYTES,clen=CRYPTO_CIPHERTEXTBYTES;
	if (pk == NULL || sk == NULL || k1 == NULL || k2 == NULL || c == NULL) {
		free(pk);
		free(sk);
		free(k1);
		free(k2);
		free(c);
		return -1;
	}
	
	start=POLARLAC_CLOCK();
    for(i=0;i<NTESTS;i++)
	{
		crypto_encrypt_keypair(pk,sk);
	}
    finish=POLARLAC_CLOCK();
	print_avg_time_us("key generate time:", start, finish);
	
	crypto_encrypt_keypair(pk, sk);
	random_bytes(k1, CRYPTO_BYTES);
	unsigned char *seed = malloc(polarlac_current_params()->seed_len);
	random_bytes(seed, polarlac_current_params()->seed_len);
    start=POLARLAC_CLOCK();
    for(i=0;i<NTESTS;i++)
	{
		crypto_encrypt(c,&clen,k1,mlen,pk, seed);
	}
    finish=POLARLAC_CLOCK();
	print_avg_time_us("encryption time  :", start, finish);
	
    start=POLARLAC_CLOCK();
    for(i=0;i<NTESTS;i++)
	{
		crypto_encrypt_open(k2,&mlen,c,clen,sk);
	}
    finish=POLARLAC_CLOCK();
	print_avg_time_us("decryption time  :", start, finish);
    LOG_D("\n");
	
	free(pk);
	free(sk);
	free(k1);
	free(k2);
	free(c);
	free(seed);
	return 0;
}

//test kem fo
int32_t test_kem_fo_speed()
{
	clock_t start,finish;
	unsigned char *pk = malloc(CRYPTO_PUBLICKEYBYTES);
	unsigned char *sk = malloc(CRYPTO_SECRETKEYBYTES);
	unsigned char *k1 = malloc(CRYPTO_SESSION);
	unsigned char *k2 = malloc(CRYPTO_SESSION);
	unsigned char *c = malloc(CRYPTO_CIPHERTEXTBYTES);
	int32_t i;
	if (pk == NULL || sk == NULL || k1 == NULL || k2 == NULL || c == NULL) {
		free(pk);
		free(sk);
		free(k1);
		free(k2);
		free(c);
		return -1;
	}
	
	start=POLARLAC_CLOCK();
    for(i=0;i<NTESTS;i++)
	{
		crypto_kem_keypair(pk,sk);
	}
    finish=POLARLAC_CLOCK();
	print_avg_time_us("key generate time:", start, finish);
	
	crypto_kem_keypair(pk,sk);
	random_bytes(k1, CRYPTO_SESSION);
    start=POLARLAC_CLOCK();
    for(i=0;i<NTESTS;i++)
	{
		crypto_kem_enc(c,k1,pk);
	}
    finish=POLARLAC_CLOCK();
	print_avg_time_us("kem_fo_enc time  :", start, finish);
	
	crypto_kem_keypair(pk,sk);
	crypto_kem_enc(c,k1,pk);
    start=POLARLAC_CLOCK();
    for(i=0;i<NTESTS;i++)
	{
		crypto_kem_dec(k2,c,sk);
	}
    finish=POLARLAC_CLOCK();
	print_avg_time_us("kem_fo_dec time  :", start, finish);
    LOG_D("\n");
	
	free(pk);
	free(sk);
	free(k1);
	free(k2);
	free(c);
	return 0;
}
