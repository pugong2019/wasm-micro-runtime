# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.14)

project(test-libc-wasi)

add_definitions(-DRUN_ON_LINUX)

set(WAMR_BUILD_LIBC_WASI 1)
set(WAMR_BUILD_APP_FRAMEWORK 0)
set(WAMR_BUILD_AOT 1)
set(WAMR_BUILD_INTERP 1)
set(WAMR_BUILD_FAST_INTERP 1)
set(WAMR_BUILD_JIT 0)
set(WAMR_BUILD_FAST_JIT 0)
set(WAMR_BUILD_MEMORY64 0)
set(WAMR_BUILD_SHARED_MEMORY 0)

# Include common unit test configuration
include(../../unit_common.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE source_all ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

set(UNIT_SOURCE ${source_all})

aux_source_directory(. SRC_LIST)

set(unit_test_sources
        ${UNIT_SOURCE}
        ${WAMR_RUNTIME_LIB_SOURCE}
        ${UNCOMMON_SHARED_SOURCE}
        ${SRC_LIST}
        )

# Create the test executable
add_executable(libc_wasi_test ${unit_test_sources})

target_link_libraries(libc_wasi_test gtest_main)

# Copy WASM files to build directory if they exist
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps)
    add_custom_command(TARGET libc_wasi_test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
            ${CMAKE_CURRENT_BINARY_DIR}/
            COMMENT "Copy test wasm files to the directory of google test"
            )
endif()

gtest_discover_tests(libc_wasi_test)