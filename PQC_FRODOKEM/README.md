# PQC_FRODOKEM

FrodoKEM implementation in the unified PQC library layout. This module supports runtime selection among FrodoKEM-640, FrodoKEM-976, and FrodoKEM-1344.

## Directory Layout

- `include/`: public headers.
- `core/common/`: algorithm implementation.
- `platform/pc/`: PC build files, operator interface, software operator adapters, tests, and KAT runner.
- `tests/`: KAT support code.

## Public Interface

Include `include/api.h` for the public KEM API. Include `include/frodokem_params.h` for parameter lookup and level-selection helpers.

### Parameter Sets

| Level | Algorithm | Public key | Secret key | Ciphertext | Shared secret |
| --- | --- | ---: | ---: | ---: | ---: |
| `FRODOKEM_640` | `FrodoKEM-640` | 9616 bytes | 19888 bytes | 9752 bytes | 16 bytes |
| `FRODOKEM_976` | `FrodoKEM-976` | 15632 bytes | 31296 bytes | 15792 bytes | 24 bytes |
| `FRODOKEM_1344` | `FrodoKEM-1344` | 21520 bytes | 43088 bytes | 21696 bytes | 32 bytes |

The max-size macros in `api.h` are:

- `CRYPTO_PUBLICKEYBYTES`: 21520
- `CRYPTO_SECRETKEYBYTES`: 43088
- `CRYPTO_CIPHERTEXTBYTES`: 21696
- `CRYPTO_BYTES`: 32

Callers may allocate max-size buffers for all levels, or use `FRODOKEM_get_params(level)` to allocate exact sizes.

### `int crypto_kem_keypair(frodokem_level_t level, uint8_t *pk, uint8_t *sk)`

Generates a FrodoKEM key pair for `level`.

Parameters:

- `level` IN: one of `FRODOKEM_640`, `FRODOKEM_976`, or `FRODOKEM_1344`.
- `pk` OUT: public key buffer. Must be at least `params->publickeybytes` bytes.
- `sk` OUT: secret key buffer. Must be at least `params->secretkeybytes` bytes.

Return value:

- `0` on success.
- `-1` if `level` is invalid.
- Other non-zero values may indicate internal failure.

### `int crypto_kem_enc(frodokem_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk)`

Encapsulates a shared secret to a FrodoKEM public key.

Parameters:

- `level` IN: selected FrodoKEM parameter set.
- `ct` OUT: ciphertext buffer. Must be at least `params->ciphertextbytes` bytes.
- `ss` OUT: shared secret buffer. Must be at least `params->bytes` bytes.
- `pk` IN: public key buffer. Must contain `params->publickeybytes` bytes.

Return value:

- `0` on success.
- `-1` if `level` is invalid.
- Other non-zero values may indicate internal failure.

### `int crypto_kem_dec(frodokem_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk)`

Decapsulates a FrodoKEM ciphertext with a secret key.

Parameters:

- `level` IN: selected FrodoKEM parameter set.
- `ss` OUT: shared secret buffer. Must be at least `params->bytes` bytes.
- `ct` IN: ciphertext buffer. Must contain `params->ciphertextbytes` bytes.
- `sk` IN: secret key buffer. Must contain `params->secretkeybytes` bytes.

Return value:

- `0` on success.
- `-1` if `level` is invalid.
- Other non-zero values may indicate internal failure.

### Parameter Helpers

```c
const frodokem_params_t *FRODOKEM_get_params(frodokem_level_t level);
int FRODOKEM_select_level(frodokem_level_t level);
int FRODOKEM_crypto_kem_keypair(frodokem_level_t level, uint8_t *pk, uint8_t *sk);
int FRODOKEM_crypto_kem_enc(frodokem_level_t level, uint8_t *ct, uint8_t *ss, const uint8_t *pk);
int FRODOKEM_crypto_kem_dec(frodokem_level_t level, uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
int FRODOKEM_crypto_kem_keypair_enc(frodokem_level_t level, uint8_t *ct, uint8_t *ss, uint8_t *pk, uint8_t *sk);
```

`FRODOKEM_get_params()` returns `NULL` for invalid levels. `FRODOKEM_select_level()` and wrapper APIs return `-1` for invalid levels.

## Operator Interface

The PC build uses `platform/pc/operator_interface.h`.

- Random bytes use `OP_trng`.
- SHAKE/SHA3 operations use `OP_hash*` when `USE_HARDWARE_HASH` is defined.
- PC software fallback implementations are under `platform/pc/soft/`.

## Build Instructions

### Makefile Build

```sh
cd PQC_Crypto/PQC_FRODOKEM/platform/pc
make clean
make
```

Build parameters:

- `CC`: C compiler. Default: `gcc`.
- `AR`: static library archiver. Default: `ar`.
- `BASE_CFLAGS`: optimization, warning, standard, platform, and include flags.

Important compile definitions in `BASE_CFLAGS`:

- `NIX`: selects the Unix-like build path.
- `_AMD64_`: selects the AMD64 platform path.
- `_REFERENCE_`: builds the reference FrodoKEM implementation.
- `_SHAKE128_FOR_A_`: uses SHAKE128 for matrix A generation.
- `USE_HARDWARE_HASH`: routes FIPS202 hash/XOF calls through the operator interface.

Build outputs:

- `libpqc_frodokem.a`: static library.
- `test_frodokem_ref`: test binary, built by `make test`.
- `kat_runtime`: KAT runner, built by `make kat_runtime`.

### CMake Build

```sh
cd PQC_Crypto/PQC_FRODOKEM/platform/pc
cmake -S . -B build
cmake --build build
```

The CMake file builds the static library and test executable with the module include paths and platform sources. Use the Makefile path for the current KAT runner.

## Test and KAT

The commands below are written relative to the repository root, shown as `<repo-root>`. Generated KAT response files are written under the module build directory and are not part of the public API.

```sh
cd <repo-root>/PQC_Crypto/PQC_FRODOKEM/platform/pc
make test
make kat_runtime
mkdir -p kat
./kat_runtime 640 kat/frodokem640.rsp
./kat_runtime 976 kat/frodokem976.rsp
./kat_runtime 1344 kat/frodokem1344.rsp
```
