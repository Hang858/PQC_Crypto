#ifndef HQC_LOG_H
#define HQC_LOG_H

#ifdef VERBOSE
#include <stdio.h>
#define HQC_LOGF(...) printf(__VA_ARGS__)
#else
#define HQC_LOGF(...) ((void)0)
#endif

#endif
