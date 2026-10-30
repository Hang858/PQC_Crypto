/*
 * Copyright (c) 2026 Hang Zhang.
 * State Key Laboratory of Cyberspace Security Defense,
 * Institute of Information Engineering, CAS
 * School of Cyber Security, University of Chinese Academy of Sciences
 */
#include "polyvec.h"
#include "align.h"
#include "params.h"
#include "symmetric.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

int polyvecl_sample_S1(polyvecl *v, const uint8_t *seed, uint16_t *nonce)
{
    for (size_t i = 0; i < BIT_L; i++)
        if (poly_sample_S1(&v->vec[i], seed, nonce) != 0) {
            return -1;
        }
    return 0;
}

int polyveck_sample_S1(polyveck *v, const uint8_t *seed, uint16_t *nonce)
{
    for (size_t i = 0; i < BIT_K; i++)
        if (poly_sample_S1(&v->vec[i], seed, nonce) != 0) {
            return -1;
        }
    return 0;
}

#define MATRIX_XOF_BUFFER_SIZE (3 * BIT_XOF128_RATE)

BIT_ALIGN_64
static int poly_uniform_ntt(poly_ntt *a, const unsigned char *seed, size_t row, size_t col)
{
    unsigned char buf[MATRIX_XOF_BUFFER_SIZE];
    unsigned char msg[BIT_SEEDBYTES + 2];
    bit_xof128_state st;
    size_t ctr = 0;

    memcpy(msg, seed, BIT_SEEDBYTES);
    msg[BIT_SEEDBYTES] = (unsigned char)col;
    msg[BIT_SEEDBYTES + 1] = (unsigned char)row;

    bit_xof128_init(&st, msg, sizeof(msg));
    while (ctr < BIT_N) {
        size_t pos = 0;
        uint32_t reservoir = 0;
        unsigned int reservoir_bits = 0;

        if (bit_xof128_squeeze(&st, buf, sizeof(buf)) != 0) {
            bit_xof128_zeroize(&st);
            return -1;
        }
        while (ctr < BIT_N) {
            uint16_t val;

            while (reservoir_bits < 13 && pos < sizeof(buf)) {
                reservoir |= (uint32_t)buf[pos++] << reservoir_bits;
                reservoir_bits += 8;
            }
            if (reservoir_bits < 13)
                break;

            /* Read disjoint 13-bit candidates; accept values below q. */
            val = (uint16_t)(reservoir & 0x1FFFU);
            reservoir >>= 13;
            reservoir_bits -= 13;
            if (val < BIT_Q) {
                a->coeffs[ctr++] = (int16_t)val;
            }
        }
    }
    bit_xof128_zeroize(&st);
    return 0;
}

int poly_matrix_expand_ntt(poly_matrix_ntt *A_ntt, const unsigned char *seed)
{
    for (size_t i = 0; i < BIT_K; ++i) {
        for (size_t j = 0; j < BIT_L; ++j) {
            if (poly_uniform_ntt(&A_ntt->vec[i].vec[j], seed, i, j) != 0) {
                return -1;
            }
        }
    }
    return 0;
}

int polyvecl_to_ntt_ws(polyvecl_ntt *result, const polyvecl *v, bit_ntt_workspace *scratch)
{
    for (int i = 0; i < BIT_L; i++) {
        if (poly_to_ntt_ws(&result->vec[i], &v->vec[i], scratch) != 0) return -1;
    }
    return 0;
}


int polyvecy_y1_to_ntt_ws(polyvecl_ntt *result, const polyvecy *y, bit_ntt_workspace *scratch)
{
    for (int i = 0; i < BIT_L; i++) {
        if (poly_to_ntt_ws(&result->vec[i], &y->vec[1 + i], scratch) != 0) return -1;
    }
    return 0;
}


int polyveck_from_ntt_ws(polyveck *result, polyveck_ntt *v_ntt, bit_ntt_workspace *scratch)
{
    for (int i = 0; i < BIT_K; i++) {
        if (poly_from_ntt_inplace_ws(&result->vec[i], &v_ntt->vec[i], scratch) != 0) return -1;
    }
    return 0;
}


int poly_matrix_mul_vector_ntt_ws(polyveck *result, const poly_matrix_ntt *A_ntt, const polyvecl_ntt *v_ntt, bit_ntt_workspace *scratch)
{
    poly_ntt acc;

    for (int i = 0; i < BIT_K; i++) {
        if (poly_ntt_dot3_raw_ws(&acc, A_ntt->vec[i].vec, v_ntt->vec, scratch) != 0) return -1;
        if (poly_ntt_montgomery_lift_ws(&acc, scratch) != 0) return -1;
        if (poly_from_ntt_inplace_ws(&result->vec[i], &acc, scratch) != 0) return -1;
    }

    return 0;
}


void polyveck_add_small(polyveck *result, const polyveck *a, const polyveck *small)
{
    for (int i = 0; i < BIT_K; i++) {
        poly_add_small(&result->vec[i], &a->vec[i], &small->vec[i]);
    }
}

void polyveck_canonicalize(polyveck *b)
{
    for (int i = 0; i < BIT_K; i++)
        poly_canonicalize(&b->vec[i]);
}

int polyveck_public_to_ntt_ws(polyveck_ntt *b_ntt, const polyveck *b, bit_ntt_workspace *scratch)
{
    for (int i = 0; i < BIT_K; i++)
        if (poly_to_ntt_ws(&b_ntt->vec[i], &b->vec[i], scratch) != 0) return -1;
    return 0;
}


int polyveck_compute_w_ntt_ws(polyveck *w, const poly_matrix_ntt *A_0_ntt, const polyveck_ntt *b_ntt, const polyvecl_ntt *y1_ntt, const poly_ntt *y0_ntt, const polyvecy *y, bit_ntt_workspace *scratch)
{
    poly_ntt w_ntt;
    poly_ntt tmp_ntt;

    for (int i = 0; i < BIT_K; i++) {
        if (poly_ntt_dot3_raw_ws(&w_ntt, A_0_ntt->vec[i].vec, y1_ntt->vec, scratch) != 0) return -1;
        if (poly_ntt_basemul_raw_ws(&tmp_ntt, &b_ntt->vec[i], y0_ntt, scratch) != 0) return -1;
        for (int j = 0; j < BIT_N; j++) {
            w_ntt.coeffs[j] = (int16_t)((int32_t)w_ntt.coeffs[j] - tmp_ntt.coeffs[j]);
        }
        if (poly_ntt_montgomery_lift_ws(&w_ntt, scratch) != 0) return -1;
        if (poly_from_ntt_inplace_ws(&w->vec[i], &w_ntt, scratch) != 0) return -1;
    }

    for (int j = 0; j < BIT_N; j++) {
        int32_t val = w->vec[0].coeffs[j];
        int32_t q_hat_y0 = (int32_t)y->vec[0].coeffs[j] * BIT_Q_HAT_MINUS;

        w->vec[0].coeffs[j] = reduce_signed_barrett(val + q_hat_y0);
    }

    for (int i = 0; i < BIT_K; i++) {
        poly_add(&w->vec[i], &w->vec[i], &y->vec[1 + BIT_L + i]);
    }
    return 0;
}


void polyveck_highbits(polyveck *w1, const polyveck *w)
{
    for (int i = 0; i < BIT_K; i++) {
        poly_highbits(&w1->vec[i], &w->vec[i]);
    }
}

void polyveck_pack_w1(unsigned char *r, const polyveck *w1)
{
    for (int i = 0; i < BIT_K; i++) {
        poly_pack_w1(r + i * BIT_POLY_W1_BYTES, &w1->vec[i]);
    }
}

void polyvecl_mul_challenge_no_reduce(polyvecl *result, const polyvecl *a,
                                      const sparse_challenge *c)
{
    for (int i = 0; i < BIT_L; i++) {
        poly_mul_challenge_no_reduce(&result->vec[i], &a->vec[i], c);
    }
}

void polyveck_mul_challenge_no_reduce(polyveck *result, const polyveck *a,
                                      const sparse_challenge *c)
{
    for (int i = 0; i < BIT_K; i++) {
        poly_mul_challenge_no_reduce(&result->vec[i], &a->vec[i], c);
    }
}

void polyveck_make_hint(polyveck *h, const polyveck *w1, const polyveck *w,
                          const polyveck *z2, const poly *c, uint8_t b)
{
    const int32_t add_c_mask = -(int32_t)(1U - b);

    for (int j = 0; j < BIT_N; j++) {
        int32_t w_hat_val = (int32_t)w->vec[0].coeffs[j] - z2->vec[0].coeffs[j];
        w_hat_val += (int32_t)c->coeffs[j] & add_c_mask;

        int16_t hb_w_hat;
        decompose_hint(&hb_w_hat, w_hat_val);

        int32_t h_raw = (int32_t)w1->vec[0].coeffs[j] - hb_w_hat;
        int32_t h_val = h_raw;
        h_val += (h_val >> 31) & BIT_HINT_MODULUS;
        int32_t over_half = (BIT_HINT_MODULUS / 2 - h_val) >> 31;
        h_val -= BIT_HINT_MODULUS & over_half;
        h->vec[0].coeffs[j] = (int16_t)h_val;
    }
    for (int i = 1; i < BIT_K; i++) {
        for (int j = 0; j < BIT_N; j++) {
            int32_t w_hat_val = (int32_t)w->vec[i].coeffs[j] - z2->vec[i].coeffs[j];

            int16_t hb_w_hat;
            decompose_hint(&hb_w_hat, w_hat_val);

            int32_t h_raw = (int32_t)w1->vec[i].coeffs[j] - hb_w_hat;
            int32_t h_val = h_raw;
            h_val += (h_val >> 31) & BIT_HINT_MODULUS;
            int32_t over_half = (BIT_HINT_MODULUS / 2 - h_val) >> 31;
            h_val -= BIT_HINT_MODULUS & over_half;
            h->vec[i].coeffs[j] = (int16_t)h_val;
        }
    }
}

int polyveck_compute_w_hat_prime_ntt_ws(polyveck *w_hat_prime, const poly_matrix_ntt *A_0_ntt, const polyveck_ntt *public_ntt, const polyvecl_ntt *z1_tail_ntt, const poly_ntt *z0_ntt, const poly *z0, const poly *c_poly, bit_ntt_workspace *scratch)
{
    poly_ntt acc;
    poly_ntt tmp_ntt;

    for (int i = 0; i < BIT_K; i++) {
        if (poly_ntt_dot3_raw_ws(&acc, A_0_ntt->vec[i].vec, z1_tail_ntt->vec, scratch) != 0) return -1;
        if (poly_ntt_basemul_raw_ws(&tmp_ntt, &public_ntt->vec[i], z0_ntt, scratch) != 0) return -1;
        for (int j = 0; j < BIT_N; j++) {
            acc.coeffs[j] = (int16_t)((int32_t)acc.coeffs[j] - tmp_ntt.coeffs[j]);
        }
        if (poly_ntt_montgomery_lift_ws(&acc, scratch) != 0) return -1;
        if (poly_from_ntt_inplace_ws(&w_hat_prime->vec[i], &acc, scratch) != 0) return -1;
    }


    for (int j = 0; j < BIT_N; j++) {
        int32_t val = w_hat_prime->vec[0].coeffs[j];
        int32_t z0_c_sum = (int32_t)z0->coeffs[j] + c_poly->coeffs[j];

        val += z0_c_sum * BIT_Q_HAT;
        w_hat_prime->vec[0].coeffs[j] = reduce_signed_barrett(val);
    }
    return 0;
}


void polyveck_compute_w_prime(polyveck *w_prime, const polyveck *w_hat_prime, const polyveck *h)
{
    for (int i = 0; i < BIT_K; i++) {
        for (int j = 0; j < BIT_N; j++) {
            int16_t hb;
            decompose_hint(&hb, w_hat_prime->vec[i].coeffs[j]);
            int32_t val = (int32_t)hb + h->vec[i].coeffs[j];
            val += (val >> 31) & BIT_HINT_MODULUS;
            val -= BIT_HINT_MODULUS;
            val += (val >> 31) & BIT_HINT_MODULUS;

            w_prime->vec[i].coeffs[j] = (int16_t)val;
        }
    }
}

int polyvecl_to_ntt(polyvecl_ntt *result, const polyvecl *v)
{
    bit_ntt_workspace *scratch = malloc(sizeof(*scratch));
    if (scratch == NULL) return -1;
    int rc = bit_ntt_init() == 0 ? polyvecl_to_ntt_ws(result, v, scratch) : -1;
    bit_secure_zero(scratch, sizeof(*scratch)); free(scratch);
    return rc;
}

int polyvecy_y1_to_ntt(polyvecl_ntt *result, const polyvecy *y)
{
    bit_ntt_workspace *scratch = malloc(sizeof(*scratch));
    if (scratch == NULL) return -1;
    int rc = bit_ntt_init() == 0 ? polyvecy_y1_to_ntt_ws(result, y, scratch) : -1;
    bit_secure_zero(scratch, sizeof(*scratch)); free(scratch);
    return rc;
}

int polyveck_from_ntt(polyveck *result, polyveck_ntt *v_ntt)
{
    bit_ntt_workspace *scratch = malloc(sizeof(*scratch));
    if (scratch == NULL) return -1;
    int rc = bit_ntt_init() == 0 ? polyveck_from_ntt_ws(result, v_ntt, scratch) : -1;
    bit_secure_zero(scratch, sizeof(*scratch)); free(scratch);
    return rc;
}

int poly_matrix_mul_vector_ntt(polyveck *result, const poly_matrix_ntt *A_ntt, const polyvecl_ntt *v_ntt)
{
    bit_ntt_workspace *scratch = malloc(sizeof(*scratch));
    if (scratch == NULL) return -1;
    int rc = bit_ntt_init() == 0 ? poly_matrix_mul_vector_ntt_ws(result, A_ntt, v_ntt, scratch) : -1;
    bit_secure_zero(scratch, sizeof(*scratch)); free(scratch);
    return rc;
}

int polyveck_public_to_ntt(polyveck_ntt *b_ntt, const polyveck *b)
{
    bit_ntt_workspace *scratch = malloc(sizeof(*scratch));
    if (scratch == NULL) return -1;
    int rc = bit_ntt_init() == 0 ? polyveck_public_to_ntt_ws(b_ntt, b, scratch) : -1;
    bit_secure_zero(scratch, sizeof(*scratch)); free(scratch);
    return rc;
}

int polyveck_compute_w_ntt(polyveck *w, const poly_matrix_ntt *A_0_ntt, const polyveck_ntt *b_ntt, const polyvecl_ntt *y1_ntt, const poly_ntt *y0_ntt, const polyvecy *y)
{
    bit_ntt_workspace *scratch = malloc(sizeof(*scratch));
    if (scratch == NULL) return -1;
    int rc = bit_ntt_init() == 0 ? polyveck_compute_w_ntt_ws(w, A_0_ntt, b_ntt, y1_ntt, y0_ntt, y, scratch) : -1;
    bit_secure_zero(scratch, sizeof(*scratch)); free(scratch);
    return rc;
}

int polyveck_compute_w_hat_prime_ntt(polyveck *w_hat_prime, const poly_matrix_ntt *A_0_ntt, const polyveck_ntt *public_ntt, const polyvecl_ntt *z1_tail_ntt, const poly_ntt *z0_ntt, const poly *z0, const poly *c_poly)
{
    bit_ntt_workspace *scratch = malloc(sizeof(*scratch));
    if (scratch == NULL) return -1;
    int rc = bit_ntt_init() == 0 ? polyveck_compute_w_hat_prime_ntt_ws(w_hat_prime, A_0_ntt, public_ntt, z1_tail_ntt, z0_ntt, z0, c_poly, scratch) : -1;
    bit_secure_zero(scratch, sizeof(*scratch)); free(scratch);
    return rc;
}
