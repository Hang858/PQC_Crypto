# PQC_HQC

HQC reference implementation adapted for the unified PQC library layout.

## Directory

- `core/common`: algorithm code.
- `include`: public headers.
- `platform/pc`: PC build files, software hash, TRNG adapter, tests.
- `tests`: MUnit support code.

## Public API

Public KEM functions are declared in `include/api.h`.

```c
int crypto_kem_keypair(hqc_level_t level, uint8_t *pk, uint8_t *sk);
int crypto_kem_enc(hqc_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int crypto_kem_dec(hqc_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
```

Supported levels:

- `HQC_1`
- `HQC_3`
- `HQC_5`

## Platform Interface

- Production random bytes use `OP_trng`.
- KAT can use deterministic PRNG after `prng_init`.
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

The Makefile target `kat_runtime` writes response files for comparison with reference KAT files.

```sh
cd platform/pc
make kat_runtime
./kat_runtime 1 unused.req /tmp/hqc-1.rsp
./kat_runtime 3 unused.req /tmp/hqc-3.rsp
./kat_runtime 5 unused.req /tmp/hqc-5.rsp
```

Reference files are under `../../../../hqc/kats/ref`.
