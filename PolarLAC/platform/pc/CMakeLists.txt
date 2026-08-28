cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_COMPILER clang)
project(polarlac C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /O2 /utf-8")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Oz -s -Wno-stringop-overread -fno-unroll-loops -fno-stack-protector -fstack-usage")
endif()

set(POLARLAC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(POLARLAC_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(POLARLAC_CORE_DIR ${POLARLAC_ROOT}/core)
set(POLARLAC_PLATFORM "pc" CACHE STRING "Target platform adapter")

if(NOT POLARLAC_PLATFORM STREQUAL "pc")
    message(FATAL_ERROR "Unsupported POLARLAC_PLATFORM: ${POLARLAC_PLATFORM}")
endif()

set(POLARLAC_PLATFORM_DIR ${POLARLAC_ROOT}/platform/${POLARLAC_PLATFORM})

file(GLOB POLARLAC_CORE_SOURCES CONFIGURE_DEPENDS
    "${POLARLAC_CORE_DIR}/*.c"
)
file(GLOB POLARLAC_BRANCH_SOURCES CONFIGURE_DEPENDS
    "${POLARLAC_CORE_DIR}/lac*/*.c"
)
file(GLOB POLARLAC_PLATFORM_SOURCES CONFIGURE_DEPENDS
    "${POLARLAC_PLATFORM_DIR}/*.c"
)

add_library(polarlac STATIC
    ${POLARLAC_CORE_SOURCES}
    ${POLARLAC_BRANCH_SOURCES}
    ${POLARLAC_PLATFORM_SOURCES}
)
target_include_directories(polarlac
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${POLARLAC_ROOT}/include
    PRIVATE
        ${POLARLAC_CORE_DIR}
        ${POLARLAC_CORE_DIR}/include
        ${POLARLAC_CORE_DIR}/laclight
        ${POLARLAC_CORE_DIR}/lac128
        ${POLARLAC_CORE_DIR}/lac256
        ${POLARLAC_PLATFORM_DIR}/include
)

file(GLOB POLARLAC_TEST_SOURCES CONFIGURE_DEPENDS
    "${POLARLAC_TEST_DIR}/*.c"
)
# add_executable(lac ${POLARLAC_TEST_SOURCES})
add_executable(lac main.c)
target_link_libraries(lac PRIVATE polarlac)
target_include_directories(lac PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${POLARLAC_TEST_DIR}
    ${POLARLAC_CORE_DIR}
    ${POLARLAC_CORE_DIR}/include
    ${POLARLAC_PLATFORM_DIR}/include
)
# target_compile_definitions(lac PRIVATE ENABLE_LOG)


add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMENT "Cleaning build artifacts"
)
