#ifndef PQC_HQC_API_H
#define PQC_HQC_API_H

#define CRYPTO_ALGNAME "HQC-1"

#define CRYPTO_SECRETKEYBYTES  2321
#define CRYPTO_PUBLICKEYBYTES  2241
#define CRYPTO_BYTES           32
#define CRYPTO_CIPHERTEXTBYTES 4433

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#endif
