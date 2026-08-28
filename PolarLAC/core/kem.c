#include <stdio.h>
#include "api.h"
#include "rand.h"
#include "fips202.h"
#include "operator_interface.h"
#include "memops.h"
#include "malloc.h"
//generate keypair
int32_t crypto_kem_keypair(unsigned char *pk, unsigned char *sk)
{
	//call the key generation algorithm of pke
	crypto_encrypt_keypair(pk, sk);
	const polarlac_params_t *p = polarlac_current_params();
	// random_bytes(sk+SK_PART_LEN+PK_LEN+HASHLEN,MESSAGE_LEN);
	OP_trng(sk + p->sk_part_len + p->pk_len + HASHLEN, p->message_len);
	return 0;
}
int32_t crypto_kem_enc( unsigned char *ct, unsigned char *ss, const unsigned char *pk)
{
	kem_enc_fo(pk,ss,ct);
	return 0;
}
int32_t crypto_kem_dec( unsigned char *ss, const unsigned char *ct, const unsigned char *sk)
{
	const unsigned char *pk;
	pk = sk + polarlac_current_params()->sk_part_len;
	kem_dec_fo(pk,sk,ct,ss);
	return 0;
}
// fo encryption for cca security 
int32_t kem_enc_fo(const unsigned char *pk, unsigned char *k, unsigned char *c)
{
	const polarlac_params_t *p = polarlac_current_params();
	unsigned char *buf = my_malloc(p->message_len + p->cipher_len);
	unsigned char *seed = my_malloc(p->seed_len);
	unsigned char *seed_buf = my_malloc(p->message_len + p->seed_len);
	uint64_t clen;

	//check parameter
	if(pk==NULL || k==NULL || c==NULL)
	{
		my_free(buf);
		my_free(seed);
		my_free(seed_buf);
		return -1;
	}
	if (buf == NULL || seed == NULL || seed_buf == NULL) {
		my_free(buf);
		my_free(seed);
		my_free(seed_buf);
		return -1;
	}

	//generate random message m, stored in buf
	// random_bytes(buf,MESSAGE_LEN);
	OP_trng(buf, p->message_len);
	//compute seed=hash(m|pk), add pk for multi key attack protection
	memcpy(seed_buf, buf, p->message_len);
	memcpy(seed_buf + p->message_len, pk, p->seed_len);
	// sha3_256(seed,seed_buf,MESSAGE_LEN+SEED_LEN);
	OP_hash(5, 0, 32, seed_buf, p->message_len + p->seed_len, 0, seed);
	//encrypt m with seed
	original_pke_enc_seed(pk, buf, p->message_len, c, &clen, seed);

	//compute k=hash(m|c)
	memcpy(buf + p->message_len, c, p->cipher_len);
	// sha3_256(k,buf,MESSAGE_LEN+CIPHER_LEN);
	OP_hash(5, 0, 32, buf, p->message_len + p->cipher_len, 0, k);

	my_free(buf);
	my_free(seed);
	my_free(seed_buf);
	return 0;
}

// fo encryption for cca security with seed
int32_t kem_enc_fo_seed(const unsigned char *pk, unsigned char *k, unsigned char *c, unsigned char *seed)
{
	const polarlac_params_t *p = polarlac_current_params();
	unsigned char *buf = my_malloc(p->message_len + p->cipher_len);
	unsigned char *local_seed = my_malloc(p->seed_len);
	unsigned char *seed_buf = my_malloc(p->message_len + p->seed_len);
	uint64_t clen;


	//check parameter
	if(pk==NULL || k==NULL || c==NULL)
	{
		my_free(buf);
		my_free(local_seed);
		my_free(seed_buf);
		return -1;
	}
	if (buf == NULL || local_seed == NULL || seed_buf == NULL) {
		my_free(buf);
		my_free(local_seed);
		my_free(seed_buf);
		return -1;
	}

	//generate random message m, stored in buf
	keccak_state state;
    // shake256_absorb_once(&state, seed, SEED_LEN);
	OP_hash_init(3, &state.s, 200+8);
    OP_hash_absorb(3, &state.s, 200+8, seed, p->seed_len);
	// shake256_squeeze(buf, MESSAGE_LEN, &state);
	OP_hash_squeeze(3, &state.s, 200+8, buf, p->message_len);
	//compute loacal_seed=hash(m|pk), add pk for multi key attack protection
	memcpy(seed_buf, buf, p->message_len);
	memcpy(seed_buf + p->message_len, pk, p->seed_len);
	// sha3_256(local_seed,seed_buf,MESSAGE_LEN+SEED_LEN);
	OP_hash(5, 0, 32, seed_buf, p->message_len + p->seed_len, 0, local_seed);
	//encrypt m with local_seed
	original_pke_enc_seed(pk, buf, p->message_len, c, &clen, local_seed);

	//compute k=hash(m|c)
	memcpy(buf + p->message_len, c, p->cipher_len);
	// sha3_256(k,buf,MESSAGE_LEN+CIPHER_LEN);
	OP_hash(5, 0, 32, buf, p->message_len + p->cipher_len, 0, k);

	my_free(buf);
	my_free(local_seed);
	my_free(seed_buf);
	return 0;
}

// decrypt of fo mode
int32_t kem_dec_fo(const unsigned char *pk, const unsigned char *sk, const unsigned char *c, unsigned char *k)
{
	const polarlac_params_t *p = polarlac_current_params();
	unsigned char *buf = my_malloc(p->message_len + p->cipher_len);
	unsigned char *seed = my_malloc(p->seed_len);
	unsigned char *seed_buf = my_malloc(p->message_len + p->seed_len);
	uint64_t mlen,clen;
	unsigned char *c_v = my_malloc(p->cipher_len);

	//check parameter
	if(pk==NULL || sk==NULL || k==NULL || c==NULL)
	{
		my_free(buf);
		my_free(seed);
		my_free(seed_buf);
		my_free(c_v);
		return -1;
	}
	if (buf == NULL || seed == NULL || seed_buf == NULL || c_v == NULL) {
		my_free(buf);
		my_free(seed);
		my_free(seed_buf);
		my_free(c_v);
		return -1;
	}

	//compute m from c
	original_pke_dec(sk, c, p->cipher_len, buf, &mlen);
	//compte k=hash(m|c)
	memcpy(buf + p->message_len, c, p->cipher_len);
	// sha3_256(k,buf,MESSAGE_LEN+CIPHER_LEN);
	OP_hash(5, 0, 32, buf, p->message_len + p->cipher_len, 0, k);
	//re-encryption with seed=hash(m|pk), add pk for multi key attack protection
	memcpy(seed_buf, buf, p->message_len);
	memcpy(seed_buf + p->message_len, pk, p->seed_len);
	// sha3_256(seed,seed_buf,MESSAGE_LEN+SEED_LEN);
	OP_hash(5, 0, 32, seed_buf, p->message_len + p->seed_len, 0, seed);
	original_pke_enc_seed(pk, buf, p->message_len, c_v, &clen, seed);

	//verify
	if(memcmp(c, c_v, p->cipher_len) != 0)
	{
		//k=hash(hash(sk)|c)
		// sha3_256(buf,sk,SK_LEN);
		OP_hash(5, 0, 32, sk, p->sk_len, 0, buf);
		memcpy(buf + p->message_len, c, p->cipher_len);
		// sha3_256(k,buf,MESSAGE_LEN+CIPHER_LEN);
		OP_hash(5, 0, 32, buf, p->message_len + p->cipher_len, 0, k);
	}

	my_free(buf);
	my_free(seed);
	my_free(seed_buf);
	my_free(c_v);
	return 0;
}

