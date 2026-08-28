#ifndef POLARLAC_H
#define POLARLAC_H

#include <stdint.h>

#define POLARLAC_LEVEL_COUNT 3

typedef enum {
    POLARLAC_LEVEL_LIGHT = 0,
    POLARLAC_LEVEL_128 = 1,
    POLARLAC_LEVEL_256 = 2
} polarlac_level_t;

typedef struct {
    polarlac_level_t level;
    const char *name;
    uint16_t dim_n;
    uint16_t dim_n_2;
    uint16_t dim_n_4;
    uint16_t dim_n_8;
    uint16_t seed_len;
    uint16_t pk_len;
    uint16_t sk_part_len;
    uint16_t sk_len;
    uint16_t message_len;
    uint16_t cipher_len;
    uint16_t c2_vec_num;
    uint16_t num_one;
    const char *hash_type;
    uint16_t sample_len;
    uint16_t data_len;
    uint16_t code_len;
    uint16_t polar_n;
    uint8_t polar_log_n;
    uint16_t polar_k;
    uint16_t polar_ecc_bytes;
    const uint8_t *polar_info_nodes;
    const int32_t *polar_lambda_offset;
    const int32_t *polar_llr_layer_vec;
    const int32_t *polar_bit_layer_vec;
    const int64_t *llr_table;
    uint16_t secret_key_bytes;
    uint16_t public_key_bytes;
    uint16_t message_bytes;
    uint16_t session_bytes;
    uint16_t ciphertext_bytes;
    uint16_t seed_bytes;
    uint32_t ntt_q;
    uint32_t ntt_inv_q;
    uint32_t ntt_inv_n;
    uint8_t ntt_k;
    uint8_t bitlen_q;
    uint8_t b_q;
    uint32_t inverse_n;
    uint32_t inverse_n_beta;
    uint8_t ntt_omega_count;
    const int32_t *ntt_omega;
    uint32_t ntt_hw_config;
} polarlac_params_t;

#define POLARLAC_DEFAULT_LEVEL POLARLAC_LEVEL_128

const polarlac_params_t *polarlac_get_params(polarlac_level_t level);
const polarlac_params_t *polarlac_current_params(void);
int32_t polarlac_set_level(polarlac_level_t level);
polarlac_level_t polarlac_get_level(void);

#define POLARLAC_ALGNAME (polarlac_current_params()->name)
#define POLARLAC_SECRETKEYBYTES (polarlac_current_params()->secret_key_bytes)
#define POLARLAC_PUBLICKEYBYTES (polarlac_current_params()->public_key_bytes)
#define POLARLAC_BYTES (polarlac_current_params()->message_bytes)
#define POLARLAC_SESSIONBYTES (polarlac_current_params()->session_bytes)
#define POLARLAC_CIPHERTEXTBYTES (polarlac_current_params()->ciphertext_bytes)
#define POLARLAC_SEEDBYTES (polarlac_current_params()->seed_bytes)

#define POLARLAC_MAX_SECRETKEYBYTES 2144
#define POLARLAC_MAX_PUBLICKEYBYTES 1056
#define POLARLAC_MAX_BYTES 32
#define POLARLAC_MAX_SESSIONBYTES 32
#define POLARLAC_MAX_CIPHERTEXTBYTES 1280
#define POLARLAC_MAX_SEEDBYTES 32

#ifndef CRYPTO_SECRETKEYBYTES
#define CRYPTO_SECRETKEYBYTES POLARLAC_SECRETKEYBYTES
#endif
#ifndef CRYPTO_PUBLICKEYBYTES
#define CRYPTO_PUBLICKEYBYTES POLARLAC_PUBLICKEYBYTES
#endif
#ifndef CRYPTO_BYTES
#define CRYPTO_BYTES POLARLAC_BYTES
#endif
#ifndef CRYPTO_SESSION
#define CRYPTO_SESSION POLARLAC_SESSIONBYTES
#endif
#ifndef CRYPTO_CIPHERTEXTBYTES
#define CRYPTO_CIPHERTEXTBYTES POLARLAC_CIPHERTEXTBYTES
#endif
#ifndef CRYPTO_SEEDBYTES
#define CRYPTO_SEEDBYTES POLARLAC_SEEDBYTES
#endif
#ifndef CRYPTO_ALGNAME
#define CRYPTO_ALGNAME POLARLAC_ALGNAME
#endif

int32_t polarlac_crypto_encrypt_keypair(polarlac_level_t level,
                                        unsigned char *pk, unsigned char *sk);
int32_t polarlac_crypto_encrypt(polarlac_level_t level, unsigned char *c,
                                uint64_t *clen, const unsigned char *m,
                                uint64_t mlen, const unsigned char *pk,
                                unsigned char *seeds);
int32_t polarlac_crypto_encrypt_open(polarlac_level_t level, unsigned char *m,
                                     uint64_t *mlen, const unsigned char *c,
                                     uint64_t clen, const unsigned char *sk);
int32_t polarlac_crypto_kem_keypair(polarlac_level_t level,
                                    unsigned char *pk, unsigned char *sk);
int32_t polarlac_crypto_kem_enc(polarlac_level_t level, unsigned char *ct,
                                unsigned char *ss, const unsigned char *pk);
int32_t polarlac_crypto_kem_dec(polarlac_level_t level, unsigned char *ss,
                                const unsigned char *ct,
                                const unsigned char *sk);

#endif
