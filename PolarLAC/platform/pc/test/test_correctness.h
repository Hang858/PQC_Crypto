#include <stdint.h>

//test correctness of original_pke_dec
int32_t test_pke_correctness();

//test kem fo correctness
int32_t test_kem_fo_correctness();

//calculate error bit number
int64_t error_bit_num(unsigned char *k1, unsigned char *k2, int32_t num);

//print bytes
int32_t print_bytes(unsigned char *buf, int32_t len);

int32_t test_mul_correctness();
