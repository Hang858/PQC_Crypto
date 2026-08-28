#ifndef NTT_DRV_H
#define NTT_DRV_H

#include <stdint.h>

#define NTT_SUCCESS 0
#define NTT_ERROR -1

extern const uint32_t DILITHIUM_OMEGA[];

//// Perform polynomial addition of two polynomials opa and opb.
////  opa:            pointer to the first polynomial coefficients
////  opb:            pointer to the second polynomial coefficients
////  res:            pointer to the result polynomial coefficients
////  length:         number of coefficients in the polynomials, max 0x1000
//// Returns NTT_SUCCESS on success, NTT_ERROR on failure.
//int32_t poly_add(void const *opa, void const *opb, void *res, uint32_t length);
//
//// Perform polynomial subtraction of two polynomials opa and opb.
////  opa:            pointer to the first polynomial coefficients
////  opb:            pointer to the second polynomial coefficients
////  res:            pointer to the result polynomial coefficients
////  length:         number of coefficients in the polynomials, max 0x1000
//// Returns NTT_SUCCESS on success, NTT_ERROR on failure.
//int32_t poly_sub(void const *opa, void const *opb, void *res, uint32_t length);

// Perform forward NTT on the input polynomial indata.
//  indata:         pointer to the input polynomial coefficients
//  res:            pointer to the result polynomial coefficients
//  length:         number of coefficients in the polynomial, max 0x1000
// Returns NTT_SUCCESS on success, NTT_ERROR on failure.
int32_t ntt(void const *indata, void *res, uint32_t length);

// Perform inverse NTT on the input polynomial indata.
//  indata:         pointer to the input polynomial coefficients
//  res:            pointer to the result polynomial coefficients
//  length:         number of coefficients in the polynomial, max 0x1000
// Returns NTT_SUCCESS on success, NTT_ERROR on failure.
int32_t intt(void const *indata, void *res, uint32_t length);

// Perform polynomial multiplication of two polynomials opa and opb.
//  opa:            pointer to the first polynomial coefficients
//  opb:            pointer to the second polynomial coefficients
//  res:            pointer to the result polynomial coefficients
//  length:         number of coefficients in the polynomials, max 0x1000
//  l:              0 for pwm8, 1 for pwm7
// Returns NTT_SUCCESS on success, NTT_ERROR on failure.
int32_t pwm(void const *opa, void const *opb, void *res, uint32_t length, uint32_t l);

#endif // NTT_DRV_H
