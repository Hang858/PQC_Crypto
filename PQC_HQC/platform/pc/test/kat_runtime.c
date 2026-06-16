/*
 * HQC KAT runtime — reads official NIST .req file, generates .rsp output.
 * Usage: ./kat_runtime <level: 1|3|5> <input.req> <output.rsp>
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "hqc_params.h"
#include "symmetric.h"

#define MAX_LINE 32768

static int read_prefixed_line(FILE *fp, char *line, size_t sz, const char *pfx) {
    size_t plen = strlen(pfx);
    while (fgets(line, (int)sz, fp)) {
        if (strncmp(line, pfx, plen) == 0) return 1;
    }
    return 0;
}

static int read_hex(FILE *fp, unsigned char *buf, int len, const char *label) {
    char line[MAX_LINE];
    if (len == 0) { buf[0] = 0x00; return 1; }
    memset(buf, 0, (size_t)len);
    if (!read_prefixed_line(fp, line, sizeof line, label)) return 0;
    const char *p = line + strlen(label);
    int pos = 0;
    while (*p && *p != '\n' && *p != '\r') {
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p || *p == '\n' || *p == '\r') break;
        int hi = (unsigned char)*p++;
        if (hi >= '0' && hi <= '9') hi -= '0';
        else if (hi >= 'A' && hi <= 'F') hi = hi - 'A' + 10;
        else if (hi >= 'a' && hi <= 'f') hi = hi - 'a' + 10;
        else return 0;
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p || *p == '\n' || *p == '\r') return 0;
        int lo = (unsigned char)*p++;
        if (lo >= '0' && lo <= '9') lo -= '0';
        else if (lo >= 'A' && lo <= 'F') lo = lo - 'A' + 10;
        else if (lo >= 'a' && lo <= 'f') lo = lo - 'a' + 10;
        else return 0;
        buf[pos++] = (unsigned char)((hi << 4) | lo);
        if (pos == len) break;
    }
    return pos == len;
}

static void fprint_bstr(FILE *fp, const char *label, const unsigned char *data, size_t len) {
    fprintf(fp, "%s", label);
    for (size_t i = 0; i < len; i++) fprintf(fp, "%02X", data[i]);
    fprintf(fp, "\n");
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s <1|3|5> <input.req> <output.rsp>\n", argv[0]);
        return 2;
    }

    hqc_level_t level;
    if (!strcmp(argv[1], "1")) level = HQC_1;
    else if (!strcmp(argv[1], "3")) level = HQC_3;
    else if (!strcmp(argv[1], "5")) level = HQC_5;
    else { fprintf(stderr, "bad level: %s\n", argv[1]); return 2; }

    if (HQC_select_level(level) != 0) { fprintf(stderr, "select_level failed\n"); return 2; }

    const hqc_params_t *params = HQC_active_params();

    FILE *req = fopen(argv[2], "r");
    FILE *rsp = fopen(argv[3], "w");
    if (!req || !rsp) {
        if (req) fclose(req);
        if (rsp) fclose(rsp);
        return 2;
    }

    fprintf(rsp, "# %s\n\n", params->algname);

    unsigned char *pk = calloc(CRYPTO_PUBLICKEYBYTES, 1);
    unsigned char *sk = calloc(CRYPTO_SECRETKEYBYTES, 1);
    unsigned char *ct = calloc(CRYPTO_CIPHERTEXTBYTES, 1);
    unsigned char ss[HQC_SHARED_SECRET_BYTES] = {0};
    unsigned char ss_dec[HQC_SHARED_SECRET_BYTES] = {0};

    if (!pk || !sk || !ct) { fclose(req); fclose(rsp); return 2; }

    int count;
    unsigned char seed[48];
    int done = 0;

    do {
        char line[MAX_LINE];
        if (!read_prefixed_line(req, line, sizeof line, "count = ")) { done = 1; break; }
        if (sscanf(line + 8, "%d", &count) != 1) break;
        fprintf(rsp, "count = %d\n", count);

        if (!read_hex(req, seed, 48, "seed = ")) break;
        fprint_bstr(rsp, "seed = ", seed, 48);

        prng_init(seed, NULL, 48, 0);

        if (crypto_kem_keypair(pk, sk) != 0 ||
            crypto_kem_enc(ct, ss, pk) != 0 ||
            crypto_kem_dec(ss_dec, ct, sk) != 0 ||
            memcmp(ss, ss_dec, params->bytes) != 0) {
            fprintf(stderr, "crypto failure at count %d\n", count);
            fclose(req); fclose(rsp);
            return 1;
        }

        fprint_bstr(rsp, "pk = ", pk, params->publickeybytes);
        fprint_bstr(rsp, "sk = ", sk, params->secretkeybytes);
        fprint_bstr(rsp, "ct = ", ct, params->ciphertextbytes);
        fprint_bstr(rsp, "ss = ", ss, params->bytes);
        fprintf(rsp, "\n");
    } while (!done);

    fclose(req);
    fclose(rsp);
    free(pk); free(sk); free(ct);

    printf("%s: %d test vectors generated\n", params->algname, count + 1);
    return 0;
}
