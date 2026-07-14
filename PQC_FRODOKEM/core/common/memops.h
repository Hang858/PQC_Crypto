#ifndef MEMOPS_H
#define MEMOPS_H

#include <stddef.h>
#include <stdint.h>

#ifdef memcpy
#undef memcpy
#endif
#ifdef memset
#undef memset
#endif

void* memcpy(void* dest, const void* src, size_t size);
void* memset(void* dest, int value, size_t size);

#endif // MEMOPS_H
