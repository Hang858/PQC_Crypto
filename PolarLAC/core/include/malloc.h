#ifndef MALLOC_H
#define MALLOC_H

#include <stdint.h>
#include <stdlib.h>

// Clear any existing malloc/free/calloc definitions from system headers
#ifdef malloc
#undef malloc
#endif
#ifdef free
#undef free
#endif
#ifdef calloc
#undef calloc
#endif
#ifdef realloc
#undef realloc
#endif

// Define heap memory size (simulated heap)
#define HEAP_SIZE (1024 * 32)  // 32 KB heap size

#define ENABLE_TEST     1  // Enable test functions for heap statistics

/**
 * Custom malloc implementation
 * @param size requested allocation size in bytes
 * @return pointer to allocated memory, or NULL on failure
 */
void* my_malloc(size_t size);
#define malloc(size)    my_malloc(size)

#define calloc(nmemb, size)    my_malloc(nmemb * size)

/**
 * Custom free implementation
 * @param ptr pointer to previously allocated memory
 */
void my_free(void* ptr);
#define free(ptr)      my_free(ptr)

#if ENABLE_TEST
size_t get_total_free_memory(void);
size_t get_total_allocated_memory(void);
uint32_t get_total_block_count(void);
void reset_max_heap_usage(void);
size_t get_max_heap_usage(void);
#else
#define reset_max_heap_usage() ((void)0)
#define get_max_heap_usage() ((size_t)0)
#endif // ENABLE_TEST

#endif // MALLOC_H
