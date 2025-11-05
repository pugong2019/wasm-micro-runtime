# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.14)
project(variable-wasm-apps)

# Copy pre-compiled WASM test files to parent directory for test execution
add_custom_target(copy_variable_wasm_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/local_get_test.wasm
        ${CMAKE_CURRENT_BINARY_DIR}/../local_get_test.wasm
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/local_get_test.wat
        ${CMAKE_CURRENT_BINARY_DIR}/../local_get_test.wat
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/local_get_invalid_index_test.wat
        ${CMAKE_CURRENT_BINARY_DIR}/../local_get_invalid_index_test.wat
    COMMENT "Copy local_get_test WASM files to the same directory as google test"
)