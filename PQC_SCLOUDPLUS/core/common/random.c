#include "random.h"
#include "operator_interface.h"

int randombytes(unsigned char *buffer, unsigned int size)
{
    return OP_trng(buffer, (int)size);
}
