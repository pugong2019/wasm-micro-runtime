# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.14)

project(test-enhanced-simd-opcodes)

# Enable required WAMR build flags for SIMD opcode testing (v128.not, v128.and)
set(WAMR_BUILD_INTERP 1)
set(WAMR_BUILD_AOT 1)
set(WAMR_BUILD_JIT 1)
set(WAMR_BUILD_FAST_JIT 0)
set(WAMR_BUILD_LIBC_WASI 1)
set(WAMR_BUILD_APP_FRAMEWORK 0)
set(WAMR_BUILD_MEMORY_PROFILING 0)

# CRITICAL: Enable SIMD support for v128.not and v128.and opcode testing
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

# Configure test discovery for CTest
include(GoogleTest)
gtest_discover_tests(enhanced_v128_not_test)
gtest_discover_tests(enhanced_v128_and_test)