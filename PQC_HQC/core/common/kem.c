/**
 * @file kem.c
 * @brief Implementation of api.h
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "crypto_memset.h"
#include "hqc.h"
#include "parameters.h"
#include "parsing.h"
#include "symmetric.h"
#include "vector.h"

#include "hqc_log.h"

/**
 * @brief Generates a keypair for the KEM (Key Encapsulation Mechanism) scheme.
 *
 * This function generates a public/private keypair used for key encapsulation and decapsulation.
 * The encapsulation key (`ek`) is used to encapsulate a shared secret, while the decapsulation key (`dk`)
 * is used to recover it.
 *
 * @param[out] ek_kem Pointer to the output buffer where the encapsulation key will be stored.
 * @param[out] dk_kem Pointer to the output buffer where the decapsulation key will be stored.
 *
 * @return 0 on success.
 *
 * @see hqc_randombytes
 */
int crypto_kem_keypair(uint8_t *ek_kem, uint8_t *dk_kem) {
#ifdef VERBOSE
    HQC_LOGF("\n\n\n### KEYGEN ###");
#endif
    uint8_t *seed_kem = calloc(SEED_BYTES, sizeof(uint8_t));
    uint8_t *sigma = calloc(HQC_MAX_SECURITY_BYTES, sizeof(uint8_t));
    uint8_t *seed_pke = calloc(SEED_BYTES, sizeof(uint8_t));
    shake256_xof_ctx ctx_kem;

    uint8_t *ek_pke = calloc(HQC_MAX_PUBLIC_KEY_BYTES, sizeof(uint8_t));
    uint8_t *dk_pke = calloc(SEED_BYTES, sizeof(uint8_t));

    if (seed_kem == NULL || sigma == NULL || seed_pke == NULL || ek_pke == NULL || dk_pke == NULL) {
        goto cleanup;
    }

    // Sample seed_kem
    if (hqc_randombytes(seed_kem, SEED_BYTES) != 0) {
        goto cleanup;
    }

    // Compute seed_pke and randomness sigma
    xof_init(&ctx_kem, seed_kem, SEED_BYTES);
    xof_get_bytes(&ctx_kem, seed_pke, SEED_BYTES);
    xof_get_bytes(&ctx_kem, sigma, PARAM_SECURITY_BYTES);

    // Compute HQC-PKE keypair
    hqc_pke_keygen(ek_pke, dk_pke, seed_pke);

    // Compute HQC-KEM keypair
    memcpy(ek_kem, ek_pke, PUBLIC_KEY_BYTES);
    memcpy(dk_kem, ek_kem, PUBLIC_KEY_BYTES);
    memcpy(dk_kem + PUBLIC_KEY_BYTES, dk_pke, SEED_BYTES);
    memcpy(dk_kem + PUBLIC_KEY_BYTES + SEED_BYTES, sigma, PARAM_SECURITY_BYTES);
    memcpy(dk_kem + PUBLIC_KEY_BYTES + SEED_BYTES + PARAM_SECURITY_BYTES, seed_kem, SEED_BYTES);

#ifdef VERBOSE
    HQC_LOGF("\n\nseed_kem: ");
    for (int i = 0; i < SEED_BYTES; ++i) HQC_LOGF("%02x", seed_kem[i]);
    HQC_LOGF("\n\nseed_pke: ");
    for (int i = 0; i < SEED_BYTES; ++i) HQC_LOGF("%02x", seed_pke[i]);
    HQC_LOGF("\n\nsigma: ");
    for (int i = 0; i < PARAM_SECURITY_BYTES; ++i) HQC_LOGF("%02x", sigma[i]);
#endif

    // Zeroize sensitive data
    if (seed_kem != NULL) {
        memset_zero(seed_kem, SEED_BYTES);
    }
    if (sigma != NULL) {
        memset_zero(sigma, HQC_MAX_SECURITY_BYTES);
    }
    if (seed_pke != NULL) {
        memset_zero(seed_pke, SEED_BYTES);
    }
    if (dk_pke != NULL) {
        memset_zero(dk_pke, SEED_BYTES);
    }
    free(seed_kem);
    free(sigma);
    free(seed_pke);
    free(ek_pke);
    free(dk_pke);
    return 0;

cleanup:
    if (seed_kem != NULL) {
        memset_zero(seed_kem, SEED_BYTES);
    }
    if (sigma != NULL) {
        memset_zero(sigma, HQC_MAX_SECURITY_BYTES);
    }
    if (seed_pke != NULL) {
        memset_zero(seed_pke, SEED_BYTES);
    }
    if (dk_pke != NULL) {
        memset_zero(dk_pke, SEED_BYTES);
    }
    free(seed_kem);
    free(sigma);
    free(seed_pke);
    free(ek_pke);
    free(dk_pke);
    return -1;
}

/**
 * @brief Performs key encapsulation using the KEM scheme.
 *
 * This function uses the encapsulation key (`ek`) to generate a ciphertext (`c_kem`) and a shared secret (`K`)..
 *
 * @param[out] c_kem   Pointer to the output buffer where the KEM ciphertext will be stored.
 * @param[out] K       Pointer to the output buffer where the shared secret will be stored.
 * @param[in]  ek_kem      Pointer to the encapsulation key.
 *
 * @return Returns 0 on success.
 *
 * @see hqc_randombytes
 */
int crypto_kem_enc(uint8_t *c_kem, uint8_t *K, const uint8_t *ek_kem) {
#ifdef VERBOSE
    HQC_LOGF("\n\n\n\n### ENCAPS ###");
#endif

    uint8_t *m = calloc(HQC_MAX_SECURITY_BYTES, sizeof(uint8_t));
    uint8_t *K_theta = calloc(HQC_SHARED_SECRET_BYTES + HQC_SEED_BYTES, sizeof(uint8_t));
    uint8_t *theta = calloc(SEED_BYTES, sizeof(uint8_t));
    uint8_t *hash_ek_kem = calloc(SEED_BYTES, sizeof(uint8_t));
    ciphertext_kem_t *c_kem_t = calloc(1, sizeof(ciphertext_kem_t));
    if (m == NULL || K_theta == NULL || theta == NULL || hash_ek_kem == NULL || c_kem_t == NULL) {
        goto cleanup;
    }

    // Sample message m and salt
    if (hqc_randombytes(m, PARAM_SECURITY_BYTES) != 0 ||
        hqc_randombytes(c_kem_t->salt, SALT_BYTES) != 0) {
        goto cleanup;
    }

    // Compute shared key K and ciphertext c_kem
    hash_h(hash_ek_kem, ek_kem);
    hash_g(K_theta, hash_ek_kem, m, c_kem_t->salt);
    memcpy(theta, K_theta + SEED_BYTES, SEED_BYTES);
    hqc_pke_encrypt(&c_kem_t->c_pke, ek_kem, (uint64_t *)m, theta);

    hqc_c_kem_to_string(c_kem, c_kem_t);
    memcpy(K, K_theta, SHARED_SECRET_BYTES);

#ifdef VERBOSE
    HQC_LOGF("\n\nek_kem: ");
    for (int i = 0; i < PUBLIC_KEY_BYTES; ++i) HQC_LOGF("%02x", ek_kem[i]);
    HQC_LOGF("\n\nm: ");
    vect_print((uint64_t *)m, PARAM_SECURITY_BYTES);
    HQC_LOGF("\n\nsalt: ");
    for (int i = 0; i < SALT_BYTES; ++i) HQC_LOGF("%02x", c_kem_t->salt[i]);
    HQC_LOGF("\n\nH(ek_kem): ");
    for (int i = 0; i < SEED_BYTES; ++i) HQC_LOGF("%02x", hash_ek_kem[i]);
    HQC_LOGF("\n\ntheta: ");
    for (int i = 0; i < SEED_BYTES; ++i) HQC_LOGF("%02x", theta[i]);
    HQC_LOGF("\n\nc_kem: ");
    for (int i = 0; i < CIPHERTEXT_BYTES; ++i) HQC_LOGF("%02x", c_kem[i]);
    HQC_LOGF("\n\nK: ");
    for (int i = 0; i < SHARED_SECRET_BYTES; ++i) HQC_LOGF("%02x", K[i]);
#endif

    // Zeroize sensitive data
    if (m != NULL) {
        memset_zero(m, HQC_MAX_SECURITY_BYTES);
    }
    if (K_theta != NULL) {
        memset_zero(K_theta, HQC_SHARED_SECRET_BYTES + HQC_SEED_BYTES);
    }
    if (theta != NULL) {
        memset_zero(theta, SEED_BYTES);
    }
    free(m);
    free(K_theta);
    free(theta);
    free(hash_ek_kem);
    free(c_kem_t);
    return 0;

cleanup:
    if (m != NULL) {
        memset_zero(m, HQC_MAX_SECURITY_BYTES);
    }
    if (K_theta != NULL) {
        memset_zero(K_theta, HQC_SHARED_SECRET_BYTES + HQC_SEED_BYTES);
    }
    if (theta != NULL) {
        memset_zero(theta, SEED_BYTES);
    }
    free(m);
    free(K_theta);
    free(theta);
    free(hash_ek_kem);
    free(c_kem_t);
    return -1;
}

/**
 * @brief Performs key decapsulation using the KEM scheme.
 *
 * This function uses the decapsulation key (`dk`) to recover the shared secret (`K_prime`)
 * from the given KEM ciphertext (`c_kem`), which was generated during encapsulation.
 *
 * @param[out] K_prime   Pointer to the output buffer where the recovered shared secret will be stored.
 * @param[in]  c_kem     Pointer to the input KEM ciphertext.
 * @param[in]  dk_kem    Pointer to the decapsulation key.
 *
 * @return Returns 0 on success.
 */
int crypto_kem_dec(uint8_t *K_prime, const uint8_t *c_kem, const uint8_t *dk_kem) {
#ifdef VERBOSE
    HQC_LOGF("\n\n\n\n### DECAPS ###");
#endif

    uint8_t *ek_pke = calloc(HQC_MAX_PUBLIC_KEY_BYTES, sizeof(uint8_t));
    uint8_t *dk_pke = calloc(SEED_BYTES, sizeof(uint8_t));
    uint8_t *sigma = calloc(HQC_MAX_SECURITY_BYTES, sizeof(uint8_t));
    uint8_t *m_prime = calloc(HQC_MAX_SECURITY_BYTES, sizeof(uint8_t));
    uint8_t *hash_ek_kem = calloc(SEED_BYTES, sizeof(uint8_t));
    uint8_t *K_theta_prime = calloc(HQC_SHARED_SECRET_BYTES + HQC_SEED_BYTES, sizeof(uint8_t));
    uint8_t *K_bar = calloc(HQC_SHARED_SECRET_BYTES, sizeof(uint8_t));
    uint8_t *theta_prime = calloc(SEED_BYTES, sizeof(uint8_t));
    ciphertext_kem_t *c_kem_t = calloc(1, sizeof(ciphertext_kem_t));
    ciphertext_kem_t *c_kem_prime_t = calloc(1, sizeof(ciphertext_kem_t));
    uint8_t result;

    if (ek_pke == NULL || dk_pke == NULL || sigma == NULL || m_prime == NULL ||
        hash_ek_kem == NULL || K_theta_prime == NULL || K_bar == NULL || theta_prime == NULL ||
        c_kem_t == NULL || c_kem_prime_t == NULL) {
        goto cleanup;
    }

    // Parse decapsulation key dk_kem
    memcpy(ek_pke, dk_kem, PUBLIC_KEY_BYTES);
    memcpy(dk_pke, dk_kem + PUBLIC_KEY_BYTES, SEED_BYTES);
    memcpy(sigma, dk_kem + PUBLIC_KEY_BYTES + SEED_BYTES, PARAM_SECURITY_BYTES);

    // Parse ciphertext c_kem
    hqc_c_kem_from_string(&c_kem_t->c_pke, c_kem_t->salt, c_kem);

    // Compute message m_prime
    result = hqc_pke_decrypt((uint64_t *)m_prime, dk_pke, &c_kem_t->c_pke);

    // Compute shared key K_prime and ciphertext c_kem_prime
    hash_h(hash_ek_kem, ek_pke);
    hash_g(K_theta_prime, hash_ek_kem, m_prime, c_kem_t->salt);
    memcpy(K_prime, K_theta_prime, SHARED_SECRET_BYTES);
    memcpy(theta_prime, K_theta_prime + SHARED_SECRET_BYTES, SEED_BYTES);

    hqc_pke_encrypt(&c_kem_prime_t->c_pke, ek_pke, (uint64_t *)m_prime, theta_prime);
    memcpy(c_kem_prime_t->salt, c_kem_t->salt, SALT_BYTES);

    // Compute rejection key K_bar
    hash_j(K_bar, hash_ek_kem, sigma, c_kem_t);
    result |= vect_compare((uint8_t *)c_kem_t->c_pke.u, (uint8_t *)c_kem_prime_t->c_pke.u, VEC_N_SIZE_BYTES);
    result |= vect_compare((uint8_t *)c_kem_t->c_pke.v, (uint8_t *)c_kem_prime_t->c_pke.v, VEC_N1N2_SIZE_BYTES);
    result |= vect_compare(c_kem_t->salt, c_kem_prime_t->salt, SALT_BYTES);
    result -= 1;
    for (size_t i = 0; i < SHARED_SECRET_BYTES; ++i) {
        K_prime[i] = (K_prime[i] & result) ^ (K_bar[i] & ~result);
    }

#ifdef VERBOSE
    HQC_LOGF("\n\nek_pke: ");
    for (int i = 0; i < PUBLIC_KEY_BYTES; ++i) HQC_LOGF("%02x", ek_pke[i]);
    HQC_LOGF("\n\ndk_pke: ");
    for (int i = 0; i < SEED_BYTES; ++i) HQC_LOGF("%02x", dk_pke[i]);
    HQC_LOGF("\n\nc_kem: ");
    for (int i = 0; i < CIPHERTEXT_BYTES; ++i) HQC_LOGF("%02x", c_kem[i]);
    HQC_LOGF("\n\nm_prime: ");
    vect_print((uint64_t *)m_prime, PARAM_SECURITY_BYTES);
    HQC_LOGF("\n\nH(ek_kem): ");
    for (int i = 0; i < SEED_BYTES; ++i) HQC_LOGF("%02x", hash_ek_kem[i]);
    HQC_LOGF("\n\ntheta_prime: ");
    for (int i = 0; i < SEED_BYTES; ++i) HQC_LOGF("%02x", theta_prime[i]);
    HQC_LOGF("\n\n\n# Checking Ciphertext - Begin #");
    HQC_LOGF("\n\nc_kem_prime_t.c_pke.u: ");
    vect_print(c_kem_prime_t->c_pke.u, VEC_N_SIZE_BYTES);
    HQC_LOGF("\n\nc_kem_prime_t.c_pke.v: ");
    vect_print(c_kem_prime_t->c_pke.v, VEC_N1N2_SIZE_BYTES);
    HQC_LOGF("\n\nsalt: ");
    for (int i = 0; i < SALT_BYTES; ++i) HQC_LOGF("%02x", c_kem_prime_t->salt[i]);
    HQC_LOGF("\n\n# Checking Ciphertext - End #\n");
    HQC_LOGF("\n\nK_prime: ");
    for (int i = 0; i < SHARED_SECRET_BYTES; ++i) HQC_LOGF("%02x", K_prime[i]);
#endif

    // Zeroize sensitive data
    if (dk_pke != NULL) {
        memset_zero(dk_pke, SEED_BYTES);
    }
    if (sigma != NULL) {
        memset_zero(sigma, HQC_MAX_SECURITY_BYTES);
    }
    if (m_prime != NULL) {
        memset_zero(m_prime, HQC_MAX_SECURITY_BYTES);
    }
    if (K_theta_prime != NULL) {
        memset_zero(K_theta_prime, HQC_SHARED_SECRET_BYTES + HQC_SEED_BYTES);
    }
    if (K_bar != NULL) {
        memset_zero(K_bar, HQC_SHARED_SECRET_BYTES);
    }
    if (theta_prime != NULL) {
        memset_zero(theta_prime, SEED_BYTES);
    }
    free(ek_pke);
    free(dk_pke);
    free(sigma);
    free(m_prime);
    free(hash_ek_kem);
    free(K_theta_prime);
    free(K_bar);
    free(theta_prime);
    free(c_kem_t);
    free(c_kem_prime_t);
    return 0;

cleanup:
    if (dk_pke != NULL) {
        memset_zero(dk_pke, SEED_BYTES);
    }
    if (sigma != NULL) {
        memset_zero(sigma, HQC_MAX_SECURITY_BYTES);
    }
    if (m_prime != NULL) {
        memset_zero(m_prime, HQC_MAX_SECURITY_BYTES);
    }
    if (K_theta_prime != NULL) {
        memset_zero(K_theta_prime, HQC_SHARED_SECRET_BYTES + HQC_SEED_BYTES);
    }
    if (K_bar != NULL) {
        memset_zero(K_bar, HQC_SHARED_SECRET_BYTES);
    }
    if (theta_prime != NULL) {
        memset_zero(theta_prime, SEED_BYTES);
    }
    free(ek_pke);
    free(dk_pke);
    free(sigma);
    free(m_prime);
    free(hash_ek_kem);
    free(K_theta_prime);
    free(K_bar);
    free(theta_prime);
    free(c_kem_t);
    free(c_kem_prime_t);
    return -1;
}
