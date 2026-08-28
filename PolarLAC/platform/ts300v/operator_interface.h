/**
 * Last updated: 2026-01-12 14:31
 * @file operator_interface.h
 * @brief Hardware operator abstract interface summary for software team to adapt algorithms.
 * * Based on the "Hardware Operator Summary" document.
 */

#ifndef HARDWARE_DRIVER_H
#define HARDWARE_DRIVER_H

#include <stdint.h>
#include <stddef.h>
#include "config.h"

 /* --- Macros and Constants --- */

//To be added
#define OP_SUCCESS 0
#define OP_FAILURE -1

#define MAX_NTT_OMEGA_COUNT 8  // Maximum 8 twiddle factors (4 forward, 4 inverse)

//1:SHA256; 2:SM3; 3:SHAKE256; TBD: 4:SHAKE128; 5:SHA3-256; 6:SHA3-384; 7:SHA3-512
#define OP_ALG_SHA256   1
#define OP_ALG_SM3      2
#define OP_ALG_SHAKE256 3
#define OP_ALG_SHAKE128 4
#define OP_ALG_SHA3_256 5
#define OP_ALG_SHA3_384 6
#define OP_ALG_SHA3_512 7
#define OP_ALG_SHA3_224 8

#define OP_MODE_NORMAL 0
#define OP_MODE_LMS 1
#define OP_MODE_SPHINCS 2

#define OP_STATE_SIZE_SM3 104
#define OP_STATE_SIZE_SHA3 208

 #if defined(windows) || defined(_WIN32) || defined(_WIN64) || defined (LINUX) || defined(__linux__) || defined(__linux)
    #include <stdlib.h>
    #define ABORT_IF_FAIL(x) if ((x) != OP_SUCCESS) { \
         abort(); \
    }
#else
    #define ABORT_IF_FAIL(x)
#endif

#ifndef NTT_HARDWARE_IMPL
 /* --- Hardware Operator Interface Declarations --- */
 /**
 * @brief Operator 1.0. Initialization
 * @param omega  Input: Twiddle factor array (count factors, each containing 256 32-bit elements. Forward factors first, then inverse factors)
 * @param count  Input: Number of twiddle factors (multiple of 2, max 8)
 * @param q      Input: Modulus (refer to 8380417)
 * @param inv_q  Input: q^(-1) mod 2^32
 * @param inv_n  Input: 256^(-1) mod q
 * @param k      Input: Remaining layers (0, 1, 2, 3)
 * @return       0: success; -1: failure.
 */
int32_t OP_ntt256_init(const int32_t *omega, int32_t count, int32_t q, int32_t inv_q, int32_t inv_n, int32_t k);


 /**
 * @brief Operator 1.1. Forward Number Theoretic Transform (NTT)
 * * Transform polynomial from time domain to frequency domain.
 * * TBD: Decompose to third last layer, or add parameter to dynamically determine stop layer.
 * 
 * @param a_out  Output: Transformed coefficient array (256 32-bit elements)
 * @param a_in   Input: Polynomial coefficients to transform (256 32-bit elements)
 * @param omega_index Input: Twiddle factor index to use (0 to count/2 - 1)
 * @return       0: success; -1: failure.
 */
int32_t OP_ntt256(int32_t a_out[256], const int32_t a_in[256], int32_t omega_index);


/**
 * @brief Operator 1.2. Inverse Number Theoretic Transform (INTT)
 * * Transform polynomial from frequency domain back to time domain.
 * 
 * @param a_out  Output: Recovered time domain polynomial (256 32-bit elements)
 * @param a_in   Input: Frequency domain polynomial coefficients (256 32-bit elements)
 * @param omega_index Input: Twiddle factor index to use (count/2 to count - 1)
 * @return       0: success; -1: failure.
 */
int32_t OP_intt256(int32_t a_out[256], const int32_t a_in[256], int32_t omega_index);


/**
 * @brief Operator 1.3. Polynomial Multiplication (CWM - Coefficient-Wise Multiplication)
 * * Perform coefficient-wise modular multiplication on two polynomials in frequency domain.
 * 
 * @param c_out  Output: Point-wise multiplication result (n 32-bit elements)
 * @param a_in   Input: Frequency domain representation of polynomial A (n 32-bit elements)
 * @param b_in   Input: Frequency domain representation of polynomial B (n 32-bit elements)
 * @param n      Input: Dimension (max 1024)
 * @param q      Input: Modulus (up to 31 bits)
 * @param k      Input: Remaining layers (hardware supports 0, 1; software implements 2, 3)
 * @return       0: success; -1: failure.
 */
int32_t OP_cwm(int32_t *c_out, const int32_t *a_in, const int32_t *b_in, int32_t n, int32_t q, int32_t k);
#else

#include "ntt_adapter.h"

#endif


/**
 * @brief Operator 2. Vector Multiplication (Variable element count)
 * * Z = (X * Y) mod q
 * 
 * @param z_out     Output: One 16-bit element.
 * @param x_in      Input: Vector (length 16-bit elements).
 * @param y_in      Input: Vector (length 16-bit elements).
 * @param length    Input: Vector length. (Max: Frodo: 1344, scloudplus: 1136)
 * @param q         Input: Modulus (16-bit) power of 2.
 * @return          0: success; -1: failure.
 * TBD: Unsigned data
 */
int32_t OP_vector_mul(uint16_t *z_out, const uint16_t *x_in, const uint16_t *y_in, uint16_t length, uint16_t q);


#ifndef SHA3_HARDWARE_IMPL
/**
 * @brief Operator 3.1 Single-Packet Hash
 * 
 * @param alg        Input: Hash algorithm type. (1:SHA256; 2:SM3; 3:SHAKE256; TBD:4:SHAKE128; 5:SHA3-256; 6:SHA3-384; 7:SHA3-512)
 * 
 * @param mode       Input: Hash algorithm mode. (0:normal; 1:hash chain)
 * 
 * Note: Hash chain mode only supports SM3 and SHAKE256
 * 
 * @param n          Input: Output hash length (bytes). (SHAKE supports arbitrary length, e.g., SHAKE256 max 1000 bytes, others <= fixed output length)
 * @param input      Input: Pointer to data buffer to hash.
 * @param input_len  Input: Length of data to hash (bytes).
 * @param link_count    Input: Number of hash iterations in hash chain mode. (Only for hash chain, default 0 for others)
 * @param output     Output: Pointer to buffer for hash result.
 * @return           0: success; -1: failure.
 */
int32_t OP_hash(uint8_t alg, uint8_t mode, int32_t n, const void *input, int32_t input_len, uint8_t link_count, void *output);

/**
 * @brief Operator 3.2 Multi-Packet Hash Initialization (software: clear to zero)
 * 
 * @param alg        Input: Hash algorithm type. (1:SHA256; 2:SM3; 3:SHAKE256; TBD:4:SHAKE128; 5:SHA3-256; 6:SHA3-384; 7:SHA3-256)
 * @param s      Input/Output: Hash state.
 * @param s_len      Input: Length of hash state (bytes).
 * @return           0: success; -1: failure.
 */
int32_t OP_hash_init(uint8_t alg, void *s, int32_t s_len);
/**
 * @brief Operator 3.3 Multi-Packet Hash Sponge Absorb (can be called multiple times)
 * 
 * @param alg        Input: Hash algorithm type. (1:SHA256; 2:SM3; 3:SHAKE256; TBD:4:SHAKE128; 5:SHA3-256; 6:SHA3-384; 7:SHA3-256)
 * @param s      Input/Output: Hash state.
 * @param s_len      Input: Length of hash state (bytes).
 * @param input      Input: Pointer to data buffer to hash.
 * @param input_len  Input: Length of data to hash (bytes).
 * @return           0: success; -1: failure.
  */
int32_t OP_hash_absorb(uint8_t alg, void *s, int32_t s_len, const void *input, int32_t input_len);
/**
 * @brief Operator 3.4 Multi-Packet Hash Output (SHAKE can be called multiple times, others only once)
 * 
 * @param alg        Input: Hash algorithm type. (1:SHA256; 2:SM3; 3:SHAKE256; TBD:4:SHAKE128; 5:SHA3-256; 6:SHA3-384; 7:SHA3-256)
 * @param s      Input/Output: Hash state.
 * @param s_len      Input: Length of hash state (bytes).
 * @param output     Output: Pointer to buffer for hash result.
 * @param output_len      Input: Length of hash result buffer (bytes).
 * @return           0: success; -1: failure.
  */
int32_t OP_hash_squeeze(uint8_t alg, void *s, int32_t s_len, void *output, int32_t output_len);
#else

#include "sha3_interface.h"
#define OP_hash(alg, mode, n, input, input_len, link_count, output)     sha3_hash(alg, input, input_len, output, n)
#define OP_hash_init(alg, s, s_len)                                     sha3_init(alg, s, s_len)
#define OP_hash_absorb(alg, s, s_len, input, input_len)                 sha3_absorb(alg, s, s_len, input, input_len)
#define OP_hash_squeeze(alg, s, s_len, output, output_len)              sha3_squeeze(alg, s, s_len, output, output_len)

#endif

 /**
 * @brief Operator 4. Hardware Random Number Generation (Rejection Sampling)
 * Internal implementation:
 * Call hardware TRNG module to generate true random seed;
 * Generate pseudo-random numbers based on the seed.
 * 
 * @param buffer  Output: Buffer to store random numbers.
 * @param size    Input: Length of random numbers needed.
 * @return        0: success; -1: hardware failure/insufficient entropy.
 * TBD: Should interface directly call hardware TRNG?
 * TBD: Pilot algorithm not determined
 */
int OP_trng(void *buffer, int size);


/**
 * @brief Operator 5. Finite Field Operations
 *  0: Modular Add     C=(a+b) mod n
 *  1: Modular Sub     C=(a-b) mod n
 *  2: Modular Mul     C=(a*b) mod n
 *  3: Modular Pow(Inv) C=a^b mod n
 * @param c_out    Output: Result C (big integer, byte array, little-endian)
 * @param opr  Input: Operation type (0: mod add; 1: mod sub; 2: mod mul; 3: mod pow/inv)
 * @param a_in     Input: Base a (big integer, byte array, little-endian)
 * @param b_in     Input: Exponent b (big integer, byte array, little-endian)
 * @param n_in     Input: Modulus n (big integer, byte array, little-endian; software only needs to support 5*2^248-1, 65*2^376-1, 27*2^500-1)
 * @param len      Input: Byte length of big integers
 * @return         0: success; -1: failure.
 */
int32_t OP_finite_field(uint8_t *c_out, uint8_t opr, const uint8_t *a_in, const uint8_t *b_in, const uint8_t *n_in,  int32_t len);
 #endif
