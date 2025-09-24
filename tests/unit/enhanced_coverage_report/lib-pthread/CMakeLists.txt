# Enhanced lib-pthread coverage tests
cmake_minimum_required(VERSION 3.14)
project(lib_pthread_coverage_test)

# Set pthread and thread manager build flags
set(WAMR_BUILD_LIB_PTHREAD 1)
set(WAMR_BUILD_SHARED_MEMORY 1)
set(WAMR_BUILD_THREAD_MGR 1)

include(../../unit_common.cmake)

set(TEST_TARGET lib_pthread_coverage_test)

add_executable(${TEST_TARGET}
    lib_pthread_add_60_step_1.cc
    lib_pthread_add_60_step_2.cc
)

target_include_directories(${TEST_TARGET} PRIVATE
    ${IWASM_DIR}/include
    ${IWASM_DIR}/common
    ${IWASM_DIR}/interpreter
    ${IWASM_DIR}/libraries/lib-pthread
    ${IWASM_DIR}/libraries/thread-mgr
    ${SHARED_DIR}/utils
    ${SHARED_DIR}/platform/${WAMR_BUILD_PLATFORM}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_definitions(${TEST_TARGET} PRIVATE
    WASM_ENABLE_LIB_PTHREAD=1
    WASM_ENABLE_SHARED_MEMORY=1 
    WASM_ENABLE_THREAD_MGR=1
    WASM_ENABLE_INTERP=1
    WASM_ENABLE_AOT=0
    WASM_ENABLE_JIT=0
    WASM_ENABLE_FAST_JIT=0
    WASM_ENABLE_MEMORY64=0
    WASM_ENABLE_SIMD=0
)

target_sources(${TEST_TARGET} PRIVATE
    ${SHARED_DIR}/utils/uncommon/bh_read_file.c
)

target_link_libraries(${TEST_TARGET}
    vmlib
    gtest_main
    ${LLVM_AVAILABLE_LIBS}
    ${UV_A_LIBS}
    -lpthread
    -lm
    -ldl
)

include(GoogleTest)
gtest_discover_tests(${TEST_TARGET})

# Copy WASM files to build directory if they exist
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps)
    add_custom_command(TARGET ${TEST_TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps
        ${CMAKE_CURRENT_BINARY_DIR}/
        COMMENT "Copy test wasm files to the directory of google test"
    )
endif()