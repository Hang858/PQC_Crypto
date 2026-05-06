#ifndef PQC_HQC_REED_SOLOMON_SELECTOR_H
#define PQC_HQC_REED_SOLOMON_SELECTOR_H

#if defined(HQC_LEVEL_HQC1)
#include "../hqc1/reed_solomon.h"
#elif defined(HQC_LEVEL_HQC3)
#include "../hqc3/reed_solomon.h"
#elif defined(HQC_LEVEL_HQC5)
#include "../hqc5/reed_solomon.h"
#else
#error "HQC level selection macro is not defined"
#endif

#endif
