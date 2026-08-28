#ifndef POLARLAC_LOG_H
#define POLARLAC_LOG_H

#ifdef ENABLE_LOG
#include <stdio.h>
#define LOG_D(...) printf(__VA_ARGS__)
#else
#define LOG_D(...) ((void)0)
#endif

#endif
