#ifndef _LOG_H_
#define _LOG_H_

#include "my_config.h"

#if ENABLE_LOGGING == 1

#include <stdint.h>

enum {
	LOG_ERROR = 0,
	LOG_WARN  = 1,
	LOG_INFO  = 2,
	LOG_DEBUG = 3
};

void print_log(int level, const char *format, ...);
void print_array(const char *name, const uint8_t *array, size_t length);

#define LOG_D(format, ...) 			print_log(LOG_DEBUG, "%s:%d: " format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_A(name, array, len)		print_array(name, array, len)

#else

#define LOG_D(format, ...)
#define LOG_A(name, array, len)

#endif /* _ENABLE_LOGGING_ */

#endif
