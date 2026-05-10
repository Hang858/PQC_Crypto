#include "random.h"
#include "operator_interface.h"

int randombytes(unsigned char *random_array, unsigned long long nbytes)
{
    return OP_trng(random_array, (int)nbytes);
}
