/*
 * Copyright (c) 2026 Hang Zhang.
 * State Key Laboratory of Cyberspace Security Defense,
 * Institute of Information Engineering, CAS
 * School of Cyber Security, University of Chinese Academy of Sciences
 */
#include "ct_audit.h"
#include "poly.h"
#include "ntt.h"
#include "symmetric.h"
#include "endian.h"
#include "packing_w1.h"
#include <stddef.h>
#include <stdlib.h>

#include "small_sample.h"

static void unpack_S1_3bit(int16_t *a, const uint8_t *buf)
{
    for (int i = 0; i < BIT_N / 8; i++) {
        uint32_t t = load24_le(buf + 3 * i);

        a[8 * i + 0] = bit_small_sample((t >> 0) & 0x07);
        a[8 * i + 1] = bit_small_sample((t >> 3) & 0x07);
        a[8 * i + 2] = bit_small_sample((t >> 6) & 0x07);
        a[8 * i + 3] = bit_small_sample((t >> 9) & 0x07);
        a[8 * i + 4] = bit_small_sample((t >> 12) & 0x07);
        a[8 * i + 5] = bit_small_sample((t >> 15) & 0x07);
        a[8 * i + 6] = bit_small_sample((t >> 18) & 0x07);
        a[8 * i + 7] = bit_small_sample((t >> 21) & 0x07);
    }
}

void decompose_hint(int16_t *highbits, int16_t r)
{
    uint32_t val = (uint32_t)reduce_barrett(r);
    uint32_t numerator = val + (BIT_GAMMA2 >> 1);
    int32_t hb =
        (int32_t)(((uint64_t)numerator * BIT_GAMMA2_BARRETT_MULT) >> BIT_GAMMA2_BARRETT_SHIFT);

    int32_t edgecase = (BIT_HINT_MODULUS - (hb + 1)) >> 31;
    hb -= BIT_HINT_MODULUS & edgecase;
    *highbits = (int16_t)hb;
}


int poly_sample_S1(poly *a, const uint8_t *seed, uint16_t *nonce)
{
    uint8_t buf[BIT_SAMPLE_S1_BUFLEN];

    if (bit_xof256_nonce(buf, sizeof(buf), seed, BIT_SEEDBYTES, *nonce) != 0) {
        bit_secure_zero(buf, sizeof(buf));
        return -1;
    }
    *nonce = (uint16_t)(*nonce + 1U);
    unpack_S1_3bit(a->coeffs, buf);
    bit_secure_zero(buf, sizeof(buf));
    return 0;
}

static int poly_to_ntt_impl(poly_ntt *result, const poly *a, poly *centered, bit_ntt_workspace *scratch)
{
    for (int i = 0; i < BIT_N; i++) {
        result->coeffs[i] = (int16_t)reduce_barrett(a->coeffs[i]);
        if (centered != NULL) centered->coeffs[i] = reduce_to_signed(result->coeffs[i]);
    }
    return ntt_forward_ws(result->coeffs, result->coeffs, scratch);
}
int poly_to_ntt_ws(poly_ntt *result, const poly *a, bit_ntt_workspace *scratch)
{ return poly_to_ntt_impl(result, a, NULL, scratch); }
int poly_to_ntt_centered_ws(poly_ntt *result, poly *a, bit_ntt_workspace *scratch)
{ return poly_to_ntt_impl(result, a, a, scratch); }
int poly_from_ntt_ws(poly *result, const poly_ntt *a, bit_ntt_workspace *scratch)
{
    if (ntt_inverse_ws(result->coeffs, a->coeffs, scratch) != 0) return -1;
    for (int i = 0; i < BIT_N; ++i) result->coeffs[i] = reduce_to_signed((uint16_t)result->coeffs[i]);
    return 0;
}
int poly_from_ntt_inplace_ws(poly *result, poly_ntt *a, bit_ntt_workspace *scratch)
{ return poly_from_ntt_ws(result, a, scratch); }
int poly_ntt_basemul_raw_ws(poly_ntt *result, const poly_ntt *a, const poly_ntt *b, bit_ntt_workspace *scratch)
{ return ntt_basemul_raw_ws(result->coeffs, a->coeffs, b->coeffs, scratch); }
int poly_ntt_dot3_raw_ws(poly_ntt *result, const poly_ntt a[BIT_L], const poly_ntt b[BIT_L], bit_ntt_workspace *scratch)
{
    poly_ntt tmp;
    int rc = -1;
    if (poly_ntt_basemul_raw_ws(result, &a[0], &b[0], scratch) != 0) goto done;
    for (int j = 1; j < BIT_L; ++j) {
        if (poly_ntt_basemul_raw_ws(&tmp, &a[j], &b[j], scratch) != 0) goto done;
        for (int i = 0; i < BIT_N; ++i) result->coeffs[i] = (int16_t)((int32_t)result->coeffs[i] + tmp.coeffs[i]);
    }
    rc = 0;
done:
    bit_secure_zero(&tmp, sizeof(tmp));
    return rc;
}
int poly_ntt_montgomery_lift_ws(poly_ntt *result, bit_ntt_workspace *scratch)
{ return ntt_montgomery_lift_ws(result->coeffs, scratch); }

void poly_add(poly *result, const poly *a, const poly *b)
{
    for (int i = 0; i < BIT_N; i++) {
        result->coeffs[i] = reduce_signed_barrett((int32_t)a->coeffs[i] + b->coeffs[i]);
    }
}

void poly_add_no_reduce(poly *result, const poly *a, const poly *b)
{
    for (int i = 0; i < BIT_N; i++) {
        result->coeffs[i] = (int16_t)((int32_t)a->coeffs[i] + b->coeffs[i]);
    }
}

void poly_add_small(poly *result, const poly *a, const poly *small)
{
    for (int i = 0; i < BIT_N; i++) {
        int32_t val = (int32_t)a->coeffs[i] + small->coeffs[i];

        result->coeffs[i] = reduce_signed_barrett(val);
    }
}

void poly_highbits(poly *w1, const poly *w)
{
    for (int i = 0; i < BIT_N; i++) {
        decompose_hint(&w1->coeffs[i], w->coeffs[i]);
    }
}

void poly_canonicalize(poly *b)
{
    for (int i = 0; i < BIT_N; i++)
        b->coeffs[i] = reduce_to_unsigned(b->coeffs[i]);
}

void poly_pack_w1(unsigned char *r, const poly *a)
{
    bit_pack_w1_5(r, a->coeffs);
}

int poly_challenge(poly *c, const unsigned char seed[BIT_CHALLENGEBYTES])
{
    unsigned int i, b, pos;
    uint8_t buf[BIT_XOF256_RATE];
    uint8_t sign_bits[(BIT_TAU + 7) / 8];
    bit_xof256_ctx ctx;

    bit_xof256_ctx_init(&ctx, seed, BIT_CHALLENGEBYTES);
    if (bit_xof256_ctx_squeezeblocks(&ctx, buf, 1) != 0) {
        bit_xof256_ctx_zeroize(&ctx);
        bit_secure_zero(buf, sizeof(buf));
        return -1;
    }
    for (size_t j = 0; j < sizeof(sign_bits); j++)
        sign_bits[j] = buf[j];
    BIT_CT_HAETAE_PUBLIC(sign_bits, sizeof sign_bits);
    pos = (unsigned int)sizeof(sign_bits);

    for (i = 0; i < BIT_N; ++i) {
        c->coeffs[i] = 0;
    }

    for (i = BIT_N - BIT_TAU; i < BIT_N; ++i) {
        do {
            if (pos >= BIT_XOF256_RATE) {
                if (bit_xof256_ctx_squeezeblocks(&ctx, buf, 1) != 0) {
                    bit_xof256_ctx_zeroize(&ctx);
                    bit_secure_zero(buf, sizeof(buf));
                    return -1;
                }
                pos = 0;
            }

            b = buf[pos++];
            BIT_CT_HAETAE_PUBLIC(&b, sizeof b);
        } while (b > i);

        c->coeffs[i] = c->coeffs[b];
        {
            unsigned int t = i - (BIT_N - BIT_TAU);
            c->coeffs[b] = 1 - 2 * ((sign_bits[t >> 3] >> (t & 7)) & 1U);
        }
    }
    bit_xof256_ctx_zeroize(&ctx);
    return 0;
}

void poly_challenge_to_sparse(sparse_challenge *s, const poly *c)
{
    int ctr = 0;

    s->bflip = 0;

    for (int i = 0; i < BIT_N; i++) {
        if (c->coeffs[i] != 0) {
            s->pos[ctr] = (uint8_t)i;
            s->sign[ctr] = (int8_t)c->coeffs[i];
            ctr++;
            if (ctr == BIT_TAU) {
                break;
            }
        }
    }
}

void poly_cneg(poly *a, uint8_t b)
{
    const int32_t mask = -(int32_t)b;

    for (int i = 0; i < BIT_N; i++) {
        a->coeffs[i] = (int16_t)(((int32_t)a->coeffs[i] ^ mask) - mask);
    }
}

void poly_mul_challenge_no_reduce(poly *result, const poly *a, const sparse_challenge *c)
{
    int16_t acc[BIT_N];
    const int16_t mask = (int16_t)(-(int32_t)c->bflip);

    for (int i = 0; i < BIT_N; i++) {
        acc[i] = 0;
    }

    for (int t = 0; t < BIT_TAU; t++) {
        int i = c->pos[t];
        if (c->sign[t] > 0) {
            for (int j = 0; j < BIT_N - i; j++)
                acc[i + j] += a->coeffs[j];
            for (int j = BIT_N - i; j < BIT_N; j++)
                acc[i + j - BIT_N] -= a->coeffs[j];
        } else {
            for (int j = 0; j < BIT_N - i; j++)
                acc[i + j] -= a->coeffs[j];
            for (int j = BIT_N - i; j < BIT_N; j++)
                acc[i + j - BIT_N] += a->coeffs[j];
        }
    }

    for (int i = 0; i < BIT_N; i++) {
        result->coeffs[i] = (int16_t)((acc[i] ^ mask) - mask);
    }
}

int poly_reject_highbits_bimodal(const poly *w1, const poly *w0, const sparse_challenge *c)
{
    uint32_t reject_flag = 0;
    const int32_t signed_one = 1 - 2 * (int32_t)c->bflip;

    for (int t = 0; t < BIT_TAU; t++) {
        int i = c->pos[t];
        int16_t hb;
        int32_t w_prime_val = (int32_t)w0->coeffs[i] + (int32_t)c->sign[t] * signed_one;

        decompose_hint(&hb, w_prime_val);
        reject_flag |= (uint32_t)((uint16_t)w1->coeffs[i] ^ (uint16_t)hb);
    }

    return (reject_flag != 0) ? 1 : 0;
}

int poly_to_ntt(poly_ntt *result, const poly *a)
{
    bit_ntt_workspace *scratch = malloc(sizeof(*scratch));
    if (scratch == NULL) return -1;
    int rc = bit_ntt_init() == 0 ? poly_to_ntt_ws(result, a, scratch) : -1;
    bit_secure_zero(scratch, sizeof(*scratch)); free(scratch);
    return rc;
}

int poly_to_ntt_centered(poly_ntt *result, poly *a)
{
    bit_ntt_workspace *scratch = malloc(sizeof(*scratch));
    if (scratch == NULL) return -1;
    int rc = bit_ntt_init() == 0 ? poly_to_ntt_centered_ws(result, a, scratch) : -1;
    bit_secure_zero(scratch, sizeof(*scratch)); free(scratch);
    return rc;
}

int poly_from_ntt(poly *result, const poly_ntt *a)
{
    bit_ntt_workspace *scratch = malloc(sizeof(*scratch));
    if (scratch == NULL) return -1;
    int rc = bit_ntt_init() == 0 ? poly_from_ntt_ws(result, a, scratch) : -1;
    bit_secure_zero(scratch, sizeof(*scratch)); free(scratch);
    return rc;
}

int poly_from_ntt_inplace(poly *result, poly_ntt *a)
{
    bit_ntt_workspace *scratch = malloc(sizeof(*scratch));
    if (scratch == NULL) return -1;
    int rc = bit_ntt_init() == 0 ? poly_from_ntt_inplace_ws(result, a, scratch) : -1;
    bit_secure_zero(scratch, sizeof(*scratch)); free(scratch);
    return rc;
}

int poly_ntt_basemul_raw(poly_ntt *result, const poly_ntt *a, const poly_ntt *b)
{
    bit_ntt_workspace *scratch = malloc(sizeof(*scratch));
    if (scratch == NULL) return -1;
    int rc = bit_ntt_init() == 0 ? poly_ntt_basemul_raw_ws(result, a, b, scratch) : -1;
    bit_secure_zero(scratch, sizeof(*scratch)); free(scratch);
    return rc;
}

int poly_ntt_dot3_raw(poly_ntt *result, const poly_ntt a[BIT_L], const poly_ntt b[BIT_L])
{
    bit_ntt_workspace *scratch = malloc(sizeof(*scratch));
    if (scratch == NULL) return -1;
    int rc = bit_ntt_init() == 0 ? poly_ntt_dot3_raw_ws(result, a, b, scratch) : -1;
    bit_secure_zero(scratch, sizeof(*scratch)); free(scratch);
    return rc;
}

int poly_ntt_montgomery_lift(poly_ntt *result)
{
    bit_ntt_workspace *scratch = malloc(sizeof(*scratch));
    if (scratch == NULL) return -1;
    int rc = bit_ntt_init() == 0 ? poly_ntt_montgomery_lift_ws(result, scratch) : -1;
    bit_secure_zero(scratch, sizeof(*scratch)); free(scratch);
    return rc;
}
