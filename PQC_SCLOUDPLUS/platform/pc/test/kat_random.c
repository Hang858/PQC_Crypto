#include <stdint.h>
#include <stddef.h>

static uint64_t kat_state;

void kat_random_init(uint64_t seed)
{
    kat_state = seed ^ 0x9e3779b97f4a7c15ULL;
    if (kat_state == 0) {
        kat_state = 0x2545f4914f6cdd1dULL;
    }
}

int randombytes(unsigned char *buffer, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++) {
        kat_state ^= kat_state >> 12;
        kat_state ^= kat_state << 25;
        kat_state ^= kat_state >> 27;
        buffer[i] = (unsigned char)((kat_state * 0x2545f4914f6cdd1dULL) >> 56);
    }
    return 0;
}

int OP_trng(void *buffer, int size)
{
    if (buffer == 0 || size < 0) {
        return -1;
    }
    return randombytes((unsigned char *)buffer, (unsigned int)size);
}
