/* TEST ONLY: feed the original deterministic stream through the real OP API.
 * Never included in the production library. */
#include "drng.h"
#include "operator_interface.h"
int OP_trng(void *out, int bytes)
{
    if (bytes < 0 || (out == NULL && bytes != 0)) return OP_FAILURE;
    if (bytes == 0) return OP_SUCCESS;
    return get_random_number(&drng_algorithm, out, (unsigned long long)bytes * 8ULL);
}
