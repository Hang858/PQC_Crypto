#!/usr/bin/env bash
set -euo pipefail

if [[ -z "${SCLOUDPLUS_ORIG:-}" ]]; then
  exec bash "$(dirname "$0")/run_scloudplus_openhitls_kat.sh"
fi

ROOT="$(cd "$(dirname "$0")/../../../.." && pwd)"
WORK="${ROOT}/PQC_SCLOUDPLUS/platform/pc/test/kat_build"
ORIG="${SCLOUDPLUS_ORIG:-${ROOT}/PQC_SCLOUDPLUS/platform/pc/test/original/src}"
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

TEST_TMP="$(mktemp -d)"
MOD_BIN="${TEST_TMP}/scloudplus_modified"
OPAQUE_BIN="${TEST_TMP}/scloudplus_modified_opaque"
OPAQUE_RSP="${TEST_TMP}/opaque.rsp"
trap 'rm -rf "${TEST_TMP}"' EXIT

cc -O2 -std=c99 -DUSE_HARDWARE_HASH -DSCLOUDPLUS_KAT_MODIFIED \
  -I"${MOD}/include" -I"${MOD}/core" -I"${MOD}/core/common" -I"${PC}" -I"${PC}/soft" \
  "${COMMON_MOD[@]}" -o "${MOD_BIN}"

cc -O2 -std=c99 -DUSE_HARDWARE_HASH -DOP_HASH_OPAQUE_STATE_TEST \
  -DSCLOUDPLUS_KAT_MODIFIED \
  -I"${MOD}/include" -I"${MOD}/core" -I"${MOD}/core/common" -I"${PC}" -I"${PC}/soft" \
  "${COMMON_MOD[@]}" -o "${OPAQUE_BIN}"

for level in 128 192 256; do
  if [[ -f "${ORIG}/pke.c" ]]; then
    cc -O2 -std=c99 -Dscloudplus_l="${level}" \
      -I"${WORK}" -I"${ORIG}" -I"${PC}" -I"${PC}/soft" \
      "${COMMON_ORIG[@]}" -o "${WORK}/scloudplus_original_${level}"
    "${WORK}/scloudplus_original_${level}" > "${WORK}/original_${level}.rsp"
  elif [[ ! -f "${WORK}/original_${level}.rsp" ]]; then
    echo "SCLOUDPLUS-${level}: missing upstream source and reference response" >&2
    exit 1
  fi

  "${MOD_BIN}" "${level}" > "${WORK}/modified_${level}.rsp"
  "${OPAQUE_BIN}" "${level}" > "${OPAQUE_RSP}"

  if ! cmp -s "${WORK}/original_${level}.rsp" "${WORK}/modified_${level}.rsp"; then
    echo "SCLOUDPLUS-${level}: normal-state FAIL"
    diff -u "${WORK}/original_${level}.rsp" "${WORK}/modified_${level}.rsp" | sed -n '1,120p'
    exit 1
  fi
  if ! cmp -s "${WORK}/original_${level}.rsp" "${OPAQUE_RSP}"; then
    echo "SCLOUDPLUS-${level}: opaque-state FAIL"
    diff -u "${WORK}/original_${level}.rsp" "${OPAQUE_RSP}" | sed -n '1,120p'
    exit 1
  fi
  echo "SCLOUDPLUS-${level}: PASS (normal state and opaque hardware state)"
done
