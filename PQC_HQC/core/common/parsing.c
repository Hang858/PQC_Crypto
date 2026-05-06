/**
 * @file parsing.c
 * @brief Functions to parse secret key, public key and ciphertext of the HQC scheme
 */

#include "parsing.h"
#include <stdint.h>
#include <string.h>
#include "crypto_memset.h"
#include "symmetric.h"
#include "vector.h"

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
 * @brief Deserializes a decryption key into its internal vectorized form.
 *
 * @param[out] y        Pointer to the output buffer where the internal vectorized key will be stored.
 * @param[in]  dk_pke   Pointer to the serialized decryption key.
 */
void hqc_dk_pke_from_string_param(const hqc_params_t *params, uint64_t *y, const uint8_t *dk_pke) {
    params = resolve_params(params);
    shake256_xof_ctx dk_xof_ctx = {0};
    xof_init(&dk_xof_ctx, dk_pke, params->seed_bytes);
    vect_sample_fixed_weight1_param(params, &dk_xof_ctx, y, params->omega);
    // Zeroize sensitive data
    memset_zero(&dk_xof_ctx, sizeof dk_xof_ctx);
}

void hqc_dk_pke_from_string(uint64_t *y, const uint8_t *dk_pke) {
    hqc_dk_pke_from_string_param(NULL, y, dk_pke);
}

/**
 * @brief Deserializes an encryption key into its internal representation.
 *
 * @param[out] h        Pointer to the output buffer for `h` the first internal component of the key.
 * @param[out] s        Pointer to the output buffer for `s` the second internal component of the key.
 * @param[in]  ek_pke   Pointer to the serialized encryption key.
 */
void hqc_ek_pke_from_string_param(const hqc_params_t *params, uint64_t *h, uint64_t *s, const uint8_t *ek_pke) {
    params = resolve_params(params);
    shake256_xof_ctx ek_xof_ctx = {0};

    xof_init(&ek_xof_ctx, ek_pke, params->seed_bytes);
    vect_set_random_param(params, &ek_xof_ctx, h);

    memcpy(s, ek_pke + params->seed_bytes, params->vec_n_size_bytes);
}

void hqc_ek_pke_from_string(uint64_t *h, uint64_t *s, const uint8_t *ek_pke) {
    hqc_ek_pke_from_string_param(NULL, h, s, ek_pke);
}

/**
 * @brief Serializes a KEM ciphertext structure into a byte array.
 *
 * @param[out] ct       Pointer to the output buffer where the serialized ciphertext will be stored.
 * @param[in]  c_kem    Pointer to the KEM ciphertext structure to be serialized.
 */
void hqc_c_kem_to_string_param(const hqc_params_t *params, uint8_t *ct, const ciphertext_kem_t *c_kem) {
    params = resolve_params(params);
    memcpy(ct, c_kem->c_pke.u, params->vec_n_size_bytes);
    memcpy(ct + params->vec_n_size_bytes, c_kem->c_pke.v, params->vec_n1n2_size_bytes);
    memcpy(ct + params->vec_n_size_bytes + params->vec_n1n2_size_bytes, c_kem->salt, params->salt_bytes);
}

void hqc_c_kem_to_string(uint8_t *ct, const ciphertext_kem_t *c_kem) {
    hqc_c_kem_to_string_param(NULL, ct, c_kem);
}

/**
 * @brief Deserializes a KEM ciphertext byte array into its structured components.
 *
 * @param[out] c_pke    Pointer to the output buffer where the deserialized PKE ciphertext will be stored.
 * @param[out] salt     Pointer to the output buffer where the extracted salt will be stored.
 * @param[in]  ct       Pointer to the serialized KEM ciphertext.
 */
void hqc_c_kem_from_string_param(const hqc_params_t *params, ciphertext_pke_t *c_pke, uint8_t *salt, const uint8_t *ct) {
    params = resolve_params(params);
    memcpy(c_pke->u, ct, params->vec_n_size_bytes);
    memcpy(c_pke->v, ct + params->vec_n_size_bytes, params->vec_n1n2_size_bytes);
    memcpy(salt, ct + params->vec_n_size_bytes + params->vec_n1n2_size_bytes, params->salt_bytes);
}

void hqc_c_kem_from_string(ciphertext_pke_t *c_pke, uint8_t *salt, const uint8_t *ct) {
    hqc_c_kem_from_string_param(NULL, c_pke, salt, ct);
}
