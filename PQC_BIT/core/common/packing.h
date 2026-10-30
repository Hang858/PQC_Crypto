/*
 * Copyright (c) 2026 Hang Zhang.
 * State Key Laboratory of Cyberspace Security Defense,
 * Institute of Information Engineering, CAS
 * School of Cyber Security, University of Chinese Academy of Sciences
 */
#ifndef PACKING_H
#define PACKING_H
#include "polyvec.h"

/* The format rejects combined entropy payloads above 381 bytes and hint
 * streams above 255 bytes. Oversized candidates need no larger scratch. */
typedef struct {
    uint8_t z_high[(BIT_L + 1) * BIT_N];
    uint8_t h_symbols[BIT_K * BIT_N];
    uint8_t enc_z[BIT_SIG_RANS_PAYLOAD_BYTES];
    uint8_t enc_h[255];
    uint8_t tmp[BIT_SIG_RANS_PAYLOAD_BYTES];
} bit_codec_workspace;
int pack_sig_ws(unsigned char *sig, const polyvecm1 *z1, const polyveck *h,
                 const unsigned char *challenge, bit_codec_workspace *scratch);
int unpack_sig_ws(polyvecm1 *z1, polyveck *h, unsigned char *challenge,
                   const unsigned char *sig, bit_codec_workspace *scratch);

void unpack_sk_se(polyvecl *s_0, polyveck *e, const unsigned char *validated_sk);

void pack_pk(unsigned char *pk, const unsigned char *seed_A, const polyveck *public_vector);
void pack_sk(unsigned char *sk, const unsigned char *seed_A, const polyveck *public_vector,
             const unsigned char *secret_seed, const unsigned char *tr, const polyvecl *s_0,
             const polyveck *e);
int unpack_sk(unsigned char *seed_A, polyveck *public_vector, unsigned char *secret_seed,
              unsigned char *tr, polyvecl *s_0, polyveck *e, const unsigned char *sk);

int pack_sig(unsigned char *sig, const polyvecm1 *z1, const polyveck *h,
             const unsigned char *challenge);
int unpack_pk(unsigned char *seed_A, polyveck *public_vector, const unsigned char *pk);
int unpack_sig(polyvecm1 *z1, polyveck *h, unsigned char *challenge, const unsigned char *sig);

#endif
