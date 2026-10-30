/*
 * Copyright (c) 2026 Hang Zhang.
 * State Key Laboratory of Cyberspace Security Defense,
 * Institute of Information Engineering, CAS
 * School of Cyber Security, University of Chinese Academy of Sciences
 */
#ifndef REDUCE_H
#define REDUCE_H

#include <stdint.h>
#include "params.h"

/* Mod-q reduction and centered/unsigned conversion. */

/* Fast mod-q reduction: 0 <= a < 2q, result in [0, q) */
static inline int16_t reduce_modq(int32_t a)
{
    a -= BIT_Q;
    a += (a >> 31) & BIT_Q;
    return (int16_t)a;
}

/* Barrett reduction: |a| <= 2^30, result in [0, q).
 * All BiT callers fit this bound; INT32_MIN is outside the contract because
 * the intermediate signed t*q must be representable. */
static inline uint16_t reduce_barrett(int32_t a)
{
    int32_t t;
    t = (int32_t)(((int64_t)a * BIT_BARRETT_MULT) >> BIT_BARRETT_SHIFT);
    a -= t * BIT_Q;
    /* Barrett estimate may be off by one; fold back into [0, q) */
    a += (a >> 31) & BIT_Q;
    a -= BIT_Q;
    a += (a >> 31) & BIT_Q;
    return (uint16_t)a;
}

/* For |a| <= q + floor(q/2), return a representative in [-floor(q/2), floor(q/2)]. */
static inline int16_t reduce_signed_modq(int32_t a)
{
    a += BIT_Q & ((a + BIT_Q_HALF) >> 31);
    a -= BIT_Q & ((BIT_Q_HALF - a) >> 31);
    return (int16_t)a;
}

/* Format conversion */
static inline uint16_t reduce_to_unsigned(int16_t c)
{
    return (uint16_t)(c + ((c >> 15) & BIT_Q));
}

static inline int16_t reduce_to_signed(uint16_t t)
{
    int32_t t32 = t;
    int32_t mask = ((BIT_Q / 2) - t32) >> 31;
    return (int16_t)(t32 - (BIT_Q & mask));
}

/* Barrett reduction: |a| <= 2^30, result in [-q/2, q/2). */
static inline int16_t reduce_signed_barrett(int32_t a)
{
    return reduce_to_signed(reduce_barrett(a));
}

#endif
