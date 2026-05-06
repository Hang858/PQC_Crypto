/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: Key Encapsulation Mechanism (KEM) based on Frodo
*********************************************************************************************/

#include <string.h>
#include <stdlib.h>
#include "fips202.h"
#include "frodo_macrify.h"
#include "random.h"

#ifdef DO_VALGRIND_CHECK
#include <valgrind/memcheck.h>
#endif


int crypto_kem_keypair(unsigned char* pk, unsigned char* sk)
{ // FrodoKEM's key generation
  // Outputs: public key pk = pk_seedA||pk_b                      (               BYTES_SEED_A + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 bytes)
  //          secret key sk = sk_s||pk_seedA||pk_b||sk_S||sk_pkh  (CRYPTO_BYTES + BYTES_SEED_A + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 + 2*PARAMS_N*PARAMS_NBAR + BYTES_PKHASH bytes)
    uint8_t *pk_seedA = &pk[0];
    uint8_t *pk_b = &pk[BYTES_SEED_A];
    uint8_t *sk_s = &sk[0];
    uint8_t *sk_pk = &sk[CRYPTO_BYTES];
    uint8_t *sk_S = &sk[CRYPTO_BYTES + CRYPTO_PUBLICKEYBYTES];
    uint8_t *sk_pkh = &sk[CRYPTO_BYTES + CRYPTO_PUBLICKEYBYTES + 2*PARAMS_N*PARAMS_NBAR];
    uint16_t *B = calloc((size_t)PARAMS_N * PARAMS_NBAR, sizeof(uint16_t));
    uint16_t *S = calloc((size_t)2 * PARAMS_N * PARAMS_NBAR, sizeof(uint16_t));  // contains secret data
    uint8_t *randomness = calloc((size_t)CRYPTO_BYTES + BYTES_SEED_SE + BYTES_SEED_A, sizeof(uint8_t));  // contains secret data via randomness_s and randomness_seedSE
    uint8_t *shake_input_seedSE = calloc((size_t)1 + BYTES_SEED_SE, sizeof(uint8_t));  // contains secret data
    uint16_t *E = NULL;                                                // contains secret data
    uint8_t *randomness_s = NULL;                                      // contains secret data
    uint8_t *randomness_seedSE = NULL;                                 // contains secret data
    uint8_t *randomness_z = NULL;
    int ret = 1;

    if (B == NULL || S == NULL || randomness == NULL || shake_input_seedSE == NULL) {
        goto cleanup;
    }
    E = &S[PARAMS_N*PARAMS_NBAR];
    randomness_s = &randomness[0];
    randomness_seedSE = &randomness[CRYPTO_BYTES];
    randomness_z = &randomness[CRYPTO_BYTES + BYTES_SEED_SE];

    // Generate the secret value s, the seed for S and E, and the seed for the seed for A. Add seed_A to the public key
    if (randombytes(randomness, CRYPTO_BYTES + BYTES_SEED_SE + BYTES_SEED_A) != 0)
        goto cleanup;
#ifdef DO_VALGRIND_CHECK
    VALGRIND_MAKE_MEM_UNDEFINED(randomness, CRYPTO_BYTES + BYTES_SEED_SE + BYTES_SEED_A);
#endif
    shake(pk_seedA, BYTES_SEED_A, randomness_z, BYTES_SEED_A);

    // Generate S and E, and compute B = A*S + E. Generate A on-the-fly
    shake_input_seedSE[0] = 0x5F;
    memcpy(&shake_input_seedSE[1], randomness_seedSE, BYTES_SEED_SE);
    shake((uint8_t*)S, 2*PARAMS_N*PARAMS_NBAR*sizeof(uint16_t), shake_input_seedSE, 1 + BYTES_SEED_SE);
    for (size_t i = 0; i < 2 * PARAMS_N * PARAMS_NBAR; i++) {
        S[i] = LE_TO_UINT16(S[i]);
    }
    frodo_sample_n(S, PARAMS_N*PARAMS_NBAR);
    frodo_sample_n(E, PARAMS_N*PARAMS_NBAR);
    if (frodo_mul_add_as_plus_e(B, S, E, pk) == 0) {
        goto cleanup;
    }

    // Encode the second part of the public key
    frodo_pack(pk_b, CRYPTO_PUBLICKEYBYTES - BYTES_SEED_A, B, PARAMS_N*PARAMS_NBAR, PARAMS_LOGQ);

    // Add s, pk and S to the secret key
    memcpy(sk_s, randomness_s, CRYPTO_BYTES);
    memcpy(sk_pk, pk, CRYPTO_PUBLICKEYBYTES);
    for (size_t i = 0; i < PARAMS_N * PARAMS_NBAR; i++) {
        S[i] = UINT16_TO_LE(S[i]);
    }
    memcpy(sk_S, S, 2*PARAMS_N*PARAMS_NBAR);

    // Add H(pk) to the secret key
    shake(sk_pkh, BYTES_PKHASH, pk, CRYPTO_PUBLICKEYBYTES);
    ret = 0;

    // Cleanup:
cleanup:
    if (S != NULL) {
        clear_bytes((uint8_t *)S, (size_t)2 * PARAMS_N * PARAMS_NBAR * sizeof(uint16_t));
    }
    if (randomness != NULL) {
        clear_bytes(randomness, (size_t)CRYPTO_BYTES + BYTES_SEED_SE);
    }
    if (shake_input_seedSE != NULL) {
        clear_bytes(shake_input_seedSE, (size_t)1 + BYTES_SEED_SE);
    }
    free(B);
    free(S);
    free(randomness);
    free(shake_input_seedSE);
#ifdef DO_VALGRIND_CHECK
    if (randomness != NULL) {
        VALGRIND_MAKE_MEM_DEFINED(randomness, CRYPTO_BYTES + BYTES_SEED_SE + BYTES_SEED_A);
    }
#endif
    return ret;
}


int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk)
{ // FrodoKEM's key encapsulation
  // Input:   public key pk = pk_seedA||pk_b      (BYTES_SEED_A + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 bytes)
  // Outputs: ciphertext ct = ct_c1||ct_c2||salt  (               (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 + (PARAMS_LOGQ*PARAMS_NBAR*PARAMS_NBAR)/8 + BYTES_SALT bytes)
  //          shared key ss                       (CRYPTO_BYTES bytes)
    const uint8_t *pk_seedA = &pk[0];
    const uint8_t *pk_b = &pk[BYTES_SEED_A];
    uint8_t *ct_c1 = &ct[0];
    uint8_t *ct_c2 = &ct[(PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8];
    uint16_t *B = calloc((size_t)PARAMS_N * PARAMS_NBAR, sizeof(uint16_t));
    uint16_t *V = calloc((size_t)PARAMS_NBAR * PARAMS_NBAR, sizeof(uint16_t));  // contains secret data
    uint16_t *C = calloc((size_t)PARAMS_NBAR * PARAMS_NBAR, sizeof(uint16_t));
    uint16_t *Bp = calloc((size_t)PARAMS_N * PARAMS_NBAR, sizeof(uint16_t));
    uint16_t *Sp = calloc((size_t)(2 * PARAMS_N + PARAMS_NBAR) * PARAMS_NBAR, sizeof(uint16_t));  // contains secret data
    uint8_t *G2in = calloc((size_t)BYTES_PKHASH + BYTES_MU + BYTES_SALT, sizeof(uint8_t));  // contains secret data via mu
    uint8_t *G2out = calloc((size_t)BYTES_SEED_SE + CRYPTO_BYTES, sizeof(uint8_t));  // contains secret data
    uint8_t *Fin = calloc((size_t)CRYPTO_CIPHERTEXTBYTES + CRYPTO_BYTES, sizeof(uint8_t));  // contains secret data via Fin_k
    uint8_t *shake_input_seedSE = calloc((size_t)1 + BYTES_SEED_SE, sizeof(uint8_t));  // contains secret data
    uint16_t *Ep = NULL;                                             // contains secret data
    uint16_t *Epp = NULL;                                            // contains secret data
    uint8_t *pkh = NULL;
    uint8_t *mu = NULL;                                              // contains secret data
    uint8_t *salt = NULL;
    uint8_t *seedSE = NULL;                                          // contains secret data
    uint8_t *k = NULL;                                               // contains secret data
    uint8_t *Fin_ct = NULL;
    uint8_t *Fin_k = NULL;                                           // contains secret data
    int ret = 1;

    if (B == NULL || V == NULL || C == NULL || Bp == NULL || Sp == NULL ||
        G2in == NULL || G2out == NULL || Fin == NULL || shake_input_seedSE == NULL) {
        goto cleanup;
    }
    Ep = &Sp[PARAMS_N*PARAMS_NBAR];
    Epp = &Sp[2*PARAMS_N*PARAMS_NBAR];
    pkh = &G2in[0];
    mu = &G2in[BYTES_PKHASH];
    salt = &G2in[BYTES_PKHASH + BYTES_MU];
    seedSE = &G2out[0];
    k = &G2out[BYTES_SEED_SE];
    Fin_ct = &Fin[0];
    Fin_k = &Fin[CRYPTO_CIPHERTEXTBYTES];

    // pkh <- G_1(pk), generate random mu and salt, compute (seedSE || k) = G_2(pkh || mu || salt)
    shake(pkh, BYTES_PKHASH, pk, CRYPTO_PUBLICKEYBYTES);
    if (randombytes(mu, BYTES_MU + BYTES_SALT) != 0)
        goto cleanup;
#ifdef DO_VALGRIND_CHECK
    VALGRIND_MAKE_MEM_UNDEFINED(mu, BYTES_MU + BYTES_SALT);
    VALGRIND_MAKE_MEM_UNDEFINED(pk, CRYPTO_PUBLICKEYBYTES);
#endif
    shake(G2out, BYTES_SEED_SE + CRYPTO_BYTES, G2in, BYTES_PKHASH + BYTES_MU + BYTES_SALT);

    // Generate Sp and Ep, and compute Bp = Sp*A + Ep. Generate A on-the-fly
    shake_input_seedSE[0] = 0x96;
    memcpy(&shake_input_seedSE[1], seedSE, BYTES_SEED_SE);
    shake((uint8_t*)Sp, (2*PARAMS_N+PARAMS_NBAR)*PARAMS_NBAR*sizeof(uint16_t), shake_input_seedSE, 1 + BYTES_SEED_SE);
    for (size_t i = 0; i < (size_t)(2 * PARAMS_N + PARAMS_NBAR) * PARAMS_NBAR; i++) {
        Sp[i] = LE_TO_UINT16(Sp[i]);
    }
    frodo_sample_n(Sp, PARAMS_N*PARAMS_NBAR);
    frodo_sample_n(Ep, PARAMS_N*PARAMS_NBAR);
    if (frodo_mul_add_sa_plus_e(Bp, Sp, Ep, pk_seedA) == 0) {
        goto cleanup;
    }
    frodo_pack(ct_c1, (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8, Bp, PARAMS_N*PARAMS_NBAR, PARAMS_LOGQ);

    // Generate Epp, and compute V = Sp*B + Epp
    frodo_sample_n(Epp, PARAMS_NBAR*PARAMS_NBAR);
    frodo_unpack(B, PARAMS_N*PARAMS_NBAR, pk_b, CRYPTO_PUBLICKEYBYTES - BYTES_SEED_A, PARAMS_LOGQ);
    frodo_mul_add_sb_plus_e(V, B, Sp, Epp);

    // Encode mu, and compute C = V + enc(mu) (mod q)
    frodo_key_encode(C, (uint16_t*)mu);
    frodo_add(C, V, C);
    frodo_pack(ct_c2, (PARAMS_LOGQ*PARAMS_NBAR*PARAMS_NBAR)/8, C, PARAMS_NBAR*PARAMS_NBAR, PARAMS_LOGQ);

    // Append salt to ct and compute ss = F(ct_c1||ct_c2||salt||k)
    memcpy(&ct[CRYPTO_CIPHERTEXTBYTES - BYTES_SALT], salt, BYTES_SALT);
    memcpy(Fin_ct, ct, CRYPTO_CIPHERTEXTBYTES);
    memcpy(Fin_k, k, CRYPTO_BYTES);
    shake(ss, CRYPTO_BYTES, Fin, CRYPTO_CIPHERTEXTBYTES + CRYPTO_BYTES);
    ret = 0;

    // Cleanup:
cleanup:
    if (V != NULL) {
        clear_bytes((uint8_t *)V, (size_t)PARAMS_NBAR * PARAMS_NBAR * sizeof(uint16_t));
    }
    if (Sp != NULL) {
        clear_bytes((uint8_t *)Sp, (size_t)(2 * PARAMS_N + PARAMS_NBAR) * PARAMS_NBAR * sizeof(uint16_t));
    }
    if (G2in != NULL) {
        clear_bytes(G2in + BYTES_PKHASH, (size_t)BYTES_MU);
    }
    if (G2out != NULL) {
        clear_bytes(G2out, (size_t)BYTES_SEED_SE + CRYPTO_BYTES);
    }
    if (Fin_k != NULL) {
        clear_bytes(Fin_k, CRYPTO_BYTES);
    }
    if (shake_input_seedSE != NULL) {
        clear_bytes(shake_input_seedSE, (size_t)1 + BYTES_SEED_SE);
    }
    free(B);
    free(V);
    free(C);
    free(Bp);
    free(Sp);
    free(G2in);
    free(G2out);
    free(Fin);
    free(shake_input_seedSE);
#ifdef DO_VALGRIND_CHECK
    if (mu != NULL) {
        VALGRIND_MAKE_MEM_DEFINED(mu, BYTES_MU);
    }
    VALGRIND_MAKE_MEM_DEFINED(pk, CRYPTO_PUBLICKEYBYTES);
#endif
    return ret;
}


int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk)
{ // FrodoKEM's key decapsulation
  // Inputs: ciphertext ct = ct_c1||ct_c2||salt                  (                              (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 + (PARAMS_LOGQ*PARAMS_NBAR*PARAMS_NBAR)/8 + BYTES_SALT bytes)
  //         secret key sk = sk_s||pk_seedA||pk_b||sk_S||sk_pkh  (CRYPTO_BYTES + BYTES_SEED_A + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 + 2*PARAMS_N*PARAMS_NBAR + BYTES_PKHASH bytes)
  // Output: shared key ss                                       (CRYPTO_BYTES bytes)
    uint16_t *B = calloc((size_t)PARAMS_N * PARAMS_NBAR, sizeof(uint16_t));
    uint16_t *Bp = calloc((size_t)PARAMS_N * PARAMS_NBAR, sizeof(uint16_t));
    uint16_t *W = calloc((size_t)PARAMS_NBAR * PARAMS_NBAR, sizeof(uint16_t));  // contains secret data
    uint16_t *C = calloc((size_t)PARAMS_NBAR * PARAMS_NBAR, sizeof(uint16_t));
    uint16_t *CC = calloc((size_t)PARAMS_NBAR * PARAMS_NBAR, sizeof(uint16_t));
    uint16_t *BBp = calloc((size_t)PARAMS_N * PARAMS_NBAR, sizeof(uint16_t));
    uint16_t *Sp = calloc((size_t)(2 * PARAMS_N + PARAMS_NBAR) * PARAMS_NBAR, sizeof(uint16_t));  // contains secret data
    uint16_t *Ep = NULL;                                             // contains secret data
    uint16_t *Epp = NULL;                                            // contains secret data
    const uint8_t *ct_c1 = &ct[0];
    const uint8_t *ct_c2 = &ct[(PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8];
    const uint8_t *salt = &ct[CRYPTO_CIPHERTEXTBYTES - BYTES_SALT];
    const uint8_t *sk_s = &sk[0];
    const uint8_t *sk_pk = &sk[CRYPTO_BYTES];
    const uint16_t *sk_S = (uint16_t *) &sk[CRYPTO_BYTES + CRYPTO_PUBLICKEYBYTES];
    uint16_t *S = calloc((size_t)PARAMS_N * PARAMS_NBAR, sizeof(uint16_t));  // contains secret data
    const uint8_t *sk_pkh = &sk[CRYPTO_BYTES + CRYPTO_PUBLICKEYBYTES + 2*PARAMS_N*PARAMS_NBAR];
    const uint8_t *pk_seedA = &sk_pk[0];
    const uint8_t *pk_b = &sk_pk[BYTES_SEED_A];
    uint8_t *G2in = calloc((size_t)BYTES_PKHASH + BYTES_MU + BYTES_SALT, sizeof(uint8_t));  // contains secret data via muprime
    uint8_t *pkh = NULL;
    uint8_t *muprime = NULL;                                          // contains secret data
    uint8_t *G2in_salt = NULL;
    uint8_t *G2out = calloc((size_t)BYTES_SEED_SE + CRYPTO_BYTES, sizeof(uint8_t));  // contains secret data
    uint8_t *seedSEprime = NULL;                                      // contains secret data
    uint8_t *kprime = NULL;                                           // contains secret data
    uint8_t *Fin = calloc((size_t)CRYPTO_CIPHERTEXTBYTES + CRYPTO_BYTES, sizeof(uint8_t));  // contains secret data via Fin_k
    uint8_t *Fin_ct = NULL;
    uint8_t *Fin_k = NULL;                                            // contains secret data
    uint8_t *shake_input_seedSEprime = calloc((size_t)1 + BYTES_SEED_SE, sizeof(uint8_t));  // contains secret data
    int ret = 1;

    if (B == NULL || Bp == NULL || W == NULL || C == NULL || CC == NULL ||
        BBp == NULL || Sp == NULL || S == NULL || G2in == NULL || G2out == NULL ||
        Fin == NULL || shake_input_seedSEprime == NULL) {
        goto cleanup;
    }
    Ep = &Sp[PARAMS_N*PARAMS_NBAR];
    Epp = &Sp[2*PARAMS_N*PARAMS_NBAR];
    pkh = &G2in[0];
    muprime = &G2in[BYTES_PKHASH];
    G2in_salt = &G2in[BYTES_PKHASH + BYTES_MU];
    seedSEprime = &G2out[0];
    kprime = &G2out[BYTES_SEED_SE];
    Fin_ct = &Fin[0];
    Fin_k = &Fin[CRYPTO_CIPHERTEXTBYTES];

#ifdef DO_VALGRIND_CHECK
    VALGRIND_MAKE_MEM_UNDEFINED(sk, CRYPTO_SECRETKEYBYTES);
    VALGRIND_MAKE_MEM_UNDEFINED(ct, CRYPTO_CIPHERTEXTBYTES);
#endif

    for (size_t i = 0; i < PARAMS_N * PARAMS_NBAR; i++) {
        S[i] = LE_TO_UINT16(sk_S[i]);
    }

    // Compute W = C - Bp*S (mod q), and decode the randomness mu
    frodo_unpack(Bp, PARAMS_N*PARAMS_NBAR, ct_c1, (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8, PARAMS_LOGQ);
    frodo_unpack(C, PARAMS_NBAR*PARAMS_NBAR, ct_c2, (PARAMS_LOGQ*PARAMS_NBAR*PARAMS_NBAR)/8, PARAMS_LOGQ);
    frodo_mul_bs(W, Bp, S);
    frodo_sub(W, C, W);
    frodo_key_decode((uint16_t*)muprime, W);

    // Generate (seedSE' || k') = G_2(pkh || mu' || salt)
    memcpy(pkh, sk_pkh, BYTES_PKHASH);
    memcpy(G2in_salt, salt, BYTES_SALT);
    shake(G2out, BYTES_SEED_SE + CRYPTO_BYTES, G2in, BYTES_PKHASH + BYTES_MU + BYTES_SALT);

    // Generate Sp and Ep, and compute BBp = Sp*A + Ep. Generate A on-the-fly
    shake_input_seedSEprime[0] = 0x96;
    memcpy(&shake_input_seedSEprime[1], seedSEprime, BYTES_SEED_SE);
    shake((uint8_t*)Sp, (2*PARAMS_N+PARAMS_NBAR)*PARAMS_NBAR*sizeof(uint16_t), shake_input_seedSEprime, 1 + BYTES_SEED_SE);
    for (size_t i = 0; i < (size_t)(2*PARAMS_N+PARAMS_NBAR)*PARAMS_NBAR; i++) {
        Sp[i] = LE_TO_UINT16(Sp[i]);
    }
    frodo_sample_n(Sp, PARAMS_N*PARAMS_NBAR);
    frodo_sample_n(Ep, PARAMS_N*PARAMS_NBAR);
    if (frodo_mul_add_sa_plus_e(BBp, Sp, Ep, pk_seedA) == 0) {
        goto cleanup;
    }

    // Generate Epp, and compute W = Sp*B + Epp
    frodo_sample_n(Epp, PARAMS_NBAR*PARAMS_NBAR);
    frodo_unpack(B, PARAMS_N*PARAMS_NBAR, pk_b, CRYPTO_PUBLICKEYBYTES - BYTES_SEED_A, PARAMS_LOGQ);
    frodo_mul_add_sb_plus_e(W, B, Sp, Epp);

    // Encode mu, and compute CC = W + enc(mu') (mod q)
    frodo_key_encode(CC, (uint16_t*)muprime);
    frodo_add(CC, W, CC);

    // Prepare input to F
    memcpy(Fin_ct, ct, CRYPTO_CIPHERTEXTBYTES);

    // Reducing BBp modulo q
    for (int i = 0; i < PARAMS_N*PARAMS_NBAR; i++) BBp[i] = BBp[i] & ((1 << PARAMS_LOGQ)-1);

    // If (Bp == BBp & C == CC) then ss = F(ct || k'), else ss = F(ct || s)
    // Needs to avoid branching on secret data using constant-time implementation.
    int8_t selector = ct_verify(Bp, BBp, PARAMS_N*PARAMS_NBAR) | ct_verify(C, CC, PARAMS_NBAR*PARAMS_NBAR);
    // If (selector == 0) then load k' to do ss = F(ct || k'), else if (selector == -1) load s to do ss = F(ct || s)
    ct_select((uint8_t*)Fin_k, (uint8_t*)kprime, (uint8_t*)sk_s, CRYPTO_BYTES, selector);
    shake(ss, CRYPTO_BYTES, Fin, CRYPTO_CIPHERTEXTBYTES + CRYPTO_BYTES);
    ret = 0;

    // Cleanup:
cleanup:
    if (W != NULL) {
        clear_bytes((uint8_t *)W, (size_t)PARAMS_NBAR * PARAMS_NBAR * sizeof(uint16_t));
    }
    if (Sp != NULL) {
        clear_bytes((uint8_t *)Sp, (size_t)(2 * PARAMS_N + PARAMS_NBAR) * PARAMS_NBAR * sizeof(uint16_t));
    }
    if (S != NULL) {
        clear_bytes((uint8_t *)S, (size_t)PARAMS_N * PARAMS_NBAR * sizeof(uint16_t));
    }
    if (muprime != NULL) {
        clear_bytes(muprime, BYTES_MU);
    }
    if (G2out != NULL) {
        clear_bytes(G2out, (size_t)BYTES_SEED_SE + CRYPTO_BYTES);
    }
    if (Fin_k != NULL) {
        clear_bytes(Fin_k, CRYPTO_BYTES);
    }
    if (shake_input_seedSEprime != NULL) {
        clear_bytes(shake_input_seedSEprime, (size_t)1 + BYTES_SEED_SE);
    }
    free(B);
    free(Bp);
    free(W);
    free(C);
    free(CC);
    free(BBp);
    free(Sp);
    free(S);
    free(G2in);
    free(G2out);
    free(Fin);
    free(shake_input_seedSEprime);
#ifdef DO_VALGRIND_CHECK
    VALGRIND_MAKE_MEM_DEFINED(sk, CRYPTO_SECRETKEYBYTES);
    VALGRIND_MAKE_MEM_DEFINED(ct, CRYPTO_CIPHERTEXTBYTES);
#endif
    return ret;
}
