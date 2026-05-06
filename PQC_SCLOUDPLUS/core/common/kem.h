#ifndef _SCLOUDPLUS_KEM_H_
#define _SCLOUDPLUS_KEM_H_
#include <stdint.h>
#include "level_namespace.h"
int scloud_kemkeygen(uint8_t *pk, uint8_t *sk);
int scloud_kemencaps(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int scloud_kemdecaps(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
#endif
