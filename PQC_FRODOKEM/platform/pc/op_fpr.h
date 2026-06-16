#ifndef OP_FPR_H
#define OP_FPR_H

#include "operator_interface.h"

static inline void fpr_add(op_fpr_t *out, op_fpr_t a, op_fpr_t b) {
    ABORT_IF_FAIL(OP_fpr_add(a, b, out));
}

static inline void fpr_sub(op_fpr_t *out, op_fpr_t a, op_fpr_t b) {
    ABORT_IF_FAIL(OP_fpr_sub(a, b, out));
}

static inline void fpr_mul(op_fpr_t *out, op_fpr_t a, op_fpr_t b) {
    ABORT_IF_FAIL(OP_fpr_mul(a, b, out));
}

static inline void fpr_div(op_fpr_t *out, op_fpr_t a, op_fpr_t b) {
    ABORT_IF_FAIL(OP_fpr_div(a, b, out));
}

static inline void fpr_sqrt(op_fpr_t *out, op_fpr_t a) {
    ABORT_IF_FAIL(OP_fpr_sqrt(a, out));
}

static inline uint8_t fpr_lt(op_fpr_t a, op_fpr_t b) {
    uint8_t out;
    ABORT_IF_FAIL(OP_fpr_lt(a, b, &out));
    return out;
}

#endif /* OP_FPR_H */
