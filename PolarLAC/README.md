# polarlac

# Copyright Notice

This repository is an optimized version based on the original **LAC v3** implementation.

## Original LAC v3  
**Copyright (c) 2019** — The original LAC team  

## This Version  
**Copyright (c) 2025**  
**Contributors**: Xianhui Lu, Ying Liu, Ziyao Liu, Yu Zhang
**Affiliations**:  
- Key Laboratory of Cyberspace Security Defense, Institute of Information Engineering, Chinese Academy of Sciences (CAS)  
- School of Cyber Security, University of Chinese Academy of Sciences

This modified implementation, named **PolarLAC**, is based on [**LAC v3**](https://github.com/pqc-lac/lac-intel64) and written in C. Compared to the original LAC v3, it introduces the following improvements:

- **Polynomial Multiplication Optimization**:  
  Replaces the original polynomial multiplication in LAC v3 with an NTT-based method using compact modulus lifting.  
  *(See: `core/bin-lwe.c`, especially the functions `poly_mul` and `poly_aff`, which route to `ntt.h`, `ntt.c`, `ntt-1024.h`, and `ntt-1024.c`)*

- **Secret and Noise Sampling Optimization**:  
  Updated the sampling distribution for secrets and noise.  
  *(See: `core/include/bin-lwe.h`, `core/bin-lwe.c`)*

- **Error-Correcting Code Update**:  
  Replaced the BCH+D2 error-correcting codes in the original LAC v3 with Polar codes.  
  *(See: `core/include/polar.h`, `core/polar.c`, `core/lac128/polar128.h`, `core/lac256/polar256.h`, and invoked in `core/ecc.c`)*

---

## Version: May 2025

### Changes Compared to the Previous Commit:
- Removed the 192-bit security level parameters and implementations, retaining the 128-bit and 256-bit security level parameters and their corresponding implementations.
- Updated the Polar code decoding algorithm and its input format.  
  *(Updated files: `core/encrypt.c`, `core/polar.c`, `core/lac128/polar128.h`, `core/lac256/polar256.h`)*

---

## Code Description

This repository provides a **reference implementation of the LAC-Polar algorithm**.  
The hash and random-number paths use the local operator/software interfaces, so the build does not require third-party cryptographic or math libraries.

### Directory Layout

```text
polarlac/
├── core/              # platform-independent algorithm implementation
│   ├── include/       # internal headers for algorithm modules
│   ├── laclight/      # LAC_LIGHT branch parameters and macros
│   ├── lac128/        # LAC128 branch parameters and macros
│   └── lac256/        # LAC256 branch parameters and macros
├── include/           # public application-facing header
│   └── polarlac.h
├── platform/          # platform adaptation layer
│   ├── pc/            # PC software operator implementation
│   │   └── include/
│   └── ts300v/        # reserved hardware-platform project area
└── test/              # correctness and performance tests
```

### Parameter Branching

The public header `include/polarlac.h` exposes a unified level-aware API:

```c
polarlac_crypto_kem_keypair(POLARLAC_LEVEL_128, pk, sk);
polarlac_crypto_kem_enc(POLARLAC_LEVEL_128, ct, ss, pk);
polarlac_crypto_kem_dec(POLARLAC_LEVEL_128, ss, ct, sk);
```

Branch parameters are described by `polarlac_params_t` and can be queried with
`polarlac_get_params(level)`. The upper API is shared across all security
levels; internal code reads dimensions, Polar tables, NTT settings, and integer
LLR tables from the selected runtime parameter structure.

### Build Instructions

To compile the program, simply run:

```bash
mkdir build && cd build
cmake ..
make
```

Then execute:

```bash
./lac correctness 128
```

Supported runtime level arguments are `light`, `128`, and `256`.

### 1. Correctness Test

Run the following command to test the correctness of the encryption and decryption functions in the **LAC-Polar.KEM**:

```bash
./lac correctness light
./lac correctness 128
./lac correctness 256
```

### 2. Performance Test

#### CPU Clock Cycle Test

Run:

```bash
./lac cpucycles 128
```

#### Execution Time Test (in microseconds)

Run:

```bash
./lac speed 128
```

> **Note:**  
> The test executable initializes the default branch through
> `polarlac_current_params()`. Branch-specific scalar parameters live under
> `core/laclight`, `core/lac128`, and `core/lac256`.
