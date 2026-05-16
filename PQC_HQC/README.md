# PQC_HQC

## Overview

This directory is a self-contained HQC KEM module for the unified PQC layout. It currently supports `HQC-1`.

## Directory Layout

- `include/`: public API and parameter headers.
- `core/common/`: algorithm implementation.
- `platform/pc/`: standalone PC build, operator interface, software operator implementations, tests, and KAT runner.
- `platform/pc/soft/`: common software operator set shared across all PQC modules.
- `platform/pc/test/`: local test sources and KAT data needed by this module.

## Supported Parameters

| Level | Algorithm | Public key | Secret key | Ciphertext | Shared secret |
| --- | --- | ---: | ---: | ---: | ---: |
| `HQC_1` | `HQC-1` | 2241 bytes | 2321 bytes | 4433 bytes | 32 bytes |

Fixed API macros:

- `CRYPTO_PUBLICKEYBYTES`: 2241
- `CRYPTO_SECRETKEYBYTES`: 2321
- `CRYPTO_CIPHERTEXTBYTES`: 4433
- `CRYPTO_BYTES`: 32

## Public API

Include `include/api.h` for the fixed HQC-1 KEM API.

### `int crypto_kem_keypair(unsigned char *pk, unsigned char *sk)`

Generates an HQC-1 KEM key pair.

- `pk` OUT: public key buffer, at least `CRYPTO_PUBLICKEYBYTES` bytes.
- `sk` OUT: secret key buffer, at least `CRYPTO_SECRETKEYBYTES` bytes.
- Returns `0` on success, non-zero on failure.

### `int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk)`

Encapsulates a shared secret to an HQC-1 public key.

- `ct` OUT: ciphertext buffer, at least `CRYPTO_CIPHERTEXTBYTES` bytes.
- `ss` OUT: shared secret buffer, at least `CRYPTO_BYTES` bytes.
- `pk` IN: public key buffer containing `CRYPTO_PUBLICKEYBYTES` bytes.
- Returns `0` on success, non-zero on failure.

### `int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk)`

Decapsulates an HQC-1 ciphertext with a secret key.

- `ss` OUT: shared secret buffer, at least `CRYPTO_BYTES` bytes.
- `ct` IN: ciphertext buffer containing `CRYPTO_CIPHERTEXTBYTES` bytes.
- `sk` IN: secret key buffer containing `CRYPTO_SECRETKEYBYTES` bytes.
- Returns `0` on success, non-zero on failure.

### Parameter Helpers

Include `include/hqc_params.h` for:

```c
const hqc_params_t *HQC_get_params(hqc_level_t level);
int HQC_select_level(hqc_level_t level);
int HQC_crypto_kem_keypair(hqc_level_t level, unsigned char *pk, unsigned char *sk);
int HQC_crypto_kem_enc(hqc_level_t level, unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int HQC_crypto_kem_dec(hqc_level_t level, unsigned char *ss, const unsigned char *ct, const unsigned char *sk);
```

Only `HQC_1` is valid. Invalid levels return `NULL` from `HQC_get_params()` and `-1` from the level-selecting wrappers.

## Operator Interface

The PC build uses `platform/pc/operator_interface.h`.

- `OP_trng` supplies random bytes.
- `OP_hash`, `OP_hash_init`, `OP_hash_absorb`, and `OP_hash_squeeze` are used by FIPS202 when `USE_HARDWARE_HASH` is enabled.
- `OP_matrix_mul_8x8` and `OP_fpr_*` are provided in `soft/` for interface consistency, even when HQC does not call them.

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
- `BASE_CFLAGS`: warning, optimization, C standard, include, and platform flags.
- `RUNTIME_CFLAGS`: common source flags, default `$(BASE_CFLAGS)`.

Important definitions:

- `USE_HARDWARE_HASH`: routes SHAKE/SHA3 through the operator interface.
- `HQC_USE_DETERMINISTIC_PRNG`: used only by the KAT runner.

Outputs:

- `libpqc_hqc.a`
- `test_hqc_ref`
- `kat_runtime`

CMake is also supported:

```sh
cd platform/pc
cmake -S . -B build
cmake --build build
```

## Test and KAT

The reference KAT response is local to this module under `platform/pc/test/kat/`.

```sh
cd platform/pc
make test
make kat_runtime
mkdir -p kat
./kat_runtime hqc-1 kat/hqc-1.rsp
cmp -s kat/hqc-1.rsp test/kat/PQCkemKAT_2321.rsp
```
