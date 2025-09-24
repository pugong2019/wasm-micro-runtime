# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.14)

project(thread_mgr_enhanced_tests)

# Include common unit test configuration
include(../../unit_common.cmake)

# Enable thread manager features for all tests
set(WAMR_BUILD_THREAD_MGR 1)

# Add thread-mgr enhanced coverage tests (revised naming)
set(unit_test_sources
    thread_mgr_add_25_step_1.cc
    ${WAMR_RUNTIME_LIB_SOURCE}
    ${WAMR_ROOT_DIR}/core/iwasm/libraries/thread-mgr/thread_manager.c
)

add_executable(thread_mgr_add_25_step_1 ${unit_test_sources})

target_link_libraries(thread_mgr_add_25_step_1 
    gtest_main 
    pthread
    ${LLVM_AVAILABLE_LIBS}
    ${UV_A_LIBS}
)

target_include_directories(thread_mgr_add_25_step_1 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${WAMR_ROOT_DIR}/core/iwasm/libraries/thread-mgr
)

# Enable thread manager compilation flags
target_compile_definitions(thread_mgr_add_25_step_1 PRIVATE
    WASM_ENABLE_THREAD_MGR=1
    WASM_ENABLE_INTERP=1
)

# Add test to CTest
add_test(NAME thread_mgr_add_25_step_1 COMMAND thread_mgr_add_25_step_1)