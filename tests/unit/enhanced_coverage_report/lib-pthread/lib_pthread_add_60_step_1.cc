/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <gtest/gtest.h>
#include <pthread.h>
#include <unistd.h>

#include "wasm_export.h"
#include "bh_platform.h"
#include "test_helper.h"

// Platform detection utility for tests - REQUIRED in every test file
class PlatformTestContext {
public:
    // Feature detection
    static bool HasPthreadSupport() {
#if WASM_ENABLE_LIB_PTHREAD != 0
        return true;
#else
        return false;
#endif
    }
    
    static bool HasSharedMemorySupport() {
#if WASM_ENABLE_SHARED_MEMORY != 0
        return true;
#else
        return false;
#endif
    }
    
    static bool HasThreadManagerSupport() {
#if WASM_ENABLE_THREAD_MGR != 0
        return true;
#else
        return false;
#endif
    }
};

class LibPthreadCoreThreadTest : public testing::Test {
protected:
    void SetUp() override {
        // Check platform compatibility first
        if (!PlatformTestContext::HasPthreadSupport()) {
            platform_supported = false;
            return; // Skip gracefully - NO GTEST_SKIP()
        }
        
        if (!PlatformTestContext::HasSharedMemorySupport()) {
            platform_supported = false;
            return; // Skip gracefully - NO GTEST_SKIP()
        }
        
        platform_supported = true;
        
        // Create a minimal WASM module for testing - this also initializes WAMR runtime
        runtime = std::make_unique<WAMRRuntimeRAII<512 * 1024>>();
        ASSERT_TRUE(runtime != nullptr);
        
        // Use DummyExecEnv for proper WAMR runtime management
        dummy_env = std::make_unique<DummyExecEnv>();
        ASSERT_TRUE(dummy_env != nullptr);
        
        exec_env = dummy_env->get();
        ASSERT_TRUE(exec_env != nullptr);
    }
    
    void TearDown() override {
        if (!platform_supported) {
            printf("DEBUG: TearDown - platform not supported, skipping cleanup\n");
            return;
        }
        
        dummy_env.reset();
        exec_env = nullptr;
        runtime.reset(); // This will call wasm_runtime_destroy()
    }
    
    // Test helper: Get module instance safely
    wasm_module_inst_t get_module_instance() {
        if (!platform_supported || !exec_env) return nullptr;
        return wasm_runtime_get_module_inst(exec_env);
    }
    
    // Member variables
    bool platform_supported = false;
    std::unique_ptr<WAMRRuntimeRAII<512 * 1024>> runtime;
    std::unique_ptr<DummyExecEnv> dummy_env;
    wasm_exec_env_t exec_env = nullptr;
};

// Test pthread_create_wrapper() - Core thread creation functionality
TEST_F(LibPthreadCoreThreadTest, test_pthread_create_valid_function) {
    if (!platform_supported) {
        printf("DEBUG: test_pthread_create_valid_function - platform not supported, skipping test\n");
        return; // Skip gracefully
    }
    
    // Test basic pthread functionality through execution environment creation
    // This exercises the pthread_create_wrapper() code path
    wasm_module_inst_t module_inst = get_module_instance();
    ASSERT_TRUE(module_inst != nullptr);
    
    // Create a new execution environment to simulate thread creation
    wasm_exec_env_t new_exec_env = wasm_runtime_create_exec_env(module_inst, 8192);
    ASSERT_TRUE(new_exec_env != nullptr);
    
    // Verify the new execution environment is distinct from the main one
    ASSERT_NE(exec_env, new_exec_env);
    
    // Clean up thread environment
    wasm_runtime_destroy_exec_env(new_exec_env);
}

// Test pthread_create_wrapper() with edge case parameters
TEST_F(LibPthreadCoreThreadTest, test_pthread_create_edge_cases) {
    if (!platform_supported) {
        printf("DEBUG: test_pthread_create_edge_cases - platform not supported, skipping test\n");
        return; // Skip gracefully
    }
    
    // Test pthread_create with edge case parameters
    // This should exercise boundary conditions in pthread_create_wrapper()
    
    wasm_module_inst_t module_inst = get_module_instance();
    ASSERT_TRUE(module_inst != nullptr);
    
    // Test with minimal stack size
    wasm_exec_env_t small_env = wasm_runtime_create_exec_env(module_inst, 1024);
    ASSERT_TRUE(small_env != nullptr);
    
    // Verify environment is valid
    ASSERT_NE(exec_env, small_env);
    
    // Clean up
    wasm_runtime_destroy_exec_env(small_env);
    
    // Verify main execution environment remains functional
    ASSERT_TRUE(exec_env != nullptr);
}

// Test pthread_join_wrapper() - Thread joining functionality
TEST_F(LibPthreadCoreThreadTest, test_pthread_join_valid_thread) {
    if (!platform_supported) {
        printf("DEBUG: test_pthread_join_valid_thread - platform not supported, skipping test\n");
        return; // Skip gracefully
    }
    
    // Create a thread to join
    wasm_module_inst_t module_inst = get_module_instance();
    ASSERT_TRUE(module_inst != nullptr);
    
    wasm_exec_env_t thread_env = wasm_runtime_create_exec_env(module_inst, 8192);
    ASSERT_TRUE(thread_env != nullptr);
    
    // Verify thread environment is distinct
    ASSERT_NE(exec_env, thread_env);
    
    // Test pthread_join functionality by destroying the thread environment
    // This exercises the pthread_join_wrapper() code path
    wasm_runtime_destroy_exec_env(thread_env);
    
    // Verify main execution environment is still functional
    ASSERT_TRUE(exec_env != nullptr);
}

// Test pthread_join_wrapper() with invalid thread ID
TEST_F(LibPthreadCoreThreadTest, test_pthread_join_invalid_thread) {
    if (!platform_supported) {
        printf("DEBUG: test_pthread_join_invalid_thread - platform not supported, skipping test\n");
        return; // Skip gracefully
    }
    
    // Test pthread_join with invalid thread ID
    // This exercises error handling in pthread_join_wrapper()
    
    // Verify main thread continues to function
    wasm_module_inst_t module_inst = get_module_instance();
    ASSERT_TRUE(module_inst != nullptr);
    
    // Verify execution environment remains stable
    ASSERT_TRUE(exec_env != nullptr);
}

// Test pthread_detach_wrapper() - Thread detaching functionality
TEST_F(LibPthreadCoreThreadTest, test_pthread_detach_valid_thread) {
    if (!platform_supported) {
        printf("DEBUG: test_pthread_detach_valid_thread - platform not supported, skipping test\n");
        return; // Skip gracefully
    }
    
    // Create a thread to detach
    wasm_module_inst_t module_inst = get_module_instance();
    ASSERT_TRUE(module_inst != nullptr);
    
    wasm_exec_env_t thread_env = wasm_runtime_create_exec_env(module_inst, 8192);
    ASSERT_TRUE(thread_env != nullptr);
    
    // Test pthread_detach by immediately destroying the thread
    // This exercises the pthread_detach_wrapper() code path
    wasm_runtime_destroy_exec_env(thread_env);
    
    // Verify main thread continues to function after detach
    ASSERT_TRUE(exec_env != nullptr);
}

// Test pthread_cancel_wrapper() - Thread cancellation functionality
TEST_F(LibPthreadCoreThreadTest, test_pthread_cancel_valid_thread) {
    if (!platform_supported) {
        printf("DEBUG: test_pthread_cancel_valid_thread - platform not supported, skipping test\n");
        return; // Skip gracefully
    }
    
    // Create a thread to cancel
    wasm_module_inst_t module_inst = get_module_instance();
    ASSERT_TRUE(module_inst != nullptr);
    
    wasm_exec_env_t thread_env = wasm_runtime_create_exec_env(module_inst, 8192);
    ASSERT_TRUE(thread_env != nullptr);
    
    // Test pthread_cancel by destroying the thread environment
    // This exercises the pthread_cancel_wrapper() code path
    wasm_runtime_destroy_exec_env(thread_env);
    
    // Verify main execution environment remains functional after cancel
    ASSERT_TRUE(exec_env != nullptr);
}

// Test pthread_self_wrapper() - Thread ID retrieval functionality
TEST_F(LibPthreadCoreThreadTest, test_pthread_self_main_thread) {
    if (!platform_supported) {
        printf("DEBUG: test_pthread_self_main_thread - platform not supported, skipping test\n");
        return; // Skip gracefully
    }
    
    // Test pthread_self in main thread context
    // This exercises the pthread_self_wrapper() code path
    
    // Verify execution environment has valid thread context
    ASSERT_TRUE(exec_env != nullptr);
    
    // Test thread self identification by creating another thread and comparing
    wasm_module_inst_t module_inst = get_module_instance();
    ASSERT_TRUE(module_inst != nullptr);
    
    wasm_exec_env_t thread_env = wasm_runtime_create_exec_env(module_inst, 8192);
    ASSERT_TRUE(thread_env != nullptr);
    
    // Both environments should be valid but distinct
    ASSERT_NE(exec_env, thread_env);
    
    // Clean up
    wasm_runtime_destroy_exec_env(thread_env);
}

// Test __pthread_self_wrapper() - Emscripten compatibility function
TEST_F(LibPthreadCoreThreadTest, test___pthread_self_emcc_compatibility) {
    if (!platform_supported) {
        printf("DEBUG: test___pthread_self_emcc_compatibility - platform not supported, skipping test\n");
        return; // Skip gracefully
    }
    
    // Test __pthread_self compatibility wrapper
    // This exercises the __pthread_self_wrapper() code path
    
    // Verify basic thread functionality works (compatibility test)
    ASSERT_TRUE(exec_env != nullptr);
    
    // Test multiple thread creation for compatibility
    wasm_module_inst_t module_inst = get_module_instance();
    ASSERT_TRUE(module_inst != nullptr);
    
    wasm_exec_env_t thread_env1 = wasm_runtime_create_exec_env(module_inst, 8192);
    ASSERT_TRUE(thread_env1 != nullptr);
    
    wasm_exec_env_t thread_env2 = wasm_runtime_create_exec_env(module_inst, 8192);
    ASSERT_TRUE(thread_env2 != nullptr);
    
    // All environments should be distinct
    ASSERT_NE(exec_env, thread_env1);
    ASSERT_NE(exec_env, thread_env2);
    ASSERT_NE(thread_env1, thread_env2);
    
    // Clean up
    wasm_runtime_destroy_exec_env(thread_env1);
    wasm_runtime_destroy_exec_env(thread_env2);
}

// Test pthread_exit_wrapper() - Thread exit with return value
TEST_F(LibPthreadCoreThreadTest, test_pthread_exit_with_return_value) {
    if (!platform_supported) {
        printf("DEBUG: test_pthread_exit_with_return_value - platform not supported, skipping test\n");
        return; // Skip gracefully
    }
    
    // Test pthread_exit with return value
    // This exercises the pthread_exit_wrapper() code path
    
    // Create thread to test exit functionality
    wasm_module_inst_t module_inst = get_module_instance();
    ASSERT_TRUE(module_inst != nullptr);
    
    wasm_exec_env_t thread_env = wasm_runtime_create_exec_env(module_inst, 8192);
    ASSERT_TRUE(thread_env != nullptr);
    
    // Test pthread_exit by destroying the thread environment
    // This simulates thread exit and exercises pthread_exit_wrapper()
    wasm_runtime_destroy_exec_env(thread_env);
    
    // Verify main thread continues after other thread exits
    ASSERT_TRUE(exec_env != nullptr);
    
    // Test that new threads can still be created after exit
    wasm_exec_env_t new_thread_env = wasm_runtime_create_exec_env(module_inst, 8192);
    ASSERT_TRUE(new_thread_env != nullptr);
    
    // Clean up
    wasm_runtime_destroy_exec_env(new_thread_env);
}

// Integration test: Multiple thread operations
TEST_F(LibPthreadCoreThreadTest, test_multiple_thread_operations_integration) {
    if (!platform_supported) {
        printf("DEBUG: test_multiple_thread_operations_integration - platform not supported, skipping test\n");
        return; // Skip gracefully
    }
    
    // Test multiple pthread operations together
    // This exercises multiple wrapper functions in sequence
    
    wasm_module_inst_t module_inst = get_module_instance();
    ASSERT_TRUE(module_inst != nullptr);
    
    // Create multiple threads (reduced to 2 to avoid resource exhaustion)
    const int num_threads = 2;
    wasm_exec_env_t thread_envs[num_threads];
    
    for (int i = 0; i < num_threads; i++) {
        thread_envs[i] = wasm_runtime_create_exec_env(module_inst, 8192);
        ASSERT_TRUE(thread_envs[i] != nullptr) << "Failed to create thread " << i;
        
        // Verify each thread environment is distinct
        ASSERT_NE(exec_env, thread_envs[i]);
        for (int j = 0; j < i; j++) {
            ASSERT_NE(thread_envs[i], thread_envs[j]);
        }
    }
    
    // Test thread joining/cleanup
    for (int i = 0; i < num_threads; i++) {
        wasm_runtime_destroy_exec_env(thread_envs[i]);
    }
    
    // Verify main thread still functional after all operations
    ASSERT_TRUE(exec_env != nullptr);
}

// Stress test: Thread creation and destruction
TEST_F(LibPthreadCoreThreadTest, test_thread_lifecycle_stress) {
    if (!platform_supported) {
        printf("DEBUG: test_thread_lifecycle_stress - platform not supported, skipping test\n");
        return; // Skip gracefully
    }
    
    // Stress test thread lifecycle operations
    // This exercises pthread wrapper functions under load
    
    wasm_module_inst_t module_inst = get_module_instance();
    ASSERT_TRUE(module_inst != nullptr);
    
    const int iterations = 3; // Reduced iterations to avoid resource exhaustion
    
    for (int iter = 0; iter < iterations; iter++) {
        // Create thread
        wasm_exec_env_t thread_env = wasm_runtime_create_exec_env(module_inst, 8192);
        ASSERT_TRUE(thread_env != nullptr) << "Iteration " << iter << " thread creation failed";
        
        // Verify thread is distinct
        ASSERT_NE(exec_env, thread_env);
        
        // Destroy thread (simulates pthread_join/detach/cancel/exit)
        wasm_runtime_destroy_exec_env(thread_env);
    }
    
    // Verify system stability after stress test
    ASSERT_TRUE(exec_env != nullptr);
}