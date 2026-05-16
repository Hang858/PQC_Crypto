#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/../../../.." && pwd)"
WORK="${ROOT}/PQC_SCLOUDPLUS/platform/pc/test/kat_build"
ORIG="${SCLOUDPLUS_ORIG:-${ROOT}/PQC_Crypto/PQC_SCLOUDPLUS/platform/pc/test/original/src}"
MOD="${ROOT}/PQC_SCLOUDPLUS"
PC="${MOD}/platform/pc"

mkdir -p "${WORK}"
mkdir -p "${WORK}/PQCDevKitAPI"
cat > "${WORK}/PQCDevKitAPI/operator_interface.h" <<'HEADER'
#ifndef SCLOUDPLUS_KAT_OPERATOR_INTERFACE_H
#define SCLOUDPLUS_KAT_OPERATOR_INTERFACE_H
#include <stdint.h>
#define OP_ALG_SHAKE128 4
#define OP_ALG_SHAKE256 3
#define OP_MODE_NORMAL 0
int OP_hash(uint8_t alg, uint8_t mode, int n, const void *input, int input_len, uint8_t link_count, void *output);
#endif
HEADER

COMMON_ORIG=(
  "${ORIG}/aes_c.c"
  "${ORIG}/encode.c"
  "${ORIG}/fips202.c"
  "${ORIG}/sample.c"
  "${ORIG}/pke.c"
  "${ORIG}/matrix.c"
  "${ORIG}/util.c"
  "${ORIG}/kem.c"
  "${PC}/soft/op_hash.c"
  "${PC}/soft/op_matrix.c"
  "${PC}/soft/sha256.c"
  "${PC}/soft/sm3.c"
  "${PC}/soft/endian.c"
  "${PC}/test/kat_random.c"
  "${PC}/test/kat_driver.c"
)

COMMON_MOD=(
  "${MOD}/core/common/aes_c.c"
  "${MOD}/core/common/encode.c"
  "${MOD}/core/common/fips202.c"
  "${MOD}/core/common/sample.c"
  "${MOD}/core/common/pke.c"
  "${MOD}/core/common/matrix.c"
  "${MOD}/core/common/util.c"
  "${MOD}/core/common/kem.c"
  "${MOD}/core/common/scloudplus_params.c"
  "${PC}/soft/op_hash.c"
  "${PC}/soft/op_matrix.c"
  "${PC}/soft/sha256.c"
  "${PC}/soft/sm3.c"
  "${PC}/soft/endian.c"
  "${PC}/test/kat_random.c"
  "${PC}/test/kat_driver.c"
)

for level in 128 192 256; do
  cc -O2 -std=c99 -Dscloudplus_l="${level}" \
    -I"${WORK}" -I"${ORIG}" -I"${PC}" -I"${PC}/soft" \
    "${COMMON_ORIG[@]}" -o "${WORK}/scloudplus_original_${level}"

  cc -O2 -std=c99 -DSCLOUDPLUS_KAT_MODIFIED \
    -I"${MOD}/include" -I"${MOD}/core" -I"${MOD}/core/common" -I"${PC}" -I"${PC}/soft" \
    "${COMMON_MOD[@]}" -o "${WORK}/scloudplus_modified"

  "${WORK}/scloudplus_original_${level}" > "${WORK}/original_${level}.rsp"
  "${WORK}/scloudplus_modified" "${level}" > "${WORK}/modified_${level}.rsp"

  if cmp -s "${WORK}/original_${level}.rsp" "${WORK}/modified_${level}.rsp"; then
    echo "SCLOUDPLUS-${level}: PASS"
  else
    echo "SCLOUDPLUS-${level}: FAIL"
    diff -u "${WORK}/original_${level}.rsp" "${WORK}/modified_${level}.rsp" | sed -n '1,120p'
    exit 1
  fi
done
