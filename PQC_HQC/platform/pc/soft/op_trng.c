#include <stdint.h>
#include "operator_interface.h"

int OP_trng(void *buffer, int size)
{
    static uint64_t state = 0x9e3779b97f4a7c15ULL;
    uint8_t *out = (uint8_t *)buffer;

    if (buffer == 0 || size < 0) {
        return OP_FAILURE;
    }

    for (int i = 0; i < size; i++) {
        state ^= state >> 12;
        state ^= state << 25;
        state ^= state >> 27;
        out[i] = (uint8_t)((state * 0x2545f4914f6cdd1dULL) >> 56);
    }

    return OP_SUCCESS;
}
