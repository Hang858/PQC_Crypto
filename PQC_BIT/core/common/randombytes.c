#include "randombytes.h"
#include "operator_interface.h"
#include "symmetric.h"
#include <limits.h>
#include <stdint.h>

int bit_randombytes(void *output, size_t bytes)
{
    if (output == NULL && bytes != 0) return -1;
    unsigned char *out = output;
    size_t left = bytes;
    while (left != 0) {
        int n = left > INT_MAX ? INT_MAX : (int)left;
        if (OP_trng(out, n) != OP_SUCCESS) {
            bit_secure_zero(output, bytes);
            return -1;
        }
        out += n;
        left -= (size_t)n;
    }
    return 0;
}

int bit_random_init(void)
{
    unsigned char probe[32];
    int rc = bit_randombytes(probe, sizeof(probe));
    bit_secure_zero(probe, sizeof(probe));
    return rc;
}
