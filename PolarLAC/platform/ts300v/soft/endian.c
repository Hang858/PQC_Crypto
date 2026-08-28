#include "endian.h"

void put_bigendian( void *target, uint64_t value, size_t bytes ) {
    unsigned char *b = target;
    int32_t i;

    for (i = (int32_t)bytes - 1; i >= 0; i--) {
        b[i] = value & 0xff;
        value >>= 8;
    }
}
    
uint64_t get_bigendian( const void *target, size_t bytes ) {
    const unsigned char *b = target;
    uint64_t result = 0;
    uint32_t i;

    for (i=0; i<bytes; i++) {
        result = 256 * result + (b[i] & 0xff);
    }

    return result;
}
