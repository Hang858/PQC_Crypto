#include "memory_probe.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#if defined(__APPLE__)
#include <malloc/malloc.h>
#elif defined(__GLIBC__)
#include <malloc.h>
#endif
typedef union { struct { size_t size, host_size; } h; max_align_t align; } header;
static size_t live, peak, calls, failure, dirty, host_live, host_peak;
void probe_reset(size_t f) { peak = calls = dirty = host_peak = 0; failure = f; }
size_t probe_peak(void) { return peak; }
size_t probe_calls(void) { return calls; }
size_t probe_live(void) { return live; }
size_t probe_dirty(void) { return dirty; }
size_t probe_host_peak(void) { return host_peak; }
void *bit_test_malloc(size_t n)
{
    if (++calls == failure) return NULL;
    if (n > SIZE_MAX - sizeof(header)) return NULL;
    header *p = malloc(sizeof(header) + n);
    if (!p) return NULL;
    p->h.size = n; live += n; if (live > peak) peak = live;
#if defined(__APPLE__)
    p->h.host_size = malloc_size(p);
#elif defined(__GLIBC__)
    p->h.host_size = malloc_usable_size(p);
#else
    p->h.host_size = sizeof(header) + n;
#endif
    host_live += p->h.host_size; if (host_live > host_peak) host_peak = host_live;
    memset(p + 1, 0xa5, n);
    return p + 1;
}
void bit_test_free(void *v)
{
    if (!v) return;
    header *p = (header *)v - 1;
    const unsigned char *b = v;
    for (size_t i = 0; i < p->h.size; ++i) if (b[i] != 0) { ++dirty; break; }
    live -= p->h.size; host_live -= p->h.host_size; free(p);
}
