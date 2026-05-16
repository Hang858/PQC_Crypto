# PQC_FALCON

Falcon signature implementation in the unified PQC library layout. This module supports runtime selection between Falcon-512 and Falcon-1024.

## Directory Layout

- `include/`: public headers.
- `core/common/`: algorithm implementation.
- `platform/pc/`: PC build files, floating-point operator interface, software operator adapter, tests, and KAT runner.

## Public Interface

Include `include/api.h` for the public signature API. Include `include/falcon_params.h` for parameter lookup.

### Parameter Sets

| Level | Algorithm | `logn` | Public key | Secret key | Max signature bytes |
| --- | --- | ---: | ---: | ---: | ---: |
| `FALCON_512` | `Falcon-512` | 9 | 897 bytes | 1281 bytes | 690 bytes |
| `FALCON_1024` | `Falcon-1024` | 10 | 1793 bytes | 2305 bytes | 1330 bytes |

The max-size macros in `api.h` are:

- `CRYPTO_PUBLICKEYBYTES`: 1793
- `CRYPTO_SECRETKEYBYTES`: 2305
- `CRYPTO_BYTES`: 1330

Callers may allocate max-size buffers for both levels, or use `Falcon_get_params(level)` to allocate exact sizes.

### `int crypto_sign_keypair(falcon_level_t level, unsigned char *pk, unsigned char *sk)`

Generates a Falcon signing key pair for `level`.

Parameters:

- `level` IN: one of `FALCON_512` or `FALCON_1024`.
- `pk` OUT: public key buffer. Must be at least `params->publickeybytes` bytes.
- `sk` OUT: secret key buffer. Must be at least `params->secretkeybytes` bytes.

Return value:

- `0` on success.
- Non-zero on invalid level or internal failure.

### `int crypto_sign(falcon_level_t level, unsigned char *sm, unsigned long long *smlen, const unsigned char *m, unsigned long long mlen, const unsigned char *sk)`

Signs a message and writes a signed-message buffer.

Parameters:

- `level` IN: selected Falcon parameter set.
- `sm` OUT: signed-message buffer. Must be at least `mlen + params->bytes` bytes.
- `smlen` OUT: number of bytes written to `sm`.
- `m` IN: message buffer. Must contain `mlen` bytes. It may be empty when `mlen == 0`.
- `mlen` IN: message length in bytes.
- `sk` IN: secret key buffer. Must contain `params->secretkeybytes` bytes.

Return value:

- `0` on success.
- Non-zero on invalid level, signing failure, or malformed input.

### `int crypto_sign_open(falcon_level_t level, unsigned char *m, unsigned long long *mlen, const unsigned char *sm, unsigned long long smlen, const unsigned char *pk)`

Verifies a signed message and recovers the message.

Parameters:

- `level` IN: selected Falcon parameter set.
- `m` OUT: recovered message buffer. Must be large enough for the original message, at most `smlen` bytes.
- `mlen` OUT: recovered message length in bytes.
- `sm` IN: signed-message buffer. Must contain `smlen` bytes.
- `smlen` IN: signed-message length in bytes.
- `pk` IN: public key buffer. Must contain `params->publickeybytes` bytes.

Return value:

- `0` on successful verification.
- Non-zero if `level` is invalid, the signed message is malformed, or verification fails.

### Parameter Helper

```c
const falcon_params_t *Falcon_get_params(falcon_level_t level);
```

`Falcon_get_params()` returns `NULL` for invalid levels.

## Operator Interface

The PC build uses `platform/pc/operator_interface.h`.

- Floating-point arithmetic in `core/common/fpr.h` calls `OP_fpr_add`, `OP_fpr_sub`, `OP_fpr_mul`, `OP_fpr_div`, `OP_fpr_sqrt`, and `OP_fpr_lt`.
- The PC software implementation is in `platform/pc/soft/op_fpr.c`.
- The software `OP_fpr_sqrt` keeps the original Falcon-style platform path where available and falls back to `sqrt()`.

## Build Instructions

### Makefile Build

```sh
cd PQC_Crypto/PQC_FALCON/platform/pc
make clean
make
```

Build parameters:

- `CC`: C compiler. Default: `gcc`.
- `AR`: static library archiver. Default: `ar`.
- `CFLAGS`: optimization, warning, C standard, include paths, and KAT generator include path.
- `LIBS`: extra link libraries. Default: `-lm`, required for the `sqrt()` fallback.

Build outputs:

- `libpqc_falcon.a`: static library.
- `test_falcon`: test binary, built by `make test`.
- `kat_runtime`: KAT runner, built by `make kat_runtime`.

The KAT build expects the official Falcon Round 3 package in a sibling directory named `falcon-round3/` at the repository root. In that layout, the generator headers are located at `<repo-root>/falcon-round3/KAT/generator`.

## Test and KAT

The commands below are written relative to the repository root, shown as `<repo-root>`. If your delivery package stores the official Falcon Round 3 KAT files elsewhere, replace the `FALCON_ROUND3` value accordingly.

```sh
cd <repo-root>/PQC_Crypto/PQC_FALCON/platform/pc
make test
make kat_runtime
mkdir -p kat
FALCON_ROUND3=<repo-root>/falcon-round3
./kat_runtime 512 "$FALCON_ROUND3/KAT/falcon512-KAT.req" kat/falcon512.rsp
./kat_runtime 1024 "$FALCON_ROUND3/KAT/falcon1024-KAT.req" kat/falcon1024.rsp
cmp -s kat/falcon512.rsp "$FALCON_ROUND3/KAT/falcon512-KAT.rsp"
cmp -s kat/falcon1024.rsp "$FALCON_ROUND3/KAT/falcon1024-KAT.rsp"
```
