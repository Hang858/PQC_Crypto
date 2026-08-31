cmake_minimum_required(VERSION 3.10)
project(PQC_FRODOKEM C)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(INCLUDE_PATH "${PROJECT_PATH}/include")
set(CORE_PATH "${PROJECT_PATH}/core")
set(COMMON_PATH "${CORE_PATH}/common")

set(FRODOKEM_COMMON_SOURCES
    ${COMMON_PATH}/fips202.c
    ${COMMON_PATH}/frodo_macrify.c
    ${COMMON_PATH}/frodokem_params.c
    ${COMMON_PATH}/kem.c
    ${COMMON_PATH}/noise.c
    ${COMMON_PATH}/random.c
    ${COMMON_PATH}/util.c
    ${COMMON_PATH}/malloc.c
    ${COMMON_PATH}/memops.c
    ${COMMON_PATH}/my_log.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/endian.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/op_hash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/op_matrix.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/op_trng.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/op_fpr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/sha256.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soft/sm3.c
)

add_library(pqc_frodokem STATIC ${FRODOKEM_COMMON_SOURCES})
target_compile_options(pqc_frodokem PRIVATE
    -fstack-usage
)

target_compile_definitions(pqc_frodokem PRIVATE
    USE_HARDWARE_HASH
)

target_include_directories(pqc_frodokem PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/soft
    ${INCLUDE_PATH}
    ${CORE_PATH}
    ${COMMON_PATH}
)

add_executable(test_frodokem_ref
    ${CMAKE_CURRENT_SOURCE_DIR}/test/tests_main.c
)

target_include_directories(test_frodokem_ref PRIVATE
    ${INCLUDE_PATH}
    ${CORE_PATH}
    ${COMMON_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/soft
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${PROJECT_PATH}/tests
)

target_link_libraries(test_frodokem_ref PRIVATE pqc_frodokem)

target_compile_options(test_frodokem_ref PRIVATE
    -fstack-usage
)

enable_testing()
add_test(NAME pqc_frodokem_ref COMMAND test_frodokem_ref)
