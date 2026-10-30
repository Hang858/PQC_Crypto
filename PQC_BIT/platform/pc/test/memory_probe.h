#ifndef BIT_TEST_MEMORY_PROBE_H
#define BIT_TEST_MEMORY_PROBE_H
#include <stddef.h>
void *bit_test_malloc(size_t);
void bit_test_free(void *);
void probe_reset(size_t fail_at);
size_t probe_peak(void);
size_t probe_calls(void);
size_t probe_live(void);
size_t probe_dirty(void);
size_t probe_host_peak(void);
#endif
