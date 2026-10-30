/*
 * Copyright (c) 2026 Hang Zhang.
 * State Key Laboratory of Cyberspace Security Defense,
 * Institute of Information Engineering, CAS
 * School of Cyber Security, University of Chinese Academy of Sciences
 */
#include "ct_audit.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "params.h"
#include "polyvec.h"
#include "randombytes.h"
#include "packing.h"
#include "sample.h"
#include "sign.h"
#include "symmetric.h"
#include "spectral.h"


/* Allocations are per operation; all buffers are wiped on every exit.
 * The signing phase union is reused only after the previous phase is dead.
 * Cached matrix and Q13 mask never overlap sampler scratch. The encoded key
 * stays with the caller; decoded secrets are reloaded after each sample. */
typedef struct {
    poly_matrix_ntt A_0_ntt;
    polyvecl_ntt s_0_ntt;
    polyvecl s_0;
    polyveck e, public_vector;
    /* Spectral rejection finishes before the first NTT. */
    union {
        bit_spectral_workspace spectral;
        bit_ntt_workspace ntt;
    } phase;
} keygen_workspace;

typedef struct {
    poly_matrix_ntt A_0_ntt;
    polyveck_ntt public_ntt;
    bit_parabola_fixed mask;
    union {
        bit_sampler_workspace sampling;
        struct {
            /* NTT scratch and decoded secrets belong to disjoint stages. */
            union {
                bit_ntt_workspace ntt;
                struct { polyvecl s_0; polyveck e; } decoded;
            } secret;
            /* y is dead after the accepted response/tail copies. */
            union { polyvecy y; bit_codec_workspace codec; } candidate;
            polyveck w, w1;
            poly c_poly;
            union {
                struct { polyvecl_ntt y1_ntt; poly_ntt y0_ntt; } ntt;
                struct { polyvecl c_s; polyveck shift; } proposal;
                struct { polyvecm1 response; polyveck tail; } accepted;
            } temps;
            unsigned char hash_input[BIT_MESSAGEBYTES + BIT_POLYVECK_W1_BYTES];
        } work;
    } phase;
} sign_workspace;
_Static_assert(sizeof(sign_workspace) <= 27968, "signing workspace exceeds RAM allocation budget");

typedef struct {
    poly_matrix_ntt A_0_ntt;
    polyveck public_vector;
    polyveck_ntt public_ntt;
    polyvecl_ntt z1_tail_ntt;
    poly_ntt z0_ntt;
    polyvecm1 z1;
    polyveck h;
    poly c_poly;
    polyveck w_hat_prime;
    unsigned char hash_input[BIT_MESSAGEBYTES + BIT_POLYVECK_W1_BYTES];
    /* Decoded response/hint live outside this union. Encoding scratch is
     * dead before NTT computation starts. */
    union {
        bit_codec_workspace codec;
        bit_ntt_workspace ntt;
    } phase;
} verify_workspace;

int bit_sig_keygen(unsigned char *pk, unsigned long long *pk_len_bytes, unsigned char *sk,
                   unsigned long long *sk_len_bytes)
{
    unsigned char keygen_seedbuf[2 * BIT_SEEDBYTES];
    unsigned char *seed_A = keygen_seedbuf;
    unsigned char *secret_seed = keygen_seedbuf + BIT_SEEDBYTES;
    unsigned char tr[BIT_TRBYTES];

    int status = -1;
    uint16_t nonce = 0;
    unsigned attempt;

    if (pk_len_bytes != NULL) *pk_len_bytes = 0;
    if (sk_len_bytes != NULL) *sk_len_bytes = 0;
    if (pk == NULL || sk == NULL || pk_len_bytes == NULL || sk_len_bytes == NULL)
        return -1;

    keygen_workspace *ws = malloc(sizeof(*ws));
    if (ws == NULL) return -1;
    if (bit_ntt_init() != 0) goto cleanup;

    if (bit_randombytes(keygen_seedbuf, sizeof(keygen_seedbuf)) != 0) {
        bit_secure_zero(keygen_seedbuf, sizeof(keygen_seedbuf));
        goto cleanup;
    }
    for (attempt = 0; attempt < BIT_KEYGEN_MAX_ITERS; attempt++) {
        if (attempt != 0 && bit_randombytes(secret_seed, BIT_SEEDBYTES) != 0)
            goto cleanup;
        nonce = 0;
        if (polyvecl_sample_S1(&ws->s_0, secret_seed, &nonce) != 0 ||
            polyveck_sample_S1(&ws->e, secret_seed, &nonce) != 0)
            goto cleanup;
        int reject = bit_keygen_spectral_bound_sq_ws(&ws->s_0, &ws->e, &ws->phase.spectral) > BIT_PARABOLA_BETA2;
        BIT_CT_DECLASSIFY(&reject, sizeof(reject));
        if (!reject)
            break;
    }
    if (attempt == BIT_KEYGEN_MAX_ITERS)
        goto cleanup;

    if (poly_matrix_expand_ntt(&ws->A_0_ntt, seed_A) != 0) {
        goto cleanup;
    }
    if (polyvecl_to_ntt_ws(&ws->s_0_ntt, &ws->s_0, &ws->phase.ntt) != 0) goto cleanup;
    if (poly_matrix_mul_vector_ntt_ws(&ws->public_vector, &ws->A_0_ntt, &ws->s_0_ntt, &ws->phase.ntt) != 0) goto cleanup;
    polyveck_add_small(&ws->public_vector, &ws->public_vector, &ws->e);
    polyveck_canonicalize(&ws->public_vector);

    pack_pk(pk, seed_A, &ws->public_vector);
    if (bit_hash(tr, pk, BIT_PUBLICKEYBYTES) != 0) {
        goto cleanup;
    }
    pack_sk(sk, seed_A, &ws->public_vector, secret_seed, tr, &ws->s_0, &ws->e);

    *pk_len_bytes = BIT_PUBLICKEYBYTES;
    *sk_len_bytes = BIT_SECRETKEYBYTES;

    status = 0;
cleanup:
    bit_secure_zero(keygen_seedbuf, sizeof(keygen_seedbuf));
    bit_secure_zero(ws, sizeof(*ws));
    free(ws);
    return status;
}

int bit_sig_sign(unsigned char *sk, unsigned long long sk_len_bytes, const unsigned char *m,
                 unsigned long long mlen, unsigned char *sig, unsigned long long *sig_len_bytes)
{
    unsigned char seed_A[BIT_SEEDBYTES];
    unsigned char challenge[BIT_CHALLENGEBYTES];
    unsigned char message_hash[BIT_MESSAGEBYTES];
    unsigned char secret_seed[BIT_SEEDBYTES];
    unsigned char tr[BIT_TRBYTES];
    unsigned char b_batch[32];
    size_t b_bit_pos = sizeof(b_batch) * 8U;
    unsigned char rnd[BIT_SEEDBYTES];
    unsigned char temp_seed_y[BIT_SEEDBYTES];
    int status = -1;
    int rejection;

    sparse_challenge c_sparse;

    if (sig_len_bytes != NULL) *sig_len_bytes = 0;
    if (sk == NULL || (m == NULL && mlen != 0) || sig == NULL || sig_len_bytes == NULL ||
        mlen > SIZE_MAX - BIT_TRBYTES || sk_len_bytes != BIT_SECRETKEYBYTES) {
        return -1;
    }
    if (m == NULL)
        m = (const unsigned char *)"";

    sign_workspace *ws = malloc(sizeof(*ws));
    if (ws == NULL) return -1;
    if (bit_ntt_init() != 0) goto cleanup;

    if (unpack_sk(seed_A, &ws->phase.work.w, secret_seed, tr, &ws->phase.work.secret.decoded.s_0, &ws->phase.work.secret.decoded.e, sk) != 0) {
        goto cleanup;
    }

    if (poly_matrix_expand_ntt(&ws->A_0_ntt, seed_A) != 0) {
        goto cleanup;
    }
    if (polyveck_public_to_ntt_ws(&ws->public_ntt, &ws->phase.work.w, &ws->phase.work.secret.ntt) != 0) goto cleanup;

    if (bit_hash_message(message_hash, tr, BIT_TRBYTES, m, (size_t)mlen) != 0) {
        goto cleanup;
    }

    if (bit_randombytes(rnd, BIT_SEEDBYTES) != 0) {
        goto cleanup;
    }
    if (bit_xof256_3(temp_seed_y, BIT_SEEDBYTES, secret_seed, BIT_SEEDBYTES, rnd, BIT_SEEDBYTES,
                     message_hash, BIT_MESSAGEBYTES) != 0)
        goto cleanup;


    uint16_t nonce_y = 0;
    if (polyvecy_sample_mask_q13_ws(&ws->phase.work.candidate.y, &ws->mask, temp_seed_y, &nonce_y, &ws->phase.sampling) != 0)
        goto cleanup;

    uint32_t sign_iter = 0;
    while (1) {
        if (++sign_iter > BIT_SIGN_MAX_ITERS)
            goto cleanup;
        if (polyvecy_y1_to_ntt_ws(&ws->phase.work.temps.ntt.y1_ntt, &ws->phase.work.candidate.y, &ws->phase.work.secret.ntt) != 0) goto cleanup;
        if (poly_to_ntt_ws(&ws->phase.work.temps.ntt.y0_ntt, &ws->phase.work.candidate.y.vec[0], &ws->phase.work.secret.ntt) != 0) goto cleanup;
        if (polyveck_compute_w_ntt_ws(&ws->phase.work.w, &ws->A_0_ntt, &ws->public_ntt, &ws->phase.work.temps.ntt.y1_ntt, &ws->phase.work.temps.ntt.y0_ntt, &ws->phase.work.candidate.y, &ws->phase.work.secret.ntt) != 0) goto cleanup;

        polyveck_highbits(&ws->phase.work.w1, &ws->phase.work.w);
        /* Sampling reuses this phase, so restore the prefix for every attempt. */
        memcpy(ws->phase.work.hash_input, message_hash, BIT_MESSAGEBYTES);
        polyveck_pack_w1(ws->phase.work.hash_input + BIT_MESSAGEBYTES, &ws->phase.work.w1);
        if (bit_hash(challenge, ws->phase.work.hash_input, sizeof(ws->phase.work.hash_input)) != 0) {
            goto cleanup;
        }

        if (b_bit_pos == sizeof(b_batch) * 8U) {
            if (bit_randombytes(b_batch, sizeof(b_batch)) != 0) {
                goto cleanup;
            }
            b_bit_pos = 0;
        }
        uint8_t b_val = (uint8_t)((b_batch[b_bit_pos >> 3] >> (b_bit_pos & 7U)) & 1U);
        b_bit_pos++;

        if (poly_challenge(&ws->phase.work.c_poly, challenge) != 0) {
            goto cleanup;
        }
        poly_challenge_to_sparse(&c_sparse, &ws->phase.work.c_poly);
        c_sparse.bflip = b_val;
        poly_cneg(&ws->phase.work.c_poly, b_val);

        /* The OP scratch is no longer live; restore secrets into that union. */
        unpack_sk_se(&ws->phase.work.secret.decoded.s_0, &ws->phase.work.secret.decoded.e, sk);
        /* Apply one global sign to every block of (c, c*s0, c*e). */
        polyvecl_mul_challenge_no_reduce(&ws->phase.work.temps.proposal.c_s, &ws->phase.work.secret.decoded.s_0, &c_sparse);
        polyveck_mul_challenge_no_reduce(&ws->phase.work.temps.proposal.shift, &ws->phase.work.secret.decoded.e, &c_sparse);
        poly_add_no_reduce(&ws->phase.work.candidate.y.vec[0], &ws->phase.work.candidate.y.vec[0], &ws->phase.work.c_poly);

        for (int i = 0; i < BIT_L; i++) {
            poly_add_no_reduce(&ws->phase.work.candidate.y.vec[1 + i], &ws->phase.work.candidate.y.vec[1 + i], &ws->phase.work.temps.proposal.c_s.vec[i]);
        }

        for (int i = 0; i < BIT_K; i++) {
            poly_add_no_reduce(&ws->phase.work.candidate.y.vec[1 + BIT_L + i], &ws->phase.work.candidate.y.vec[1 + BIT_L + i],
                               &ws->phase.work.temps.proposal.shift.vec[i]);
        }

        rejection = bit_parabola_reject_response(&ws->phase.work.c_poly, &ws->phase.work.temps.proposal.c_s, &ws->phase.work.temps.proposal.shift,
                                                 &ws->mask, temp_seed_y, &nonce_y);
        if (rejection < 0)
            goto cleanup;
        if (rejection != 0) {
            goto retry;
        }

        /* Reject if the signed challenge changes HighBits in the first block. */
        if (poly_reject_highbits_bimodal(&ws->phase.work.w1.vec[0], &ws->phase.work.w.vec[0], &c_sparse)) {
            goto retry;
        }

        for (int i = 0; i < BIT_L + 1; i++) {
            ws->phase.work.temps.accepted.response.vec[i] = ws->phase.work.candidate.y.vec[i];
        }
        for (int i = 0; i < BIT_K; i++) {
            ws->phase.work.temps.accepted.tail.vec[i] = ws->phase.work.candidate.y.vec[1 + BIT_L + i];
        }

        polyveck_make_hint(&ws->phase.work.w1, &ws->phase.work.w1, &ws->phase.work.w, &ws->phase.work.temps.accepted.tail,
                          &ws->phase.work.c_poly, b_val);

        /* TIMECOP assumes these candidates public, including later rejections. */
        BIT_CT_HAETAE_PUBLIC(&ws->phase.work.temps.accepted.response, sizeof ws->phase.work.temps.accepted.response);
        BIT_CT_HAETAE_PUBLIC(&ws->phase.work.w1, sizeof ws->phase.work.w1);

        if (check_signature_bounds(&ws->phase.work.temps.accepted.response, &ws->phase.work.w1)) {
            goto retry;
        }

        /* Reject unsupported tails and oversized entropy streams. */
        if (pack_sig_ws(sig, &ws->phase.work.temps.accepted.response, &ws->phase.work.w1, challenge, &ws->phase.work.candidate.codec) != 0) {
            goto retry;
        }

        break;
    retry:;
        if (polyvecy_sample_mask_q13_ws(&ws->phase.work.candidate.y, &ws->mask, temp_seed_y, &nonce_y, &ws->phase.sampling) != 0)
            goto cleanup;
    }
    status = 0;
    *sig_len_bytes = BIT_SIGNBYTES;
cleanup:;
    bit_secure_zero(temp_seed_y, sizeof(temp_seed_y));
    bit_secure_zero(secret_seed, sizeof(secret_seed));
    bit_secure_zero(rnd, sizeof(rnd));
    bit_secure_zero(b_batch, sizeof(b_batch));

    bit_secure_zero(&c_sparse, sizeof(c_sparse));
    bit_secure_zero(ws, sizeof(*ws));
    free(ws);
    return status;
}

int bit_sig_verify(const unsigned char *sig, unsigned long long siglen, const unsigned char *m,
                   unsigned long long mlen, const unsigned char *pk,
                   unsigned long long pk_len_bytes)
{
    unsigned char seed_A[BIT_SEEDBYTES];
    unsigned char challenge[BIT_CHALLENGEBYTES];
    unsigned char c_prime[BIT_CHALLENGEBYTES];
    unsigned char message_hash[BIT_MESSAGEBYTES];

    unsigned char tr[BIT_TRBYTES];
    if (sig == NULL || (m == NULL && mlen != 0) || pk == NULL || mlen > SIZE_MAX - BIT_TRBYTES ||
        siglen != BIT_SIGNBYTES || pk_len_bytes != BIT_PUBLICKEYBYTES) {
        return -1;
    }
    if (m == NULL)
        m = (const unsigned char *)"";

    verify_workspace *ws = malloc(sizeof(*ws));
    if (ws == NULL) return -1;
    int status = -1;
    if (bit_ntt_init() != 0) goto cleanup;

    if (unpack_pk(seed_A, &ws->public_vector, pk) != 0)
        goto cleanup;

    if (unpack_sig_ws(&ws->z1, &ws->h, challenge, sig, &ws->phase.codec) != 0)
        goto cleanup;

    if (check_signature_verification_bounds(&ws->z1, &ws->h)) {
        goto cleanup;
    }

    for (int i = 0; i < BIT_L; i++)
        if (poly_to_ntt_centered_ws(&ws->z1_tail_ntt.vec[i], &ws->z1.vec[i + 1], &ws->phase.ntt) != 0) goto cleanup;
    if (poly_to_ntt_centered_ws(&ws->z0_ntt, &ws->z1.vec[0], &ws->phase.ntt) != 0) goto cleanup;

    if (bit_hash(tr, pk, BIT_PUBLICKEYBYTES) != 0) {
        goto cleanup;
    }
    if (bit_hash_message(message_hash, tr, BIT_TRBYTES, m, (size_t)mlen) != 0)
        goto cleanup;
    if (poly_matrix_expand_ntt(&ws->A_0_ntt, seed_A) != 0) {
        goto cleanup;
    }
    if (polyveck_public_to_ntt_ws(&ws->public_ntt, &ws->public_vector, &ws->phase.ntt) != 0) goto cleanup;
    if (poly_challenge(&ws->c_poly, challenge) != 0) {
        goto cleanup;
    }

    if (polyveck_compute_w_hat_prime_ntt_ws(&ws->w_hat_prime, &ws->A_0_ntt, &ws->public_ntt, &ws->z1_tail_ntt, &ws->z0_ntt,
                                     &ws->z1.vec[0], &ws->c_poly, &ws->phase.ntt) != 0) goto cleanup;

    polyveck_compute_w_prime(&ws->w_hat_prime, &ws->w_hat_prime, &ws->h);

    memcpy(ws->hash_input, message_hash, BIT_MESSAGEBYTES);
    polyveck_pack_w1(ws->hash_input + BIT_MESSAGEBYTES, &ws->w_hat_prime);

    if (bit_hash(c_prime, ws->hash_input, sizeof(ws->hash_input)) != 0) {
        goto cleanup;
    }
    if (memcmp(challenge, c_prime, BIT_CHALLENGEBYTES) != 0) {
        goto cleanup;
    }

    status = 0;
cleanup:
    bit_secure_zero(ws, sizeof(*ws));
    free(ws);
    return status;
}
