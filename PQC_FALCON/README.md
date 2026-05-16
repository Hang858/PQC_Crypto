# PQC_FALCON

## Overview

This directory is a self-contained Falcon signature module for the unified PQC layout. It supports runtime selection between Falcon-512 and Falcon-1024.

## Directory Layout

- `include/`: public API and parameter headers.
- `core/common/`: algorithm implementation.
- `platform/pc/`: standalone PC build, operator interface, software operator implementations, tests, KAT RNG, and KAT runner.
- `platform/pc/soft/`: common software operator set shared across all PQC modules.
- `platform/pc/test/`: local KAT request/response files.

## Supported Parameters

| Level | Algorithm | `logn` | Public key | Secret key | Max signature bytes |
| --- | --- | ---: | ---: | ---: | ---: |
| `FALCON_512` | `Falcon-512` | 9 | 897 bytes | 1281 bytes | 690 bytes |
| `FALCON_1024` | `Falcon-1024` | 10 | 1793 bytes | 2305 bytes | 1330 bytes |

Max-size API macros:

- `CRYPTO_PUBLICKEYBYTES`: 1793
- `CRYPTO_SECRETKEYBYTES`: 2305
- `CRYPTO_BYTES`: 1330

Callers may allocate max-size buffers for both levels, or call `Falcon_get_params(level)` for exact sizes.

## Public API

Include `include/api.h` for the signature API.

### `int crypto_sign_keypair(falcon_level_t level, unsigned char *pk, unsigned char *sk)`

Generates a Falcon signing key pair for `level`.

- `level` IN: `FALCON_512` or `FALCON_1024`.
- `pk` OUT: public key buffer, at least `params->publickeybytes` bytes.
- `sk` OUT: secret key buffer, at least `params->secretkeybytes` bytes.
- Returns `0` on success, non-zero on invalid level or internal failure.

### `int crypto_sign(falcon_level_t level, unsigned char *sm, unsigned long long *smlen, const unsigned char *m, unsigned long long mlen, const unsigned char *sk)`

Signs a message and writes a signed-message buffer.

- `level` IN: selected Falcon parameter set.
- `sm` OUT: signed-message buffer, at least `mlen + params->bytes` bytes.
- `smlen` OUT: number of bytes written to `sm`.
- `m` IN: message buffer containing `mlen` bytes. It may be empty when `mlen == 0`.
- `mlen` IN: message length in bytes.
- `sk` IN: secret key buffer containing `params->secretkeybytes` bytes.
- Returns `0` on success, non-zero on invalid level, signing failure, or malformed input.

### `int crypto_sign_open(falcon_level_t level, unsigned char *m, unsigned long long *mlen, const unsigned char *sm, unsigned long long smlen, const unsigned char *pk)`

Verifies a signed message and recovers the message.

- `level` IN: selected Falcon parameter set.
- `m` OUT: recovered message buffer. Must be large enough for the original message, at most `smlen` bytes.
- `mlen` OUT: recovered message length in bytes.
- `sm` IN: signed-message buffer containing `smlen` bytes.
- `smlen` IN: signed-message length in bytes.
- `pk` IN: public key buffer containing `params->publickeybytes` bytes.
- Returns `0` on successful verification, non-zero on invalid level, malformed signed message, or verification failure.

### Parameter Helper

Include `include/falcon_params.h` for:

```c
const falcon_params_t *Falcon_get_params(falcon_level_t level);
```

Invalid levels return `NULL`.

## Operator Interface

The PC build uses `platform/pc/operator_interface.h`.

- `OP_fpr_add`, `OP_fpr_sub`, `OP_fpr_mul`, `OP_fpr_div`, `OP_fpr_sqrt`, and `OP_fpr_lt` are used by Falcon floating-point arithmetic.
- `OP_hash_init`, `OP_hash_absorb`, and `OP_hash_squeeze` are used by SHAKE256 when `USE_HARDWARE_HASH` is enabled.
- `OP_trng` and `OP_matrix_mul_8x8` are provided in `soft/` for interface consistency.

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
- `LIBS`: extra link libraries, default `-lm`.

Important definitions:

- `USE_HARDWARE_HASH`: routes SHAKE256 through the operator interface.

Outputs:

- `libpqc_falcon.a`
- `test_falcon`
- `kat_runtime`

## Test and KAT

The KAT build is self-contained. It uses `platform/pc/katrng.c` and local request/response files under `platform/pc/test/kat/`.

```sh
cd platform/pc
make test
make kat_runtime
mkdir -p kat
./kat_runtime 512 test/kat/falcon512-KAT.req kat/falcon512.rsp
./kat_runtime 1024 test/kat/falcon1024-KAT.req kat/falcon1024.rsp
cmp -s kat/falcon512.rsp test/kat/falcon512-KAT.rsp
cmp -s kat/falcon1024.rsp test/kat/falcon1024-KAT.rsp
```
