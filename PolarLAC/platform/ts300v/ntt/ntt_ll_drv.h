#ifndef _NTT_LL_DRV_H_
#define _NTT_LL_DRV_H_

#include <stdint.h>


// Initialize the NTT module with the given parameters.
//  omega:          pointer to the array of roots of unity, pass NULL in poly-add and poly-sub or if omega is not needed to update
//  omega_length:   length of the omega array, pass 0 in poly-add and poly-sub or if omega is not needed to update
//  mod_q:          the modulus q
//  mr_value:       the value of NTT_MR register
// Returns NTT_SUCCESS on success, NTT_ERROR on failure.
int32_t ntt_init(uint32_t const *omega, uint32_t omega_length, uint32_t mod_q, int32_t mr_value);

int32_t ntt_set_opa(void const *opa, uint32_t ram_off, uint32_t length);
int32_t ntt_set_opb(void const *opb, uint32_t ram_off, uint32_t length);
int32_t ntt_set_res_addr(uint32_t ram_off);
int32_t ntt_read_res(void *res, uint32_t ram_off, uint32_t length);

int32_t ntt_op(uint32_t mode);

#endif  // end of _NTT_LL_DRV_H_
