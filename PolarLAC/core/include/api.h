// Original copyright: LAC v3

// Modified by:
// Copyright (c) 2025 Ziyao Liu
// Affiliation: Key Laboratory of Cyberspace Security Defense, Institute of Information Engineering, CAS
//              School of Cyber Security, University of Chinese Academy of Sciences


#ifndef _API_H_
#define _API_H_

#include  "polarlac_param.h"
//  Set these three values apropriately for your algorithm
#define CRYPTO_SECRETKEYBYTES POLARLAC_PARAM_SK_LEN+POLARLAC_PARAM_PK_LEN
#define CRYPTO_PUBLICKEYBYTES POLARLAC_PARAM_PK_LEN
#define CRYPTO_BYTES POLARLAC_PARAM_MESSAGE_LEN
#define CRYPTO_SESSION HASHLEN
#define CRYPTO_CIPHERTEXTBYTES POLARLAC_PARAM_CIPHER_LEN

#define CRYPTO_SEEDBYTES POLARLAC_PARAM_SEED_LEN

// Change the algorithm name
#define CRYPTO_ALGNAME STRENGTH
//functions for pke
int32_t crypto_encrypt_keypair( unsigned char *pk, unsigned char *sk);
int32_t crypto_encrypt( unsigned char *c, uint64_t *clen, const unsigned char *m, uint64_t mlen, const unsigned char *pk, unsigned char *seeds);
int32_t crypto_encrypt_open(unsigned char *m, uint64_t *mlen,const unsigned char *c, uint64_t clen,const unsigned char *sk);
//key generation
int32_t kg(unsigned char *pk, unsigned char *sk);
//key generation with seed
int32_t kg_seed(unsigned char *pk, unsigned char *sk, unsigned char *seed);
// encryption
int32_t original_pke_enc(const unsigned char *pk, const unsigned char *m, uint64_t mlen, unsigned char *c, uint64_t *clen, unsigned char *seeds);
// encryption with seed
int32_t pke_enc_seed(const unsigned char *pk, const unsigned char *m, uint64_t mlen, unsigned char *c, uint64_t *clen, unsigned char *seeds);
int32_t original_pke_enc_seed(const unsigned char *pk, const unsigned char *m, uint64_t mlen, unsigned char *c, uint64_t *clen, unsigned char *seed);
// decrypt
int32_t original_pke_dec(const unsigned char *sk, const unsigned char *c, uint64_t clen, unsigned char *m, uint64_t *mlen);


//functions for kem
int32_t crypto_kem_keypair( unsigned char *pk, unsigned char *sk);
int32_t crypto_kem_enc( unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int32_t crypto_kem_dec( unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

int32_t kem_enc_fo(const unsigned char *pk, unsigned char *k, unsigned char *c);
// fo encryption for cca security with seed
int32_t kem_enc_fo_seed(const unsigned char *pk, unsigned char *k, unsigned char *c, unsigned char *seed);
// decrypt of fo mode
int32_t kem_dec_fo(const unsigned char *pk, const unsigned char *sk, const  unsigned char *c, unsigned char *k);

#endif /* _API_H_ */
