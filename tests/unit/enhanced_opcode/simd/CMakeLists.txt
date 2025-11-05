# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.14)

project(test-enhanced-simd-opcodes)

set(WAMR_BUILD_INTERP 1)
set(WAMR_BUILD_AOT 1)
set(WAMR_BUILD_JIT 1)
set(WAMR_BUILD_FAST_JIT 0)
set(WAMR_BUILD_LIBC_WASI 1)
set(WAMR_BUILD_APP_FRAMEWORK 0)
set(WAMR_BUILD_MEMORY_PROFILING 0)

set(WAMR_BUILD_SIMD 1)

# Platform-specific definitions
add_definitions(-DRUN_ON_LINUX)

# Include shared unit test configuration
include(../../unit_common.cmake)

# Configure LLVM for AOT compilation support
set(LLVM_SRC_ROOT "${WAMR_ROOT_DIR}/core/deps/llvm")

if (NOT EXISTS "${LLVM_SRC_ROOT}/build")
    message(FATAL_ERROR "Cannot find LLVM dir: ${LLVM_SRC_ROOT}/build")
endif ()

set(CMAKE_PREFIX_PATH "${LLVM_SRC_ROOT}/build;${CMAKE_PREFIX_PATH}")
find_package(LLVM REQUIRED CONFIG)

# Add LLVM definitions and include directories
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

# Link against LLVM libraries for AOT support
llvm_map_components_to_libnames(llvm_libs
    core
    mcjit
    native
    nativecodegen
    object
    orcjit
    support
    target
    x86asmparser
    x86codegen
    x86desc
    x86disassembler
    x86info
)

# Prepare unit test sources
set(unit_test_sources
    ${UNIT_SOURCE}
    ${WAMR_RUNTIME_LIB_SOURCE}
    ${UNCOMMON_SHARED_SOURCE}
)

# Enhanced v128.not opcode test executable
add_executable(enhanced_v128_not_test
    enhanced_v128_not_test.cc
    ${unit_test_sources}
)

# Enhanced v128.and opcode test executable
add_executable(enhanced_v128_and_test
    enhanced_v128_and_test.cc
    ${unit_test_sources}
)

# Enhanced v128.andnot opcode test executable
add_executable(enhanced_v128_andnot_test
    enhanced_v128_andnot_test.cc
    ${unit_test_sources}
)

# Enhanced v128.or opcode test executable
add_executable(enhanced_v128_or_test
    enhanced_v128_or_test.cc
    ${unit_test_sources}
)

# Enhanced v128.xor opcode test executable
add_executable(enhanced_v128_xor_test
    enhanced_v128_xor_test.cc
    ${unit_test_sources}
)

# Enhanced v128.bitselect opcode test executable
add_executable(enhanced_v128_bitselect_test
    enhanced_v128_bitselect_test.cc
    ${unit_test_sources}
)

# Enhanced i8x16.eq opcode test executable
add_executable(enhanced_i8x16_eq_test
    enhanced_i8x16_eq_test.cc
    ${unit_test_sources}
)

# Enhanced i8x16.ne opcode test executable
add_executable(enhanced_i8x16_ne_test
    enhanced_i8x16_ne_test.cc
    ${unit_test_sources}
)

# Enhanced v128.any_true opcode test executable
add_executable(enhanced_v128_any_true_test
    enhanced_v128_any_true_test.cc
    ${unit_test_sources}
)

# Enhanced i8x16.swizzle opcode test executable
add_executable(enhanced_i8x16_swizzle_test
    enhanced_i8x16_swizzle_test.cc
    ${unit_test_sources}
)

# Enhanced i8x16.bitmask opcode test executable
add_executable(enhanced_i8x16_bitmask_test
    enhanced_i8x16_bitmask_test.cc
    ${unit_test_sources}
)

# Enhanced i8x16.popcnt opcode test executable
add_executable(enhanced_i8x16_popcnt_test
    enhanced_i8x16_popcnt_test.cc
    ${unit_test_sources}
)

# Enhanced i8x16.all_true opcode test executable
add_executable(enhanced_i8x16_all_true_test
    enhanced_i8x16_all_true_test.cc
    ${unit_test_sources}
)

# Enhanced f32x4.ceil opcode test executable
add_executable(enhanced_f32x4_ceil_test
    enhanced_f32x4_ceil_test.cc
    ${unit_test_sources}
)

# Link libraries and dependencies for v128.not test
target_link_libraries(enhanced_v128_not_test
    ${LLVM_AVAILABLE_LIBS}
    gtest_main
)

# Link libraries and dependencies for v128.and test
target_link_libraries(enhanced_v128_and_test
    ${LLVM_AVAILABLE_LIBS}
    gtest_main
)

# Link libraries and dependencies for v128.andnot test
target_link_libraries(enhanced_v128_andnot_test
    ${LLVM_AVAILABLE_LIBS}
    gtest_main
)

# Link libraries and dependencies for v128.or test
target_link_libraries(enhanced_v128_or_test
    ${LLVM_AVAILABLE_LIBS}
    gtest_main
)

# Link libraries and dependencies for v128.xor test
target_link_libraries(enhanced_v128_xor_test
    ${LLVM_AVAILABLE_LIBS}
    gtest_main
)

# Link libraries and dependencies for v128.bitselect test
target_link_libraries(enhanced_v128_bitselect_test
    ${LLVM_AVAILABLE_LIBS}
    gtest_main
)

# Link libraries and dependencies for i8x16.eq test
target_link_libraries(enhanced_i8x16_eq_test
    ${LLVM_AVAILABLE_LIBS}
    gtest_main
)

# Link libraries and dependencies for i8x16.ne test
target_link_libraries(enhanced_i8x16_ne_test
    ${LLVM_AVAILABLE_LIBS}
    gtest_main
)

# Link libraries and dependencies for v128.any_true test
target_link_libraries(enhanced_v128_any_true_test
    ${LLVM_AVAILABLE_LIBS}
    gtest_main
)

# Link libraries and dependencies for i8x16.swizzle test
target_link_libraries(enhanced_i8x16_swizzle_test
    ${LLVM_AVAILABLE_LIBS}
    gtest_main
)

# Link libraries and dependencies for i8x16.bitmask test
target_link_libraries(enhanced_i8x16_bitmask_test
    ${LLVM_AVAILABLE_LIBS}
    gtest_main
)

# Link libraries and dependencies for i8x16.popcnt test
target_link_libraries(enhanced_i8x16_popcnt_test
    ${LLVM_AVAILABLE_LIBS}
    gtest_main
)

# Link libraries and dependencies for i8x16.all_true test
target_link_libraries(enhanced_i8x16_all_true_test
    ${LLVM_AVAILABLE_LIBS}
    gtest_main
)

# Link libraries and dependencies for f32x4.ceil test
target_link_libraries(enhanced_f32x4_ceil_test
    ${LLVM_AVAILABLE_LIBS}
    gtest_main
)

# Copy WASM test files to build directory for test execution
add_custom_command(TARGET enhanced_v128_not_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying WASM test files to build directory"
)

add_custom_command(TARGET enhanced_v128_and_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying WASM test files to build directory for v128.and tests"
)

add_custom_command(TARGET enhanced_v128_andnot_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying WASM test files to build directory for v128.andnot tests"
)

add_custom_command(TARGET enhanced_v128_or_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying WASM test files to build directory for v128.or tests"
)

add_custom_command(TARGET enhanced_v128_xor_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying WASM test files to build directory for v128.xor tests"
)

add_custom_command(TARGET enhanced_v128_bitselect_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying WASM test files to build directory for v128.bitselect tests"
)

add_custom_command(TARGET enhanced_i8x16_eq_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying WASM test files to build directory for i8x16.eq tests"
)

add_custom_command(TARGET enhanced_i8x16_ne_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying WASM test files to build directory for i8x16.ne tests"
)

add_custom_command(TARGET enhanced_v128_any_true_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying WASM test files to build directory for v128.any_true tests"
)

add_custom_command(TARGET enhanced_i8x16_swizzle_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying WASM test files to build directory for i8x16.swizzle tests"
)

add_custom_command(TARGET enhanced_i8x16_bitmask_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying WASM test files to build directory for i8x16.bitmask tests"
)

add_custom_command(TARGET enhanced_i8x16_popcnt_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying WASM test files to build directory for i8x16.popcnt tests"
)

add_custom_command(TARGET enhanced_i8x16_all_true_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying WASM test files to build directory for i8x16.all_true tests"
)

add_custom_command(TARGET enhanced_f32x4_ceil_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying WASM test files to build directory for f32x4.ceil tests"
)

# Configure test discovery for CTest
include(GoogleTest)
gtest_discover_tests(enhanced_v128_not_test)
gtest_discover_tests(enhanced_v128_and_test)
gtest_discover_tests(enhanced_v128_andnot_test)
gtest_discover_tests(enhanced_v128_or_test)
gtest_discover_tests(enhanced_v128_xor_test)
gtest_discover_tests(enhanced_v128_bitselect_test)
gtest_discover_tests(enhanced_i8x16_eq_test)
gtest_discover_tests(enhanced_i8x16_ne_test)
gtest_discover_tests(enhanced_v128_any_true_test)
gtest_discover_tests(enhanced_i8x16_swizzle_test)
gtest_discover_tests(enhanced_i8x16_bitmask_test)
gtest_discover_tests(enhanced_i8x16_popcnt_test)
gtest_discover_tests(enhanced_i8x16_all_true_test)
gtest_discover_tests(enhanced_f32x4_ceil_test)
