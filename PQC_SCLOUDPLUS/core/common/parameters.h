#ifndef PQC_SCLOUDPLUS_PARAMETERS_SELECTOR_H
#define PQC_SCLOUDPLUS_PARAMETERS_SELECTOR_H

#if defined(SCLOUDPLUS_LEVEL_128)
#include "../scloudplus128/parameters.h"
#elif defined(SCLOUDPLUS_LEVEL_192)
#include "../scloudplus192/parameters.h"
#elif defined(SCLOUDPLUS_LEVEL_256)
#include "../scloudplus256/parameters.h"
#else
#error "SCLOUDPLUS level selection macro is not defined"
#endif

#endif
