# Enhanced Unit Tests for i32.add WASM Opcode
cmake_minimum_required(VERSION 3.14)
project(enhanced_i32_add_test)

add_definitions(-DRUN_ON_LINUX)

set(WAMR_BUILD_INTERP 1)
set(WAMR_BUILD_AOT 1)
set(WAMR_BUILD_LIBC_BUILTIN 1)

include(../../unit_common.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE source_all ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

set(UNIT_SOURCE ${source_all})

set(unit_test_sources
    ${UNIT_SOURCE}
    ${PLATFORM_SHARED_SOURCE}
    ${UTILS_SHARED_SOURCE}
    ${MEM_ALLOC_SHARED_SOURCE}
    ${NATIVE_INTERFACE_SOURCE}
    ${LIBC_BUILTIN_SOURCE}
    ${IWASM_COMMON_SOURCE}
    ${IWASM_INTERP_SOURCE}
    ${IWASM_AOT_SOURCE}
)

add_executable(enhanced_i32_add_test ${unit_test_sources})

target_link_libraries(enhanced_i32_add_test ${LLVM_AVAILABLE_LIBS} gtest_main)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps/i32_add_test.wasm
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps/i32_add_test.wasm
    COPYONLY
)

gtest_discover_tests(enhanced_i32_add_test)