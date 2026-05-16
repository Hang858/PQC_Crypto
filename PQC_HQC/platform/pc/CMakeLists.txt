cmake_minimum_required(VERSION 3.10)
project(PQC_HQC C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(INCLUDE_PATH "${PROJECT_PATH}/include")
set(CORE_PATH "${PROJECT_PATH}/core")
set(COMMON_PATH "${CORE_PATH}/common")
set(MUNIT_PATH "${PROJECT_PATH}/tests/external/munit")

set(HQC_COMMON_SOURCES
    ${COMMON_PATH}/code.c
    ${COMMON_PATH}/fft.c
    ${COMMON_PATH}/gf.c
    ${COMMON_PATH}/gf2x.c
    ${COMMON_PATH}/hqc.c
    ${COMMON_PATH}/hqc_params.c
    ${COMMON_PATH}/kem.c
    ${COMMON_PATH}/parsing.c
    ${COMMON_PATH}/reed_muller.c
    ${COMMON_PATH}/reed_solomon.c
    ${COMMON_PATH}/symmetric.c
    ${COMMON_PATH}/vector.c
    ${COMMON_PATH}/crypto_memset.c
    ${COMMON_PATH}/fips202.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/endian.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/op_hash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/op_matrix.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/op_trng.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/sha256.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/sm3.c
)

add_library(pqc_hqc STATIC ${HQC_COMMON_SOURCES})

target_include_directories(pqc_hqc PUBLIC
    ${INCLUDE_PATH}
    ${CORE_PATH}
    ${COMMON_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/soft
)

target_compile_definitions(pqc_hqc PRIVATE USE_HARDWARE_HASH)

add_executable(test_hqc_ref
    ${CMAKE_CURRENT_SOURCE_DIR}/test/tests_main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_kem_ref.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_pke.c
    ${MUNIT_PATH}/munit.c
)

target_include_directories(test_hqc_ref PRIVATE
    ${INCLUDE_PATH}
    ${CORE_PATH}
    ${COMMON_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/soft
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${PROJECT_PATH}/tests
    ${MUNIT_PATH}
)

target_link_libraries(test_hqc_ref PRIVATE pqc_hqc)

add_executable(kat_runtime
    ${CMAKE_CURRENT_SOURCE_DIR}/test/kat_runtime.c
)

target_compile_definitions(kat_runtime PRIVATE HQC_USE_DETERMINISTIC_PRNG)

target_include_directories(kat_runtime PRIVATE
    ${INCLUDE_PATH}
    ${CORE_PATH}
    ${COMMON_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/soft
)

target_link_libraries(kat_runtime PRIVATE pqc_hqc)

enable_testing()
add_test(NAME pqc_hqc_ref COMMAND test_hqc_ref)
