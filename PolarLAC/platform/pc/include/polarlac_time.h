#ifndef POLARLAC_TIME_H
#define POLARLAC_TIME_H

#include <stdint.h>
#include <time.h>

#ifndef POLARLAC_CLOCK
#define POLARLAC_CLOCK() clock()
#endif

#ifndef POLARLAC_CLOCKS_PER_SEC
#define POLARLAC_CLOCKS_PER_SEC CLOCKS_PER_SEC
#endif

#ifndef POLARLAC_CPUCYCLES
#define POLARLAC_CPUCYCLES() ((uint64_t)POLARLAC_CLOCK())
#endif

#endif
