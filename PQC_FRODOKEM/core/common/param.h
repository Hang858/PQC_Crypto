#ifndef PQC_FRODOKEM_PARAM_H
#define PQC_FRODOKEM_PARAM_H

#if defined(FRODOKEM_LEVEL_640)
#include "../frodo640/level.h"
#define FRODOKEM_SHAKE shake128
#elif defined(FRODOKEM_LEVEL_976)
#include "../frodo976/level.h"
#define FRODOKEM_SHAKE shake256
#elif defined(FRODOKEM_LEVEL_1344)
#include "../frodo1344/level.h"
#define FRODOKEM_SHAKE shake256
#else
#error "FRODOKEM level not selected"
#endif

#endif
