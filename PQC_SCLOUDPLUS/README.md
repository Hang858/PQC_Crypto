# PQC_SCLOUDPLUS

## Overview

This directory is a self-contained SCloud+ KEM module for the unified PQC layout. It supports runtime selection among 128-bit, 192-bit, and 256-bit parameter sets.

## Directory Layout

- `include/`: public API and parameter headers.
- `core/common/`: algorithm implementation.
- `platform/pc/`: standalone PC build, operator interface, software operator implementations, tests, and optional differential KAT script.
- `platform/pc/soft/`: common software operator set shared across all PQC modules.
- `platform/pc/test/`: local test sources and differential KAT helper files.

## Supported Parameters

| Level | Algorithm | Public key | Secret key | Ciphertext | Shared secret |
| --- | --- | ---: | ---: | ---: | ---: |
| `SCLOUDPLUS_128` | `SCLOUDPLUS-128` | 7216 bytes | 8480 bytes | 5456 bytes | 16 bytes |
| `SCLOUDPLUS_192` | `SCLOUDPLUS-192` | 11152 bytes | 13008 bytes | 10832 bytes | 24 bytes |
| `SCLOUDPLUS_256` | `SCLOUDPLUS-256` | 18760 bytes | 21904 bytes | 16916 bytes | 32 bytes |

Max-size API macros:

- `CRYPTO_PUBLICKEYBYTES`: 18760
- `CRYPTO_SECRETKEYBYTES`: 21904
- `CRYPTO_CIPHERTEXTBYTES`: 16916
- `CRYPTO_BYTES`: 32

Callers may allocate max-size buffers for all levels, or call `SCLOUDPLUS_get_params(level)` for exact sizes.

## Public API

Include `include/api.h` for the KEM API.

### `int crypto_kem_keypair(scloudplus_level_t level, uint8_t *pk, uint8_t *sk)`

Generates an SCloud+ KEM key pair for `level`.

- `level` IN: `SCLOUDPLUS_128`, `SCLOUDPLUS_192`, or `SCLOUDPLUS_256`.
- `pk` OUT: public key buffer, at least `params->publickeybytes` bytes.
- `sk` OUT: secret key buffer, at least `params->secretkeybytes` bytes.
- Returns `0` on success, `-1` for invalid level, or another non-zero value on internal failure.

### `int crypto_kem_enc(scloudplus_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk)`

Encapsulates a shared secret to an SCloud+ public key.

- `level` IN: selected SCloud+ parameter set.
- `ct` OUT: ciphertext buffer, at least `params->ciphertextbytes` bytes.
- `ss` OUT: shared secret buffer, at least `params->bytes` bytes.
- `pk` IN: public key buffer containing `params->publickeybytes` bytes.
- Returns `0` on success, `-1` for invalid level, or another non-zero value on internal failure.

### `int crypto_kem_dec(scloudplus_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk)`

Decapsulates an SCloud+ ciphertext with a secret key.

- `level` IN: selected SCloud+ parameter set.
- `ss` OUT: shared secret buffer, at least `params->bytes` bytes.
- `ct` IN: ciphertext buffer containing `params->ciphertextbytes` bytes.
- `sk` IN: secret key buffer containing `params->secretkeybytes` bytes.
- Returns `0` on success, `-1` for invalid level, or another non-zero value on internal failure.

### Parameter Helpers

Include `include/scloudplus_params.h` for:

```c
const scloudplus_params_t *SCLOUDPLUS_get_params(scloudplus_level_t level);
int SCLOUDPLUS_select_level(scloudplus_level_t level);
int SCLOUDPLUS_crypto_kem_keypair(scloudplus_level_t level, uint8_t *pk, uint8_t *sk);
int SCLOUDPLUS_crypto_kem_enc(scloudplus_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int SCLOUDPLUS_crypto_kem_dec(scloudplus_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
```

Invalid levels return `NULL` from `SCLOUDPLUS_get_params()` and `-1` from selection/wrapper APIs.

## Operator Interface

The PC build uses `platform/pc/operator_interface.h`.

- `OP_trng` supplies random bytes.
- `OP_hash*` is used by FIPS202 when `USE_HARDWARE_HASH` is enabled.
- `OP_matrix_mul_8x8` and `OP_fpr_*` are provided in `soft/` for interface consistency.

The `platform/pc/soft/` directory intentionally contains the same software operator files as the other PQC modules:

```text
endian.c endian.h op_fpr.c op_hash.c op_matrix.c op_trng.c sha256.c sha256.h sm3.c sm3.h
```

## Build

Run from this module directory or adjust the path accordingly.

```sh
cd platform/pc
make clean
make
```

Build variables:

- `CC`: C compiler, default `gcc`.
- `AR`: static library archiver, default `ar`.
- `CFLAGS`: warning, optimization, C standard, hash, and include flags.

Important definitions:

- `USE_HARDWARE_HASH`: routes SHAKE/SHA3 through the operator interface.

Outputs:

- `libpqc_scloudplus.a`
- `test_scloudplus_ref`

CMake is also supported:

```sh
cd platform/pc
cmake -S . -B build
cmake --build build
```

## Test and KAT

```sh
cd platform/pc
make test
```

An optional differential KAT script is available at `platform/pc/test/run_scloudplus_diff_kat.sh`. It compares this module with an original SCloud+ source tree. By default the script looks for that tree under `platform/pc/test/original/src`; set `SCLOUDPLUS_ORIG` to use another location.

```sh
SCLOUDPLUS_ORIG=/path/to/original/scloudplus/src bash platform/pc/test/run_scloudplus_diff_kat.sh
```
