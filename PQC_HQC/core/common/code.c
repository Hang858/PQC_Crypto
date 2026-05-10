/**
 * @file code.c
 * @brief Implementation of concatenated code
 */

#include "code.h"
#include <stdint.h>
#include <stdlib.h>
#include "crypto_memset.h"
#include "parameters.h"
#include "reed_muller.h"
#include "reed_solomon.h"
#include "hqc_log.h"
#ifdef VERBOSE
#include "vector.h"
#endif

/**
 *
 * @brief Encoding the message m to a code word em using the concatenated code
 *
 * First we encode the message using the Reed-Solomon code, then with the duplicated Reed-Muller code we obtain
 * a concatenated code word.
 *
 * @param[out] em Pointer to an array that is a code word
 * @param[in] m Pointer to an array that is the message
 */
void code_encode(uint64_t *em, const uint64_t *m) {
    uint64_t *tmp = calloc(HQC_MAX_VEC_N1_SIZE_64, sizeof(uint64_t));
    if (tmp == NULL) {
        return;
    }

    reed_solomon_encode(tmp, m);
    reed_muller_encode(em, tmp);

#ifdef VERBOSE
    HQC_LOGF("\n\nReed-Solomon code word: ");
    vect_print(tmp, VEC_N1_SIZE_BYTES);
    HQC_LOGF("\n\nConcatenated code word: ");
    vect_print(em, VEC_N1N2_SIZE_BYTES);
#endif
    // Zeroize sensitive data
    memset_zero(tmp, HQC_MAX_VEC_N1_SIZE_64 * sizeof(uint64_t));
    free(tmp);
}

/**
 * @brief Decoding the code word em to a message m using the concatenated code
 *
 * @param[out] m Pointer to an array that is the message
 * @param[in] em Pointer to an array that is the code word
 */
void code_decode(uint64_t *m, const uint64_t *em) {
    uint64_t *tmp = calloc(HQC_MAX_VEC_N1_SIZE_64, sizeof(uint64_t));
    if (tmp == NULL) {
        return;
    }

    reed_muller_decode(tmp, em);
    reed_solomon_decode(m, tmp);

#ifdef VERBOSE
    HQC_LOGF("\n\nReed-Muller decoding result (the input for the Reed-Solomon decoding algorithm): ");
    vect_print(tmp, VEC_N1_SIZE_BYTES);
#endif

    // Zeroize sensitive data
    memset_zero(tmp, HQC_MAX_VEC_N1_SIZE_64 * sizeof(uint64_t));
    free(tmp);
}
