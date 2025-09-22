# Copyright (C) 2024 Xiaomi Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.14)

project(test-shared-heap-enhanced)

add_definitions(-DRUN_ON_LINUX)

set(WAMR_BUILD_APP_FRAMEWORK 0)
set(WAMR_BUILD_AOT 1)
set(WAMR_BUILD_INTERP 1)
set(WAMR_BUILD_FAST_INTERP 1)
set(WAMR_BUILD_JIT 0)
if(WAMR_BUILD_TARGET STREQUAL "X86_32")
    set(WAMR_BUILD_MEMORY64 0)
else()
    set(WAMR_BUILD_MEMORY64 1)
endif()
set(WAMR_BUILD_SHARED_HEAP 1)

# Set WAMR root directory
set(WAMR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)

# Include unit test common configuration
include(../../unit_common.cmake)

# Set LLVM configuration for AOT compilation
set(LLVM_SRC_ROOT "${WAMR_ROOT_DIR}/core/deps/llvm")

if (NOT EXISTS "${LLVM_SRC_ROOT}/build")
    message(FATAL_ERROR "Cannot find LLVM dir: ${LLVM_SRC_ROOT}/build")
endif ()

set(CMAKE_PREFIX_PATH "${LLVM_SRC_ROOT}/build;${CMAKE_PREFIX_PATH}")
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Include compilation module for AOT
include(${IWASM_DIR}/compilation/iwasm_compl.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Find all test source files (exclude gmock files to avoid conflicts)
file(GLOB source_all ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

set(UNIT_SOURCE ${source_all})

set(unit_test_sources
        ${UNIT_SOURCE}
        ${WAMR_RUNTIME_LIB_SOURCE}
        ${UNCOMMON_SHARED_SOURCE}
        )

# Create the test executable
add_executable(shared_heap_enhanced_test ${unit_test_sources})

# Link with required libraries
target_link_libraries(shared_heap_enhanced_test ${LLVM_AVAILABLE_LIBS} gtest_main)

# Include GoogleTest module for test discovery
include(GoogleTest)

# Discover and register tests
gtest_discover_tests(shared_heap_enhanced_test)