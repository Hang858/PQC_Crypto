#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/../../../.." && pwd)"
MOD="${ROOT}/PQC_SCLOUDPLUS"
PC="${MOD}/platform/pc"
PQCP_ROOT="${OPENHITLS_PQCP_ROOT:-/tmp/openhitls-pqcp}"
DATA="${SCLOUDPLUS_OPENHITLS_DATA:-${PQCP_ROOT}/testcode/sdv/scloudplus/test_suite_sdv_pqcp_scloudplus.data}"

if [[ ! -f "${DATA}" ]]; then
  echo "openHiTLS/PQCP vector file not found: ${DATA}" >&2
  echo "clone https://gitcode.com/openHiTLS/pqcp.git or set SCLOUDPLUS_OPENHITLS_DATA" >&2
  exit 2
fi

COMMON=(
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
  "${PC}/test/openhitls_vector_driver.c"
)

TEST_TMP="$(mktemp -d)"
NORMAL_BIN="${TEST_TMP}/scloudplus_openhitls"
OPAQUE_BIN="${TEST_TMP}/scloudplus_openhitls_opaque"
trap 'rm -rf "${TEST_TMP}"' EXIT

build_driver() {
  local output=$1
  shift
  cc -O2 -std=c99 -DUSE_HARDWARE_HASH "$@" \
    -I"${MOD}/include" -I"${MOD}/core" -I"${MOD}/core/common" \
    -I"${PC}" -I"${PC}/soft" "${COMMON[@]}" -o "${output}"
}

unquote() {
  REPLY=${1#\"}
  REPLY=${REPLY%\"}
}

build_driver "${NORMAL_BIN}"
build_driver "${OPAQUE_BIN}" -DOP_HASH_OPAQUE_STATE_TEST

count=0
while IFS=: read -r test_name level alpha z message expected_pk expected_sk expected_ct expected_ss ||
      [[ -n "${test_name:-}" ]]; do
  if [[ "${test_name}" != "SDV_CRYPTO_PQCP_SCLOUDPLUS_VECTOR_TC001" ]]; then
    continue
  fi
  unquote "${alpha}"; alpha=${REPLY}
  unquote "${z}"; z=${REPLY}
  unquote "${message}"; message=${REPLY}
  unquote "${expected_pk}"; expected_pk=${REPLY}
  unquote "${expected_sk}"; expected_sk=${REPLY}
  unquote "${expected_ct}"; expected_ct=${REPLY}
  unquote "${expected_ss}"; expected_ss=${REPLY}

  if ! "${NORMAL_BIN}" "${level}" "${alpha}" "${z}" "${message}" \
    "${expected_pk}" "${expected_sk}" "${expected_ct}" "${expected_ss}"; then
    echo "${level} vector $((count + 1)): normal-state FAIL" >&2
    exit 1
  fi
  if ! "${OPAQUE_BIN}" "${level}" "${alpha}" "${z}" "${message}" \
    "${expected_pk}" "${expected_sk}" "${expected_ct}" "${expected_ss}"; then
    echo "${level} vector $((count + 1)): opaque-state FAIL" >&2
    exit 1
  fi
  count=$((count + 1))
done < "${DATA}"

if [[ ${count} -eq 0 ]]; then
  echo "no SCloudPlus vectors found in ${DATA}" >&2
  exit 1
fi
echo "openHiTLS/PQCP SCloudPlus: ${count} vectors PASS (normal and opaque state)"
