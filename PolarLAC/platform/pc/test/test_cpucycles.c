#include "my_log.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "polarlac_time.h"
#include "api.h"
#include "rand.h"
#include "bin-lwe.h"
#include "ntt.h"
#include "ntt-1024.h"
#include "fips202.h"

static uint64_t cpucycles(void)
{
	return POLARLAC_CPUCYCLES();
}

#define NTESTS 100000

uint64_t sum, average;

static void print_uint64(uint64_t num)
{
	if (num >= 10)
		print_uint64(num / 10);
	LOG_D("%d", (int32_t)(num % 10));
}

int32_t print_cpucycles(uint64_t *t)  // print cpu cycles
{
	int32_t i;
	sum = 0;
	for (i = 0; i < NTESTS - 1; i++)
	{
		t[i] = t[i + 1] - t[i];
		sum += t[i];
	}
	average = sum / (NTESTS - 1);
	LOG_D(": ");
	print_uint64(average);
	LOG_D(" cpucycles\n");

	return 0;
}

static void print_fixed_avg(uint64_t total, uint64_t denom)
{
	uint64_t scaled = (total * 1000ULL + denom / 2ULL) / denom;
	LOG_D("%llu.%03llu",
		   (uint64_t)(scaled / 1000ULL),
		   (uint64_t)(scaled % 1000ULL));
}

// test pke
int32_t test_pke_cpucycles()
{
	LOG_D("\n=============================test_pke_cpucycles==================================\n");
	uint64_t *t = malloc(sizeof(uint64_t) * NTESTS);
	unsigned char *pk = malloc(CRYPTO_PUBLICKEYBYTES);
	unsigned char *sk = malloc(CRYPTO_SECRETKEYBYTES);
	unsigned char *k1 = malloc(CRYPTO_BYTES);
	unsigned char *k2 = malloc(CRYPTO_BYTES);
	unsigned char *c = malloc(CRYPTO_CIPHERTEXTBYTES);
	unsigned char *seed = malloc(polarlac_current_params()->seed_len);
	int32_t i;
	uint64_t mlen = CRYPTO_BYTES, clen = CRYPTO_CIPHERTEXTBYTES;
	if (t == NULL || pk == NULL || sk == NULL || k1 == NULL || k2 == NULL || c == NULL || seed == NULL) {
		free(t);
		free(pk);
		free(sk);
		free(k1);
		free(k2);
		free(c);
		free(seed);
		return -1;
	}

	random_bytes(seed, polarlac_current_params()->seed_len);
	for (i = 0; i < NTESTS; i++)
	{
		t[i] = cpucycles();
		crypto_encrypt_keypair(pk, sk);
	}
	LOG_D("pke kg     ");
	print_cpucycles(t);

	crypto_encrypt_keypair(pk, sk);
	random_bytes(k1, CRYPTO_BYTES);
    for(i=0;i<NTESTS;i++)
	{
		t[i]=cpucycles();
		crypto_encrypt(c,&clen,k1,mlen,pk,seed);
	}
	LOG_D("encryption ");
	print_cpucycles(t);



	crypto_encrypt_keypair(pk, sk);
	random_bytes(k1, CRYPTO_BYTES);
	crypto_encrypt(c,&clen,k1,mlen,pk, seed);
	for (i = 0; i < NTESTS; i++)
	{
		t[i] = cpucycles();
		crypto_encrypt_open(k2, &mlen, c, clen, sk);
	}
	LOG_D("decryption ");
	print_cpucycles(t);



	LOG_D("\n");

	free(t);
	free(pk);
	free(sk);
	free(k1);
	free(k2);
	free(c);
	free(seed);
	return 0;
}

// test kem fo
int32_t test_kem_fo_cpucycles()
{
	LOG_D("\n=============================test_kem_cpucycles==================================\n");
	uint64_t *t = malloc(sizeof(uint64_t) * NTESTS);
	unsigned char *pk = malloc(CRYPTO_PUBLICKEYBYTES);
	unsigned char *sk = malloc(CRYPTO_SECRETKEYBYTES);
	unsigned char *k1 = malloc(CRYPTO_SESSION);
	unsigned char *k2 = malloc(CRYPTO_SESSION);
	unsigned char *c = malloc(CRYPTO_CIPHERTEXTBYTES);
	int32_t i;
	if (t == NULL || pk == NULL || sk == NULL || k1 == NULL || k2 == NULL || c == NULL) {
		free(t);
		free(pk);
		free(sk);
		free(k1);
		free(k2);
		free(c);
		return -1;
	}

	for (i = 0; i < NTESTS; i++)
	{
		t[i] = cpucycles();
		crypto_kem_keypair(pk, sk);
	}
	LOG_D("kem_fo kg  ");
	print_cpucycles(t);

	crypto_kem_keypair(pk, sk);
	random_bytes(k1, CRYPTO_SESSION);
	for (i = 0; i < NTESTS; i++)
	{
		t[i] = cpucycles();
		crypto_kem_enc(c, k1, pk);
	}
	LOG_D("encryption ");
	print_cpucycles(t);


	crypto_kem_keypair(pk, sk);
	crypto_kem_enc(c, k1, pk);
	for (i = 0; i < NTESTS; i++)
	{
		t[i] = cpucycles();
		crypto_kem_dec(k2, c, sk);
	}
	LOG_D("decryption ");
	print_cpucycles(t);


	LOG_D("\n");

	free(t);
	free(pk);
	free(sk);
	free(k1);
	free(k2);
	free(c);
	return 0;
}

// test hash
int32_t test_hash_cpucycles()
{
	const polarlac_params_t *p = polarlac_current_params();
	int32_t i, loop = 1000000;
	unsigned char *buf = malloc(p->dim_n);
	unsigned char *seed = malloc(p->seed_len);
	unsigned char *out = malloc(p->message_len);
	uint64_t t0, t1;
	if (buf == NULL || seed == NULL || out == NULL) {
		free(buf);
		free(seed);
		free(out);
		return -1;
	}
	random_bytes(seed, p->seed_len);
	keccak_state state;
	shake256_absorb_once(&state, seed, p->seed_len);
	shake256_squeeze(buf, p->dim_n, &state);
	t0 = cpucycles();
	for (i = 0; i < loop; i++)
	{
		// hash(buf, DIM_N, out);
		sha3_256(out, buf, p->dim_n);
	}
	t1 = cpucycles();
	LOG_D("test hash speed:\n");
	LOG_D("cpucycles: ");
	print_fixed_avg(t1 - t0, (uint64_t)loop);
	LOG_D(" \n");
	LOG_D("CPB: ");
	print_fixed_avg(t1 - t0, (uint64_t)loop * (uint64_t)p->dim_n);
	LOG_D(" \n");
	LOG_D("\n");

	free(buf);
	free(seed);
	free(out);
	return 0;
}


// test gen_psi
int32_t test_gen_psi_cpucycles()
{
	int32_t i, loop = 1000000;
	const polarlac_params_t *p = polarlac_current_params();
	unsigned char *seed = malloc(p->seed_len);
	unsigned char *e = malloc(p->dim_n);
	uint64_t t0, t1;
	if (seed == NULL || e == NULL) {
		free(seed);
		free(e);
		return -1;
	}
	random_bytes(seed, p->seed_len);
	keccak_state state;
    shake256_absorb_once(&state, seed, p->seed_len);

	t0 = cpucycles();
	for (i = 0; i < loop; i++)
	{
		gen_e(e, &state);
	}
	t1 = cpucycles();
	LOG_D("gen_e cpucycles: ");
	print_fixed_avg(t1 - t0, (uint64_t)loop);
	LOG_D(" \n");
	LOG_D("\n");

	free(seed);
	free(e);
	return 0;
}

// test gen_a
int32_t test_gen_a_cpucycles()
{
	const polarlac_params_t *p = polarlac_current_params();
	int32_t i, loop = 1000000;
	unsigned char *seed = malloc(p->seed_len);
	unsigned char *a = malloc(p->dim_n);
	uint64_t t0, t1, sum;
	if (seed == NULL || a == NULL) {
		free(seed);
		free(a);
		return -1;
	}
	random_bytes(seed, p->seed_len);
	sum = 0;
	for (i = 0; i < loop; i++)
	{
		t0 = cpucycles();
		gen_a(a, seed);
		t1 = cpucycles();
		sum += (t1 - t0);
	}

	LOG_D("test gen_a speed:  ");
	LOG_D("cpucycles: ");
	print_fixed_avg(sum, (uint64_t)loop);
	LOG_D(" \n");
	LOG_D("\n");

	free(seed);
	free(a);
	return 0;
}

// test polymul
int32_t test_poly_mul_cpucycles()
{
	const polarlac_params_t *p = polarlac_current_params();
	int32_t i, loop = 100000;
	unsigned char *a = malloc(p->dim_n);
	unsigned char *pk = malloc(p->dim_n);
	unsigned char *sk = malloc(p->dim_n);
	unsigned char *seed = malloc(p->seed_len);
	uint64_t t0, t1, sum;
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
	gen_e(sk, &state);

	sum = 0;
	for (i = 0; i < loop; i++)
	{
		t0 = cpucycles();
		poly_mul(a, sk, pk, p->dim_n);
		t1 = cpucycles();
		sum += (t1 - t0);
	}

	LOG_D("test poly_mul speed:\n");
	LOG_D("cpucycles: ");
	print_fixed_avg(sum, (uint64_t)loop);
	LOG_D(" \n");
	LOG_D("\n");

	free(a);
	free(pk);
	free(sk);
	free(seed);
	return 0;
}

int32_t test_init()
{
	const polarlac_params_t *p = polarlac_current_params();
	int32_t i, loop = 10000;
	unsigned char *a = malloc(p->dim_n);
	unsigned char *pk = malloc(p->dim_n);
	unsigned char *sk = malloc(p->dim_n);
	unsigned char *seed = malloc(p->seed_len);
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
	gen_e(sk, &state);

	for (i = 0; i < loop; i++)
	{
		poly_mul(a, sk, pk, p->dim_n);
	}

	free(a);
	free(pk);
	free(sk);
	free(seed);
	return 0;
}
