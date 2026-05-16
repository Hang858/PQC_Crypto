#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "katrng.h"

#define MAX_MARKER_LEN 50

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
        if (!strncmp(line, marker, (size_t)len)) {
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

static int read_hex(FILE *infile, unsigned char *a, int length, const char *str) {
    int ch;
    int started = 0;
    int pos = 0;

    if (length == 0) {
        a[0] = 0x00;
        return 1;
    }
    memset(a, 0, (size_t)length);
    if (!find_marker(infile, str)) {
        return 0;
    }
    while ((ch = fgetc(infile)) != EOF) {
        unsigned char ich;
        if (!isxdigit(ch)) {
            if (!started && ch == '\n') {
                break;
            }
            if (started) {
                break;
            }
            continue;
        }
        started = 1;
        if (ch >= '0' && ch <= '9') {
            ich = (unsigned char)(ch - '0');
        } else if (ch >= 'A' && ch <= 'F') {
            ich = (unsigned char)(ch - 'A' + 10);
        } else {
            ich = (unsigned char)(ch - 'a' + 10);
        }

        do {
            ch = fgetc(infile);
        } while (ch != EOF && !isxdigit(ch));
        if (ch == EOF) {
            return 0;
        }
        if (ch >= '0' && ch <= '9') {
            ich = (unsigned char)((ich << 4) | (unsigned char)(ch - '0'));
        } else if (ch >= 'A' && ch <= 'F') {
            ich = (unsigned char)((ich << 4) | (unsigned char)(ch - 'A' + 10));
        } else if (ch >= 'a' && ch <= 'f') {
            ich = (unsigned char)((ich << 4) | (unsigned char)(ch - 'a' + 10));
        } else {
            return 0;
        }
        a[pos++] = ich;
        if (pos == length) {
            break;
        }
    }
    return pos == length;
}

static void fprint_bstr(FILE *fp, const char *label, const unsigned char *data, unsigned long long len) {
    fprintf(fp, "%s", label);
    for (unsigned long long i = 0; i < len; i++) {
        fprintf(fp, "%02X", data[i]);
    }
    fprintf(fp, "\n");
}

static int parse_level(const char *s, falcon_level_t *level) {
    if (strcmp(s, "512") == 0 || strcmp(s, "falcon512") == 0) {
        *level = FALCON_512;
        return 0;
    }
    if (strcmp(s, "1024") == 0 || strcmp(s, "falcon1024") == 0) {
        *level = FALCON_1024;
        return 0;
    }
    return -1;
}

int main(int argc, char **argv) {
    falcon_level_t level;
    const falcon_params_t *params;
    FILE *req;
    FILE *rsp;
    unsigned char seed[48];
    int count;
    int done = 0;

    if (argc != 4 || parse_level(argv[1], &level) != 0) {
        fprintf(stderr, "usage: %s <512|1024> <input.req> <output.rsp>\n", argv[0]);
        return 2;
    }

    params = Falcon_get_params(level);
    req = fopen(argv[2], "r");
    rsp = fopen(argv[3], "w");
    if (params == NULL || req == NULL || rsp == NULL) {
        if (req != NULL) {
            fclose(req);
        }
        if (rsp != NULL) {
            fclose(rsp);
        }
        return 2;
    }

    fprintf(rsp, "# %s\n\n", params->algname);
    do {
        unsigned long long mlen;
        unsigned long long smlen;
        unsigned long long mlen1;
        unsigned char *m;
        unsigned char *m1;
        unsigned char *sm;
        unsigned char *pk;
        unsigned char *sk;

        if (find_marker(req, "count = ")) {
            if (fscanf(req, "%d", &count) != 1) {
                return 2;
            }
        } else {
            done = 1;
            break;
        }
        fprintf(rsp, "count = %d\n", count);

        if (!read_hex(req, seed, 48, "seed = ")) {
            return 2;
        }
        fprint_bstr(rsp, "seed = ", seed, 48);
        randombytes_init(seed, NULL, 256);

        if (!find_marker(req, "mlen = ") || fscanf(req, "%llu", &mlen) != 1) {
            return 2;
        }
        fprintf(rsp, "mlen = %llu\n", mlen);

        m = calloc((size_t)mlen, 1);
        m1 = calloc((size_t)mlen, 1);
        sm = calloc((size_t)mlen + params->bytes, 1);
        pk = calloc(params->publickeybytes, 1);
        sk = calloc(params->secretkeybytes, 1);
        if (m == NULL || m1 == NULL || sm == NULL || pk == NULL || sk == NULL) {
            return 2;
        }

        if (!read_hex(req, m, (int)mlen, "msg = ")) {
            return 2;
        }
        fprint_bstr(rsp, "msg = ", m, mlen);

        if (crypto_sign_keypair(level, pk, sk) != 0) {
            return 1;
        }
        fprint_bstr(rsp, "pk = ", pk, params->publickeybytes);
        fprint_bstr(rsp, "sk = ", sk, params->secretkeybytes);

        if (crypto_sign(level, sm, &smlen, m, mlen, sk) != 0) {
            return 1;
        }
        fprintf(rsp, "smlen = %llu\n", smlen);
        fprint_bstr(rsp, "sm = ", sm, smlen);
        fprintf(rsp, "\n");

        if (crypto_sign_open(level, m1, &mlen1, sm, smlen, pk) != 0 ||
            mlen1 != mlen || memcmp(m, m1, (size_t)mlen) != 0) {
            return 1;
        }

        free(m);
        free(m1);
        free(sm);
        free(pk);
        free(sk);
    } while (!done);

    fclose(req);
    fclose(rsp);
    return 0;
}
