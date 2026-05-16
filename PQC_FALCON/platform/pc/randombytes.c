#include <stdio.h>

void randombytes_init(unsigned char *entropy_input,
    unsigned char *personalization_string,
    int security_strength) {
    (void)entropy_input;
    (void)personalization_string;
    (void)security_strength;
}

int randombytes(unsigned char *x, unsigned long long xlen) {
    FILE *fp = fopen("/dev/urandom", "rb");
    if (fp == NULL) {
        return -1;
    }
    if (fread(x, 1, (size_t)xlen, fp) != (size_t)xlen) {
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return 0;
}
