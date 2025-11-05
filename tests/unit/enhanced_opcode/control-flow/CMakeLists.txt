# Enhanced Control Flow Opcode Unit Tests
# Comprehensive testing for WASM control flow opcodes functionality

cmake_minimum_required(VERSION 3.10)

project(enhanced_control_flow_test)

# Platform-specific definitions
add_definitions(-DRUN_ON_LINUX)

# Include common unit test configuration
include(${CMAKE_CURRENT_SOURCE_DIR}/../../unit_common.cmake)

# Include directories for WAMR and test headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${WAMR_ROOT_DIR}/tests/unit/common)

# Enhanced Control Flow Test Sources
set(UNIT_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/enhanced_block_bt_test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/enhanced_call_test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/enhanced_call_indirect_test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/enhanced_return_call_test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/enhanced_return_call_indirect_test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/enhanced_return_call_ref_test.cc
)

# Combine all sources including WAMR runtime sources
set(unit_test_sources
    ${UNIT_SOURCE}
    ${WAMR_RUNTIME_LIB_SOURCE}
    ${UNCOMMON_SHARED_SOURCE}
)

add_executable(enhanced_block_bt_test ${CMAKE_CURRENT_SOURCE_DIR}/enhanced_block_bt_test.cc ${WAMR_RUNTIME_LIB_SOURCE} ${UNCOMMON_SHARED_SOURCE})
add_executable(enhanced_call_test ${CMAKE_CURRENT_SOURCE_DIR}/enhanced_call_test.cc ${WAMR_RUNTIME_LIB_SOURCE} ${UNCOMMON_SHARED_SOURCE})
add_executable(enhanced_call_indirect_test ${CMAKE_CURRENT_SOURCE_DIR}/enhanced_call_indirect_test.cc ${WAMR_RUNTIME_LIB_SOURCE} ${UNCOMMON_SHARED_SOURCE})
add_executable(enhanced_return_call_test ${CMAKE_CURRENT_SOURCE_DIR}/enhanced_return_call_test.cc ${WAMR_RUNTIME_LIB_SOURCE} ${UNCOMMON_SHARED_SOURCE})
add_executable(enhanced_return_call_indirect_test ${CMAKE_CURRENT_SOURCE_DIR}/enhanced_return_call_indirect_test.cc ${WAMR_RUNTIME_LIB_SOURCE} ${UNCOMMON_SHARED_SOURCE})
add_executable(enhanced_return_call_ref_test ${CMAKE_CURRENT_SOURCE_DIR}/enhanced_return_call_ref_test.cc ${WAMR_RUNTIME_LIB_SOURCE} ${UNCOMMON_SHARED_SOURCE})

# Link against GTest only (WAMR sources are compiled directly)
target_link_libraries(enhanced_block_bt_test gtest_main)
target_link_libraries(enhanced_call_test gtest_main)
target_link_libraries(enhanced_call_indirect_test gtest_main)
target_link_libraries(enhanced_return_call_test gtest_main)
target_link_libraries(enhanced_return_call_indirect_test gtest_main)
target_link_libraries(enhanced_return_call_ref_test gtest_main)

# Copy WASM test files to build directory for block bt test
add_custom_command(TARGET enhanced_block_bt_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    $<TARGET_FILE_DIR:enhanced_block_bt_test>/wasm-apps
    COMMENT "Copying WASM test files to build directory"
)

# Copy WASM test files to build directory for call test
add_custom_command(TARGET enhanced_call_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    $<TARGET_FILE_DIR:enhanced_call_test>/wasm-apps
    COMMENT "Copying WASM test files to build directory"
)

# Copy WASM test files to build directory for call_indirect test
add_custom_command(TARGET enhanced_call_indirect_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    $<TARGET_FILE_DIR:enhanced_call_indirect_test>/wasm-apps
    COMMENT "Copying WASM test files to build directory"
)

# Copy WASM test files to build directory for return_call test
add_custom_command(TARGET enhanced_return_call_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    $<TARGET_FILE_DIR:enhanced_return_call_test>/wasm-apps
    COMMENT "Copying WASM test files to build directory"
)

# Copy WASM test files to build directory for return_call_indirect test
add_custom_command(TARGET enhanced_return_call_indirect_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    $<TARGET_FILE_DIR:enhanced_return_call_indirect_test>/wasm-apps
    COMMENT "Copying WASM test files to build directory"
)

# Copy WASM test files to build directory for return_call_ref test
add_custom_command(TARGET enhanced_return_call_ref_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
    $<TARGET_FILE_DIR:enhanced_return_call_ref_test>/wasm-apps
    COMMENT "Copying WASM test files to build directory"
)

# GoogleTest discovery
include(GoogleTest)
gtest_discover_tests(enhanced_block_bt_test)
gtest_discover_tests(enhanced_call_test)
gtest_discover_tests(enhanced_call_indirect_test)
gtest_discover_tests(enhanced_return_call_test)
gtest_discover_tests(enhanced_return_call_indirect_test)
gtest_discover_tests(enhanced_return_call_ref_test)