# Enhanced Block BT Opcode Unit Tests
# Comprehensive testing for WASM 'block bt' opcode functionality

cmake_minimum_required(VERSION 3.10)

project(enhanced_block_bt_test)

# Platform-specific definitions
add_definitions(-DRUN_ON_LINUX)

# Include common unit test configuration
include(${CMAKE_CURRENT_SOURCE_DIR}/../../unit_common.cmake)

# Include directories for WAMR and test headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${WAMR_ROOT_DIR}/tests/unit/common)

# Enhanced Block BT Test Sources
set(UNIT_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/enhanced_block_bt_test.cc
)

# Combine all sources including WAMR runtime sources
set(unit_test_sources
    ${UNIT_SOURCE}
    ${WAMR_RUNTIME_LIB_SOURCE}
    ${UNCOMMON_SHARED_SOURCE}
)

add_executable(enhanced_block_bt_test ${unit_test_sources})

# Link against GTest only (WAMR sources are compiled directly)
target_link_libraries(enhanced_block_bt_test gtest_main)

# Copy WASM test files to build directory
add_custom_command(TARGET enhanced_block_bt_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    $<TARGET_FILE_DIR:enhanced_block_bt_test>/wasm-apps
    COMMENT "Copying WASM test files to build directory"
)

# GoogleTest discovery
include(GoogleTest)
gtest_discover_tests(enhanced_block_bt_test)