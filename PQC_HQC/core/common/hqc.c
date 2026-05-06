/**
 * @file hqc.c
 * @brief High-level HQC-PKE API implementation
 */

#include "hqc.h"
#include <stdint.h>
#include <string.h>
#include "code.h"
#include "crypto_memset.h"
#include "gf2x.h"
#include "parameters.h"
#include "symmetric.h"
#include "vector.h"
#ifdef VERBOSE
#include <stdio.h>
#endif

static const hqc_params_t *resolve_params(const hqc_params_t *params) {
    if (params != NULL) {
        return params;
    }
#if defined(HQC_LEVEL_HQC1)
    return HQC_get_params(HQC_1);
#elif defined(HQC_LEVEL_HQC3)
    return HQC_get_params(HQC_3);
#elif defined(HQC_LEVEL_HQC5)
    return HQC_get_params(HQC_5);
#else
    return NULL;
#endif
}

/**
 * @brief Generates a key pair for the HQC public-key encryption (PKE) scheme.
 *
 * This function creates a public encryption key (`ek_pke`) and a private decryption key (`dk_pke`)
 * for use in the HQC PKE scheme. The key generation process is seeded with the given `seed` input.
 *
 * @param[out] ek_pke  Pointer to the buffer that will receive the encryption key.
 * @param[out] dk_pke  Pointer to the buffer that will receive the decryption key.
 * @param[in]  seed    Pointer to the seed used to deterministically generate the key pair.
 *
 */
void hqc_pke_keygen_param(const hqc_params_t *params, uint8_t *ek_pke, uint8_t *dk_pke, uint8_t *seed) {
    params = resolve_params(params);
    uint8_t keypair_seed[2 * SEED_BYTES] = {0};
    uint8_t *seed_dk = keypair_seed;
    uint8_t *seed_ek = keypair_seed + params->seed_bytes;
    shake256_xof_ctx dk_xof_ctx = {0};
    shake256_xof_ctx ek_xof_ctx = {0};

    uint64_t x[VEC_N_SIZE_64] = {0};
    uint64_t y[VEC_N_SIZE_64] = {0};
    uint64_t h[VEC_N_SIZE_64] = {0};
    uint64_t s[VEC_N_SIZE_64] = {0};

    // Derive keypair seeds
    hash_i(keypair_seed, seed);

    // Compute decryption key
    xof_init(&dk_xof_ctx, seed_dk, params->seed_bytes);
    vect_sample_fixed_weight1_param(params, &dk_xof_ctx, y, params->omega);
    vect_sample_fixed_weight1_param(params, &dk_xof_ctx, x, params->omega);

    // Compute encryption key
    xof_init(&ek_xof_ctx, seed_ek, params->seed_bytes);
    vect_set_random_param(params, &ek_xof_ctx, h);
    vect_mul(s, y, h);
    vect_add(s, x, s, VEC_N_SIZE_64);

    // Parse encryption key to string
    memcpy(ek_pke, seed_ek, params->seed_bytes);
    memcpy(ek_pke + params->seed_bytes, s, params->vec_n_size_bytes);

    // Parse decryption key to string
    memcpy(dk_pke, seed_dk, params->seed_bytes);

#ifdef VERBOSE
    printf("\n\nseed_dk: ");
    for (int i = 0; i < (int)params->seed_bytes; ++i) printf("%02x", seed_dk[i]);
    printf("\n\nseed_ek: ");
    for (int i = 0; i < (int)params->seed_bytes; ++i) printf("%02x", seed_ek[i]);
    printf("\n\ny: ");
    vect_print(y, VEC_N_SIZE_BYTES);
    printf("\n\nx: ");
    vect_print(x, VEC_N_SIZE_BYTES);
    printf("\n\nh: ");
    vect_print(h, VEC_N_SIZE_BYTES);
    printf("\n\ns: ");
    vect_print(s, VEC_N_SIZE_BYTES);
#endif

    // Zeroize sensitive data
    memset_zero(keypair_seed, sizeof keypair_seed);
    memset_zero(x, sizeof x);
    memset_zero(y, sizeof y);
    memset_zero(&dk_xof_ctx, sizeof dk_xof_ctx);
}

void hqc_pke_keygen(uint8_t *ek_pke, uint8_t *dk_pke, uint8_t *seed) {
    hqc_pke_keygen_param(NULL, ek_pke, dk_pke, seed);
}

/**
 * @brief Encrypts a message using the HQC public-key encryption (PKE) scheme.
 *
 * This function performs encryption in the HQC PKE scheme. It uses the given encryption key (`ek_pke`)
 * and encryption randomness (`theta`) to encrypt the message `m`, producing a ciphertext `c_pke`.
 *
 * @param[out] c_pke     Pointer to the output ciphertext structure (PKE ciphertext).
 * @param[in]  ek_pke    Pointer to the encryption key.
 * @param[in]  m         Pointer to the message to be encrypted.
 * @param[in]  theta     Pointer to the encryption randomness used during encryption.
 *
 */
void hqc_pke_encrypt_param(const hqc_params_t *params, ciphertext_pke_t *c_pke, const uint8_t *ek_pke, const uint64_t *m, const uint8_t *theta) {
    params = resolve_params(params);
    shake256_xof_ctx theta_xof_ctx = {0};
    uint64_t h[VEC_N_SIZE_64] = {0};
    uint64_t s[VEC_N_SIZE_64] = {0};
    uint64_t r1[VEC_N_SIZE_64] = {0};
    uint64_t r2[VEC_N_SIZE_64] = {0};
    uint64_t e[VEC_N_SIZE_64] = {0};
    uint64_t tmp[VEC_N_SIZE_64] = {0};

    // Initialize Xof using theta
    xof_init(&theta_xof_ctx, theta, params->seed_bytes);

    // Retrieve h and s from public key
    hqc_ek_pke_from_string_param(params, h, s, ek_pke);

    // Generate re, e and r1
    vect_sample_fixed_weight2_param(params, &theta_xof_ctx, r2, params->omega_r);
    vect_sample_fixed_weight2_param(params, &theta_xof_ctx, e, params->omega_e);
    vect_sample_fixed_weight2_param(params, &theta_xof_ctx, r1, params->omega_r);

    // Compute u = r1 + r2.h
    vect_mul(c_pke->u, r2, h);
    vect_add(c_pke->u, r1, c_pke->u, VEC_N_SIZE_64);

    // Compute v = C.encode(m)
    code_encode_param(params, c_pke->v, m);

    // Compute v = C.encode(m) + Truncate(s.r2 + e)
    vect_mul(tmp, r2, s);
    vect_add(tmp, e, tmp, VEC_N_SIZE_64);
    vect_truncate_param(params, tmp);
    vect_add(c_pke->v, c_pke->v, tmp, VEC_N1N2_SIZE_64);

#ifdef VERBOSE
    printf("\n\nh: ");
    vect_print(h, VEC_N_SIZE_BYTES);
    printf("\n\ns: ");
    vect_print(s, VEC_N_SIZE_BYTES);
    printf("\n\nr1: ");
    vect_print(r1, VEC_N_SIZE_BYTES);
    printf("\n\nr2: ");
    vect_print(r2, VEC_N_SIZE_BYTES);
    printf("\n\ne: ");
    vect_print(e, VEC_N_SIZE_BYTES);
    printf("\n\nTruncate(s.r2 + e): ");
    vect_print(tmp, VEC_N1N2_SIZE_BYTES);
    printf("\n\nc_pke->u: ");
    vect_print(c_pke->u, VEC_N_SIZE_BYTES);
    printf("\n\nc_pke->v: ");
    vect_print(c_pke->v, VEC_N1N2_SIZE_BYTES);
#endif
    // Zeroize sensitive data
    memset_zero(r1, sizeof r1);
    memset_zero(r2, sizeof r2);
    memset_zero(e, sizeof e);
    memset_zero(tmp, sizeof tmp);
    memset_zero(&theta_xof_ctx, sizeof theta_xof_ctx);
}

void hqc_pke_encrypt(ciphertext_pke_t *c_pke, const uint8_t *ek_pke, const uint64_t *m, const uint8_t *theta) {
    hqc_pke_encrypt_param(NULL, c_pke, ek_pke, m, theta);
}

/**
 * @brief Decrypts a ciphertext using the HQC public-key encryption (PKE) scheme.
 *
 * This function performs decryption in the HQC PKE scheme. It uses the given decryption key (`dk_pke`)
 * to decrypt the ciphertext `c_pke`, recovering the original message `m`.
 *
 * @param[out] m         Pointer to the output buffer where the decrypted message will be stored.
 * @param[in]  dk_pke    Pointer to the decryption key.
 * @param[in]  c_pke     Pointer to the input ciphertext structure (PKE ciphertext).
 *
 * @return Returns 0 on success.
 *
 */
uint8_t hqc_pke_decrypt_param(const hqc_params_t *params, uint64_t *m, const uint8_t *dk_pke, const ciphertext_pke_t *c_pke) {
    params = resolve_params(params);
    uint64_t y[VEC_N_SIZE_64] = {0};
    uint64_t tmp1[VEC_N_SIZE_64] = {0};
    uint64_t tmp2[VEC_N_SIZE_64] = {0};

    // Parse decryption key dk_pke
    hqc_dk_pke_from_string_param(params, y, dk_pke);

    // Compute u.y
    vect_mul(tmp1, y, c_pke->u);
    // Truncate(u.y)
    vect_truncate_param(params, tmp1);
    // Compute v - Truncate(u.y)
    vect_add(tmp2, c_pke->v, tmp1, VEC_N1N2_SIZE_64);

#ifdef VERBOSE
    printf("\n\nc_pke.u: ");
    vect_print(c_pke->u, VEC_N_SIZE_BYTES);
    printf("\n\nc_pke.v: ");
    vect_print(c_pke->v, VEC_N1N2_SIZE_BYTES);
    printf("\n\ny: ");
    vect_print(y, VEC_N_SIZE_BYTES);
    printf("\n\nTruncate(u.y): ");
    vect_print(tmp1, VEC_N1N2_SIZE_BYTES);
    printf("\n\nv - Truncate(u.y): ");
    vect_print(tmp2, VEC_N1N2_SIZE_BYTES);
#endif

    // Compute plaintext m
    code_decode_param(params, m, tmp2);

    // Zeroize sensitive data
    memset_zero(y, sizeof y);
    memset_zero(tmp1, sizeof tmp1);
    memset_zero(tmp2, sizeof tmp2);

    return 0;
}

uint8_t hqc_pke_decrypt(uint64_t *m, const uint8_t *dk_pke, const ciphertext_pke_t *c_pke) {
    return hqc_pke_decrypt_param(NULL, m, dk_pke, c_pke);
}
