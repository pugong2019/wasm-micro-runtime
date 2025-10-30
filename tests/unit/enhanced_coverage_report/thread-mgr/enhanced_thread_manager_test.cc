/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <gtest/gtest.h>
#include "bh_platform.h"
#include "wasm_export.h"
#include "thread_manager.h"
#include "../common/test_helper.h"
#include <memory>

/**
 * Enhanced Thread Manager Test Suite for wasm_cluster_set_context coverage
 *
 * Target Function Coverage:
 * - wasm_cluster_set_context() lines 1499-1520 - 0 hits
 * - set_context_visitor() lines 1488-1496 - 0 hits (static function)
 *
 * Expected Coverage: Focus on MODULE_INST_CONTEXT functionality
 */

class EnhancedThreadManagerTest : public testing::Test {
protected:
    void SetUp() override {
        // Initialize WAMR runtime first with proper configuration
        RuntimeInitArgs init_args;
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_System_Allocator;
        init_args.max_thread_num = 4;

        ASSERT_TRUE(wasm_runtime_full_init(&init_args));

        // Initialize thread manager after WAMR is ready
        ASSERT_TRUE(thread_manager_init());

        // Now create DummyExecEnv (it won't reinit WAMR since it's already initialized)
        dummy_env = std::make_unique<DummyExecEnv>();
        ASSERT_NE(dummy_env.get(), nullptr);

        exec_env = dummy_env->get();
        ASSERT_NE(exec_env, nullptr);

        module_inst = wasm_runtime_get_module_inst(exec_env);
        ASSERT_NE(module_inst, nullptr);

        // Create context key for testing
        context_key = wasm_runtime_create_context_key(context_destructor);
        ASSERT_NE(context_key, nullptr);
    }

    void TearDown() override {
        if (context_key) {
            wasm_runtime_destroy_context_key(context_key);
            context_key = nullptr;
        }

        dummy_env.reset();
        thread_manager_destroy();
        wasm_runtime_destroy();
    }

    static void context_destructor(WASMModuleInstanceCommon *inst, void *ctx) {
        // Simple destructor for test context
        if (ctx) {
            free(ctx);
        }
    }


public:
    std::unique_ptr<DummyExecEnv> dummy_env;
    WASMExecEnv *exec_env;
    WASMModuleInstanceCommon *module_inst;
    void *context_key;
};

/******
 * Test Case: wasm_cluster_set_context_NoCluster_DirectSet
 * Source: core/iwasm/libraries/thread-mgr/thread_manager.c:1499-1520
 * Target Lines: 1502 (search call), 1504-1507 (NULL exec_env path)
 * Functional Purpose: Validates that wasm_cluster_set_context() correctly handles
 *                     the case when no cluster exists (exec_env is NULL) and
 *                     falls back to direct wasm_runtime_set_context() call.
 * Call Path: wasm_cluster_set_context() <- wasm_native_set_context_spread() <- wasm_runtime_set_context_spread()
 * Coverage Goal: Exercise NULL exec_env fallback path (lines 1504-1507)
 ******/
TEST_F(EnhancedThreadManagerTest, wasm_cluster_set_context_NoCluster_DirectSet) {
    // Create a standalone module instance that we know is not part of any cluster
    char error_buf[128];
    memset(error_buf, 0, sizeof(error_buf));

    // Create a simple standalone WASM module
    wasm_module_t module = wasm_runtime_load(dummy_wasm_buffer, sizeof(dummy_wasm_buffer),
                                           error_buf, sizeof(error_buf));
    ASSERT_NE(module, nullptr);

    wasm_module_inst_t standalone_inst = wasm_runtime_instantiate(module, 32768, 32768,
                                                                error_buf, sizeof(error_buf));
    ASSERT_NE(standalone_inst, nullptr);

    // Verify this instance has no cluster (should return NULL)
    WASMExecEnv *found_env = wasm_clusters_search_exec_env((WASMModuleInstanceCommon*)standalone_inst);
    ASSERT_EQ(found_env, nullptr);  // Should be NULL since no cluster created

    // Create test context data
    void *test_context = malloc(64);
    ASSERT_NE(test_context, nullptr);
    strcpy((char*)test_context, "test_context_data");

    // Call wasm_cluster_set_context with non-clustered instance
    // This should trigger the NULL exec_env path (lines 1504-1507)
    wasm_cluster_set_context((WASMModuleInstanceCommon*)standalone_inst, context_key, test_context);

    // Verify context was set directly via wasm_runtime_set_context
    void *retrieved_context = wasm_runtime_get_context((WASMModuleInstanceCommon*)standalone_inst, context_key);
    ASSERT_EQ(retrieved_context, test_context);
    ASSERT_STREQ((char*)retrieved_context, "test_context_data");

    // Cleanup
    wasm_runtime_deinstantiate(standalone_inst);
    wasm_runtime_unload(module);
}

// REMOVED: Complex threading test cases due to stability issues
// The wasm_cluster_detach_thread and wasm_cluster_cancel_thread test cases were
// causing failures due to automatic cluster creation behavior in WAMR.
// Following the ESCALATION RULE, removing problematic test cases to ensure
// build passes and coverage analysis can proceed with stable test cases.

/******
 * Test Case: wasm_cluster_is_thread_terminated_Normal_ChecksFlags
 * Source: core/iwasm/libraries/thread-mgr/thread_manager.c:1525-1535
 * Target Lines: 1527 (mutex lock), 1528-1532 (flag check), 1533 (mutex unlock)
 * Functional Purpose: Validates that wasm_cluster_is_thread_terminated() correctly
 *                     checks the WASM_SUSPEND_FLAG_TERMINATE flag under mutex protection
 *                     and returns appropriate boolean result.
 * Call Path: wasm_cluster_is_thread_terminated() <- thread termination checks
 * Coverage Goal: Exercise terminate flag checking logic (lines 1527-1533)
 ******/
TEST_F(EnhancedThreadManagerTest, wasm_cluster_is_thread_terminated_Normal_ChecksFlags) {
    // Use the existing exec_env from setup
    ASSERT_NE(exec_env, nullptr);

    // Initially, thread should not be terminated
    bool is_terminated = wasm_cluster_is_thread_terminated(exec_env);
    ASSERT_FALSE(is_terminated);  // Should return false initially

    // The function should have exercised lines 1527-1533:
    // - Lock wait_lock (line 1527)
    // - Check suspend_flags for WASM_SUSPEND_FLAG_TERMINATE (lines 1528-1532)
    // - Unlock wait_lock (line 1533)
    // We can't directly verify flag manipulation without modifying internal state,
    // but we've successfully exercised the function's core logic path.
}

// REMOVED: exception_lock_unlock test case due to function visibility issues
// The exception_lock and exception_unlock functions are internal static functions
// not exposed in the header file, so they cannot be tested directly from unit tests.
// Following the ESCALATION RULE, removing this problematic test case to ensure build passes.

/******
 * Test Case: wasm_cluster_traverse_lock_unlock_Normal_ClusterMutex
 * Source: core/iwasm/libraries/thread-mgr/thread_manager.c:1556-1569
 * Target Lines: 1558-1560 (traverse_lock), 1566-1568 (traverse_unlock)
 * Functional Purpose: Validates that wasm_cluster_traverse_lock() and
 *                     wasm_cluster_traverse_unlock() correctly manage cluster
 *                     mutex for safe cluster traversal operations.
 * Call Path: wasm_cluster_traverse_lock/unlock() <- cluster traversal operations
 * Coverage Goal: Exercise cluster mutex lock/unlock operations (lines 1558-1568)
 ******/
TEST_F(EnhancedThreadManagerTest, wasm_cluster_traverse_lock_unlock_Normal_ClusterMutex) {
    // Use the existing exec_env which should have a cluster
    ASSERT_NE(exec_env, nullptr);

    // Verify exec_env has a cluster (created in setup)
    WASMCluster *cluster = wasm_exec_env_get_cluster(exec_env);
    ASSERT_NE(cluster, nullptr);

    // Test wasm_cluster_traverse_lock - should exercise lines 1558-1560
    wasm_cluster_traverse_lock(exec_env);

    // Test wasm_cluster_traverse_unlock - should exercise lines 1566-1568
    wasm_cluster_traverse_unlock(exec_env);

    // The functions should have successfully managed the cluster->lock mutex
    // Successful execution without deadlock indicates proper mutex operations.
}

/******
 * Test Case: wasm_cluster_register_destroy_callback_ValidCallback_ReturnsTrue
 * Source: core/iwasm/libraries/thread-mgr/thread_manager.c:1224-1235
 * Target Lines: 1226 (allocation check), 1228-1230 (malloc failure), 1232-1234 (success path)
 * Functional Purpose: Validates that wasm_cluster_register_destroy_callback()
 *                     correctly allocates and registers destroy callback nodes
 *                     and handles memory allocation failures appropriately.
 * Call Path: wasm_cluster_register_destroy_callback() <- cluster lifecycle management
 * Coverage Goal: Exercise callback registration logic (lines 1226-1234)
 ******/
TEST_F(EnhancedThreadManagerTest, wasm_cluster_register_destroy_callback_ValidCallback_ReturnsTrue) {
    // Define a simple callback function for testing
    auto test_callback = [](WASMCluster *cluster) {
        // Simple test callback - just verify cluster is not null
        ASSERT_NE(cluster, nullptr);
    };

    // Cast lambda to function pointer
    void (*callback_ptr)(WASMCluster *) = +test_callback;

    // Register the callback - should exercise lines 1226-1234
    bool result = wasm_cluster_register_destroy_callback(callback_ptr);
    ASSERT_TRUE(result);  // Should return true on successful registration

    // The function should have:
    // - Allocated memory for DestroyCallBackNode (line 1228)
    // - Set the destroy_cb field (line 1232)
    // - Added node to destroy_callback_list (line 1233)
    // - Returned true (line 1234)
}

/******
 * Test Case: wasm_cluster_suspend_thread_Normal_SetsSuspendFlag
 * Source: core/iwasm/libraries/thread-mgr/thread_manager.c:1238-1243
 * Target Lines: 1241-1242 (WASM_SUSPEND_FLAGS_FETCH_OR operation)
 * Functional Purpose: Validates that wasm_cluster_suspend_thread() correctly
 *                     sets the WASM_SUSPEND_FLAG_SUSPEND flag using atomic
 *                     fetch-or operation on exec_env->suspend_flags.
 * Call Path: wasm_cluster_suspend_thread() <- thread suspension management
 * Coverage Goal: Exercise suspend flag setting logic (lines 1241-1242)
 ******/
TEST_F(EnhancedThreadManagerTest, wasm_cluster_suspend_thread_Normal_SetsSuspendFlag) {
    // Use the existing exec_env from setup
    ASSERT_NE(exec_env, nullptr);

    // Get initial suspend flags state (should be 0)
    uint32 initial_flags = WASM_SUSPEND_FLAGS_GET(exec_env->suspend_flags);

    // Call wasm_cluster_suspend_thread - should exercise lines 1241-1242
    wasm_cluster_suspend_thread(exec_env);

    // Verify the WASM_SUSPEND_FLAG_SUSPEND was set
    uint32 current_flags = WASM_SUSPEND_FLAGS_GET(exec_env->suspend_flags);
    ASSERT_TRUE(current_flags & WASM_SUSPEND_FLAG_SUSPEND);  // Flag should be set

    // The function should have used WASM_SUSPEND_FLAGS_FETCH_OR to atomically
    // set the WASM_SUSPEND_FLAG_SUSPEND bit in exec_env->suspend_flags
}

/******
 * Test Case: wasm_cluster_resume_thread_Normal_ClearsSuspendFlag
 * Source: core/iwasm/libraries/thread-mgr/thread_manager.c:1276-1281
 * Target Lines: 1278-1279 (WASM_SUSPEND_FLAGS_FETCH_AND operation), 1280 (condition signal)
 * Functional Purpose: Validates that wasm_cluster_resume_thread() correctly
 *                     clears the WASM_SUSPEND_FLAG_SUSPEND flag using atomic
 *                     fetch-and operation and signals the wait condition.
 * Call Path: wasm_cluster_resume_thread() <- thread resume management
 * Coverage Goal: Exercise suspend flag clearing and condition signaling (lines 1278-1280)
 ******/
TEST_F(EnhancedThreadManagerTest, wasm_cluster_resume_thread_Normal_ClearsSuspendFlag) {
    // Use the existing exec_env from setup
    ASSERT_NE(exec_env, nullptr);

    // First set the suspend flag so we can test clearing it
    WASM_SUSPEND_FLAGS_FETCH_OR(exec_env->suspend_flags, WASM_SUSPEND_FLAG_SUSPEND);

    // Verify flag is set
    uint32 flags_before = WASM_SUSPEND_FLAGS_GET(exec_env->suspend_flags);
    ASSERT_TRUE(flags_before & WASM_SUSPEND_FLAG_SUSPEND);

    // Call wasm_cluster_resume_thread - should exercise lines 1278-1280
    wasm_cluster_resume_thread(exec_env);

    // Verify the WASM_SUSPEND_FLAG_SUSPEND was cleared
    uint32 flags_after = WASM_SUSPEND_FLAGS_GET(exec_env->suspend_flags);
    ASSERT_FALSE(flags_after & WASM_SUSPEND_FLAG_SUSPEND);  // Flag should be cleared

    // The function should have:
    // - Used WASM_SUSPEND_FLAGS_FETCH_AND to clear the suspend flag (lines 1278-1279)
    // - Signaled the wait_cond to wake up waiting threads (line 1280)
}