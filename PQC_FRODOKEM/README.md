# PQC_FRODOKEM

## Overview

This directory is a self-contained FrodoKEM module for the unified PQC layout. It supports runtime selection among FrodoKEM-640, FrodoKEM-976, and FrodoKEM-1344.

## Directory Layout

- `include/`: public API and parameter headers.
- `core/common/`: algorithm implementation.
- `platform/pc/`: standalone PC build, operator interface, software operator implementations, tests, and KAT runner.
- `platform/pc/soft/`: common software operator set shared across all PQC modules.
- `platform/pc/test/`: local test/KAT sources and KAT response files needed by this module.

## Supported Parameters

| Level | Algorithm | Public key | Secret key | Ciphertext | Shared secret |
| --- | --- | ---: | ---: | ---: | ---: |
| `FRODOKEM_640` | `FrodoKEM-640` | 9616 bytes | 19888 bytes | 9752 bytes | 16 bytes |
| `FRODOKEM_976` | `FrodoKEM-976` | 15632 bytes | 31296 bytes | 15792 bytes | 24 bytes |
| `FRODOKEM_1344` | `FrodoKEM-1344` | 21520 bytes | 43088 bytes | 21696 bytes | 32 bytes |

Max-size API macros:

- `CRYPTO_PUBLICKEYBYTES`: 21520
- `CRYPTO_SECRETKEYBYTES`: 43088
- `CRYPTO_CIPHERTEXTBYTES`: 21696
- `CRYPTO_BYTES`: 32

Callers may allocate max-size buffers for all levels, or call `FRODOKEM_get_params(level)` for exact sizes.

## Public API

Include `include/api.h` for the KEM API.

### `int crypto_kem_keypair(frodokem_level_t level, uint8_t *pk, uint8_t *sk)`

Generates a FrodoKEM key pair for `level`.

- `level` IN: `FRODOKEM_640`, `FRODOKEM_976`, or `FRODOKEM_1344`.
- `pk` OUT: public key buffer, at least `params->publickeybytes` bytes.
- `sk` OUT: secret key buffer, at least `params->secretkeybytes` bytes.
- Returns `0` on success, `-1` for invalid level, or another non-zero value on internal failure.

### `int crypto_kem_enc(frodokem_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk)`

Encapsulates a shared secret to a FrodoKEM public key.

- `level` IN: selected FrodoKEM parameter set.
- `ct` OUT: ciphertext buffer, at least `params->ciphertextbytes` bytes.
- `ss` OUT: shared secret buffer, at least `params->bytes` bytes.
- `pk` IN: public key buffer containing `params->publickeybytes` bytes.
- Returns `0` on success, `-1` for invalid level, or another non-zero value on internal failure.

### `int crypto_kem_dec(frodokem_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk)`

Decapsulates a FrodoKEM ciphertext with a secret key.

- `level` IN: selected FrodoKEM parameter set.
- `ss` OUT: shared secret buffer, at least `params->bytes` bytes.
- `ct` IN: ciphertext buffer containing `params->ciphertextbytes` bytes.
- `sk` IN: secret key buffer containing `params->secretkeybytes` bytes.
- Returns `0` on success, `-1` for invalid level, or another non-zero value on internal failure.

### Parameter Helpers

Include `include/frodokem_params.h` for:

```c
const frodokem_params_t *FRODOKEM_get_params(frodokem_level_t level);
int FRODOKEM_select_level(frodokem_level_t level);
int FRODOKEM_crypto_kem_keypair(frodokem_level_t level, uint8_t *pk, uint8_t *sk);
int FRODOKEM_crypto_kem_enc(frodokem_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int FRODOKEM_crypto_kem_dec(frodokem_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
int FRODOKEM_crypto_kem_keypair_enc(frodokem_level_t level, uint8_t *ct, uint8_t *ss, uint8_t *pk, uint8_t *sk);
```

Invalid levels return `NULL` from `FRODOKEM_get_params()` and `-1` from selection/wrapper APIs.

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
- `BASE_CFLAGS`: warning, optimization, C standard, platform, and include flags.

Important definitions:

- `NIX`: Unix-like build path.
- `_AMD64_`: AMD64 build path.
- `_REFERENCE_`: reference implementation.
- `_SHAKE128_FOR_A_`: SHAKE128 for matrix A generation.
- `USE_HARDWARE_HASH`: routes SHAKE/SHA3 through the operator interface.

Outputs:

- `libpqc_frodokem.a`
- `test_frodokem_ref`
- `kat_runtime`

CMake is also supported for the library and normal test executable:

```sh
cd platform/pc
cmake -S . -B build
cmake --build build
```

## Test and KAT

The SHAKE KAT response files are local to this module under `platform/pc/test/kat/`.

```sh
cd platform/pc
make test
make kat_runtime
./kat_runtime 640 test/kat/PQCkemKAT_19888_shake.rsp
./kat_runtime 976 test/kat/PQCkemKAT_31296_shake.rsp
./kat_runtime 1344 test/kat/PQCkemKAT_43088_shake.rsp
```
