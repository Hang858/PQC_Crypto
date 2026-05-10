#include <stdint.h>
#include "operator_interface.h"

int OP_matrix_mul_8x8(uint16_t z_out[8][8], const uint16_t x_in[8][8], const uint16_t y_in[8][8], uint16_t q)
{
    uint16_t mask;

    if (z_out == 0 || x_in == 0 || y_in == 0) {
        return OP_FAILURE;
    }

    mask = (uint16_t)(q - 1u);
    for (uint16_t i = 0; i < 8; i++) {
        for (uint16_t j = 0; j < 8; j++) {
            uint16_t sum = 0;
            for (uint16_t k = 0; k < 8; k++) {
                sum = (uint16_t)(sum + x_in[i][k] * y_in[k][j]);
            }
            z_out[i][j] = (uint16_t)(sum & mask);
        }
    }

    return OP_SUCCESS;
}
