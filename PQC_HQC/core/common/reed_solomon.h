#ifndef HQC_REED_SOLOMON_H
#define HQC_REED_SOLOMON_H

#include <stdint.h>

void reed_solomon_encode(uint64_t *cdw, const uint64_t *msg);
void reed_solomon_decode(uint64_t *msg, uint64_t *cdw);
void compute_generator_poly(uint16_t *poly);

#endif
