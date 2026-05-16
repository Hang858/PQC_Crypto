# PQC_HQC

HQC KEM implementation in the unified PQC library layout. This module currently supports the `HQC-1` parameter set only.

## Directory Layout

- `include/`: public headers.
- `core/common/`: algorithm implementation.
- `platform/pc/`: PC build files, operator interface, software operator adapters, tests, and KAT runner.
- `tests/`: MUnit support code.

## Public Interface

Include `include/api.h` for the NIST-style fixed-level KEM API. Include `include/hqc_params.h` if explicit level selection or parameter lookup is needed.

### Parameter Set

| Level | Algorithm | Public key | Secret key | Ciphertext | Shared secret |
| --- | --- | ---: | ---: | ---: | ---: |
| `HQC_1` | `HQC-1` | 2241 bytes | 2321 bytes | 4433 bytes | 32 bytes |

The fixed API macros are:

- `CRYPTO_PUBLICKEYBYTES`: 2241
- `CRYPTO_SECRETKEYBYTES`: 2321
- `CRYPTO_CIPHERTEXTBYTES`: 4433
- `CRYPTO_BYTES`: 32

### `int crypto_kem_keypair(unsigned char *pk, unsigned char *sk)`

Generates an HQC-1 KEM key pair.

Parameters:

- `pk` OUT: public key buffer. Must be at least `CRYPTO_PUBLICKEYBYTES` bytes.
- `sk` OUT: secret key buffer. Must be at least `CRYPTO_SECRETKEYBYTES` bytes.

Return value:

- `0` on success.
- Non-zero on failure.

### `int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk)`

Encapsulates a shared secret to an HQC-1 public key.

Parameters:

- `ct` OUT: ciphertext buffer. Must be at least `CRYPTO_CIPHERTEXTBYTES` bytes.
- `ss` OUT: shared secret buffer. Must be at least `CRYPTO_BYTES` bytes.
- `pk` IN: public key buffer. Must contain `CRYPTO_PUBLICKEYBYTES` bytes.

Return value:

- `0` on success.
- Non-zero on failure.

### `int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk)`

Decapsulates an HQC-1 ciphertext with a secret key.

Parameters:

- `ss` OUT: shared secret buffer. Must be at least `CRYPTO_BYTES` bytes.
- `ct` IN: ciphertext buffer. Must contain `CRYPTO_CIPHERTEXTBYTES` bytes.
- `sk` IN: secret key buffer. Must contain `CRYPTO_SECRETKEYBYTES` bytes.

Return value:

- `0` on success.
- Non-zero on failure.

### Runtime-Level Helpers

`hqc_params.h` exposes:

```c
const hqc_params_t *HQC_get_params(hqc_level_t level);
int HQC_select_level(hqc_level_t level);
int HQC_crypto_kem_keypair(hqc_level_t level, unsigned char *pk, unsigned char *sk);
int HQC_crypto_kem_enc(hqc_level_t level, unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int HQC_crypto_kem_dec(hqc_level_t level, unsigned char *ss, const unsigned char *ct, const unsigned char *sk);
```

Only `HQC_1` is valid. `HQC_get_params()` returns `NULL` for invalid levels. The level-selecting KEM wrappers return `-1` for invalid levels.

## Operator Interface

The PC build uses `platform/pc/operator_interface.h`.

- Random bytes use `OP_trng`.
- SHAKE/SHA3 operations in FIPS202 are compiled with `USE_HARDWARE_HASH` and call `OP_hash`, `OP_hash_init`, `OP_hash_absorb`, and `OP_hash_squeeze`.
- PC software fallback implementations are under `platform/pc/soft/`.

## Build Instructions

### Makefile Build

```sh
cd PQC_Crypto/PQC_HQC/platform/pc
make clean
make
```

Build parameters:

- `CC`: C compiler. Default: `gcc`.
- `AR`: static library archiver. Default: `ar`.
- `BASE_CFLAGS`: optimization, warning, standard, include, and platform flags.
- `RUNTIME_CFLAGS`: flags used for common runtime sources. Defaults to `$(BASE_CFLAGS)`.

Important compile definitions:

- `USE_HARDWARE_HASH`: enabled for `fips202.c`; routes SHAKE/SHA3 calls through the operator interface.
- `HQC_USE_DETERMINISTIC_PRNG`: used only by the KAT runner object to reproduce official KAT output.

Build outputs:

- `libpqc_hqc.a`: static library.
- `test_hqc_ref`: unit test binary, built by `make test`.
- `kat_runtime`: KAT runner, built by `make kat_runtime`.

### CMake Build

```sh
cd PQC_Crypto/PQC_HQC/platform/pc
cmake -S . -B build
cmake --build build
```

CMake target `pqc_hqc` defines `USE_HARDWARE_HASH`. The `kat_runtime` executable defines `HQC_USE_DETERMINISTIC_PRNG`.

## Test and KAT

The commands below are written relative to the repository root, shown as `<repo-root>`. The official HQC reference KAT files are expected in a sibling directory named `hqc/` at `<repo-root>/hqc`. If your delivery package stores reference files elsewhere, replace the reference path accordingly.

```sh
cd <repo-root>/PQC_Crypto/PQC_HQC/platform/pc
make test
make kat_runtime
mkdir -p kat
./kat_runtime hqc-1 kat/hqc-1.rsp
cmp -s kat/hqc-1.rsp <repo-root>/hqc/kats/ref/hqc-1/PQCkemKAT_2321.rsp
```
