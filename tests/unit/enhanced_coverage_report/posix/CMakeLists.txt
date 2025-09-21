# Copyright (C) 2024 Intel Corporation. All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.14)

project(test-posix-enhanced)

add_definitions(-DRUN_ON_LINUX)

set(WAMR_BUILD_LIBC_WASI 1)
set(WAMR_BUILD_APP_FRAMEWORK 0)

# Include common unit test configuration
include(${CMAKE_CURRENT_SOURCE_DIR}/../../unit_common.cmake)

# Set test executable name
set(POSIX_ENHANCED_TEST_NAME posix_coverage_improve_test)

# Include directories for POSIX platform headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Enhanced test source files (step-by-step implementation)
set(POSIX_ENHANCED_SOURCES
    posix_coverage_improve_step_1.cc  # Core file operations
    posix_coverage_improve_step_2.cc  # Directory operations  
    posix_coverage_improve_step_3.cc  # Socket core operations
    posix_coverage_improve_step_4.cc  # Socket configuration
)

# Filter source files that actually exist
set(EXISTING_SOURCES)
foreach(SOURCE ${POSIX_ENHANCED_SOURCES})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE})
        list(APPEND EXISTING_SOURCES ${SOURCE})
    endif()
endforeach()

# Only create test if we have source files
if(EXISTING_SOURCES)
    # Include WAMR runtime library sources
    set(unit_test_sources
        ${EXISTING_SOURCES}
        ${WAMR_RUNTIME_LIB_SOURCE}
        ${UNCOMMON_SHARED_SOURCE}
    )
    
    # Create enhanced test executable
    add_executable(${POSIX_ENHANCED_TEST_NAME} ${unit_test_sources})
    
    # Link with required libraries
    target_link_libraries(${POSIX_ENHANCED_TEST_NAME} gtest_main)
    
    # Platform-specific linking for POSIX features
    if(UNIX AND NOT APPLE)
        target_link_libraries(${POSIX_ENHANCED_TEST_NAME} pthread dl m)
    elseif(APPLE)
        target_link_libraries(${POSIX_ENHANCED_TEST_NAME} pthread dl m)
    endif()
    
    # Register with GTest discovery
    gtest_discover_tests(${POSIX_ENHANCED_TEST_NAME})
    
    # Set test properties for coverage collection
    set_target_properties(${POSIX_ENHANCED_TEST_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
    )
    
    # Add coverage compilation flags if enabled
    if(COLLECT_CODE_COVERAGE)
        target_compile_options(${POSIX_ENHANCED_TEST_NAME} PRIVATE --coverage)
        target_link_options(${POSIX_ENHANCED_TEST_NAME} PRIVATE --coverage)
    endif()
    
    list(LENGTH EXISTING_SOURCES source_count)
    message(STATUS "Enhanced POSIX coverage tests configured with ${source_count} source files")
else()
    message(STATUS "No enhanced POSIX test source files found - skipping test creation")
endif()

# Create test data directory if needed
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_data)
set(WAMR_BUILD_PLATFORM "linux")
set(WAMR_BUILD_TARGET "X86_64")
set(WAMR_BUILD_INTERP 1)
set(WAMR_BUILD_AOT 1)
set(WAMR_BUILD_JIT 0)
set(WAMR_BUILD_FAST_INTERP 1)
set(WAMR_BUILD_SHARED_MEMORY 1)

# Include unit test common configuration

# Include IWASM compilation configuration
include(${IWASM_DIR}/compilation/iwasm_compl.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../common)

# Collect all test source files
file(GLOB_RECURSE source_all ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

set(UNIT_SOURCE ${source_all})

aux_source_directory(. SRC_LIST)

set(unit_test_sources
    ${UNIT_SOURCE}
    ${WAMR_RUNTIME_LIB_SOURCE}
    ${UNCOMMON_SHARED_SOURCE}
    ${SRC_LIST}
)

# Create POSIX enhanced test executable
add_executable(posix_enhanced_test ${unit_test_sources})

target_link_libraries(posix_enhanced_test ${LLVM_AVAILABLE_LIBS} gtest_main -lm -ldl -lpthread)

gtest_discover_tests(posix_enhanced_test)
