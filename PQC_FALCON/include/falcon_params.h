#ifndef PQC_FALCON_PARAMS_H
#define PQC_FALCON_PARAMS_H

#include <stddef.h>

typedef enum {
    FALCON_512 = 0,
    FALCON_1024 = 1
} falcon_level_t;

typedef struct {
    const char *algname;
    unsigned logn;
    size_t secretkeybytes;
    size_t publickeybytes;
    size_t bytes;
} falcon_params_t;

#define FALCON_MAX_SECRETKEYBYTES 2305u
#define FALCON_MAX_PUBLICKEYBYTES 1793u
#define FALCON_MAX_BYTES 1330u
#define FALCON_MAX_LOGN 10u
#define FALCON_MAX_N 1024u

const falcon_params_t *Falcon_get_params(falcon_level_t level);

#endif
