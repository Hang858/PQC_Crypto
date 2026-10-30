/*
 * Copyright (c) 2026 Hang Zhang.
 * State Key Laboratory of Cyberspace Security Defense,
 * Institute of Information Engineering, CAS
 * School of Cyber Security, University of Chinese Academy of Sciences
 */
#ifndef POLYVEC_H
#define POLYVEC_H

#include <stddef.h>
#include <stdint.h>
#include "params.h"
#include "poly.h"

typedef struct {
    poly vec[BIT_K];
} polyveck;

typedef struct {
    poly vec[BIT_L];
} polyvecl;

typedef struct {
    poly vec[BIT_L + 1];
} polyvecm1;

typedef struct {
    poly vec[BIT_L + BIT_K + 1];
} polyvecy;

typedef struct {
    poly_ntt vec[BIT_K];
} polyveck_ntt;

typedef struct {
    poly_ntt vec[BIT_L];
} polyvecl_ntt;

typedef struct {
    polyvecl_ntt vec[BIT_K];
} poly_matrix_ntt;

int poly_matrix_expand_ntt(poly_matrix_ntt *A_ntt, const unsigned char *seed);
int polyvecl_sample_S1(polyvecl *v, const uint8_t *seed, uint16_t *nonce);
int polyveck_sample_S1(polyveck *v, const uint8_t *seed, uint16_t *nonce);

int poly_matrix_mul_vector_ntt(polyveck *result, const poly_matrix_ntt *A_ntt, const polyvecl_ntt *v_ntt);
int poly_matrix_mul_vector_ntt_ws(polyveck *result, const poly_matrix_ntt *A_ntt, const polyvecl_ntt *v_ntt, bit_ntt_workspace *scratch);

int polyvecl_to_ntt(polyvecl_ntt *result, const polyvecl *v);
int polyvecl_to_ntt_ws(polyvecl_ntt *result, const polyvecl *v, bit_ntt_workspace *scratch);
int polyvecy_y1_to_ntt(polyvecl_ntt *result, const polyvecy *y);
int polyvecy_y1_to_ntt_ws(polyvecl_ntt *result, const polyvecy *y, bit_ntt_workspace *scratch);
/* Destructive conversion: v_ntt is consumed. */
int polyveck_from_ntt(polyveck *result, polyveck_ntt *v_ntt);
int polyveck_from_ntt_ws(polyveck *result, polyveck_ntt *v_ntt, bit_ntt_workspace *scratch);

void polyveck_add_small(polyveck *result, const polyveck *a, const polyveck *small);
void polyveck_canonicalize(polyveck *b);
int polyveck_public_to_ntt(polyveck_ntt *b_ntt, const polyveck *b);
int polyveck_public_to_ntt_ws(polyveck_ntt *b_ntt, const polyveck *b, bit_ntt_workspace *scratch);

void polyvecl_mul_challenge_no_reduce(polyvecl *result, const polyvecl *v,
                                      const sparse_challenge *c);
void polyveck_mul_challenge_no_reduce(polyveck *result, const polyveck *v,
                                      const sparse_challenge *c);

int polyveck_compute_w_ntt(polyveck *w, const poly_matrix_ntt *A_0_ntt, const polyveck_ntt *b_ntt, const polyvecl_ntt *y1_ntt, const poly_ntt *y0_ntt, const polyvecy *y);
int polyveck_compute_w_ntt_ws(polyveck *w, const poly_matrix_ntt *A_0_ntt, const polyveck_ntt *b_ntt, const polyvecl_ntt *y1_ntt, const poly_ntt *y0_ntt, const polyvecy *y, bit_ntt_workspace *scratch);
void polyveck_highbits(polyveck *w1, const polyveck *w);
void polyveck_pack_w1(unsigned char *packed, const polyveck *w1);
void polyveck_make_hint(polyveck *h, const polyveck *w1, const polyveck *w,
                          const polyveck *z2, const poly *c, uint8_t b);

int polyveck_compute_w_hat_prime_ntt(polyveck *w_hat_prime, const poly_matrix_ntt *A_0_ntt, const polyveck_ntt *public_ntt, const polyvecl_ntt *z1_tail_ntt, const poly_ntt *z0_ntt, const poly *z0, const poly *c_poly);
int polyveck_compute_w_hat_prime_ntt_ws(polyveck *w_hat_prime, const poly_matrix_ntt *A_0_ntt, const polyveck_ntt *public_ntt, const polyvecl_ntt *z1_tail_ntt, const poly_ntt *z0_ntt, const poly *z0, const poly *c_poly, bit_ntt_workspace *scratch);
void polyveck_compute_w_prime(polyveck *w_prime, const polyveck *w_hat_prime, const polyveck *h);
#endif
