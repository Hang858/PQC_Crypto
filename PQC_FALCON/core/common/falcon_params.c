#include "falcon_params.h"

static const falcon_params_t FALCON_PARAM_TABLE[] = {
    {"Falcon-512", 9, 1281, 897, 690},
    {"Falcon-1024", 10, 2305, 1793, 1330},
};

const falcon_params_t *Falcon_get_params(falcon_level_t level) {
    if ((unsigned)level > (unsigned)FALCON_1024) {
        return 0;
    }
    return &FALCON_PARAM_TABLE[level];
}
