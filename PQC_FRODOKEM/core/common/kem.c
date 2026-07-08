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


// Expand seedSE into the re-encryption secret Sp (N x N_BAR), the error Ep' (N_BAR x N) and
// the error Epp' (N_BAR x N_BAR), sampled. The three share one SHAKE stream (domain byte 0x96),
// squeezed incrementally so Ep' can be written straight into the caller's Bp/BBp buffer
// (ep_out) — no standalone Ep matrix is ever held. Byte-for-byte identical to the one-shot
// expansion it replaces.
static void frodo_gen_sp_ep_epp(uint16_t *Sp, uint16_t *ep_out, uint16_t *Epp, const uint8_t *seedSE)
{
    uint8_t shake_input[1 + BYTES_SEED_SE];
    uint64_t st[FRODO_SHA3_STATE_U64];
    uint8_t alg = frodokem_shake_alg();

    shake_input[0] = 0x96;
    memcpy(&shake_input[1], seedSE, BYTES_SEED_SE);
    OP_hash_init(alg, st, (int)sizeof(st));
    OP_hash_absorb(alg, st, (int)sizeof(st), shake_input, 1 + BYTES_SEED_SE);

    OP_hash_squeeze(alg, st, (int)sizeof(st), (uint8_t *)Sp, (int)(PARAMS_N * PARAMS_NBAR * sizeof(uint16_t)));
    for (size_t i = 0; i < (size_t)PARAMS_N * PARAMS_NBAR; i++) Sp[i] = LE_TO_UINT16(Sp[i]);
    frodo_sample_n(Sp, PARAMS_N * PARAMS_NBAR);

    OP_hash_squeeze(alg, st, (int)sizeof(st), (uint8_t *)ep_out, (int)(PARAMS_N * PARAMS_NBAR * sizeof(uint16_t)));
    for (size_t i = 0; i < (size_t)PARAMS_N * PARAMS_NBAR; i++) ep_out[i] = LE_TO_UINT16(ep_out[i]);
    frodo_sample_n(ep_out, PARAMS_N * PARAMS_NBAR);

    OP_hash_squeeze(alg, st, (int)sizeof(st), (uint8_t *)Epp, (int)(PARAMS_NBAR * PARAMS_NBAR * sizeof(uint16_t)));
    for (size_t i = 0; i < (size_t)PARAMS_NBAR * PARAMS_NBAR; i++) Epp[i] = LE_TO_UINT16(Epp[i]);
    frodo_sample_n(Epp, PARAMS_NBAR * PARAMS_NBAR);

    clear_bytes(shake_input, sizeof(shake_input));
    clear_bytes((uint8_t *)st, sizeof(st));
}


int crypto_kem_keypair_impl(unsigned char* pk, unsigned char* sk)
{ // FrodoKEM's key generation
  // Outputs: public key pk = pk_seedA||pk_b                      (               BYTES_SEED_A + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 bytes)
  //          secret key sk = sk_s||pk_seedA||pk_b||sk_S||sk_pkh  (CRYPTO_BYTES + BYTES_SEED_A + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 + 2*PARAMS_N*PARAMS_NBAR + BYTES_PKHASH bytes)
    uint8_t *pk_seedA = &pk[0];
    uint8_t *pk_b = &pk[BYTES_SEED_A];
    uint8_t *sk_s = &sk[0];
    uint8_t *sk_pk = &sk[CRYPTO_BYTES];
    uint8_t *sk_S = &sk[CRYPTO_BYTES + CRYPTO_PUBLICKEYBYTES];
    uint8_t *sk_pkh = &sk[CRYPTO_BYTES + CRYPTO_PUBLICKEYBYTES + 2*PARAMS_N*PARAMS_NBAR];
    uint16_t *S = calloc((size_t)2 * PARAMS_N * PARAMS_NBAR, sizeof(uint16_t));  // contains secret data
    uint8_t *randomness = calloc((size_t)CRYPTO_BYTES + BYTES_SEED_SE + BYTES_SEED_A, sizeof(uint8_t));  // contains secret data via randomness_s and randomness_seedSE
    uint8_t *shake_input_seedSE = calloc((size_t)1 + BYTES_SEED_SE, sizeof(uint8_t));  // contains secret data
    uint16_t *B = NULL;                                                // aliases the E region (E is discarded after B = A*S + E)
    uint16_t *E = NULL;                                                // contains secret data
    uint8_t *randomness_s = NULL;                                      // contains secret data
    uint8_t *randomness_seedSE = NULL;                                 // contains secret data
    uint8_t *randomness_z = NULL;
    int ret = 1;

    if (S == NULL || randomness == NULL || shake_input_seedSE == NULL) {
        goto cleanup;
    }
    E = &S[PARAMS_N*PARAMS_NBAR];
    B = E;  // low-memory: reuse E's storage for B = A*S + E (E not needed afterwards)
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
    free(S);  // also frees B (aliases the E region inside S)
    free(randomness);
    free(shake_input_seedSE);
#ifdef DO_VALGRIND_CHECK
    if (randomness != NULL) {
        VALGRIND_MAKE_MEM_DEFINED(randomness, CRYPTO_BYTES + BYTES_SEED_SE + BYTES_SEED_A);
    }
#endif
    return ret;
}


int crypto_kem_enc_impl(unsigned char *ct, unsigned char *ss, const unsigned char *pk)
{ // FrodoKEM's key encapsulation
  // Input:   public key pk = pk_seedA||pk_b      (BYTES_SEED_A + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 bytes)
  // Outputs: ciphertext ct = ct_c1||ct_c2||salt  (               (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 + (PARAMS_LOGQ*PARAMS_NBAR*PARAMS_NBAR)/8 + BYTES_SALT bytes)
  //          shared key ss                       (CRYPTO_BYTES bytes)
    const uint8_t *pk_seedA = &pk[0];
    const uint8_t *pk_b = &pk[BYTES_SEED_A];
    uint8_t *ct_c1 = &ct[0];
    uint8_t *ct_c2 = &ct[(PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8];
    uint16_t *V = calloc((size_t)PARAMS_NBAR * PARAMS_NBAR, sizeof(uint16_t));  // contains secret data
    uint16_t *C = calloc((size_t)PARAMS_NBAR * PARAMS_NBAR, sizeof(uint16_t));
    uint16_t *Bp = calloc((size_t)PARAMS_N * PARAMS_NBAR, sizeof(uint16_t));  // holds Ep', then Sp*A + Ep'
    uint16_t *Sp = calloc((size_t)(PARAMS_N + PARAMS_NBAR) * PARAMS_NBAR, sizeof(uint16_t));  // Sp | Epp; contains secret data
    uint8_t *G2in = calloc((size_t)BYTES_PKHASH + BYTES_MU + BYTES_SALT, sizeof(uint8_t));  // contains secret data via mu
    uint8_t *G2out = calloc((size_t)BYTES_SEED_SE + CRYPTO_BYTES, sizeof(uint8_t));  // contains secret data
    uint16_t *Epp = NULL;                                           // contains secret data
    uint8_t *pkh = NULL;
    uint8_t *mu = NULL;                                              // contains secret data
    uint8_t *salt = NULL;
    uint8_t *seedSE = NULL;                                          // contains secret data
    uint8_t *k = NULL;                                               // contains secret data
    int ret = 1;

    if (V == NULL || C == NULL || Bp == NULL || Sp == NULL ||
        G2in == NULL || G2out == NULL) {
        goto cleanup;
    }
    Epp = &Sp[PARAMS_N*PARAMS_NBAR];
    pkh = &G2in[0];
    mu = &G2in[BYTES_PKHASH];
    salt = &G2in[BYTES_PKHASH + BYTES_MU];
    seedSE = &G2out[0];
    k = &G2out[BYTES_SEED_SE];

    // pkh <- G_1(pk), generate random mu and salt, compute (seedSE || k) = G_2(pkh || mu || salt)
    shake(pkh, BYTES_PKHASH, pk, CRYPTO_PUBLICKEYBYTES);
    if (randombytes(mu, BYTES_MU + BYTES_SALT) != 0)
        goto cleanup;
#ifdef DO_VALGRIND_CHECK
    VALGRIND_MAKE_MEM_UNDEFINED(mu, BYTES_MU + BYTES_SALT);
    VALGRIND_MAKE_MEM_UNDEFINED(pk, CRYPTO_PUBLICKEYBYTES);
#endif
    shake(G2out, BYTES_SEED_SE + CRYPTO_BYTES, G2in, BYTES_PKHASH + BYTES_MU + BYTES_SALT);

    // Generate Sp, Ep' (written into Bp) and Epp, then compute Bp = Sp*A + Ep'. Generate A on-the-fly
    frodo_gen_sp_ep_epp(Sp, Bp, Epp, seedSE);
    if (frodo_mul_add_sa_plus_e(Bp, Sp, pk_seedA) == 0) {
        goto cleanup;
    }
    frodo_pack(ct_c1, (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8, Bp, PARAMS_N*PARAMS_NBAR, PARAMS_LOGQ);

    // Compute V = Sp*B + Epp (B unpacked on the fly from pk_b)
    frodo_mul_add_sb_plus_e(V, pk_b, Sp, Epp);

    // Encode mu, and compute C = V + enc(mu) (mod q)
    frodo_key_encode(C, (uint16_t*)mu);
    frodo_add(C, V, C);
    frodo_pack(ct_c2, (PARAMS_LOGQ*PARAMS_NBAR*PARAMS_NBAR)/8, C, PARAMS_NBAR*PARAMS_NBAR, PARAMS_LOGQ);

    // Append salt to ct and compute ss = F(ct_c1||ct_c2||salt||k) by streaming ct then k
    // through the incremental hash operator, avoiding a full ct||k copy buffer.
    memcpy(&ct[CRYPTO_CIPHERTEXTBYTES - BYTES_SALT], salt, BYTES_SALT);
    {
        uint64_t f_state[FRODO_SHA3_STATE_U64];
        uint8_t alg = frodokem_shake_alg();
        OP_hash_init(alg, f_state, (int)sizeof(f_state));
        OP_hash_absorb(alg, f_state, (int)sizeof(f_state), ct, (int)CRYPTO_CIPHERTEXTBYTES);
        OP_hash_absorb(alg, f_state, (int)sizeof(f_state), k, (int)CRYPTO_BYTES);
        OP_hash_squeeze(alg, f_state, (int)sizeof(f_state), ss, (int)CRYPTO_BYTES);
    }
    ret = 0;

    // Cleanup:
cleanup:
    if (V != NULL) {
        clear_bytes((uint8_t *)V, (size_t)PARAMS_NBAR * PARAMS_NBAR * sizeof(uint16_t));
    }
    if (Bp != NULL) {  // held Ep' (secret) then Bp
        clear_bytes((uint8_t *)Bp, (size_t)PARAMS_N * PARAMS_NBAR * sizeof(uint16_t));
    }
    if (Sp != NULL) {  // Sp | Epp
        clear_bytes((uint8_t *)Sp, (size_t)(PARAMS_N + PARAMS_NBAR) * PARAMS_NBAR * sizeof(uint16_t));
    }
    if (G2in != NULL) {
        clear_bytes(G2in + BYTES_PKHASH, (size_t)BYTES_MU);
    }
    if (G2out != NULL) {
        clear_bytes(G2out, (size_t)BYTES_SEED_SE + CRYPTO_BYTES);
    }
    free(V);
    free(C);
    free(Bp);
    free(Sp);
    free(G2in);
    free(G2out);
#ifdef DO_VALGRIND_CHECK
    if (mu != NULL) {
        VALGRIND_MAKE_MEM_DEFINED(mu, BYTES_MU);
    }
    VALGRIND_MAKE_MEM_DEFINED(pk, CRYPTO_PUBLICKEYBYTES);
#endif
    return ret;
}


int crypto_kem_dec_impl(unsigned char *ss, const unsigned char *ct, const unsigned char *sk)
{ // FrodoKEM's key decapsulation
  // Inputs: ciphertext ct = ct_c1||ct_c2||salt                  (                              (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 + (PARAMS_LOGQ*PARAMS_NBAR*PARAMS_NBAR)/8 + BYTES_SALT bytes)
  //         secret key sk = sk_s||pk_seedA||pk_b||sk_S||sk_pkh  (CRYPTO_BYTES + BYTES_SEED_A + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 + 2*PARAMS_N*PARAMS_NBAR + BYTES_PKHASH bytes)
  // Output: shared key ss                                       (CRYPTO_BYTES bytes)
    uint16_t *Bp = calloc((size_t)PARAMS_N * PARAMS_NBAR, sizeof(uint16_t));  // holds received Bp, then reused for recomputed BBp
    uint16_t *W = calloc((size_t)PARAMS_NBAR * PARAMS_NBAR, sizeof(uint16_t));  // contains secret data
    uint16_t *C = calloc((size_t)PARAMS_NBAR * PARAMS_NBAR, sizeof(uint16_t));
    uint16_t *CC = calloc((size_t)PARAMS_NBAR * PARAMS_NBAR, sizeof(uint16_t));
    uint16_t *Sp = calloc((size_t)(PARAMS_N + PARAMS_NBAR) * PARAMS_NBAR, sizeof(uint16_t));  // Sp | Epp; contains secret data
    uint16_t *S = NULL;                                              // low-memory: reuses the Sp buffer (used before Sp is sampled)
    uint16_t *Epp = NULL;                                            // contains secret data
    const uint8_t *ct_c1 = &ct[0];
    const uint8_t *ct_c2 = &ct[(PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8];
    const uint8_t *salt = &ct[CRYPTO_CIPHERTEXTBYTES - BYTES_SALT];
    const uint8_t *sk_s = &sk[0];
    const uint8_t *sk_pk = &sk[CRYPTO_BYTES];
    const uint16_t *sk_S = (uint16_t *) &sk[CRYPTO_BYTES + CRYPTO_PUBLICKEYBYTES];
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
    uint8_t fin_k[FRODOKEM_MAX_SHARED_SECRET_BYTES];                  // selected k for F, contains secret data
    int ret = 1;

    if (Bp == NULL || W == NULL || C == NULL || CC == NULL ||
        Sp == NULL || G2in == NULL || G2out == NULL) {
        goto cleanup;
    }
    Epp = &Sp[PARAMS_N*PARAMS_NBAR];
    // The secret S (for W = C - Bp*S) is used only before Sp is expanded, so it reuses the
    // Sp buffer. The public B is unpacked on the fly from pk_b, so it needs no buffer.
    S = Sp;
    pkh = &G2in[0];
    muprime = &G2in[BYTES_PKHASH];
    G2in_salt = &G2in[BYTES_PKHASH + BYTES_MU];
    seedSEprime = &G2out[0];
    kprime = &G2out[BYTES_SEED_SE];

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

    // Generate Sp, Ep' (written into Bp) and Epp, then compute BBp = Sp*A + Ep'. The received Bp
    // is no longer needed unpacked (it is compared in packed form below), so its buffer holds Ep'
    // and then the recomputed BBp. Generate A on-the-fly.
    frodo_gen_sp_ep_epp(Sp, Bp, Epp, seedSEprime);
    if (frodo_mul_add_sa_plus_e(Bp, Sp, pk_seedA) == 0) {
        goto cleanup;
    }

    // Compute W = Sp*B + Epp (B unpacked on the fly from pk_b)
    frodo_mul_add_sb_plus_e(W, pk_b, Sp, Epp);

    // Encode mu, and compute CC = W + enc(mu') (mod q)
    frodo_key_encode(CC, (uint16_t*)muprime);
    frodo_add(CC, W, CC);

    // Reducing recomputed BBp (stored in Bp) modulo q
    for (int i = 0; i < PARAMS_N*PARAMS_NBAR; i++) Bp[i] = Bp[i] & ((1 << PARAMS_LOGQ)-1);

    // If (Bp == BBp & C == CC) then ss = F(ct || k'), else ss = F(ct || s)
    // Compare the recomputed BBp/CC against the received ciphertext directly in packed form
    // (constant time), avoiding a second unpacked copy of the received Bp/C.
    int8_t selector = ct_verify_packed(Bp, ct_c1, PARAMS_N*PARAMS_NBAR, PARAMS_LOGQ)
                    | ct_verify_packed(CC, ct_c2, PARAMS_NBAR*PARAMS_NBAR, PARAMS_LOGQ);
    // If (selector == 0) then load k' to do ss = F(ct || k'), else if (selector == -1) load s to do ss = F(ct || s)
    ct_select(fin_k, (uint8_t*)kprime, (uint8_t*)sk_s, CRYPTO_BYTES, selector);
    // ss = F(ct || k): stream ct then the selected k through the incremental hash operator.
    {
        uint64_t f_state[FRODO_SHA3_STATE_U64];
        uint8_t alg = frodokem_shake_alg();
        OP_hash_init(alg, f_state, (int)sizeof(f_state));
        OP_hash_absorb(alg, f_state, (int)sizeof(f_state), ct, (int)CRYPTO_CIPHERTEXTBYTES);
        OP_hash_absorb(alg, f_state, (int)sizeof(f_state), fin_k, (int)CRYPTO_BYTES);
        OP_hash_squeeze(alg, f_state, (int)sizeof(f_state), ss, (int)CRYPTO_BYTES);
    }
    ret = 0;

    // Cleanup:
cleanup:
    if (W != NULL) {
        clear_bytes((uint8_t *)W, (size_t)PARAMS_NBAR * PARAMS_NBAR * sizeof(uint16_t));
    }
    if (Sp != NULL) {  // Sp | Epp; also clears S (reuses the Sp buffer)
        clear_bytes((uint8_t *)Sp, (size_t)(PARAMS_N + PARAMS_NBAR) * PARAMS_NBAR * sizeof(uint16_t));
    }
    if (muprime != NULL) {
        clear_bytes(muprime, BYTES_MU);
    }
    if (G2out != NULL) {
        clear_bytes(G2out, (size_t)BYTES_SEED_SE + CRYPTO_BYTES);
    }
    clear_bytes(fin_k, CRYPTO_BYTES);
    free(Bp);  // held received Bp, then Ep' and the recomputed BBp
    free(W);
    free(C);
    free(CC);
    free(Sp);  // also frees S (reuses the Sp buffer)
    free(G2in);
    free(G2out);
#ifdef DO_VALGRIND_CHECK
    VALGRIND_MAKE_MEM_DEFINED(sk, CRYPTO_SECRETKEYBYTES);
    VALGRIND_MAKE_MEM_DEFINED(ct, CRYPTO_CIPHERTEXTBYTES);
#endif
    return ret;
}
