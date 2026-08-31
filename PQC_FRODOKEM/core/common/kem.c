/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: Key Encapsulation Mechanism (KEM) based on Frodo
*********************************************************************************************/

#include "malloc.h"
#include "memops.h"
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
static int frodo_gen_sp_ep_epp(uint16_t *Sp, uint16_t *ep_out, uint16_t *Epp, const uint8_t *seedSE)
{
    uint8_t shake_input[1 + BYTES_SEED_SE];
    uint64_t *st = malloc(FRODO_SHA3_STATE_U64 * sizeof(*st));
    uint8_t alg = frodokem_shake_alg();

    if (st == NULL) {
        return 0;
    }

    shake_input[0] = 0x96;
    memcpy(&shake_input[1], seedSE, BYTES_SEED_SE);
    OP_hash_init(alg, st, (int)(FRODO_SHA3_STATE_U64 * sizeof(*st)));
    OP_hash_absorb(alg, st, (int)(FRODO_SHA3_STATE_U64 * sizeof(*st)), shake_input, 1 + BYTES_SEED_SE);

    OP_hash_squeeze(alg, st, (int)(FRODO_SHA3_STATE_U64 * sizeof(*st)), (uint8_t *)Sp, (int)(PARAMS_N * PARAMS_NBAR * sizeof(uint16_t)));
    for (size_t i = 0; i < (size_t)PARAMS_N * PARAMS_NBAR; i++) Sp[i] = LE_TO_UINT16(Sp[i]);
    frodo_sample_n(Sp, PARAMS_N * PARAMS_NBAR);

    OP_hash_squeeze(alg, st, (int)(FRODO_SHA3_STATE_U64 * sizeof(*st)), (uint8_t *)ep_out, (int)(PARAMS_N * PARAMS_NBAR * sizeof(uint16_t)));
    for (size_t i = 0; i < (size_t)PARAMS_N * PARAMS_NBAR; i++) ep_out[i] = LE_TO_UINT16(ep_out[i]);
    frodo_sample_n(ep_out, PARAMS_N * PARAMS_NBAR);

    OP_hash_squeeze(alg, st, (int)(FRODO_SHA3_STATE_U64 * sizeof(*st)), (uint8_t *)Epp, (int)(PARAMS_NBAR * PARAMS_NBAR * sizeof(uint16_t)));
    for (size_t i = 0; i < (size_t)PARAMS_NBAR * PARAMS_NBAR; i++) Epp[i] = LE_TO_UINT16(Epp[i]);
    frodo_sample_n(Epp, PARAMS_NBAR * PARAMS_NBAR);

    clear_bytes(shake_input, sizeof(shake_input));
    clear_bytes((uint8_t *)st, FRODO_SHA3_STATE_U64 * sizeof(*st));
    free(st);
    return 1;
}


int crypto_kem_keypair_impl(unsigned char* pk, unsigned char* sk)
{ // FrodoKEM's key generation — low-memory: S generated directly into sk_S, B tiled.
  // Outputs: public key pk = pk_seedA||pk_b                      (               BYTES_SEED_A + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 bytes)
  //          secret key sk = sk_s||pk_seedA||pk_b||sk_S||sk_pkh  (CRYPTO_BYTES + BYTES_SEED_A + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 + 2*PARAMS_N*PARAMS_NBAR + BYTES_PKHASH bytes)
    uint8_t *pk_seedA = &pk[0];
    uint8_t *pk_b = &pk[BYTES_SEED_A];
    uint8_t *sk_s = &sk[0];
    uint8_t *sk_pk = &sk[CRYPTO_BYTES];
    uint8_t *sk_S = &sk[CRYPTO_BYTES + CRYPTO_PUBLICKEYBYTES];
    uint8_t *sk_pkh = &sk[CRYPTO_BYTES + CRYPTO_PUBLICKEYBYTES + 2*PARAMS_N*PARAMS_NBAR];
    uint8_t *randomness = calloc((size_t)CRYPTO_BYTES + BYTES_SEED_SE + BYTES_SEED_A, sizeof(uint8_t));
    uint8_t *shake_input_seedSE = calloc((size_t)1 + BYTES_SEED_SE, sizeof(uint8_t));
    uint8_t *randomness_s = NULL;
    uint8_t *randomness_seedSE = NULL;
    uint8_t *randomness_z = NULL;
    int ret = 1;

    if (randomness == NULL || shake_input_seedSE == NULL) {
        goto cleanup;
    }
    randomness_s = &randomness[0];
    randomness_seedSE = &randomness[CRYPTO_BYTES];
    randomness_z = &randomness[CRYPTO_BYTES + BYTES_SEED_SE];

    // Generate the secret value s, the seed for S and E, and the seed for the seed for A.
    if (randombytes(randomness, CRYPTO_BYTES + BYTES_SEED_SE + BYTES_SEED_A) != 0)
        goto cleanup;
#ifdef DO_VALGRIND_CHECK
    VALGRIND_MAKE_MEM_UNDEFINED(randomness, CRYPTO_BYTES + BYTES_SEED_SE + BYTES_SEED_A);
#endif
    shake(pk_seedA, BYTES_SEED_A, randomness_z, BYTES_SEED_A);

    // --- Generate S directly into sk_S using incremental SHAKE ---
    // The SHAKE stream is: S (N*NBAR uint16) || E (N*NBAR uint16)
    // We squeeze S tile-by-tile (128 bytes = 64 uint16 per tile),
    // byte-swap, sample, convert to LE, and write into sk_S.
    shake_input_seedSE[0] = 0x5F;
    memcpy(&shake_input_seedSE[1], randomness_seedSE, BYTES_SEED_SE);
    {
        uint8_t alg = frodokem_shake_alg();
        uint64_t *st = malloc(FRODO_SHA3_STATE_U64 * sizeof(*st));
        if (st == NULL) {
            goto cleanup;
        }
        OP_hash_init(alg, st, (int)(FRODO_SHA3_STATE_U64 * sizeof(*st)));
        OP_hash_absorb(alg, st, (int)(FRODO_SHA3_STATE_U64 * sizeof(*st)), shake_input_seedSE, 1 + BYTES_SEED_SE);

        // Squeeze S tile-by-tile into sk_S
        uint16_t S_tile[64];  // 64 uint16 = 8 rows × 8 cols max
        const int tile_elems = 64;  // 8 × NBAR = 64 for NBAR=8
        const int num_tiles = (int)(PARAMS_N * PARAMS_NBAR / tile_elems);

        for (int t = 0; t < num_tiles; t++) {
            OP_hash_squeeze(alg, st, (int)(FRODO_SHA3_STATE_U64 * sizeof(*st)),
                            (uint8_t *)S_tile, (int)(tile_elems * sizeof(uint16_t)));
            for (int k = 0; k < tile_elems; k++)
                S_tile[k] = LE_TO_UINT16(S_tile[k]);
            frodo_sample_n(S_tile, (size_t)tile_elems);
            // Convert to LE and store in sk_S
            for (int k = 0; k < tile_elems; k++) {
                ((uint16_t *)sk_S)[t * tile_elems + k] = UINT16_TO_LE(S_tile[k]);
            }
        }

        // Consume the remaining stream as E and temporarily store packed E
        // in pk_b.  This finishes the level-dependent SHAKE128/SHAKE256
        // stream before SHAKE128 is used to generate A rows below.
        if (frodo_pack_e_from_state(pk_b, st, alg) == 0) {
            clear_bytes((uint8_t *)st, FRODO_SHA3_STATE_U64 * sizeof(*st));
            free(st);
            goto cleanup;
        }
        // Note: E is fully consumed.  No hash state is paused across the
        // subsequent A generation, which is required for hardware SHAKE
        // engines with non-exportable state.
        clear_bytes((uint8_t *)st, FRODO_SHA3_STATE_U64 * sizeof(*st));
        free(st);

        if (frodo_mul_add_as_plus_packed_e_from_sk(pk_b, sk_S, pk_seedA) == 0) {
            goto cleanup;
        }
    }

    // Add s, pk and S to the secret key
    memcpy(sk_s, randomness_s, CRYPTO_BYTES);
    memcpy(sk_pk, pk, CRYPTO_PUBLICKEYBYTES);
    // S is already in sk_S (stored tile-by-tile above)

    // Add H(pk) to the secret key
    shake(sk_pkh, BYTES_PKHASH, pk, CRYPTO_PUBLICKEYBYTES);
    ret = 0;

    // Cleanup:
cleanup:
    if (randomness != NULL) {
        clear_bytes(randomness, (size_t)CRYPTO_BYTES + BYTES_SEED_SE);
    }
    if (shake_input_seedSE != NULL) {
        clear_bytes(shake_input_seedSE, (size_t)1 + BYTES_SEED_SE);
    }
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
{ // FrodoKEM's key encapsulation — low-memory: Bp is column-tiled (128 B tile).
  // Input:   public key pk = pk_seedA||pk_b      (BYTES_SEED_A + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 bytes)
  // Outputs: ciphertext ct = ct_c1||ct_c2||salt  (               (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 + (PARAMS_LOGQ*PARAMS_NBAR*PARAMS_NBAR)/8 + BYTES_SALT bytes)
  //          shared key ss                       (CRYPTO_BYTES bytes)
    const uint8_t *pk_seedA = &pk[0];
    const uint8_t *pk_b = &pk[BYTES_SEED_A];
    uint8_t *ct_c1 = &ct[0];
    uint8_t *ct_c2 = &ct[(PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8];
    uint16_t *V = calloc((size_t)PARAMS_NBAR * PARAMS_NBAR, sizeof(uint16_t));  // contains secret data
    uint16_t *C = calloc((size_t)PARAMS_NBAR * PARAMS_NBAR, sizeof(uint16_t));
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

    if (V == NULL || C == NULL || Sp == NULL ||
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

    {
        uint16_t *Bp = calloc((size_t)PARAMS_N * PARAMS_NBAR, sizeof(uint16_t));
        if (Bp == NULL) {
            goto cleanup;
        }
        if (frodo_gen_sp_ep_epp(Sp, Bp, Epp, seedSE) == 0) {
            clear_bytes((uint8_t *)Bp,
                        (size_t)PARAMS_N * PARAMS_NBAR * sizeof(uint16_t));
            free(Bp);
            goto cleanup;
        }
        if (frodo_mul_add_sa_plus_e(Bp, Sp, pk_seedA) == 0) {
            clear_bytes((uint8_t *)Bp,
                        (size_t)PARAMS_N * PARAMS_NBAR * sizeof(uint16_t));
            free(Bp);
            goto cleanup;
        }
        frodo_pack(ct_c1, (PARAMS_LOGQ * PARAMS_N * PARAMS_NBAR) / 8,
                   Bp, PARAMS_N * PARAMS_NBAR, PARAMS_LOGQ);
        clear_bytes((uint8_t *)Bp,
                    (size_t)PARAMS_N * PARAMS_NBAR * sizeof(uint16_t));
        free(Bp);
    }

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
        uint64_t *f_state = malloc(FRODO_SHA3_STATE_U64 * sizeof(*f_state));
        uint8_t alg = frodokem_shake_alg();
        if (f_state == NULL) {
            goto cleanup;
        }
        OP_hash_init(alg, f_state, (int)(FRODO_SHA3_STATE_U64 * sizeof(*f_state)));
        OP_hash_absorb(alg, f_state, (int)(FRODO_SHA3_STATE_U64 * sizeof(*f_state)), ct, (int)CRYPTO_CIPHERTEXTBYTES);
        OP_hash_absorb(alg, f_state, (int)(FRODO_SHA3_STATE_U64 * sizeof(*f_state)), k, (int)CRYPTO_BYTES);
        OP_hash_squeeze(alg, f_state, (int)(FRODO_SHA3_STATE_U64 * sizeof(*f_state)), ss, (int)CRYPTO_BYTES);
        clear_bytes((uint8_t *)f_state, FRODO_SHA3_STATE_U64 * sizeof(*f_state));
        free(f_state);
    }
    ret = 0;

    // Cleanup:
cleanup:
    if (V != NULL) {
        clear_bytes((uint8_t *)V, (size_t)PARAMS_NBAR * PARAMS_NBAR * sizeof(uint16_t));
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
{ // FrodoKEM's key decapsulation — low-memory: Bp and BBp column-tiled.
  // Inputs: ciphertext ct = ct_c1||ct_c2||salt                  (                              (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 + (PARAMS_LOGQ*PARAMS_NBAR*PARAMS_NBAR)/8 + BYTES_SALT bytes)
  //         secret key sk = sk_s||pk_seedA||pk_b||sk_S||sk_pkh  (CRYPTO_BYTES + BYTES_SEED_A + (PARAMS_LOGQ*PARAMS_N*PARAMS_NBAR)/8 + 2*PARAMS_N*PARAMS_NBAR + BYTES_PKHASH bytes)
  // Output: shared key ss                                       (CRYPTO_BYTES bytes)
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
    uint16_t *Bp = NULL;                                             // recomputed BBp
    int ret = 1;

    if (W == NULL || C == NULL || CC == NULL ||
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

    // Load S from sk_S (LE → native) into Sp buffer (reused as S first)
    for (size_t i = 0; i < (size_t)PARAMS_N * PARAMS_NBAR; i++) {
        S[i] = LE_TO_UINT16(sk_S[i]);
    }

    // Compute W = C - Bp*S (mod q), and decode the randomness mu.
    // Bp is unpacked tile-by-tile from ct_c1 to avoid a full unpacked Bp buffer.
    frodo_unpack(C, PARAMS_NBAR*PARAMS_NBAR, ct_c2,
                 (PARAMS_LOGQ*PARAMS_NBAR*PARAMS_NBAR)/8, PARAMS_LOGQ);
    memset(W, 0, PARAMS_NBAR * PARAMS_NBAR * sizeof(uint16_t));
    {
        const int c1_group_bytes = (int)(PARAMS_NBAR * PARAMS_LOGQ / 8);
        const int c1_row_stride  = (int)(PARAMS_N / 8) * c1_group_bytes;
        for (int block = 0; block < PARAMS_N; block += 8) {
            int g = block / 8;
            uint16_t Bp_tile[8][8];
            for (int r = 0; r < 8; r++) {
                frodo_unpack(Bp_tile[r], 8,
                             ct_c1 + (size_t)r * c1_row_stride + (size_t)g * c1_group_bytes,
                             c1_group_bytes, PARAMS_LOGQ);
            }
            uint16_t x[8][8], y[8][8];
            for (int i = 0; i < 8; i++) {
                for (int k = 0; k < 8; k++) {
                    x[i][k] = Bp_tile[i][k];
                    y[k][i] = S[i * PARAMS_N + (block + k)];
                }
            }
            uint16_t z[8][8];
            if (OP_matrix_mul_8x8(z, (const uint16_t (*)[8])x,
                                  (const uint16_t (*)[8])y,
                                  (uint16_t)PARAMS_Q) != OP_SUCCESS) {
                goto cleanup;
            }
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    W[i * PARAMS_NBAR + j] = (uint16_t)(W[i * PARAMS_NBAR + j] + z[i][j]);
                }
            }
        }
        for (int i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++) {
            W[i] = (uint16_t)(W[i] & ((1 << PARAMS_LOGQ) - 1));
        }
    }
    frodo_sub(W, C, W);
    frodo_key_decode((uint16_t*)muprime, W);

    // Generate (seedSE' || k') = G_2(pkh || mu' || salt)
    memcpy(pkh, sk_pkh, BYTES_PKHASH);
    memcpy(G2in_salt, salt, BYTES_SALT);
    shake(G2out, BYTES_SEED_SE + CRYPTO_BYTES, G2in, BYTES_PKHASH + BYTES_MU + BYTES_SALT);

    // Re-encrypt using the same full-Bp path that was used by the stable 640 flow.
    // This keeps each SHAKE stream contiguous and avoids replaying Ep by tiles.
    int8_t selector = 0;
    Bp = calloc((size_t)PARAMS_N * PARAMS_NBAR, sizeof(uint16_t));
    if (Bp == NULL) {
        goto cleanup;
    }
    if (frodo_gen_sp_ep_epp(Sp, Bp, Epp, seedSEprime) == 0) {
        goto cleanup;
    }
    if (frodo_mul_add_sa_plus_e(Bp, Sp, pk_seedA) == 0) {
        goto cleanup;
    }
    for (int i = 0; i < PARAMS_N * PARAMS_NBAR; i++) {
        Bp[i] &= (uint16_t)((1 << PARAMS_LOGQ) - 1);
    }
    selector = ct_verify_packed(Bp, ct_c1,
                                PARAMS_N * PARAMS_NBAR, PARAMS_LOGQ);

    frodo_mul_add_sb_plus_e(W, pk_b, Sp, Epp);
    frodo_key_encode(CC, (uint16_t*)muprime);
    frodo_add(CC, W, CC);
    selector |= ct_verify_packed(CC, ct_c2,
                                 PARAMS_NBAR * PARAMS_NBAR, PARAMS_LOGQ);
    ct_select(fin_k, (uint8_t*)kprime, (uint8_t*)sk_s,
              CRYPTO_BYTES, selector);

    // ss = F(ct || k): stream ct then the selected k through the incremental hash operator.
    {
        uint64_t *f_state = malloc(FRODO_SHA3_STATE_U64 * sizeof(*f_state));
        uint8_t alg = frodokem_shake_alg();
        if (f_state == NULL) {
            goto cleanup;
        }
        OP_hash_init(alg, f_state, (int)(FRODO_SHA3_STATE_U64 * sizeof(*f_state)));
        OP_hash_absorb(alg, f_state, (int)(FRODO_SHA3_STATE_U64 * sizeof(*f_state)), ct, (int)CRYPTO_CIPHERTEXTBYTES);
        OP_hash_absorb(alg, f_state, (int)(FRODO_SHA3_STATE_U64 * sizeof(*f_state)), fin_k, (int)CRYPTO_BYTES);
        OP_hash_squeeze(alg, f_state, (int)(FRODO_SHA3_STATE_U64 * sizeof(*f_state)), ss, (int)CRYPTO_BYTES);
        clear_bytes((uint8_t *)f_state, FRODO_SHA3_STATE_U64 * sizeof(*f_state));
        free(f_state);
    }
    ret = 0;

    // Cleanup:
cleanup:
    if (Bp != NULL) {
        clear_bytes((uint8_t *)Bp,
                    (size_t)PARAMS_N * PARAMS_NBAR * sizeof(uint16_t));
        free(Bp);
    }
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
