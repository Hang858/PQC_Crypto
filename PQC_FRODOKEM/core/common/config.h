/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: fixed portable reference configuration
*********************************************************************************************/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>

#define USE_REFERENCE
#define USE_SHAKE128_FOR_A

#define ALIGN_HEADER(N)
#define ALIGN_FOOTER(N)

static inline uint16_t
LE_TO_UINT16(uint16_t x)
{
	const uint8_t *p = (const uint8_t *)&x;

	return (uint16_t)(p[0] | ((uint16_t)p[1] << 8));
}

static inline uint16_t
UINT16_TO_LE(uint16_t x)
{
	uint16_t y;
	uint8_t *p = (uint8_t *)&y;

	p[0] = (uint8_t)x;
	p[1] = (uint8_t)(x >> 8);
	return y;
}

#endif
