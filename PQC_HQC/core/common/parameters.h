#ifndef PQC_HQC_PARAMETERS_SELECTOR_H
#define PQC_HQC_PARAMETERS_SELECTOR_H

#if defined(HQC_LEVEL_HQC1)
#include "../hqc1/parameters.h"
#elif defined(HQC_LEVEL_HQC3)
#include "../hqc3/parameters.h"
#elif defined(HQC_LEVEL_HQC5)
#include "../hqc5/parameters.h"
#else
#error "HQC level selection macro is not defined"
#endif

#endif
