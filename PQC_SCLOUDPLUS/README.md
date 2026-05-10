# PQC_SCLOUDPLUS

Scloud+ implementation adapted for the unified PQC library layout.

## Directory

- `core/common`: algorithm code.
- `include`: public headers.
- `platform/pc`: PC build files, software hash, TRNG adapter, tests.
- `tests`: reserved test directory.

## Public API

Public KEM functions are declared in `include/api.h`.

```c
int crypto_kem_keypair(scloudplus_level_t level, uint8_t *pk, uint8_t *sk);
int crypto_kem_enc(scloudplus_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int crypto_kem_dec(scloudplus_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
```

Supported levels:

- `SCLOUDPLUS_128`
- `SCLOUDPLUS_192`
- `SCLOUDPLUS_256`

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

## Differential KAT

The differential KAT compares this implementation with the original Scloud+ source in `../../scloudplus/src`.

```sh
cd ../..
bash PQC_Crypto/PQC_SCLOUDPLUS/platform/pc/test/run_scloudplus_diff_kat.sh
```

Expected result:

```text
SCLOUDPLUS-128: PASS
SCLOUDPLUS-192: PASS
SCLOUDPLUS-256: PASS
```
