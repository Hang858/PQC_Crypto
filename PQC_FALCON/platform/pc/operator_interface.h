#ifndef PQC_FALCON_OPERATOR_INTERFACE_H
#define PQC_FALCON_OPERATOR_INTERFACE_H

#include <stdint.h>

#define OP_SUCCESS 0
#define OP_FAILURE -1

typedef double op_fpr_t;

int OP_fpr_add(op_fpr_t a, op_fpr_t b, op_fpr_t *out);
int OP_fpr_sub(op_fpr_t a, op_fpr_t b, op_fpr_t *out);
int OP_fpr_mul(op_fpr_t a, op_fpr_t b, op_fpr_t *out);
int OP_fpr_div(op_fpr_t a, op_fpr_t b, op_fpr_t *out);
int OP_fpr_sqrt(op_fpr_t a, op_fpr_t *out);
int OP_fpr_lt(op_fpr_t a, op_fpr_t b, uint8_t *out);

#endif
