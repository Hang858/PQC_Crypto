#ifndef OP_MATRIX_H
#define OP_MATRIX_H

#include "operator_interface.h"

static inline void matrix_mul_8x8(uint16_t z_out[8][8],
                                   const uint16_t x_in[8][8],
                                   const uint16_t y_in[8][8],
                                   uint16_t q) {
    ABORT_IF_FAIL(OP_matrix_mul_8x8(z_out, x_in, y_in, q));
}

#endif /* OP_MATRIX_H */
