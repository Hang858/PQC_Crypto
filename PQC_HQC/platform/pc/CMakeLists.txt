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
    ${COMMON_PATH}/kem.c
    ${COMMON_PATH}/parsing.c
    ${COMMON_PATH}/reed_muller.c
    ${COMMON_PATH}/reed_solomon.c
    ${COMMON_PATH}/symmetric.c
    ${COMMON_PATH}/vector.c
)

function(add_hqc_level_library target level_macro level_include)
    add_library(${target} STATIC ${HQC_COMMON_SOURCES})
    target_compile_definitions(${target} PRIVATE ${level_macro})
    target_include_directories(${target} PUBLIC
        ${INCLUDE_PATH}
        ${CORE_PATH}
        ${COMMON_PATH}
        ${level_include}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/soft
    )
endfunction()

add_hqc_level_library(hqc1_ref HQC_LEVEL_HQC1 ${CORE_PATH}/hqc1)
add_hqc_level_library(hqc3_ref HQC_LEVEL_HQC3 ${CORE_PATH}/hqc3)
add_hqc_level_library(hqc5_ref HQC_LEVEL_HQC5 ${CORE_PATH}/hqc5)

add_library(pqc_hqc STATIC
    ${CORE_PATH}/api.c
    ${COMMON_PATH}/crypto_memset.c
    ${COMMON_PATH}/fips202.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/endian.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/op_hash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/sha256.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/sm3.c
)

target_include_directories(pqc_hqc PUBLIC
    ${INCLUDE_PATH}
    ${CORE_PATH}
    ${COMMON_PATH}
    ${CORE_PATH}/hqc1
    ${CORE_PATH}/hqc3
    ${CORE_PATH}/hqc5
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/soft
)

target_link_libraries(pqc_hqc PUBLIC hqc1_ref hqc3_ref hqc5_ref)

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
    ${CORE_PATH}/hqc1
    ${CORE_PATH}/hqc3
    ${CORE_PATH}/hqc5
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/soft
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${PROJECT_PATH}/tests
    ${MUNIT_PATH}
)

target_link_libraries(test_hqc_ref PRIVATE pqc_hqc)

enable_testing()
add_test(NAME pqc_hqc_ref COMMAND test_hqc_ref)
