/**
 * @file code.h
 * @brief Header file of code.c
 */

#ifndef HQC_CODE_H
#define HQC_CODE_H

#include <stddef.h>
#include <stdint.h>
#include "hqc_params.h"
#include "parameters.h"
#include "level_namespace.h"

void code_encode_param(const hqc_params_t *params, uint64_t *em, const uint64_t *m);
void code_decode_param(const hqc_params_t *params, uint64_t *m, const uint64_t *em);

void code_encode(uint64_t *em, const uint64_t *m);
void code_decode(uint64_t *m, const uint64_t *em);

#endif  // HQC_CODE_H
