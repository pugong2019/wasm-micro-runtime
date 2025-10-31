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

// ===== NEW TEST CASES FOR LINES 702-768: wasm_cluster_create_thread =====

// Simple thread routine for testing
static void *test_thread_routine(void *arg) {
    WASMExecEnv *exec_env = (WASMExecEnv *)arg;
    // Simple test logic - just return a success value
    return (void*)1;
}

/******
 * Test Case: wasm_cluster_create_thread_ValidParams_ReturnsSuccess
 * Source: core/iwasm/libraries/thread-mgr/thread_manager.c:702-768
 * Target Lines: 702-720 (function entry, parameter validation),
 *               721-725 (exec env creation), 734-740 (aux stack disable path),
 *               741-750 (suspend flags and cluster add), 751-768 (thread creation and wait)
 * Functional Purpose: Validates that wasm_cluster_create_thread() successfully creates
 *                     a new thread when provided with valid parameters and proper
 *                     cluster setup, exercising the main success path.
 * Call Path: wasm_cluster_create_thread() <- lib_pthread_wrapper.c:pthread_create()
 * Coverage Goal: Exercise main success path for thread creation (60+ lines)
 ******/
TEST_F(EnhancedThreadManagerTest, wasm_cluster_create_thread_ValidParams_ReturnsSuccess) {
    // Create a cluster by creating an execution environment
    char error_buf[128];
    memset(error_buf, 0, sizeof(error_buf));

    // Create a simple WASM module and instantiate it
    wasm_module_t module = wasm_runtime_load(dummy_wasm_buffer, sizeof(dummy_wasm_buffer),
                                           error_buf, sizeof(error_buf));
    ASSERT_NE(module, nullptr);

    wasm_module_inst_t module_inst = wasm_runtime_instantiate(module, 32768, 32768,
                                                            error_buf, sizeof(error_buf));
    ASSERT_NE(module_inst, nullptr);

    // Create an execution environment which will create a cluster
    WASMExecEnv *main_exec_env = wasm_runtime_create_exec_env(module_inst, 32768);
    ASSERT_NE(main_exec_env, nullptr);

    // Verify cluster was created
    WASMCluster *cluster = wasm_exec_env_get_cluster(main_exec_env);
    ASSERT_NE(cluster, nullptr);

    // Test wasm_cluster_create_thread with valid parameters (no aux stack)
    int32 result = wasm_cluster_create_thread(main_exec_env, module_inst,
                                            false, 0, 0, // no aux stack
                                            test_thread_routine, (void*)0x12345);

    // Should return 0 for success
    ASSERT_EQ(result, 0);

    // Give some time for thread creation to complete
    usleep(10000); // 10ms

    // Cleanup
    wasm_runtime_destroy_exec_env(main_exec_env);
    wasm_runtime_deinstantiate(module_inst);
    wasm_runtime_unload(module);
}
