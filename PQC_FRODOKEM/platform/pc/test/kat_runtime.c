#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "rng.h"

#define MAX_MARKER_LEN 50
#define KAT_SUCCESS 0
#define KAT_FILE_OPEN_ERROR -1
#define KAT_VERIFICATION_ERROR -2
#define KAT_DATA_ERROR -3
#define KAT_CRYPTO_FAILURE -4

static int find_marker(FILE *infile, const char *marker) {
    char line[MAX_MARKER_LEN];
    int len = (int)strlen(marker);
    int curr_line;

    if (len > MAX_MARKER_LEN - 1) {
        len = MAX_MARKER_LEN - 1;
    }

    for (int i = 0; i < len; i++) {
        curr_line = fgetc(infile);
        line[i] = (char)curr_line;
        if (curr_line == EOF) {
            return 0;
        }
    }
    line[len] = '\0';

    for (;;) {
        if (strncmp(line, marker, (size_t)len) == 0) {
            return 1;
        }
        for (int i = 0; i < len - 1; i++) {
            line[i] = line[i + 1];
        }
        curr_line = fgetc(infile);
        line[len - 1] = (char)curr_line;
        if (curr_line == EOF) {
            return 0;
        }
        line[len] = '\0';
    }
}

static int read_hex(FILE *infile, unsigned char *out, int length, const char *marker) {
    size_t line_len = 256;
    size_t used = 0;
    char *line = NULL;
    size_t hex_count = 0;
    int ch;
    size_t marker_len = strlen(marker);

    memset(out, 0, (size_t)length);
    line = malloc(line_len);
    if (line == NULL) {
        return 0;
    }

    for (;;) {
        used = 0;
        while ((ch = fgetc(infile)) != EOF) {
            if (used + 1 >= line_len) {
                char *new_line;
                line_len *= 2;
                new_line = realloc(line, line_len);
                if (new_line == NULL) {
                    free(line);
                    return 0;
                }
                line = new_line;
            }
            line[used++] = (char)ch;
            if (ch == '\n') {
                break;
            }
        }
        if (used == 0 && ch == EOF) {
            free(line);
            return 0;
        }
        line[used] = '\0';
        if (strncmp(line, marker, marker_len) == 0) {
            break;
        }
    }

    for (size_t i = marker_len; line[i] != '\0' && hex_count < (size_t)length * 2; i++) {
        ch = (unsigned char)line[i];
        unsigned char nibble;
        if (ch >= '0' && ch <= '9') {
            nibble = (unsigned char)(ch - '0');
        } else if (ch >= 'A' && ch <= 'F') {
            nibble = (unsigned char)(ch - 'A' + 10);
        } else if (ch >= 'a' && ch <= 'f') {
            nibble = (unsigned char)(ch - 'a' + 10);
        } else {
            continue;
        }
        if ((hex_count & 1u) == 0) {
            out[hex_count / 2] = (unsigned char)(nibble << 4);
        } else {
            out[hex_count / 2] |= nibble;
        }
        hex_count++;
    }

    free(line);
    return hex_count == (size_t)length * 2;
}

static int parse_level(const char *arg, frodokem_level_t *level) {
    if (strcmp(arg, "640") == 0 || strcmp(arg, "frodo640") == 0 || strcmp(arg, "FrodoKEM-640") == 0) {
        *level = FRODOKEM_640;
        return 0;
    }
    if (strcmp(arg, "976") == 0 || strcmp(arg, "frodo976") == 0 || strcmp(arg, "FrodoKEM-976") == 0) {
        *level = FRODOKEM_976;
        return 0;
    }
    if (strcmp(arg, "1344") == 0 || strcmp(arg, "frodo1344") == 0 || strcmp(arg, "FrodoKEM-1344") == 0) {
        *level = FRODOKEM_1344;
        return 0;
    }
    return -1;
}

int main(int argc, char **argv) {
    frodokem_level_t level;
    const frodokem_params_t *params;
    FILE *rsp = NULL;
    unsigned char seed[48];
    unsigned char *pk = NULL;
    unsigned char *sk = NULL;
    unsigned char *ct = NULL;
    unsigned char *pk_rsp = NULL;
    unsigned char *sk_rsp = NULL;
    unsigned char *ct_rsp = NULL;
    unsigned char ss[FRODOKEM_MAX_SHARED_SECRET_BYTES] = {0};
    unsigned char ss_dec[FRODOKEM_MAX_SHARED_SECRET_BYTES] = {0};
    unsigned char ss_rsp[FRODOKEM_MAX_SHARED_SECRET_BYTES] = {0};
    int result = KAT_CRYPTO_FAILURE;

    if (argc != 3 || parse_level(argv[1], &level) != 0 || FRODOKEM_select_level(level) != 0) {
        fprintf(stderr, "usage: %s <640|976|1344> <PQCkemKAT_*_shake.rsp>\n", argv[0]);
        return KAT_DATA_ERROR;
    }

    params = FRODOKEM_get_params(level);
    rsp = fopen(argv[2], "r");
    if (rsp == NULL) {
        fprintf(stderr, "failed to open <%s>\n", argv[2]);
        return KAT_FILE_OPEN_ERROR;
    }

    pk = calloc(params->publickeybytes, 1);
    sk = calloc(params->secretkeybytes, 1);
    ct = calloc(params->ciphertextbytes, 1);
    pk_rsp = calloc(params->publickeybytes, 1);
    sk_rsp = calloc(params->secretkeybytes, 1);
    ct_rsp = calloc(params->ciphertextbytes, 1);
    if (pk == NULL || sk == NULL || ct == NULL || pk_rsp == NULL || sk_rsp == NULL || ct_rsp == NULL) {
        goto cleanup;
    }

    printf("# %s SHAKE KAT\n", params->algname);
    for (;;) {
        int count;
        if (!find_marker(rsp, "count = ")) {
            result = KAT_SUCCESS;
            break;
        }
        if (fscanf(rsp, "%d", &count) != 1) {
            result = KAT_DATA_ERROR;
            break;
        }
        if (!read_hex(rsp, seed, 48, "seed = ")) {
            fprintf(stderr, "failed to read seed at count %d\n", count);
            result = KAT_DATA_ERROR;
            break;
        }

        randombytes_init(seed, NULL, 256);
        if (crypto_kem_keypair(level, pk, sk) != 0) {
            result = KAT_CRYPTO_FAILURE;
            break;
        }
        if (!read_hex(rsp, pk_rsp, (int)params->publickeybytes, "pk = ") ||
            !read_hex(rsp, sk_rsp, (int)params->secretkeybytes, "sk = ")) {
            fprintf(stderr, "failed to read keypair response at count %d\n", count);
            result = KAT_DATA_ERROR;
            break;
        }
        if (memcmp(pk, pk_rsp, params->publickeybytes) != 0 || memcmp(sk, sk_rsp, params->secretkeybytes) != 0) {
            fprintf(stderr, "KAT keypair mismatch at count %d\n", count);
            fprintf(stderr, "pk[0..15] got ");
            for (size_t i = 0; i < 16 && i < params->publickeybytes; i++) {
                fprintf(stderr, "%02X", pk[i]);
            }
            fprintf(stderr, " expected ");
            for (size_t i = 0; i < 16 && i < params->publickeybytes; i++) {
                fprintf(stderr, "%02X", pk_rsp[i]);
            }
            fprintf(stderr, "\n");
            result = KAT_VERIFICATION_ERROR;
            break;
        }

        if (crypto_kem_enc(level, ct, ss, pk) != 0) {
            result = KAT_CRYPTO_FAILURE;
            break;
        }
        if (!read_hex(rsp, ct_rsp, (int)params->ciphertextbytes, "ct = ") ||
            !read_hex(rsp, ss_rsp, (int)params->bytes, "ss = ")) {
            fprintf(stderr, "failed to read encaps response at count %d\n", count);
            result = KAT_DATA_ERROR;
            break;
        }
        if (memcmp(ct, ct_rsp, params->ciphertextbytes) != 0 || memcmp(ss, ss_rsp, params->bytes) != 0) {
            fprintf(stderr, "KAT encaps mismatch at count %d\n", count);
            result = KAT_VERIFICATION_ERROR;
            break;
        }

        if (crypto_kem_dec(level, ss_dec, ct, sk) != 0 || memcmp(ss, ss_dec, params->bytes) != 0) {
            fprintf(stderr, "KAT decaps mismatch at count %d\n", count);
            result = KAT_CRYPTO_FAILURE;
            break;
        }
    }

cleanup:
    fclose(rsp);
    free(pk);
    free(sk);
    free(ct);
    free(pk_rsp);
    free(sk_rsp);
    free(ct_rsp);
    if (result == KAT_SUCCESS) {
        printf("Known Answer Tests PASSED.\n");
    }
    return result;
}
