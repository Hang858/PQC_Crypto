/**
* @file parameters.h
* @brief Parameters of the HQC-KEM IND-CCA2 scheme
 */

#ifndef PQC_HQC_HQC1_PARAMETERS_H
#define PQC_HQC_HQC1_PARAMETERS_H

#include "level.h"
#include "../common/params_common.h"

#define PARAM_N                     17669       ///< Define the parameter n of the scheme
#define PARAM_N1                    46          ///< Define the parameter n1 of the scheme (length of Reed–Solomon code)
#define PARAM_N2                    384         ///< Define the parameter n2 of the scheme (length of Duplicated Reed–Muller code)
#define PARAM_N1N2                  17664       ///< Define the length in bits of the concatenated code
#define PARAM_OMEGA                 66          ///< Define the parameter omega of the scheme
#define PARAM_OMEGA_E               75          ///< Define the parameter omega_e of the scheme
#define PARAM_OMEGA_R               75          ///< Define the parameter omega_r of the scheme
#define PARAM_SECURITY              128         ///< Define the security level corresponding to the chosen parameters
#define PARAM_SECURITY_BYTES        16          ///< Define the security level in bytes
#define PARAM_DFR_EXP               128         ///< Define the decryption failure rate exponent

#define PARAM_DELTA                 15          ///< Define the error-correcting capacity (delta) of the Reed–Solomon code
#define PARAM_M                     8           ///< Define the degree m of the Galois field GF(2^m)
#define PARAM_GF_POLY               0x11D       ///< Generator polynomial of GF(2^PARAM_M) in hexadecimal form
#define PARAM_GF_MUL_ORDER          255         ///< Size of the multiplicative group of GF(2^PARAM_M) (2^PARAM_M−1)
#define PARAM_K                     16          ///< Define the size of the information bits of the Reed–Solomon code
#define PARAM_G                     31          ///< Define the size of the generator polynomial of the Reed–Solomon code
#define PARAM_FFT                   4           ///< Exponent for additive FFT (2^PARAM_FFT points)

#define RS_POLY_COEFS                                                                                                \
    89,  69, 153, 116, 176, 117, 111, 75,  73, 233, 242, 233, 65,  210, 21,  139, 103, 173, 67,  118,                   \
    105, 210, 174, 110, 74,  69, 228, 82,  255, 181, 1                                                     ///< Coefficients of the Reed–Solomon generator polynomial

#define SEED_BYTES                  32          ///< Define the size of the seed in bytes
#define SALT_BYTES                  16          ///< Define the size of a salt in bytes

#define PARAM_N_MU 243079ULL   ///<  Define a precomputed multiplier for Barrett reduction mu = floor(2^32 / PARAM_N)
#define UTILS_REJECTION_THRESHOLD             16767881  ///< Rejection threshold for uniform sampling in [0, PARAM_N)

#endif
