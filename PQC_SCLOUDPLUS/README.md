# PQC_SCLOUDPLUS

SCloud+ KEM implementation in the unified PQC library layout. This module supports runtime selection among 128-bit, 192-bit, and 256-bit parameter sets.

## Directory Layout

- `include/`: public headers.
- `core/common/`: algorithm implementation.
- `platform/pc/`: PC build files, operator interface, software operator adapters, tests, and differential KAT script.
- `tests/`: reserved test support directory.

## Public Interface

Include `include/api.h` for the public KEM API. Include `include/scloudplus_params.h` for parameter lookup and level-selection helpers.

### Parameter Sets

| Level | Algorithm | Public key | Secret key | Ciphertext | Shared secret |
| --- | --- | ---: | ---: | ---: | ---: |
| `SCLOUDPLUS_128` | `SCLOUDPLUS-128` | 7216 bytes | 8480 bytes | 5456 bytes | 16 bytes |
| `SCLOUDPLUS_192` | `SCLOUDPLUS-192` | 11152 bytes | 13008 bytes | 10832 bytes | 24 bytes |
| `SCLOUDPLUS_256` | `SCLOUDPLUS-256` | 18760 bytes | 21904 bytes | 16916 bytes | 32 bytes |

The max-size macros in `api.h` are:

- `CRYPTO_PUBLICKEYBYTES`: 18760
- `CRYPTO_SECRETKEYBYTES`: 21904
- `CRYPTO_CIPHERTEXTBYTES`: 16916
- `CRYPTO_BYTES`: 32

Callers may allocate max-size buffers for all levels, or use `SCLOUDPLUS_get_params(level)` to allocate exact sizes.

### `int crypto_kem_keypair(scloudplus_level_t level, uint8_t *pk, uint8_t *sk)`

Generates an SCloud+ KEM key pair for `level`.

Parameters:

- `level` IN: one of `SCLOUDPLUS_128`, `SCLOUDPLUS_192`, or `SCLOUDPLUS_256`.
- `pk` OUT: public key buffer. Must be at least `params->publickeybytes` bytes.
- `sk` OUT: secret key buffer. Must be at least `params->secretkeybytes` bytes.

Return value:

- `0` on success.
- `-1` if `level` is invalid.
- Other non-zero values may indicate internal failure.

### `int crypto_kem_enc(scloudplus_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk)`

Encapsulates a shared secret to an SCloud+ public key.

Parameters:

- `level` IN: selected SCloud+ parameter set.
- `ct` OUT: ciphertext buffer. Must be at least `params->ciphertextbytes` bytes.
- `ss` OUT: shared secret buffer. Must be at least `params->bytes` bytes.
- `pk` IN: public key buffer. Must contain `params->publickeybytes` bytes.

Return value:

- `0` on success.
- `-1` if `level` is invalid.
- Other non-zero values may indicate internal failure.

### `int crypto_kem_dec(scloudplus_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk)`

Decapsulates an SCloud+ ciphertext with a secret key.

Parameters:

- `level` IN: selected SCloud+ parameter set.
- `ss` OUT: shared secret buffer. Must be at least `params->bytes` bytes.
- `ct` IN: ciphertext buffer. Must contain `params->ciphertextbytes` bytes.
- `sk` IN: secret key buffer. Must contain `params->secretkeybytes` bytes.

Return value:

- `0` on success.
- `-1` if `level` is invalid.
- Other non-zero values may indicate internal failure.

### Parameter Helpers

```c
const scloudplus_params_t *SCLOUDPLUS_get_params(scloudplus_level_t level);
int SCLOUDPLUS_select_level(scloudplus_level_t level);
int SCLOUDPLUS_crypto_kem_keypair(scloudplus_level_t level, uint8_t *pk, uint8_t *sk);
int SCLOUDPLUS_crypto_kem_enc(scloudplus_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int SCLOUDPLUS_crypto_kem_dec(scloudplus_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
```

`SCLOUDPLUS_get_params()` returns `NULL` for invalid levels. `SCLOUDPLUS_select_level()` and wrapper APIs return `-1` for invalid levels.

## Operator Interface

The PC build uses `platform/pc/operator_interface.h`.

- Random bytes use `OP_trng`.
- SHAKE/SHA3 operations use `OP_hash*` when `USE_HARDWARE_HASH` is defined.
- PC software fallback implementations are under `platform/pc/soft/`.

## Build Instructions

### Makefile Build

```sh
cd PQC_Crypto/PQC_SCLOUDPLUS/platform/pc
make clean
make
```

Build parameters:

- `CC`: C compiler. Default: `gcc`.
- `AR`: static library archiver. Default: `ar`.
- `CFLAGS`: optimization, warning, standard, platform, hash, and include flags.

Important compile definitions:

- `USE_HARDWARE_HASH`: routes FIPS202 hash/XOF calls through the operator interface.

Build outputs:

- `libpqc_scloudplus.a`: static library.
- `test_scloudplus_ref`: test binary, built by `make test`.

### CMake Build

```sh
cd PQC_Crypto/PQC_SCLOUDPLUS/platform/pc
cmake -S . -B build
cmake --build build
```

The CMake file builds the same library and test executable with the module include paths and platform sources.

## Test and Differential KAT

```sh
cd PQC_Crypto/PQC_SCLOUDPLUS/platform/pc
make test
```

The differential KAT compares this module with the original SCloud+ source tree:

```sh
cd <repo-root>
bash PQC_Crypto/PQC_SCLOUDPLUS/platform/pc/test/run_scloudplus_diff_kat.sh
```

The script expects the original SCloud+ source tree at `<repo-root>/scloudplus/src`. If the original source is placed elsewhere, update the script path before running it.

Expected result:

```text
SCLOUDPLUS-128: PASS
SCLOUDPLUS-192: PASS
SCLOUDPLUS-256: PASS
```
