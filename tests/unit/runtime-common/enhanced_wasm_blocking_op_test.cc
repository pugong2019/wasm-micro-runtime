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
#include "wasm_suspend_flags.h"

using namespace std;

// Enhanced test fixture for wasm_blocking_op.c functions - Lines 26-38
class EnhancedWasmBlockingOpTest : public testing::Test {
protected:
    void SetUp() override {
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_Pool;
        init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
        init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);

        wasm_runtime_full_init(&init_args);

        module_inst = nullptr;
        exec_env = nullptr;
        test_exec_env = nullptr;

        // Initialize test data
        error_buf[0] = '\0';
        simple_wasm_size = 0;
        simple_wasm = nullptr;

        CreateSimpleWasmModule();
        CreateTestExecEnv();
    }

    void TearDown() override {
        if (test_exec_env) {
            // Clean up test exec env
            CleanupTestExecEnv();
        }
        if (exec_env) {
            wasm_runtime_destroy_exec_env(exec_env);
            exec_env = nullptr;
        }
        if (module_inst) {
            wasm_runtime_deinstantiate(module_inst);
            module_inst = nullptr;
        }
        if (simple_wasm) {
            free(simple_wasm);
            simple_wasm = nullptr;
        }
        wasm_runtime_destroy();
    }

    // Create a simple WASM module for testing
    void CreateSimpleWasmModule() {
        // Minimal WASM module with a simple function
        uint8_t wasm_bytes[] = {
            0x00, 0x61, 0x73, 0x6d, // WASM magic
            0x01, 0x00, 0x00, 0x00, // version
            0x01, 0x07,             // type section
            0x01,                   // 1 type
            0x60, 0x02, 0x7f, 0x7f, 0x01, 0x7f, // func type: (i32,i32)->i32
            0x03, 0x02,             // function section
            0x01, 0x00,             // 1 function, type 0
            0x0a, 0x09,             // code section
            0x01, 0x07,             // 1 function body
            0x00,                   // 0 locals
            0x20, 0x00,             // local.get 0
            0x20, 0x01,             // local.get 1
            0x6a,                   // i32.add
            0x0b                    // end
        };

        simple_wasm_size = sizeof(wasm_bytes);
        simple_wasm = (uint8_t*)malloc(simple_wasm_size);
        memcpy(simple_wasm, wasm_bytes, simple_wasm_size);
    }

    // Create a test exec_env with proper initialization
    void CreateTestExecEnv() {
        // Load and instantiate module
        wasm_module_t module = wasm_runtime_load(simple_wasm, simple_wasm_size, error_buf, sizeof(error_buf));
        if (!module) return;

        module_inst = wasm_runtime_instantiate(module, 8192, 0, error_buf, sizeof(error_buf));
        if (!module_inst) {
            wasm_runtime_unload(module);
            return;
        }

        exec_env = wasm_runtime_create_exec_env(module_inst, 8192);
        if (!exec_env) {
            wasm_runtime_deinstantiate(module_inst);
            module_inst = nullptr;
            wasm_runtime_unload(module);
            return;
        }

        // Create manual test exec_env for flag manipulation
        test_exec_env = (WASMExecEnv*)malloc(sizeof(WASMExecEnv));
        memset(test_exec_env, 0, sizeof(WASMExecEnv));

        // Initialize wait_lock
        if (os_mutex_init(&test_exec_env->wait_lock) != 0) {
            free(test_exec_env);
            test_exec_env = nullptr;
            return;
        }

        // Initialize suspend flags to clean state
        BH_ATOMIC_32_STORE(test_exec_env->suspend_flags.flags, 0);

        wasm_runtime_unload(module);
    }

    void CleanupTestExecEnv() {
        if (test_exec_env) {
            os_mutex_destroy(&test_exec_env->wait_lock);
            free(test_exec_env);
            test_exec_env = nullptr;
        }
    }

public:
    char global_heap_buf[512 * 1024];
    RuntimeInitArgs init_args;
    WASMModuleInstanceCommon *module_inst;
    WASMExecEnv *exec_env;
    WASMExecEnv *test_exec_env;
    char error_buf[128];
    uint8_t *simple_wasm;
    uint32 simple_wasm_size;
};

/******
 * Test Case: wasm_runtime_begin_blocking_op_NormalSuccess_ReturnsTrue
 * Source: core/iwasm/common/wasm_blocking_op.c:26-38
 * Target Lines: 28 (LOCK), 29 (assert), 30 (SET BLOCKING), 36 (UNLOCK), 37 (os_begin_blocking_op), 38 (return true)
 * Functional Purpose: Tests normal successful execution path where no TERMINATE flag is set.
 *                     Verifies proper lock/unlock sequence and BLOCKING flag management.
 * Call Path: Direct API call to wasm_runtime_begin_blocking_op()
 * Coverage Goal: Exercise normal success path through all lock/unlock operations
 ******/
TEST_F(EnhancedWasmBlockingOpTest, wasm_runtime_begin_blocking_op_NormalSuccess_ReturnsTrue) {
    ASSERT_NE(nullptr, test_exec_env);

    // Start with clean suspend flags (no BLOCKING, no TERMINATE)
    BH_ATOMIC_32_STORE(test_exec_env->suspend_flags.flags, 0);

    // Call the function under test
    bool result = wasm_runtime_begin_blocking_op(test_exec_env);

    // Verify success
    ASSERT_TRUE(result);

    // Verify BLOCKING flag is now set
    uint32 flags = WASM_SUSPEND_FLAGS_GET(test_exec_env->suspend_flags);
    ASSERT_NE(0, flags & WASM_SUSPEND_FLAG_BLOCKING);

    // Clean up for next tests
    wasm_runtime_end_blocking_op(test_exec_env);
}

/******
 * Test Case: wasm_runtime_begin_blocking_op_TerminateSet_ReturnsFalse
 * Source: core/iwasm/common/wasm_blocking_op.c:26-38
 * Target Lines: 28 (LOCK), 29 (assert), 30 (SET BLOCKING), 31 (if TERMINATE), 32 (CLR BLOCKING), 33 (UNLOCK), 34 (return false)
 * Functional Purpose: Tests early termination path when TERMINATE flag is already set.
 *                     Verifies proper cleanup of BLOCKING flag and early return.
 * Call Path: Direct API call to wasm_runtime_begin_blocking_op()
 * Coverage Goal: Exercise error/termination path with proper flag cleanup
 ******/
TEST_F(EnhancedWasmBlockingOpTest, wasm_runtime_begin_blocking_op_TerminateSet_ReturnsFalse) {
    ASSERT_NE(nullptr, test_exec_env);

    // Pre-set TERMINATE flag to trigger early return
    BH_ATOMIC_32_STORE(test_exec_env->suspend_flags.flags, WASM_SUSPEND_FLAG_TERMINATE);

    // Call the function under test
    bool result = wasm_runtime_begin_blocking_op(test_exec_env);

    // Verify failure return
    ASSERT_FALSE(result);

    // Verify BLOCKING flag is NOT set (should be cleared in error path)
    uint32 flags = WASM_SUSPEND_FLAGS_GET(test_exec_env->suspend_flags);
    ASSERT_EQ(0, flags & WASM_SUSPEND_FLAG_BLOCKING);

    // Verify TERMINATE flag is still set
    ASSERT_NE(0, flags & WASM_SUSPEND_FLAG_TERMINATE);
}

/******
 * Test Case: wasm_runtime_begin_blocking_op_ValidExecEnv_ProperlySetsFlags
 * Source: core/iwasm/common/wasm_blocking_op.c:26-38
 * Target Lines: 28 (LOCK), 29 (assert check), 30 (SET BLOCKING), 36 (UNLOCK), 37 (os_begin_blocking_op), 38 (return true)
 * Functional Purpose: Tests that the function properly manages suspend flags with proper
 *                     exec_env from actual WASM module instantiation.
 * Call Path: Direct API call to wasm_runtime_begin_blocking_op()
 * Coverage Goal: Verify flag management works with real exec_env structures
 ******/
TEST_F(EnhancedWasmBlockingOpTest, wasm_runtime_begin_blocking_op_ValidExecEnv_ProperlySetsFlags) {
    ASSERT_NE(nullptr, exec_env);

    // Ensure clean starting state
    BH_ATOMIC_32_STORE(exec_env->suspend_flags.flags, 0);

    // Call the function under test
    bool result = wasm_runtime_begin_blocking_op(exec_env);

    // Verify success
    ASSERT_TRUE(result);

    // Verify BLOCKING flag is set
    uint32 flags = WASM_SUSPEND_FLAGS_GET(exec_env->suspend_flags);
    ASSERT_NE(0, flags & WASM_SUSPEND_FLAG_BLOCKING);

    // Clean up properly
    wasm_runtime_end_blocking_op(exec_env);

    // Verify BLOCKING flag is cleared after end
    flags = WASM_SUSPEND_FLAGS_GET(exec_env->suspend_flags);
    ASSERT_EQ(0, flags & WASM_SUSPEND_FLAG_BLOCKING);
}

/******
 * Test Case: wasm_runtime_begin_blocking_op_MultipleFlags_HandlesCorrectly
 * Source: core/iwasm/common/wasm_blocking_op.c:26-38
 * Target Lines: 28 (LOCK), 29 (assert), 30 (SET BLOCKING), 31 (if TERMINATE check), 36 (UNLOCK), 37 (os_begin_blocking_op), 38 (return true)
 * Functional Purpose: Tests behavior when other suspend flags are set but not TERMINATE.
 *                     Verifies function continues normally with non-blocking flags.
 * Call Path: Direct API call to wasm_runtime_begin_blocking_op()
 * Coverage Goal: Test flag combination scenarios and proper flag isolation
 ******/
TEST_F(EnhancedWasmBlockingOpTest, wasm_runtime_begin_blocking_op_MultipleFlags_HandlesCorrectly) {
    ASSERT_NE(nullptr, test_exec_env);

    // Set some other suspend flags but NOT TERMINATE
    uint32 initial_flags = WASM_SUSPEND_FLAG_SUSPEND | WASM_SUSPEND_FLAG_BREAKPOINT;
    BH_ATOMIC_32_STORE(test_exec_env->suspend_flags.flags, initial_flags);

    // Call the function under test
    bool result = wasm_runtime_begin_blocking_op(test_exec_env);

    // Verify success (should not be affected by non-TERMINATE flags)
    ASSERT_TRUE(result);

    // Verify BLOCKING flag is now also set along with original flags
    uint32 flags = WASM_SUSPEND_FLAGS_GET(test_exec_env->suspend_flags);
    ASSERT_NE(0, flags & WASM_SUSPEND_FLAG_BLOCKING);
    ASSERT_NE(0, flags & WASM_SUSPEND_FLAG_SUSPEND);
    ASSERT_NE(0, flags & WASM_SUSPEND_FLAG_BREAKPOINT);
    ASSERT_EQ(0, flags & WASM_SUSPEND_FLAG_TERMINATE);

    // Clean up
    wasm_runtime_end_blocking_op(test_exec_env);
}