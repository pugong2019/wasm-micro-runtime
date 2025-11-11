# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.14)

project(test-running-modes)

# Compile wasm modules
add_subdirectory(wasm-apps)

add_definitions(-DRUN_ON_LINUX)

set(WAMR_BUILD_LIBC_WASI 1)
set(WAMR_BUILD_APP_FRAMEWORK 0)
set(WAMR_BUILD_JIT 1)
set(WAMR_BUILD_FAST_JIT 1)

# if only load this CMake other than load it as subdirectory
include(../unit_common.cmake)

set(LLVM_SRC_ROOT "${WAMR_ROOT_DIR}/core/deps/llvm")

if (NOT EXISTS "${LLVM_SRC_ROOT}/build")
    message(FATAL_ERROR "Cannot find LLVM dir: ${LLVM_SRC_ROOT}/build")
endif ()

set(CMAKE_PREFIX_PATH "${LLVM_SRC_ROOT}/build;${CMAKE_PREFIX_PATH}")
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include(${IWASM_DIR}/compilation/iwasm_compl.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# list(FILTER source_all EXCLUDE REGEX "${CMAKE_CURRENT_BINARY_DIR}/.*")

# set(UNIT_SOURCE ${source_all})

# Define source file categories
set(NUMERIC_SOURCES
    enhanced_f32_abs_test.cc
    enhanced_f32_ceil_test.cc
    enhanced_f32_copysign_test.cc
    enhanced_f32_div_test.cc
    enhanced_f32_floor_test.cc
    enhanced_f32_gt_test.cc
    enhanced_f32_lt_test.cc
    enhanced_f32_max_test.cc
    enhanced_f32_min_test.cc
    enhanced_f32_mul_test.cc
    enhanced_f32_nearest_test.cc
    enhanced_f32_reinterpret_i32_test.cc
    enhanced_f32_sqrt_test.cc
    enhanced_f32_sub_test.cc
    enhanced_f64_convert_i32_s_test.cc
    enhanced_f64_convert_i32_u_test.cc
    enhanced_f64_convert_i64_s_test.cc
    enhanced_f64_convert_i64_u_test.cc
    enhanced_f64_copysign_test.cc
    enhanced_f64_div_test.cc
    enhanced_f64_floor_test.cc
    enhanced_f64_ge_test.cc
    enhanced_f64_le_test.cc
    enhanced_f64_max_test.cc
    enhanced_f64_min_test.cc
    enhanced_f64_mul_test.cc
    enhanced_f64_nearest_test.cc
    enhanced_f64_promote_f32_test.cc
    enhanced_f64_reinterpret_i64_test.cc
    enhanced_f64_sqrt_test.cc
    enhanced_f64_sub_test.cc
    enhanced_f64_trunc_test.cc
    enhanced_i32_add_test.cc
    enhanced_i32_and_test.cc
    enhanced_i32_clz_test.cc
    enhanced_i32_ctz_test.cc
    enhanced_i32_div_s_test.cc
    enhanced_i32_div_u_test.cc
    enhanced_i32_eq_test.cc
    enhanced_i32_eqz_test.cc
    enhanced_i32_extend16_s_test.cc
    enhanced_i32_extend8_s_test.cc
    enhanced_i32_gt_s_test.cc
    enhanced_i32_gt_u_test.cc
    enhanced_i32_le_s_test.cc
    enhanced_i32_le_u_test.cc
    enhanced_i32_lt_s_test.cc
    enhanced_i32_lt_u_test.cc
    enhanced_i32_mul_test.cc
    enhanced_i32_ne_test.cc
    enhanced_i32_popcnt_test.cc
    enhanced_i32_reinterpret_f32_test.cc
    enhanced_i32_rem_s_test.cc
    enhanced_i32_rem_u_test.cc
    enhanced_i32_rotl_test.cc
    enhanced_i32_rotr_test.cc
    enhanced_i32_shl_test.cc
    enhanced_i32_shr_s_test.cc
    enhanced_i32_shr_u_test.cc
    enhanced_i32_sub_test.cc
    enhanced_i32_trunc_f32_s_test.cc
    enhanced_i32_trunc_f32_u_test.cc
    enhanced_i32_trunc_f64_s_test.cc
    enhanced_i32_trunc_sat_f32_s_test.cc
    enhanced_i32_trunc_sat_f32_u_test.cc
    enhanced_i32_trunc_sat_f64_s_test.cc
    enhanced_i32_trunc_sat_f64_u_test.cc
    enhanced_i32_wrap_i64_test.cc
    enhanced_i32_xor_test.cc
    enhanced_i64_clz_test.cc
    enhanced_i64_ctz_test.cc
    enhanced_i64_extend16_s_test.cc
    enhanced_i64_extend32_s_test.cc
    enhanced_i64_extend8_s_test.cc
    enhanced_i64_extend_i32_s_test.cc
    enhanced_i64_extend_i32_u_test.cc
    enhanced_i64_ge_s_test.cc
    enhanced_i64_ge_u_test.cc
    enhanced_i64_gt_s_test.cc
    enhanced_i64_gt_u_test.cc
    enhanced_i64_le_s_test.cc
    enhanced_i64_le_u_test.cc
    enhanced_i64_lt_s_test.cc
    enhanced_i64_lt_u_test.cc
    enhanced_i64_popcnt_test.cc
    enhanced_i64_reinterpret_f64_test.cc
    enhanced_i64_rem_s_test.cc
    enhanced_i64_rem_u_test.cc
    enhanced_i64_rotl_test.cc
    enhanced_i64_rotr_test.cc
    enhanced_i64_shl_test.cc
    enhanced_i64_shr_s_test.cc
    enhanced_i64_shr_u_test.cc
    enhanced_i64_trunc_sat_f32_s_test.cc
    enhanced_i64_trunc_sat_f32_u_test.cc
    enhanced_i64_trunc_sat_f64_s_test.cc
    enhanced_i64_trunc_sat_f64_u_test.cc
    enhanced_i64_trunc_s_test.cc
    enhanced_i64_trunc_u_test.cc
    enhanced_i64_xor_test.cc
)

set(CTRL_FLOW_SOURCES
    enhanced_block_bt_test.cc
    enhanced_call_test.cc
    enhanced_call_indirect_test.cc
    enhanced_return_call_test.cc
    enhanced_return_call_indirect_test.cc
    enhanced_return_call_ref_test.cc
    enhanced_end_test.cc
    enhanced_select_test.cc
)

set(VARIABLE_SOURCES
    enhanced_local_get_test.cc
    enhanced_local_set_test.cc
    enhanced_local_tee_test.cc
    enhanced_global_get_test.cc
    enhanced_global_set_test.cc
    enhanced_drop_test.cc
)

set(MEMORY_SOURCES
    enhanced_f32_load_test.cc
    enhanced_f32_store_test.cc
    enhanced_f64_load_test.cc
    enhanced_f64_store_test.cc
    enhanced_i32_load_test.cc
    enhanced_i32_load8_s_test.cc
    enhanced_i32_load8_u_test.cc
    enhanced_i32_load16_s_test.cc
    enhanced_i32_load16_u_test.cc
    enhanced_i32_store_test.cc
    enhanced_i32_store8_test.cc
    enhanced_i32_store16_test.cc
    enhanced_i64_load_test.cc
    enhanced_i64_load8_s_test.cc
    enhanced_i64_load8_u_test.cc
    enhanced_i64_load16_s_test.cc
    enhanced_i64_load16_u_test.cc
    enhanced_i64_load32_s_test.cc
    enhanced_i64_load32_u_test.cc
    enhanced_i64_store_test.cc
    enhanced_i64_store8_test.cc
    enhanced_i64_store16_test.cc
    enhanced_i64_store32_test.cc
    enhanced_memory_copy_test.cc
    enhanced_memory_fill_test.cc
    enhanced_memory_grow_test.cc
    enhanced_memory_init_test.cc
    enhanced_memory_size_test.cc
)

# Common sources shared by all binaries
set(RUNNING_MODES_SOURCES
    wasm_running_modes_test.cc
    enhanced_jit_dump_test.cc
    enhanced_jit_frontend_test.cc
)

aux_source_directory(. SRC_LIST)

# Create four separate executables
set(common_test_sources
    ${WAMR_RUNTIME_LIB_SOURCE}
    ${UNCOMMON_SHARED_SOURCE}
    # ${SRC_LIST}
)

# FastJIT Numeric Tests
add_executable(fastjit_numeric ${NUMERIC_SOURCES} ${common_test_sources})
target_link_libraries(fastjit_numeric ${LLVM_AVAILABLE_LIBS} gtest_main)

# FastJIT Control Flow Tests
add_executable(fastjit_ctrl_flow ${CTRL_FLOW_SOURCES} ${common_test_sources})
target_link_libraries(fastjit_ctrl_flow ${LLVM_AVAILABLE_LIBS} gtest_main)

# FastJIT Variable Tests
add_executable(fastjit_variable ${VARIABLE_SOURCES} ${common_test_sources})
target_link_libraries(fastjit_variable ${LLVM_AVAILABLE_LIBS} gtest_main)

# FastJIT Memory Tests
add_executable(fastjit_memory ${MEMORY_SOURCES} ${common_test_sources})
target_link_libraries(fastjit_memory ${LLVM_AVAILABLE_LIBS} gtest_main)

# FastJIT Memory Tests
add_executable(wasm_running_modes_test ${RUNNING_MODES_SOURCES} ${common_test_sources})
target_link_libraries(wasm_running_modes_test ${LLVM_AVAILABLE_LIBS} gtest_main)

include(GoogleTest)
gtest_discover_tests(fastjit_numeric)
gtest_discover_tests(fastjit_ctrl_flow)
gtest_discover_tests(fastjit_variable)
gtest_discover_tests(fastjit_memory)
gtest_discover_tests(wasm_running_modes_test)

# Copy WASM test files to build directory for test execution
add_custom_command(TARGET fastjit_numeric POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying fastjit_numeric WASM test files to build directory"
)

add_custom_command(TARGET fastjit_ctrl_flow POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying fastjit_ctrl_flow WASM test files to build directory"
)

add_custom_command(TARGET fastjit_variable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying fastjit_variable WASM test files to build directory"
)

add_custom_command(TARGET fastjit_memory POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying fastjit_memory WASM test files to build directory"
)

add_custom_command(TARGET wasm_running_modes_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    ${CMAKE_CURRENT_BINARY_DIR}/wasm-apps
    COMMENT "Copying wasm_running_modes_test WASM test files to build directory"
)