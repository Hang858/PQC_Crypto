#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "katrng.h"

#define MAX_LINE_LEN 32768

#ifdef FALCON_KAT_DEBUG
#define KAT_DBG(...) do { \
        fprintf(stderr, __VA_ARGS__); \
        fflush(stderr); \
    } while (0)
#else
#define KAT_DBG(...) do { } while (0)
#endif

static int read_prefixed_line(FILE *infile, char *line, size_t line_size, const char *prefix) {
    size_t prefix_len = strlen(prefix);

    while (fgets(line, (int)line_size, infile) != NULL) {
        if (strncmp(line, prefix, prefix_len) == 0) {
            return 1;
        }
    }
    return 0;
}

static int read_hex(FILE *infile, unsigned char *a, int length, const char *str) {
    char line[MAX_LINE_LEN];
    const char *p;
    int pos = 0;

    if (length == 0) {
        a[0] = 0x00;
        return 1;
    }
    memset(a, 0, (size_t)length);
    if (!read_prefixed_line(infile, line, sizeof line, str)) {
        return 0;
    }
    p = line + strlen(str);
    while (*p != '\0' && *p != '\n' && *p != '\r') {
        unsigned char hi;
        unsigned char lo;
        int ch;

        while (*p != '\0' && isspace((unsigned char)*p)) {
            p++;
        }
        if (*p == '\0' || *p == '\n' || *p == '\r') {
            break;
        }
        ch = (unsigned char)*p++;
        if (ch >= '0' && ch <= '9') {
            hi = (unsigned char)(ch - '0');
        } else if (ch >= 'A' && ch <= 'F') {
            hi = (unsigned char)(ch - 'A' + 10);
        } else if (ch >= 'a' && ch <= 'f') {
            hi = (unsigned char)(ch - 'a' + 10);
        } else {
            return 0;
        }
        while (*p != '\0' && isspace((unsigned char)*p)) {
            p++;
        }
        if (*p == '\0' || *p == '\n' || *p == '\r') {
            return 0;
        }
        ch = (unsigned char)*p++;
        if (ch >= '0' && ch <= '9') {
            lo = (unsigned char)(ch - '0');
        } else if (ch >= 'A' && ch <= 'F') {
            lo = (unsigned char)(ch - 'A' + 10);
        } else if (ch >= 'a' && ch <= 'f') {
            lo = (unsigned char)(ch - 'a' + 10);
        } else {
            return 0;
        }

        a[pos++] = (unsigned char)((hi << 4) | lo);
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

        {
            char line[MAX_LINE_LEN];
            if (!read_prefixed_line(req, line, sizeof line, "count = ")) {
                done = 1;
                break;
            }
            if (sscanf(line + 8, "%d", &count) != 1) {
                return 2;
            }
        }
        fprintf(rsp, "count = %d\n", count);
        KAT_DBG("[kat] count=%d start\n", count);

        if (!read_hex(req, seed, 48, "seed = ")) {
            return 2;
        }
        fprint_bstr(rsp, "seed = ", seed, 48);
        KAT_DBG("[kat] count=%d randombytes_init begin\n", count);
        randombytes_init(seed, NULL, 256);
        KAT_DBG("[kat] count=%d randombytes_init end\n", count);

        {
            char line[MAX_LINE_LEN];
            if (!read_prefixed_line(req, line, sizeof line, "mlen = ")) {
                return 2;
            }
            if (sscanf(line + 7, "%llu", &mlen) != 1) {
                return 2;
            }
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

        KAT_DBG("[kat] count=%d keypair begin\n", count);
        if (crypto_sign_keypair(level, pk, sk) != 0) {
            KAT_DBG("[kat] count=%d keypair failed\n", count);
            return 1;
        }
        KAT_DBG("[kat] count=%d keypair end\n", count);
        fprint_bstr(rsp, "pk = ", pk, params->publickeybytes);
        fprint_bstr(rsp, "sk = ", sk, params->secretkeybytes);

        KAT_DBG("[kat] count=%d sign begin\n", count);
        if (crypto_sign(level, sm, &smlen, m, mlen, sk) != 0) {
            KAT_DBG("[kat] count=%d sign failed\n", count);
            return 1;
        }
        KAT_DBG("[kat] count=%d sign end smlen=%llu\n", count, smlen);
        fprintf(rsp, "smlen = %llu\n", smlen);
        fprint_bstr(rsp, "sm = ", sm, smlen);
        fprintf(rsp, "\n");

        KAT_DBG("[kat] count=%d open begin\n", count);
        if (crypto_sign_open(level, m1, &mlen1, sm, smlen, pk) != 0 ||
            mlen1 != mlen || memcmp(m, m1, (size_t)mlen) != 0) {
            KAT_DBG("[kat] count=%d open failed\n", count);
            return 1;
        }
        KAT_DBG("[kat] count=%d open end\n", count);

        free(m);
        free(m1);
        free(sm);
        free(pk);
        free(sk);
    } while (!done);

    KAT_DBG("[kat] done\n");
    fclose(req);
    fclose(rsp);
    return 0;
}
