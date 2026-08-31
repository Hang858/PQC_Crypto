/* Small, dependency-free logging helpers used by the FrodoKEM core. */
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "my_log.h"

///* Messages above this level are ignored.  Applications may change it. */
int log_level = LOG_INFO;

static const char *level_name(int level)
{
	switch (level) {
	case LOG_ERROR: return "ERROR";
	case LOG_WARN:  return "WARN";
	case LOG_INFO:  return "INFO";
	case LOG_DEBUG: return "DEBUG";
	default:        return "LOG";
	}
}

#if UART_LOGGING == 0
void print_log(int level, const char *format, ...)
{
	va_list args;

	if (format == NULL || level > log_level) {
		return;
	}

	(void)fprintf(stderr, "[%s] ", level_name(level));
	va_start(args, format);
	(void)vfprintf(stderr, format, args);
	va_end(args);
	(void)fputc('\n', stderr);
}

/* Print each byte of an array as a two-digit hexadecimal value. */
void print_array(const char *name, const uint8_t *array, size_t length)
{
	size_t i;

	if (array == NULL) {
		print_log(LOG_ERROR, "%s: <null>", name != NULL ? name : "array");
		return;
	}

	if (name != NULL) {
		(void)fprintf(stderr, "%s (%zu bytes):", name, length);
	} else {
		(void)fprintf(stderr, "array (%zu bytes):", length);
	}
	for (i = 0; i < length; ++i) {
		(void)fprintf(stderr, "%02x", (unsigned)array[i]);
	}
	(void)fputc('\n', stderr);
}

#else

#include "uart.h"
void print_log(int level, const char *format, ...) 
{
	va_list args;

	if (format == NULL || level > log_level) {
		return;
	}

	UART_SendString("[");
	UART_SendString(level_name(level));
	UART_SendString("] ");

	va_start(args, format);
	char buffer[256];
	vsnprintf(buffer, sizeof(buffer), format, args);
	UART_SendString(buffer);
	va_end(args);

	UART_SendString("\n");
}

/* Print each byte of an array as a two-digit hexadecimal value. */
void print_array(const char *name, const uint8_t *array, size_t length)
{
	size_t i;

	if (array == NULL) {
		print_log(LOG_ERROR, "%s: <null>", name != NULL ? name : "array");
		return;
	}

	if (name != NULL) {
		UART_SendString(name);
		UART_SendString(" (");
		char len_str[16];
		snprintf(len_str, sizeof(len_str), "%d", length);
		UART_SendString(len_str);
		UART_SendString(" bytes):");
	} else {
		UART_SendString("array (");
		char len_str[16];
		snprintf(len_str, sizeof(len_str), "%d", length);
		UART_SendString(len_str);
		UART_SendString(" bytes):");
	}
	for (i = 0; i < length; ++i) {
		char byte_str[3];
		snprintf(byte_str, sizeof(byte_str), "%02x", (unsigned)array[i]);
		UART_SendString(byte_str);
	}
	UART_SendString("\n");
}

#endif /* UART_LOGGING == 0 */
