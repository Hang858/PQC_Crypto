/**
 * @file hqc.h
 * @brief Header file for hqc.c
 */

#ifndef HQC_HQC_H
#define HQC_HQC_H

//#include <immintrin.h>
#include <stdint.h>
#include "hqc_params.h"
#include "parameters.h"
#include "level_namespace.h"
#include "parsing.h"

void hqc_pke_keygen_param(const hqc_params_t *params, uint8_t *ek_pke, uint8_t *dk_pke, uint8_t *seed);
void hqc_pke_encrypt_param(const hqc_params_t *params, ciphertext_pke_t *c_pke, const uint8_t *ek_pke, const uint64_t *m, const uint8_t *theta);
uint8_t hqc_pke_decrypt_param(const hqc_params_t *params, uint64_t *m, const uint8_t *dk_pke, const ciphertext_pke_t *c_pke);

void hqc_pke_keygen(uint8_t *ek_pke, uint8_t *dk_pke, uint8_t *seed);
void hqc_pke_encrypt(ciphertext_pke_t *c_pke, const uint8_t *ek_pke, const uint64_t *m, const uint8_t *theta);
uint8_t hqc_pke_decrypt(uint64_t *m, const uint8_t *dk_pke, const ciphertext_pke_t *c_pke);

#endif  // HQC_HQC_H
