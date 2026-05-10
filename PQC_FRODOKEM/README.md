# PQC_FRODOKEM

FrodoKEM reference implementation adapted for the unified PQC library layout.

## Directory

- `core/common`: algorithm code.
- `include`: public headers.
- `platform/pc`: PC build files, software hash, TRNG adapter, tests.
- `tests`: KAT support code.

## Public API

Public KEM functions are declared in `include/api.h`.

```c
int crypto_kem_keypair(frodokem_level_t level, uint8_t *pk, uint8_t *sk);
int crypto_kem_enc(frodokem_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int crypto_kem_dec(frodokem_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
```

Supported levels:

- `FRODOKEM_640`
- `FRODOKEM_976`
- `FRODOKEM_1344`

## Platform Interface

- Random bytes use `OP_trng`.
- Hash functions use `OP_hash`.
- PC software adapters are in `platform/pc/soft`.

## Build

```sh
cd platform/pc
cmake -S . -B build
cmake --build build
```

Makefile build:

```sh
cd platform/pc
make
```

## Test

```sh
cd platform/pc/build
ctest --output-on-failure
```

Makefile test:

```sh
cd platform/pc
make test
```

## KAT

The Makefile target `kat_runtime` is used for SHAKE KAT verification.

```sh
cd platform/pc
make kat_runtime
./kat_runtime 640 <rsp-file>
./kat_runtime 976 <rsp-file>
./kat_runtime 1344 <rsp-file>
```

KAT uses deterministic test RNG code from `tests/`.
