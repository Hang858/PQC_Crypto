cmake_minimum_required(VERSION 3.10)
project(PQC_SCLOUDPLUS C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(INCLUDE_PATH "${PROJECT_PATH}/include")
set(CORE_PATH "${PROJECT_PATH}/core")
set(COMMON_PATH "${CORE_PATH}/common")

set(SCLOUDPLUS_COMMON_SOURCES
    ${COMMON_PATH}/aes_c.c
    ${COMMON_PATH}/encode.c
    ${COMMON_PATH}/fips202.c
    ${COMMON_PATH}/kem.c
    ${COMMON_PATH}/matrix.c
    ${COMMON_PATH}/pke.c
    ${COMMON_PATH}/random.c
    ${COMMON_PATH}/sample.c
    ${COMMON_PATH}/scloudplus_params.c
    ${COMMON_PATH}/util.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/endian.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/op_hash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/op_matrix.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/op_trng.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/op_fpr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/sha256.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/sm3.c
)

add_library(pqc_scloudplus STATIC ${SCLOUDPLUS_COMMON_SOURCES})

target_compile_definitions(pqc_scloudplus PRIVATE USE_HARDWARE_HASH)

target_include_directories(pqc_scloudplus PUBLIC
    ${INCLUDE_PATH}
    ${CORE_PATH}
    ${COMMON_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/soft
)

add_executable(test_scloudplus_ref
    ${CMAKE_CURRENT_SOURCE_DIR}/test/tests_main.c
)

target_include_directories(test_scloudplus_ref PRIVATE
    ${INCLUDE_PATH}
    ${CORE_PATH}
    ${COMMON_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/soft
    ${CMAKE_CURRENT_SOURCE_DIR}/test
)

target_link_libraries(test_scloudplus_ref PRIVATE pqc_scloudplus)

enable_testing()
add_test(NAME pqc_scloudplus_ref COMMAND test_scloudplus_ref)
