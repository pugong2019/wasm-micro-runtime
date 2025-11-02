# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.14)

project(test-enhanced-i32-rotr-opcode)

# Enable required WAMR build flags for i32.rotr opcode testing
set(WAMR_BUILD_INTERP 1)
set(WAMR_BUILD_AOT 1)
set(WAMR_BUILD_JIT 1)
set(WAMR_BUILD_FAST_JIT 0)
set(WAMR_BUILD_LIBC_WASI 1)
set(WAMR_BUILD_APP_FRAMEWORK 0)
set(WAMR_BUILD_MEMORY_PROFILING 0)

# Platform-specific definitions
add_definitions(-DRUN_ON_LINUX)

# Include shared unit test configuration
include(${CMAKE_CURRENT_SOURCE_DIR}/../../../unit_common.cmake)

# Add the enhanced i32.rotr test executable
set(SOURCES enhanced_i32_rotr_test.cc)

add_executable(enhanced_i32_rotr_test ${SOURCES})

# Link against required WAMR libraries and GTest
target_link_libraries(enhanced_i32_rotr_test
    ${WAMR_RUNTIME_LIB_SOURCE}
    gtest_main
    ${LLVM_AVAILABLE_LIBS}
    ${UV_A_LIBS}
    -lm -ldl -lpthread)

# Set target properties for proper compilation
set_target_properties(enhanced_i32_rotr_test PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON)

# Add compiler flags for coverage collection (if enabled)
if(COLLECT_CODE_COVERAGE)
    target_compile_options(enhanced_i32_rotr_test PRIVATE --coverage)
    target_link_options(enhanced_i32_rotr_test PRIVATE --coverage)
endif()

# Copy WASM test files to build directory for test execution
add_custom_command(TARGET enhanced_i32_rotr_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying i32_rotr WASM test files to build directory")

# Register the test with CTest for automated testing
add_test(NAME enhanced_i32_rotr_test
         COMMAND enhanced_i32_rotr_test
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})