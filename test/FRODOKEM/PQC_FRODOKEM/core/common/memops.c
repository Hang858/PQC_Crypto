#include "memops.h"

#ifndef _WIN32 | _WIN64
__attribute__((optimize("O0")))
#endif
void* memcpy(void* dest, const void* src, size_t size) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;

    if (size == 0 || dest == src) {
        return dest;
    }

    while (size > 0 && (((uintptr_t)d | (uintptr_t)s) & 3U) != 0U) {
        *d++ = *s++;
        size--;
    }

    uint32_t* dw = (uint32_t*)d;
    const uint32_t* sw = (const uint32_t*)s;

    while (size >= 4) {
        *dw++ = *sw++;
        size -= 4;
    }

    d = (uint8_t*)dw;
    s = (const uint8_t*)sw;
    while (size--) {
        *d++ = *s++;
    }

    return dest;
}

#ifndef _WIN32 | _WIN64
__attribute__((optimize("O0")))
#endif
void* memset(void* dest, int value, size_t size) {
    uint8_t* d = (uint8_t*)dest;
    uint8_t byte_value = (uint8_t)value;
    uint32_t word_value = ((uint32_t)byte_value << 24) | ((uint32_t)byte_value << 16) | ((uint32_t)byte_value << 8) | byte_value;

    while (size > 0 && ((uintptr_t)d & 3U) != 0U) {
        *d++ = byte_value;
        size--;
    }

    uint32_t* dw = (uint32_t*)d;
    while (size >= 4) {
        *dw++ = word_value;
        size -= 4;
    }

    d = (uint8_t*)dw;
    while (size--) {
        *d++ = byte_value;
    }

    return dest;
}
