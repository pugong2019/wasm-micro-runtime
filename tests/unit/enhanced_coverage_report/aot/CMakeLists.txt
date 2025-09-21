# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.14)

project (test-aot-enhanced-coverage)

add_definitions (-DRUN_ON_LINUX)

add_definitions (-Dattr_container_malloc=malloc)
add_definitions (-Dattr_container_free=free)
add_definitions (-DWASM_ENABLE_WAMR_COMPILER=1)
add_definitions (-DWASM_ENABLE_DUMP_CALL_STACK=1)
add_definitions (-DWASM_ENABLE_AOT_STACK_FRAME=1)

set (WAMR_BUILD_LIBC_WASI 0)
set (WAMR_BUILD_APP_FRAMEWORK 1)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set (BUILD_GMOCK OFF CACHE BOOL "" FORCE)

# Fetch Google test
include (FetchContent)
FetchContent_Declare (
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable (googletest)

include (../../unit_common.cmake)

set (LLVM_SRC_ROOT "${WAMR_ROOT_DIR}/core/deps/llvm")
if (NOT EXISTS "${LLVM_SRC_ROOT}/build")
  message (FATAL_ERROR "Cannot find LLVM dir: ${LLVM_SRC_ROOT}/build")
endif ()
set (CMAKE_PREFIX_PATH "${LLVM_SRC_ROOT}/build;${CMAKE_PREFIX_PATH}")
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include (${IWASM_DIR}/compilation/iwasm_compl.cmake)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

file (GLOB source_all ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

set (UNIT_SOURCE ${source_all})

set (unit_test_sources
     ${UNIT_SOURCE}
     ${PLATFORM_SHARED_SOURCE}
     ${UTILS_SHARED_SOURCE}
     ${MEM_ALLOC_SHARED_SOURCE}
     ${NATIVE_INTERFACE_SOURCE}
     ${LIBC_BUILTIN_SOURCE}
     ${IWASM_COMMON_SOURCE}
     ${IWASM_INTERP_SOURCE}
     ${IWASM_AOT_SOURCE}
     ${IWASM_COMPL_SOURCE}
    )

# Filter out gmock files to prevent compilation issues
list(FILTER unit_test_sources EXCLUDE REGEX ".*gmock.*")
list(FILTER unit_test_sources EXCLUDE REGEX ".*googlemock.*")

# Now simply link against gtest or gtest_main as needed. Eg
add_executable (aot_enhanced_coverage_test ${unit_test_sources})

target_link_libraries (aot_enhanced_coverage_test ${LLVM_AVAILABLE_LIBS} gtest_main )

include(GoogleTest)
gtest_discover_tests(aot_enhanced_coverage_test)