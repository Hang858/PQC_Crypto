#include "rand.h"
#include "operator_interface.h"
#include <stdint.h>

int32_t random_bytes(unsigned char *out, uint32_t outlen)
{
  if (out == NULL || outlen > (uint32_t)INT32_MAX) {
    return OP_FAILURE;
  }

  return OP_trng(out, (int32_t)outlen);
}
