# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.14)

project(test-enhanced-simd-opcodes)

# Enable required WAMR build flags for SIMD opcode testing (v128.not, v128.and, v128.andnot, v128.or, v128.xor, v128.bitselect, i8x16.eq)
set(WAMR_BUILD_INTERP 1)
set(WAMR_BUILD_AOT 1)
set(WAMR_BUILD_JIT 1)
set(WAMR_BUILD_FAST_JIT 0)
set(WAMR_BUILD_LIBC_WASI 1)
set(WAMR_BUILD_APP_FRAMEWORK 0)
set(WAMR_BUILD_MEMORY_PROFILING 0)

# CRITICAL: Enable SIMD support for v128.not, v128.and, v128.andnot, v128.or, v128.xor, v128.bitselect, and i8x16.eq opcode testing
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

# Configure test discovery for CTest
include(GoogleTest)
gtest_discover_tests(enhanced_v128_not_test)
gtest_discover_tests(enhanced_v128_and_test)
gtest_discover_tests(enhanced_v128_andnot_test)
gtest_discover_tests(enhanced_v128_or_test)
gtest_discover_tests(enhanced_v128_xor_test)
gtest_discover_tests(enhanced_v128_bitselect_test)
gtest_discover_tests(enhanced_i8x16_eq_test)