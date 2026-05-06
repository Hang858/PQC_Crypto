/**
 * @file code.c
 * @brief Implementation of concatenated code
 */

#include "code.h"
#include <stdint.h>
#include "crypto_memset.h"
#include "parameters.h"
#include "reed_muller.h"
#include "reed_solomon.h"
#ifdef VERBOSE
#include <stdio.h>
#include "vector.h"
#endif

static const hqc_params_t *resolve_params(const hqc_params_t *params) {
    if (params != NULL) {
        return params;
    }
#if defined(HQC_LEVEL_HQC1)
    return HQC_get_params(HQC_1);
#elif defined(HQC_LEVEL_HQC3)
    return HQC_get_params(HQC_3);
#elif defined(HQC_LEVEL_HQC5)
    return HQC_get_params(HQC_5);
#else
    return NULL;
#endif
}

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
void code_encode_param(const hqc_params_t *params, uint64_t *em, const uint64_t *m) {
    params = resolve_params(params);
    uint64_t tmp[VEC_N1_SIZE_64] = {0};

    reed_solomon_encode(tmp, m);
    reed_muller_encode(em, tmp);

#ifdef VERBOSE
    printf("\n\nReed-Solomon code word: ");
    vect_print_param(params, tmp, params->n1);
    printf("\n\nConcatenated code word: ");
    vect_print_param(params, em, params->vec_n1n2_size_bytes);
#endif
    // Zeroize sensitive data
    memset_zero(tmp, sizeof tmp);
}

void code_encode(uint64_t *em, const uint64_t *m) {
    code_encode_param(NULL, em, m);
}

/**
 * @brief Decoding the code word em to a message m using the concatenated code
 *
 * @param[out] m Pointer to an array that is the message
 * @param[in] em Pointer to an array that is the code word
 */
void code_decode_param(const hqc_params_t *params, uint64_t *m, const uint64_t *em) {
    params = resolve_params(params);
    uint64_t tmp[VEC_N1_SIZE_64] = {0};

    reed_muller_decode(tmp, em);
    reed_solomon_decode(m, tmp);

#ifdef VERBOSE
    printf("\n\nReed-Muller decoding result (the input for the Reed-Solomon decoding algorithm): ");
    vect_print_param(params, tmp, params->n1);
#endif

    // Zeroize sensitive data
    memset_zero(tmp, sizeof tmp);
}

void code_decode(uint64_t *m, const uint64_t *em) {
    code_decode_param(NULL, m, em);
}
