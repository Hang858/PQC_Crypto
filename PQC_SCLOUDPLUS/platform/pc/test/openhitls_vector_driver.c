#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "fips202.h"
#include "scloudplus_params.h"

static uint8_t g_random[3][32];
static unsigned int g_random_index;

int randombytes(unsigned char *buffer, unsigned int size)
{
	if (buffer == NULL || size > sizeof g_random[0] || g_random_index >= 3) {
		return -1;
	}
	memcpy(buffer, g_random[g_random_index], size);
	g_random_index++;
	return 0;
}

static int
hex_nibble(char c)
{
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}
	if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}
	return -1;
}

static int
decode_hex(uint8_t *out, size_t out_len, const char *hex)
{
	if (strlen(hex) != 2 * out_len) {
		return -1;
	}
	for (size_t i = 0; i < out_len; i++) {
		int high = hex_nibble(hex[2 * i]);
		int low = hex_nibble(hex[2 * i + 1]);

		if (high < 0 || low < 0) {
			return -1;
		}
		out[i] = (uint8_t)((high << 4) | low);
	}
	return 0;
}

static int
compare_value(const char *name, const uint8_t *actual,
			  const uint8_t *expected, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		if (actual[i] != expected[i]) {
			fprintf(stderr, "%s mismatch at byte %zu: %02X != %02X\n",
					name, i, actual[i], expected[i]);
			return -1;
		}
	}
	return 0;
}

static int
parse_level(const char *name, scloudplus_level_t *level)
{
	if (strcmp(name, "PQCP_SCLOUDPLUS_128") == 0) {
		*level = SCLOUDPLUS_128;
	} else if (strcmp(name, "PQCP_SCLOUDPLUS_192") == 0) {
		*level = SCLOUDPLUS_192;
	} else if (strcmp(name, "PQCP_SCLOUDPLUS_256") == 0) {
		*level = SCLOUDPLUS_256;
	} else {
		return -1;
	}
	return 0;
}

int
main(int argc, char **argv)
{
	scloudplus_level_t level;
	const scloudplus_params_t *params;
	uint8_t *pk = NULL, *sk = NULL, *ct = NULL, *ss = NULL, *ss_dec = NULL;
	uint8_t *expected_pk = NULL, *expected_sk = NULL, *expected_ct = NULL;
	uint8_t *fallback_input = NULL;
	uint8_t expected_ss[SCLOUDPLUS_MAX_SHARED_SECRET_BYTES];
	int result = 1;

	if (argc != 9 || parse_level(argv[1], &level) != 0) {
		fprintf(stderr, "usage: %s <level> <alpha> <z> <m> <pk> <sk> <ct> <ss>\n",
				argv[0]);
		return 2;
	}
	params = SCLOUDPLUS_get_params(level);
	pk = malloc(params->publickeybytes);
	sk = malloc(params->secretkeybytes);
	ct = malloc(params->ciphertextbytes);
	ss = malloc(params->bytes);
	ss_dec = malloc(params->bytes);
	expected_pk = malloc(params->publickeybytes);
	expected_sk = malloc(params->secretkeybytes);
	expected_ct = malloc(params->ciphertextbytes);
	fallback_input = malloc(32 + params->ciphertextbytes);
	if (pk == NULL || sk == NULL || ct == NULL || ss == NULL || ss_dec == NULL ||
		expected_pk == NULL || expected_sk == NULL || expected_ct == NULL ||
		fallback_input == NULL) {
		goto exit;
	}
	if (decode_hex(g_random[1], 32, argv[2]) != 0 ||
		decode_hex(g_random[0], 32, argv[3]) != 0 ||
		decode_hex(g_random[2], params->bytes, argv[4]) != 0 ||
		decode_hex(expected_pk, params->publickeybytes, argv[5]) != 0 ||
		decode_hex(expected_sk, params->secretkeybytes, argv[6]) != 0 ||
		decode_hex(expected_ct, params->ciphertextbytes, argv[7]) != 0 ||
		decode_hex(expected_ss, params->bytes, argv[8]) != 0) {
		fprintf(stderr, "invalid vector encoding for %s\n", argv[1]);
		goto exit;
	}
	g_random_index = 0;
	if (crypto_kem_keypair(level, pk, sk) != 0 ||
		crypto_kem_enc(level, ct, ss, pk) != 0 ||
		crypto_kem_dec(level, ss_dec, ct, sk) != 0) {
		fprintf(stderr, "KEM operation failed for %s\n", argv[1]);
		goto exit;
	}
	if (compare_value("pk", pk, expected_pk, params->publickeybytes) != 0 ||
		compare_value("sk", sk, expected_sk, params->secretkeybytes) != 0 ||
		compare_value("ct", ct, expected_ct, params->ciphertextbytes) != 0 ||
		compare_value("ss", ss, expected_ss, params->bytes) != 0 ||
		compare_value("decapsulated ss", ss_dec, expected_ss, params->bytes) != 0) {
		goto exit;
	}
	ct[0] ^= 1;
	memcpy(fallback_input, sk + params->secretkeybytes - 32, 32);
	memcpy(fallback_input + 32, ct, params->ciphertextbytes);
	scloudplus_K(expected_ss, params->bytes, fallback_input,
			   32 + params->ciphertextbytes);
	if (crypto_kem_dec(level, ss_dec, ct, sk) != 0 ||
		compare_value("implicit-rejection ss", ss_dec, expected_ss,
					  params->bytes) != 0) {
		goto exit;
	}
	result = 0;
exit:
	free(pk);
	free(sk);
	free(ct);
	free(ss);
	free(ss_dec);
	free(expected_pk);
	free(expected_sk);
	free(expected_ct);
	free(fallback_input);
	return result;
}
