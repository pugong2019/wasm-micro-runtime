# Enhanced Interpreter Coverage Tests CMakeLists.txt
cmake_minimum_required(VERSION 3.12)

add_definitions(-DRUN_ON_LINUX)
add_definitions(-Dattr_container_malloc=malloc)
add_definitions(-Dattr_container_free=free)

set(WAMR_BUILD_LIBC_WASI 0)
set(WAMR_BUILD_APP_FRAMEWORK 1)
set(WAMR_BUILD_AOT 0)

# Include common unit test configuration
include(${CMAKE_CURRENT_SOURCE_DIR}/../../unit_common.cmake)

# Set up interpreter coverage test executable
set(INTERPRETER_COVERAGE_TEST_SOURCES
    interpreter_coverage_improve_step_1.cc
    interpreter_coverage_improve_step_2.cc
    interpreter_coverage_improve_step_3.cc
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(unit_test_sources
    ${INTERPRETER_COVERAGE_TEST_SOURCES}
    ${PLATFORM_SHARED_SOURCE}
    ${UTILS_SHARED_SOURCE}
    ${MEM_ALLOC_SHARED_SOURCE}
    ${NATIVE_INTERFACE_SOURCE}
    ${LIBC_BUILTIN_SOURCE}
    ${IWASM_COMMON_SOURCE}
    ${IWASM_INTERP_SOURCE}
)

add_executable(interpreter_enhanced_coverage_test ${unit_test_sources})

target_include_directories(interpreter_enhanced_coverage_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common
    ${WAMR_ROOT_DIR}/core/iwasm/interpreter
    ${WAMR_ROOT_DIR}/core/iwasm/include
    ${WAMR_ROOT_DIR}/core/iwasm/common
    ${WAMR_ROOT_DIR}/core/shared/utils
    ${WAMR_ROOT_DIR}/core/shared/platform/include
)

target_link_libraries(interpreter_enhanced_coverage_test
    gtest_main
    ${LLVM_AVAILABLE_LIBS}
)

target_compile_definitions(interpreter_enhanced_coverage_test PRIVATE
    WASM_ENABLE_INTERP=1
    WASM_ENABLE_FAST_INTERP=0
    WASM_ENABLE_LIBC_BUILTIN=1
    WASM_ENABLE_MULTI_MODULE=1
    WASM_ENABLE_TAIL_CALL=1
    WASM_ENABLE_BULK_MEMORY=1
    WASM_ENABLE_REF_TYPES=1
)

# Copy WASM files to build directory
add_custom_command(TARGET interpreter_enhanced_coverage_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps/*.wasm
    ${CMAKE_CURRENT_BINARY_DIR}/
    COMMENT "Copy test wasm files to the directory of google test"
)

# Add test to CTest
include(GoogleTest)
gtest_discover_tests(interpreter_enhanced_coverage_test)