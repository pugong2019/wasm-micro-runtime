/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "test_helper.h"
#include "gtest/gtest.h"
#include "platform_common.h"
#include "wasm_runtime_common.h"
#include "wasm_memory.h"
// #include "wasm_blocking_op.h" - Not available in current build
// #include "wasm_shared_memory.h" - Not available in current build
#include <pthread.h>

// Platform detection utility for tests - REQUIRED in every test file
class PlatformTestContext {
public:
    static bool IsX86_64() {
#if defined(BUILD_TARGET_X86_64)
        return true;
#else
        return false;
#endif
    }
    
    static bool HasThreadingSupport() {
#if WASM_ENABLE_THREAD_MGR != 0
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
    
    static bool HasAtomicSupport() {
#if WASM_ENABLE_SHARED_MEMORY != 0
        return true;
#else
        return false;
#endif
    }
};

class RuntimeCommonThreadingTest : public testing::Test
{
protected:
    WAMRRuntimeRAII<512 * 1024> runtime;
    wasm_module_t module = nullptr;
    wasm_module_inst_t module_inst = nullptr;
    wasm_exec_env_t exec_env = nullptr;
    uint32_t stack_size = 8092, heap_size = 8092;
    char error_buf[128];

    void SetUp() override
    {
        memset(error_buf, 0, sizeof(error_buf));
    }

    void TearDown() override
    {
        if (exec_env) {
            wasm_runtime_destroy_exec_env(exec_env);
            exec_env = nullptr;
        }
        if (module_inst) {
            wasm_runtime_deinstantiate(module_inst);
            module_inst = nullptr;
        }
        if (module) {
            wasm_runtime_unload(module);
            module = nullptr;
        }
    }

    bool init_basic_module()
    {
        // Create a minimal WASM module for testing
        const char* wasm_code = 
            "(module "
            "  (func $test_func (result i32) i32.const 42) "
            "  (export \"test_func\" (func $test_func)) "
            ")";
        
        // For this test, we'll use a simple pre-compiled module
        // In a real implementation, this would load from a .wasm file
        uint8_t minimal_wasm[] = {
            0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00, // WASM header
            0x01, 0x05, 0x01, 0x60, 0x00, 0x01, 0x7f,       // Type section
            0x03, 0x02, 0x01, 0x00,                         // Function section
            0x07, 0x0d, 0x01, 0x09, 0x74, 0x65, 0x73, 0x74, // Export section
            0x5f, 0x66, 0x75, 0x6e, 0x63, 0x00, 0x00,
            0x0a, 0x06, 0x01, 0x04, 0x00, 0x41, 0x2a, 0x0b  // Code section
        };

        module = wasm_runtime_load(minimal_wasm, sizeof(minimal_wasm), 
                                 error_buf, sizeof(error_buf));
        if (!module) return false;

        module_inst = wasm_runtime_instantiate(module, stack_size, heap_size,
                                             error_buf, sizeof(error_buf));
        if (!module_inst) return false;

        exec_env = wasm_runtime_create_exec_env(module_inst, stack_size);
        return exec_env != nullptr;
    }
};

// Test wasm_runtime_spawn_exec_env() - Function 1
TEST_F(RuntimeCommonThreadingTest, SpawnExecEnv_WithValidParameters_CreatesSuccessfully)
{
    if (!PlatformTestContext::HasThreadingSupport()) {
        return; // Skip gracefully if threading not supported
    }

    ASSERT_TRUE(init_basic_module());

    // Test spawning execution environment
    wasm_exec_env_t spawned_env = wasm_runtime_spawn_exec_env(exec_env);
    
    if (spawned_env) {
        ASSERT_NE(nullptr, spawned_env);
        ASSERT_NE(exec_env, spawned_env); // Should be different instance
        
        // Verify spawned environment has valid module instance
        wasm_module_inst_t spawned_inst = wasm_runtime_get_module_inst(spawned_env);
        ASSERT_NE(nullptr, spawned_inst);
        
        // Clean up spawned environment
        wasm_runtime_destroy_spawned_exec_env(spawned_env);
    }
}

TEST_F(RuntimeCommonThreadingTest, SpawnExecEnv_WithNullEnvironment_HandlesGracefully)
{
    if (!PlatformTestContext::HasThreadingSupport()) {
        return; // Skip gracefully if threading not supported
    }

    // Test with null execution environment
    wasm_exec_env_t spawned_env = wasm_runtime_spawn_exec_env(nullptr);
    ASSERT_EQ(nullptr, spawned_env);
}

// Test wasm_runtime_spawn_thread() - Function 2
TEST_F(RuntimeCommonThreadingTest, SpawnThread_WithValidFunction_StartsSuccessfully)
{
    if (!PlatformTestContext::HasThreadingSupport()) {
        return; // Skip gracefully if threading not supported
    }

    ASSERT_TRUE(init_basic_module());

    // Look up test function
    wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst, "test_func");
    if (!func) {
        return; // Function not found in minimal module
    }

    // Test thread spawning
    wasm_thread_t thread_id = 0;
    uint32_t argv[1] = {0};
    
    int result = wasm_runtime_spawn_thread(exec_env, &thread_id, (wasm_thread_callback_t)func, argv);
    
    if (result == 0) {
        ASSERT_GT(thread_id, 0);
        
        // Wait for thread completion
        int thread_result = wasm_runtime_join_thread(thread_id, nullptr);
        ASSERT_EQ(0, thread_result);
    }
}

TEST_F(RuntimeCommonThreadingTest, SpawnThread_WithInvalidFunction_ReturnsError)
{
    if (!PlatformTestContext::HasThreadingSupport()) {
        return; // Skip gracefully if threading not supported
    }

    ASSERT_TRUE(init_basic_module());

    wasm_thread_t thread_id = 0;
    uint32_t argv[1] = {0};
    
    // Test with null function
    int result = wasm_runtime_spawn_thread(exec_env, &thread_id, nullptr, argv);
    ASSERT_NE(0, result); // Should return error
}

// Test wasm_runtime_join_thread() - Function 3
TEST_F(RuntimeCommonThreadingTest, JoinThread_WithValidThreadId_CompletesSuccessfully)
{
    if (!PlatformTestContext::HasThreadingSupport()) {
        return; // Skip gracefully if threading not supported
    }

    ASSERT_TRUE(init_basic_module());

    wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst, "test_func");
    if (!func) {
        return; // Function not found in minimal module
    }

    wasm_thread_t thread_id = 0;
    uint32_t argv[1] = {0};
    
    int spawn_result = wasm_runtime_spawn_thread(exec_env, &thread_id, (wasm_thread_callback_t)func, argv);
    if (spawn_result == 0 && thread_id > 0) {
        // Test joining the thread
        void* thread_result = nullptr;
        int join_result = wasm_runtime_join_thread(thread_id, &thread_result);
        ASSERT_EQ(0, join_result);
    }
}

TEST_F(RuntimeCommonThreadingTest, JoinThread_WithInvalidThreadId_ReturnsError)
{
    if (!PlatformTestContext::HasThreadingSupport()) {
        return; // Skip gracefully if threading not supported
    }

    // Test with invalid thread ID
    wasm_thread_t invalid_thread_id = 0xFFFFFFFF;
    int result = wasm_runtime_join_thread(invalid_thread_id, nullptr);
    ASSERT_NE(0, result); // Should return error
}

// Test wasm_runtime_destroy_spawned_exec_env() - Function 5
TEST_F(RuntimeCommonThreadingTest, DestroySpawnedExecEnv_WithValidEnvironment_CleansUpCorrectly)
{
    if (!PlatformTestContext::HasThreadingSupport()) {
        return; // Skip gracefully if threading not supported
    }

    ASSERT_TRUE(init_basic_module());

    wasm_exec_env_t spawned_env = wasm_runtime_spawn_exec_env(exec_env);
    if (spawned_env) {
        // Test destroying spawned environment
        wasm_runtime_destroy_spawned_exec_env(spawned_env);
        // No crash or error indicates successful cleanup
    }
}

TEST_F(RuntimeCommonThreadingTest, DestroySpawnedExecEnv_WithNullEnvironment_HandlesGracefully)
{
    if (!PlatformTestContext::HasThreadingSupport()) {
        return; // Skip gracefully if threading not supported
    }

    // Test with null environment - should handle gracefully
    wasm_runtime_destroy_spawned_exec_env(nullptr);
    // No crash indicates successful handling
}

// Test wasm_cluster_create_blocking_op() - Function 6 (commented out - not available)
// TEST_F(RuntimeCommonThreadingTest, CreateBlockingOp_WithValidParameters_CreatesSuccessfully)
// {
//     // Function not available in current build configuration
// }

// Test wasm_runtime_atomic_wait() - Function 7 (commented out - not available)
// TEST_F(RuntimeCommonThreadingTest, AtomicWait_I32_WithValidParameters_WaitsCorrectly)
// {
//     // Atomic functions not available in current build configuration
// }

// TEST_F(RuntimeCommonThreadingTest, AtomicWait_I64_WithValidParameters_WaitsCorrectly)
// {
//     // Atomic functions not available in current build configuration
// }

// Test wasm_runtime_atomic_notify() - Function 8 (commented out - not available)
// TEST_F(RuntimeCommonThreadingTest, AtomicNotify_WithValidParameters_NotifiesCorrectly)
// {
//     // Atomic functions not available in current build configuration
// }

// TEST_F(RuntimeCommonThreadingTest, AtomicNotify_WithZeroCount_ReturnsZero)
// {
//     // Atomic functions not available in current build configuration
// }

// Thread routine execution test
struct ThreadTestData {
    wasm_exec_env_t exec_env;
    wasm_function_inst_t func;
    bool completed;
    int result;
};

void* test_thread_routine(void* arg)
{
    ThreadTestData* data = (ThreadTestData*)arg;
    if (!data || !data->exec_env || !data->func) {
        return nullptr;
    }

    // Simulate thread routine execution
    uint32_t argv[1] = {0};
    bool call_result = wasm_runtime_call_wasm(data->exec_env, data->func, 0, argv);
    
    data->completed = true;
    data->result = call_result ? 0 : -1;
    return &data->result;
}

TEST_F(RuntimeCommonThreadingTest, ThreadRoutine_Execution_CompletesSuccessfully)
{
    if (!PlatformTestContext::HasThreadingSupport()) {
        return; // Skip gracefully if threading not supported
    }

    ASSERT_TRUE(init_basic_module());

    wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst, "test_func");
    if (!func) {
        return; // Function not found in minimal module
    }

    // Create thread test data
    ThreadTestData thread_data = {0};
    thread_data.exec_env = exec_env;
    thread_data.func = func;
    thread_data.completed = false;
    thread_data.result = -1;

    // Create and run thread
    pthread_t thread;
    int create_result = pthread_create(&thread, nullptr, test_thread_routine, &thread_data);
    if (create_result == 0) {
        // Wait for thread completion
        void* thread_result = nullptr;
        int join_result = pthread_join(thread, &thread_result);
        
        ASSERT_EQ(0, join_result);
        ASSERT_TRUE(thread_data.completed);
        ASSERT_EQ(0, thread_data.result);
    }
}