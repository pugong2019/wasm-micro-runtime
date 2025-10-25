/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "../common/test_helper.h"
#include "gtest/gtest.h"

#include "platform_common.h"
#include "wasm_runtime_common.h"
#include "bh_read_file.h"
#include "wasm_runtime.h"
#include "bh_platform.h"
#include "wasm_export.h"
#include "wasm_exec_env.h"

using namespace std;

// Enhanced test fixture for wasm_runtime_common.c functions
class EnhancedWasmRuntimeCommonTest : public testing::Test {
protected:
    void SetUp() override {
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_Pool;
        init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
        init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);

        wasm_runtime_full_init(&init_args);

        // Create a minimal module instance for exec env creation
        module_inst = nullptr;
        exec_env = nullptr;
    }

    void TearDown() override {
        if (exec_env) {
            wasm_runtime_destroy_exec_env(exec_env);
            exec_env = nullptr;
        }
        if (module_inst) {
            wasm_runtime_deinstantiate(module_inst);
            module_inst = nullptr;
        }
        wasm_runtime_destroy();
    }

    WASMExecEnv* CreateMockExecEnv(uint8 *boundary_value) {
        // Create a minimal bytecode module for testing
        uint8_t minimal_wasm[] = {
            0x00, 0x61, 0x73, 0x6D, // WASM magic
            0x01, 0x00, 0x00, 0x00, // version
        };

        wasm_module_t module = wasm_runtime_load(minimal_wasm, sizeof(minimal_wasm), nullptr, 0);
        if (!module) {
            return nullptr;
        }

        module_inst = wasm_runtime_instantiate(module, 8192, 8192, nullptr, 0);
        wasm_runtime_unload(module);

        if (!module_inst) {
            return nullptr;
        }

        exec_env = wasm_runtime_create_exec_env(module_inst, 8192);
        if (exec_env) {
            // Set the native stack boundary for testing
            exec_env->native_stack_boundary = boundary_value;
        }

        return exec_env;
    }

public:
    char global_heap_buf[512 * 1024];
    RuntimeInitArgs init_args;
    wasm_module_inst_t module_inst;
    WASMExecEnv *exec_env;
};

/******
 * Test Case: wasm_runtime_detect_native_stack_overflow_size_NullBoundary_ReturnsTrue
 * Source: core/iwasm/common/wasm_runtime_common.c:7892-7913
 * Target Lines: 7897-7900 (NULL boundary check and early return)
 * Functional Purpose: Verifies that when exec_env->native_stack_boundary is NULL,
 *                     the function returns true immediately without further processing
 * Call Path: Direct call to wasm_runtime_detect_native_stack_overflow_size()
 * Coverage Goal: Exercise early return path when platform doesn't support stack boundary detection
 ******/
TEST_F(EnhancedWasmRuntimeCommonTest, wasm_runtime_detect_native_stack_overflow_size_NullBoundary_ReturnsTrue) {
    // Create exec env with NULL boundary
    WASMExecEnv *test_exec_env = CreateMockExecEnv(nullptr);
    ASSERT_NE(test_exec_env, nullptr);

    // Test with various requested sizes - all should return true
    ASSERT_TRUE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, 0));
    ASSERT_TRUE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, 1024));
    ASSERT_TRUE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, 65536));
    ASSERT_TRUE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, UINT32_MAX));
}

/******
 * Test Case: wasm_runtime_detect_native_stack_overflow_size_ValidBoundarySmallSize_ReturnsTrue
 * Source: core/iwasm/common/wasm_runtime_common.c:7892-7913
 * Target Lines: 7895-7896, 7907, 7913 (valid boundary with small requested size)
 * Functional Purpose: Verifies successful execution path when boundary is valid and
 *                     requested size is small enough to not cause overflow
 * Call Path: Direct call to wasm_runtime_detect_native_stack_overflow_size()
 * Coverage Goal: Exercise success path with boundary adjustment and no overflow detection
 ******/
TEST_F(EnhancedWasmRuntimeCommonTest, wasm_runtime_detect_native_stack_overflow_size_ValidBoundarySmallSize_ReturnsTrue) {
    // Create exec env with valid boundary (use stack address)
    uint8 stack_buffer[8192];
    uint8 *boundary = &stack_buffer[4096];  // Set boundary in middle of buffer

    WASMExecEnv *test_exec_env = CreateMockExecEnv(boundary);
    ASSERT_NE(test_exec_env, nullptr);

    // Test with small requested size that won't cause overflow
    ASSERT_TRUE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, 64));
    ASSERT_TRUE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, 512));
    ASSERT_TRUE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, 1024));
}

/******
 * Test Case: wasm_runtime_detect_native_stack_overflow_size_ValidBoundaryLargeSize_DetectsOverflow
 * Source: core/iwasm/common/wasm_runtime_common.c:7892-7913
 * Target Lines: 7907-7912 (boundary adjustment, overflow detection, exception setting)
 * Functional Purpose: Verifies overflow detection when requested size causes stack boundary
 *                     comparison to fail, triggering exception and returning false
 * Call Path: Direct call to wasm_runtime_detect_native_stack_overflow_size()
 * Coverage Goal: Exercise overflow detection path and exception handling
 ******/
TEST_F(EnhancedWasmRuntimeCommonTest, wasm_runtime_detect_native_stack_overflow_size_ValidBoundaryLargeSize_DetectsOverflow) {
    // Create exec env with boundary at lower memory address
    uint8 *boundary = (uint8 *)0x1000;  // Low address boundary

    WASMExecEnv *test_exec_env = CreateMockExecEnv(boundary);
    ASSERT_NE(test_exec_env, nullptr);

    // Test with very large requested size that will cause overflow
    // The overflow condition is: (uint8 *)&boundary < boundary
    // After adjustment: boundary = boundary - WASM_STACK_GUARD_SIZE + requested_size
    uint32 large_size = UINT32_MAX - 1024;  // Very large size to trigger overflow

    ASSERT_FALSE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, large_size));

    // Verify that exception was set
    const char* exception = wasm_runtime_get_exception(test_exec_env->module_inst);
    ASSERT_NE(exception, nullptr);
    ASSERT_STREQ(exception, "native stack overflow");
}

/******
 * Test Case: wasm_runtime_detect_native_stack_overflow_size_BoundaryEdgeCase_HandlesBoundaryCalculation
 * Source: core/iwasm/common/wasm_runtime_common.c:7892-7913
 * Target Lines: 7907-7908 (boundary adjustment calculation edge case)
 * Functional Purpose: Tests boundary calculation with requested size close to WASM_STACK_GUARD_SIZE
 *                     to verify proper arithmetic and edge case handling
 * Call Path: Direct call to wasm_runtime_detect_native_stack_overflow_size()
 * Coverage Goal: Exercise boundary adjustment arithmetic with edge case values
 ******/
TEST_F(EnhancedWasmRuntimeCommonTest, wasm_runtime_detect_native_stack_overflow_size_BoundaryEdgeCase_HandlesBoundaryCalculation) {
    // Create exec env with reasonable boundary
    uint8 stack_buffer[32768];
    uint8 *boundary = &stack_buffer[16384];

    WASMExecEnv *test_exec_env = CreateMockExecEnv(boundary);
    ASSERT_NE(test_exec_env, nullptr);

    // Test with requested size equal to WASM_STACK_GUARD_SIZE
    // This tests the boundary calculation: boundary - WASM_STACK_GUARD_SIZE + requested_size
    ASSERT_TRUE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, WASM_STACK_GUARD_SIZE));

    // Test with requested size slightly larger than WASM_STACK_GUARD_SIZE
    ASSERT_TRUE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, WASM_STACK_GUARD_SIZE + 64));

    // Test with requested size smaller than WASM_STACK_GUARD_SIZE
    ASSERT_TRUE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, WASM_STACK_GUARD_SIZE / 2));
}

/******
 * Test Case: wasm_runtime_detect_native_stack_overflow_size_RecordStackUsage_ExecutesMacro
 * Source: core/iwasm/common/wasm_runtime_common.c:7892-7913
 * Target Lines: 7896 (RECORD_STACK_USAGE macro execution)
 * Functional Purpose: Verifies that RECORD_STACK_USAGE macro is called correctly and
 *                     executes without errors when memory profiling is enabled
 * Call Path: Direct call to wasm_runtime_detect_native_stack_overflow_size()
 * Coverage Goal: Exercise RECORD_STACK_USAGE macro path (conditional compilation)
 ******/
TEST_F(EnhancedWasmRuntimeCommonTest, wasm_runtime_detect_native_stack_overflow_size_RecordStackUsage_ExecutesMacro) {
    // Create exec env with valid boundary
    uint8 stack_buffer[16384];
    uint8 *boundary = &stack_buffer[8192];

    WASMExecEnv *test_exec_env = CreateMockExecEnv(boundary);
    ASSERT_NE(test_exec_env, nullptr);

    // Initialize native_stack_top_min if memory profiling is enabled
#if WASM_ENABLE_MEMORY_PROFILING != 0
    test_exec_env->native_stack_top_min = (uint8 *)UINTPTR_MAX;  // Set to max value initially
#endif

    // Call the function to trigger RECORD_STACK_USAGE
    ASSERT_TRUE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, 1024));

#if WASM_ENABLE_MEMORY_PROFILING != 0
    // Verify that native_stack_top_min was updated (should be less than max now)
    ASSERT_LT((uintptr_t)test_exec_env->native_stack_top_min, UINTPTR_MAX);
#endif
}

#if defined(OS_ENABLE_HW_BOUND_CHECK) && WASM_DISABLE_STACK_HW_BOUND_CHECK == 0
/******
 * Test Case: wasm_runtime_detect_native_stack_overflow_size_HwBoundCheckEnabled_AdjustsBoundary
 * Source: core/iwasm/common/wasm_runtime_common.c:7892-7913
 * Target Lines: 7901-7905 (hardware bound check conditional compilation block)
 * Functional Purpose: Verifies that when hardware bound checking is enabled, the boundary
 *                     is properly adjusted by adding page_size * guard_page_count
 * Call Path: Direct call to wasm_runtime_detect_native_stack_overflow_size()
 * Coverage Goal: Exercise hardware bound check adjustment path
 ******/
TEST_F(EnhancedWasmRuntimeCommonTest, wasm_runtime_detect_native_stack_overflow_size_HwBoundCheckEnabled_AdjustsBoundary) {
    // Create exec env with valid boundary
    uint8 stack_buffer[65536];  // Large buffer to accommodate HW bound check adjustment
    uint8 *boundary = &stack_buffer[32768];

    WASMExecEnv *test_exec_env = CreateMockExecEnv(boundary);
    ASSERT_NE(test_exec_env, nullptr);

    // Call with moderate size - should succeed even with HW bound check adjustment
    ASSERT_TRUE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, 2048));

    // Call with larger size to test boundary adjustment calculation
    ASSERT_TRUE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, 8192));
}
#endif

/******
 * Test Case: wasm_runtime_detect_native_stack_overflow_size_ZeroRequestedSize_ReturnsTrue
 * Source: core/iwasm/common/wasm_runtime_common.c:7892-7913
 * Target Lines: 7907, 7913 (boundary calculation with zero size, success return)
 * Functional Purpose: Verifies proper handling when requested_size is zero, ensuring
 *                     boundary calculation works correctly and returns success
 * Call Path: Direct call to wasm_runtime_detect_native_stack_overflow_size()
 * Coverage Goal: Exercise boundary calculation with edge case of zero requested size
 ******/
TEST_F(EnhancedWasmRuntimeCommonTest, wasm_runtime_detect_native_stack_overflow_size_ZeroRequestedSize_ReturnsTrue) {
    // Create exec env with valid boundary
    uint8 stack_buffer[16384];
    uint8 *boundary = &stack_buffer[8192];

    WASMExecEnv *test_exec_env = CreateMockExecEnv(boundary);
    ASSERT_NE(test_exec_env, nullptr);

    // Test with zero requested size
    ASSERT_TRUE(wasm_runtime_detect_native_stack_overflow_size(test_exec_env, 0));
}