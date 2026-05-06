/**
 * @file kem.c
 * @brief Implementation of api.h
 */

#include <stdint.h>
#include <string.h>
#include "api.h"
#include "crypto_memset.h"
#include "hqc.h"
#include "parameters.h"
#include "parsing.h"
#include "symmetric.h"
#include "vector.h"

#ifdef VERBOSE
#include <stdio.h>
#endif

static const hqc_params_t *hqc_active_params(void) {
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
 * @pre The PRNG **must be seeded** with ::prng_init() before calling this function.
 * @warning This function calls ::prng_get_bytes() to sample `seed_kem`. If the PRNG has not been
 *          properly seeded beforehand, the generated keys will be insecure/predictable.
 * @note An example of correct seeding is provided in `main_hqc.c` (see `init_randomness()`), which
 *       seeds the PRNG using `syscall(SYS_getrandom, ...)` (32 bytes) by default..
 * @see prng_init, prng_get_bytes, main_hqc.c
 */
int crypto_kem_keypair(uint8_t *ek_kem, uint8_t *dk_kem) {
    const hqc_params_t *params = hqc_active_params();
#ifdef VERBOSE
    printf("\n\n\n### KEYGEN ###");
#endif
    const size_t seed_bytes = params->seed_bytes;
    const size_t security_bytes = params->security_bytes;
    const size_t public_key_bytes = params->publickeybytes;
    uint8_t seed_kem[SEED_BYTES] = {0};
    uint8_t sigma[PARAM_SECURITY_BYTES] = {0};
    uint8_t seed_pke[SEED_BYTES] = {0};
    shake256_xof_ctx ctx_kem;

    uint8_t ek_pke[PUBLIC_KEY_BYTES] = {0};
    uint8_t dk_pke[SEED_BYTES] = {0};

    // Sample seed_kem
    prng_get_bytes(seed_kem, seed_bytes);

    // Compute seed_pke and randomness sigma
    xof_init(&ctx_kem, seed_kem, seed_bytes);
    xof_get_bytes(&ctx_kem, seed_pke, seed_bytes);
    xof_get_bytes(&ctx_kem, sigma, security_bytes);

    // Compute HQC-PKE keypair
    hqc_pke_keygen_param(params, ek_pke, dk_pke, seed_pke);

    // Compute HQC-KEM keypair
    memcpy(ek_kem, ek_pke, public_key_bytes);
    memcpy(dk_kem, ek_kem, public_key_bytes);
    memcpy(dk_kem + public_key_bytes, dk_pke, seed_bytes);
    memcpy(dk_kem + public_key_bytes + seed_bytes, sigma, security_bytes);
    memcpy(dk_kem + public_key_bytes + seed_bytes + security_bytes, seed_kem, seed_bytes);

#ifdef VERBOSE
    printf("\n\nseed_kem: ");
    for (int i = 0; i < (int)seed_bytes; ++i) printf("%02x", seed_kem[i]);
    printf("\n\nseed_pke: ");
    for (int i = 0; i < (int)seed_bytes; ++i) printf("%02x", seed_pke[i]);
    printf("\n\nsigma: ");
    for (int i = 0; i < (int)security_bytes; ++i) printf("%02x", sigma[i]);
#endif

    // Zeroize sensitive data
    memset_zero(seed_kem, sizeof seed_kem);
    memset_zero(sigma, sizeof sigma);
    memset_zero(seed_pke, sizeof seed_pke);
    memset_zero(dk_pke, sizeof dk_pke);

    return 0;
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
 * @pre The PRNG **must be seeded** with ::prng_init() before calling this function.
 * @warning This function calls ::prng_get_bytes() to sample `seed_kem`. If the PRNG has not been
 *          properly seeded beforehand, the generated keys will be insecure/predictable.
 * @note An example of correct seeding is provided in `main_hqc.c` (see `init_randomness()`), which
 *       seeds the PRNG using `syscall(SYS_getrandom, ...)` (32 bytes) by default..
 * @see prng_init, prng_get_bytes, main_hqc.c
 */
int crypto_kem_enc(uint8_t *c_kem, uint8_t *K, const uint8_t *ek_kem) {
    const hqc_params_t *params = hqc_active_params();
#ifdef VERBOSE
    printf("\n\n\n\n### ENCAPS ###");
#endif
    const size_t seed_bytes = params->seed_bytes;
    const size_t salt_bytes = params->salt_bytes;
    const size_t security_bytes = params->security_bytes;
    const size_t public_key_bytes = params->publickeybytes;
    const size_t shared_secret_bytes = params->bytes;
    const size_t ciphertext_bytes = params->ciphertextbytes;

    uint8_t m[PARAM_SECURITY_BYTES] = {0};
    uint8_t K_theta[SHARED_SECRET_BYTES + SEED_BYTES] = {0};
    uint8_t theta[SEED_BYTES] = {0};
    uint8_t hash_ek_kem[SEED_BYTES] = {0};
    ciphertext_kem_t c_kem_t = {0};

    // Sample message m and salt
    prng_get_bytes(m, security_bytes);
    prng_get_bytes(c_kem_t.salt, salt_bytes);

    // Compute shared key K and ciphertext c_kem
    hash_h_param(params, hash_ek_kem, ek_kem);
    hash_g_param(params, K_theta, hash_ek_kem, m, c_kem_t.salt);
    memcpy(theta, K_theta + shared_secret_bytes, seed_bytes);
    hqc_pke_encrypt_param(params, &c_kem_t.c_pke, ek_kem, (uint64_t *)m, theta);

    hqc_c_kem_to_string_param(params, c_kem, &c_kem_t);
    memcpy(K, K_theta, shared_secret_bytes);

#ifdef VERBOSE
    printf("\n\nek_kem: ");
    for (int i = 0; i < (int)public_key_bytes; ++i) printf("%02x", ek_kem[i]);
    printf("\n\nm: ");
    vect_print_param(params, (uint64_t *)m, security_bytes);
    printf("\n\nsalt: ");
    for (int i = 0; i < (int)salt_bytes; ++i) printf("%02x", c_kem_t.salt[i]);
    printf("\n\nH(ek_kem): ");
    for (int i = 0; i < (int)seed_bytes; ++i) printf("%02x", hash_ek_kem[i]);
    printf("\n\ntheta: ");
    for (int i = 0; i < (int)seed_bytes; ++i) printf("%02x", theta[i]);
    printf("\n\nc_kem: ");
    for (int i = 0; i < (int)ciphertext_bytes; ++i) printf("%02x", c_kem[i]);
    printf("\n\nK: ");
    for (int i = 0; i < (int)shared_secret_bytes; ++i) printf("%02x", K[i]);
#endif

    // Zeroize sensitive data
    memset_zero(m, sizeof m);
    memset_zero(K_theta, sizeof K_theta);
    memset_zero(theta, sizeof theta);

    return 0;
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
    const hqc_params_t *params = hqc_active_params();
#ifdef VERBOSE
    printf("\n\n\n\n### DECAPS ###");
#endif
    const size_t seed_bytes = params->seed_bytes;
    const size_t salt_bytes = params->salt_bytes;
    const size_t security_bytes = params->security_bytes;
    const size_t public_key_bytes = params->publickeybytes;
    const size_t shared_secret_bytes = params->bytes;
    const size_t ciphertext_bytes = params->ciphertextbytes;
    const size_t vec_n_size_bytes = params->vec_n_size_bytes;
    const size_t vec_n1n2_size_bytes = params->vec_n1n2_size_bytes;

    uint8_t ek_pke[PUBLIC_KEY_BYTES] = {0};
    uint8_t dk_pke[SEED_BYTES] = {0};
    uint8_t sigma[PARAM_SECURITY_BYTES] = {0};
    uint8_t m_prime[PARAM_SECURITY_BYTES] = {0};
    uint8_t hash_ek_kem[SEED_BYTES] = {0};
    uint8_t K_theta_prime[SHARED_SECRET_BYTES + SEED_BYTES] = {0};
    uint8_t K_bar[SHARED_SECRET_BYTES] = {0};
    uint8_t theta_prime[SEED_BYTES] = {0};
    ciphertext_kem_t c_kem_t = {0};
    ciphertext_kem_t c_kem_prime_t = {0};
    uint8_t result;

    // Parse decapsulation key dk_kem
    memcpy(ek_pke, dk_kem, public_key_bytes);
    memcpy(dk_pke, dk_kem + public_key_bytes, seed_bytes);
    memcpy(sigma, dk_kem + public_key_bytes + seed_bytes, security_bytes);

    // Parse ciphertext c_kem
    hqc_c_kem_from_string_param(params, &c_kem_t.c_pke, c_kem_t.salt, c_kem);

    // Compute message m_prime
    result = hqc_pke_decrypt_param(params, (uint64_t *)m_prime, dk_pke, &c_kem_t.c_pke);

    // Compute shared key K_prime and ciphertext c_kem_prime
    hash_h_param(params, hash_ek_kem, ek_pke);
    hash_g_param(params, K_theta_prime, hash_ek_kem, m_prime, c_kem_t.salt);
    memcpy(K_prime, K_theta_prime, shared_secret_bytes);
    memcpy(theta_prime, K_theta_prime + shared_secret_bytes, seed_bytes);

    hqc_pke_encrypt_param(params, &c_kem_prime_t.c_pke, ek_pke, (uint64_t *)m_prime, theta_prime);
    memcpy(c_kem_prime_t.salt, c_kem_t.salt, salt_bytes);

    // Compute rejection key K_bar
    hash_j_param(params, K_bar, hash_ek_kem, sigma, &c_kem_t);
    result |= vect_compare((uint8_t *)c_kem_t.c_pke.u, (uint8_t *)c_kem_prime_t.c_pke.u, vec_n_size_bytes);
    result |= vect_compare((uint8_t *)c_kem_t.c_pke.v, (uint8_t *)c_kem_prime_t.c_pke.v, vec_n1n2_size_bytes);
    result |= vect_compare(c_kem_t.salt, c_kem_prime_t.salt, salt_bytes);
    result -= 1;
    for (size_t i = 0; i < shared_secret_bytes; ++i) {
        K_prime[i] = (K_prime[i] & result) ^ (K_bar[i] & ~result);
    }

#ifdef VERBOSE
    printf("\n\nek_pke: ");
    for (int i = 0; i < (int)public_key_bytes; ++i) printf("%02x", ek_pke[i]);
    printf("\n\ndk_pke: ");
    for (int i = 0; i < (int)seed_bytes; ++i) printf("%02x", dk_pke[i]);
    printf("\n\nc_kem: ");
    for (int i = 0; i < (int)ciphertext_bytes; ++i) printf("%02x", c_kem[i]);
    printf("\n\nm_prime: ");
    vect_print_param(params, (uint64_t *)m_prime, security_bytes);
    printf("\n\nH(ek_kem): ");
    for (int i = 0; i < (int)seed_bytes; ++i) printf("%02x", hash_ek_kem[i]);
    printf("\n\ntheta_prime: ");
    for (int i = 0; i < (int)seed_bytes; ++i) printf("%02x", theta_prime[i]);
    printf("\n\n\n# Checking Ciphertext - Begin #");
    printf("\n\nc_kem_prime_t.c_pke.u: ");
    vect_print_param(params, c_kem_prime_t.c_pke.u, vec_n_size_bytes);
    printf("\n\nc_kem_prime_t.c_pke.v: ");
    vect_print_param(params, c_kem_prime_t.c_pke.v, vec_n1n2_size_bytes);
    printf("\n\nsalt: ");
    for (int i = 0; i < (int)salt_bytes; ++i) printf("%02x", c_kem_prime_t.salt[i]);
    printf("\n\n# Checking Ciphertext - End #\n");
    printf("\n\nK_prime: ");
    for (int i = 0; i < (int)shared_secret_bytes; ++i) printf("%02x", K_prime[i]);
#endif

    // Zeroize sensitive data
    memset_zero(dk_pke, sizeof dk_pke);
    memset_zero(sigma, sizeof sigma);
    memset_zero(m_prime, sizeof m_prime);
    memset_zero(K_theta_prime, sizeof K_theta_prime);
    memset_zero(K_bar, sizeof K_bar);
    memset_zero(theta_prime, sizeof theta_prime);

    return 0;
}
