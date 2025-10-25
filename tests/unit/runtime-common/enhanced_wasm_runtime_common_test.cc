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
#include "wasm_c_api.h"
#include "wasm_c_api_internal.h"

using namespace std;

// Enhanced test fixture for wasm_runtime_common.c functions - Lines 7227-7310
class EnhancedWasmRuntimeCommonCApiTest : public testing::Test {
protected:
    void SetUp() override {
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_Pool;
        init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
        init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);

        wasm_runtime_full_init(&init_args);

        module_inst = nullptr;
        exec_env = nullptr;

        // Initialize test data
        error_buf[0] = '\0';
        simple_wasm_size = 0;
        simple_wasm = nullptr;

        CreateSimpleWasmModule();
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

    // Create WASMFuncType for testing
    WASMFuncType* CreateTestFuncType(uint32 param_count, uint32 result_count,
                                     bool include_unsupported = false) {
        WASMFuncType *func_type = (WASMFuncType*)malloc(
            sizeof(WASMFuncType) + (param_count + result_count) * sizeof(uint8));
        if (!func_type) return nullptr;

        func_type->param_count = param_count;
        func_type->result_count = result_count;

        // Fill with supported types by default
        for (uint32 i = 0; i < param_count; i++) {
            func_type->types[i] = VALUE_TYPE_I32;
        }
        for (uint32 i = 0; i < result_count; i++) {
            func_type->types[param_count + i] = VALUE_TYPE_I32;
        }

        // Add unsupported type for error testing
        if (include_unsupported && param_count > 0) {
            func_type->types[0] = 0xFF; // Unsupported type
        }

        return func_type;
    }

    void FreeFuncType(WASMFuncType *func_type) {
        if (func_type) {
            free(func_type);
        }
    }

public:
    char global_heap_buf[512 * 1024];
    RuntimeInitArgs init_args;
    WASMModuleInstanceCommon *module_inst;
    WASMExecEnv *exec_env;
    char error_buf[128];
    uint8_t *simple_wasm;
    uint32 simple_wasm_size;
};

// Mock callback function for testing - no environment
static wasm_trap_t* mock_callback_no_env(const wasm_val_vec_t *params, wasm_val_vec_t *results) {
    // Simple successful callback
    if (results && results->size > 0) {
        results->num_elems = results->size;
        for (size_t i = 0; i < results->size; i++) {
            results->data[i].kind = WASM_I32;
            results->data[i].of.i32 = 42; // Test value
        }
    }
    return nullptr; // No trap
}

// Mock callback function for testing - with environment
static wasm_trap_t* mock_callback_with_env(void *env, const wasm_val_vec_t *params, wasm_val_vec_t *results) {
    // Simple successful callback
    if (results && results->size > 0) {
        results->num_elems = results->size;
        for (size_t i = 0; i < results->size; i++) {
            results->data[i].kind = WASM_I32;
            results->data[i].of.i32 = 24; // Different test value
        }
    }
    return nullptr; // No trap
}

// Mock callback that returns a trap
static wasm_trap_t* mock_callback_with_trap(const wasm_val_vec_t *params, wasm_val_vec_t *results) {
    // Create a trap with message
    wasm_message_t message;
    const char *trap_msg = "Test trap message";
    message.size = strlen(trap_msg);
    message.data = (char*)trap_msg;

    wasm_trap_t *trap = wasm_trap_new(nullptr, &message);
    return trap;
}

// Mock callback that returns a trap without message
static wasm_trap_t* mock_callback_with_empty_trap(const wasm_val_vec_t *params, wasm_val_vec_t *results) {
    return wasm_trap_new(nullptr, nullptr);
}

/******
 * Test Case: wasm_runtime_invoke_c_api_native_SmallParams_Success
 * Source: core/iwasm/common/wasm_runtime_common.c:7227-7310
 * Target Lines: 7232-7237, 7247-7250, 7261-7267, 7269-7271, 7303-7310
 * Functional Purpose: Tests successful invocation with small parameter and result counts
 *                     that use stack-allocated buffers (<=16 params, <=4 results)
 * Call Path: Direct API call to wasm_runtime_invoke_c_api_native()
 * Coverage Goal: Exercise normal successful path with small counts
 ******/
TEST_F(EnhancedWasmRuntimeCommonCApiTest, wasm_runtime_invoke_c_api_native_SmallParams_Success) {
    // Load and instantiate module
    wasm_module_t module = wasm_runtime_load(simple_wasm, simple_wasm_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module);

    module_inst = wasm_runtime_instantiate(module, 8192, 0, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module_inst);

    // Create function type with small counts
    WASMFuncType *func_type = CreateTestFuncType(4, 2); // 4 params, 2 results
    ASSERT_NE(nullptr, func_type);

    // Prepare arguments
    uint32 argv[6] = {10, 20, 30, 40, 0, 0}; // 4 inputs + space for 2 results

    // Call the function under test
    bool result = wasm_runtime_invoke_c_api_native(module_inst, (void*)mock_callback_no_env,
                                                   func_type, 4, argv, false, nullptr);

    ASSERT_TRUE(result);
    ASSERT_EQ(42, argv[4]); // Check first result
    ASSERT_EQ(42, argv[5]); // Check second result

    FreeFuncType(func_type);
    wasm_runtime_unload(module);
}

/******
 * Test Case: wasm_runtime_invoke_c_api_native_LargeParams_DynamicAllocation
 * Source: core/iwasm/common/wasm_runtime_common.c:7238-7245
 * Target Lines: 7238-7244 (param_count > 16 allocation path)
 * Functional Purpose: Tests dynamic memory allocation path when parameter count exceeds 16
 * Call Path: Direct API call to wasm_runtime_invoke_c_api_native()
 * Coverage Goal: Exercise dynamic allocation for parameters buffer
 ******/
TEST_F(EnhancedWasmRuntimeCommonCApiTest, wasm_runtime_invoke_c_api_native_LargeParams_DynamicAllocation) {
    // Load and instantiate module
    wasm_module_t module = wasm_runtime_load(simple_wasm, simple_wasm_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module);

    module_inst = wasm_runtime_instantiate(module, 8192, 0, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module_inst);

    // Create function type with large parameter count
    WASMFuncType *func_type = CreateTestFuncType(20, 2); // 20 params > 16, 2 results
    ASSERT_NE(nullptr, func_type);

    // Prepare arguments - 20 params + 2 results
    uint32 argv[22];
    for (int i = 0; i < 20; i++) {
        argv[i] = i + 1;
    }
    argv[20] = 0;
    argv[21] = 0;

    // Call the function under test
    bool result = wasm_runtime_invoke_c_api_native(module_inst, (void*)mock_callback_no_env,
                                                   func_type, 20, argv, false, nullptr);

    ASSERT_TRUE(result);
    ASSERT_EQ(42, argv[20]); // Check first result
    ASSERT_EQ(42, argv[21]); // Check second result

    FreeFuncType(func_type);
    wasm_runtime_unload(module);
}

/******
 * Test Case: wasm_runtime_invoke_c_api_native_LargeResults_DynamicAllocation
 * Source: core/iwasm/common/wasm_runtime_common.c:7252-7258
 * Target Lines: 7252-7258 (result_count > 4 allocation path)
 * Functional Purpose: Tests dynamic memory allocation path when result count exceeds 4
 * Call Path: Direct API call to wasm_runtime_invoke_c_api_native()
 * Coverage Goal: Exercise dynamic allocation for results buffer
 ******/
TEST_F(EnhancedWasmRuntimeCommonCApiTest, wasm_runtime_invoke_c_api_native_LargeResults_DynamicAllocation) {
    // Load and instantiate module
    wasm_module_t module = wasm_runtime_load(simple_wasm, simple_wasm_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module);

    module_inst = wasm_runtime_instantiate(module, 8192, 0, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module_inst);

    // Create function type with large result count
    WASMFuncType *func_type = CreateTestFuncType(2, 8); // 2 params, 8 results > 4
    ASSERT_NE(nullptr, func_type);

    // Prepare arguments - 2 params + 8 results
    uint32 argv[10];
    argv[0] = 100;
    argv[1] = 200;
    for (int i = 2; i < 10; i++) {
        argv[i] = 0;
    }

    // Call the function under test
    bool result = wasm_runtime_invoke_c_api_native(module_inst, (void*)mock_callback_no_env,
                                                   func_type, 2, argv, false, nullptr);

    ASSERT_TRUE(result);
    // Verify all 8 results are set
    for (int i = 2; i < 10; i++) {
        ASSERT_EQ(42, argv[i]);
    }

    FreeFuncType(func_type);
    wasm_runtime_unload(module);
}

/******
 * Test Case: wasm_runtime_invoke_c_api_native_WithEnv_CallbackSuccess
 * Source: core/iwasm/common/wasm_runtime_common.c:7273-7277
 * Target Lines: 7273-7277 (with_env=true callback path)
 * Functional Purpose: Tests callback invocation with environment parameter
 * Call Path: Direct API call to wasm_runtime_invoke_c_api_native()
 * Coverage Goal: Exercise callback path with environment
 ******/
TEST_F(EnhancedWasmRuntimeCommonCApiTest, wasm_runtime_invoke_c_api_native_WithEnv_CallbackSuccess) {
    // Load and instantiate module
    wasm_module_t module = wasm_runtime_load(simple_wasm, simple_wasm_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module);

    module_inst = wasm_runtime_instantiate(module, 8192, 0, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module_inst);

    // Create function type
    WASMFuncType *func_type = CreateTestFuncType(2, 2);
    ASSERT_NE(nullptr, func_type);

    // Prepare arguments and environment
    uint32 argv[4] = {10, 20, 0, 0};
    int dummy_env = 12345;

    // Call the function under test with environment
    bool result = wasm_runtime_invoke_c_api_native(module_inst, (void*)mock_callback_with_env,
                                                   func_type, 2, argv, true, &dummy_env);

    ASSERT_TRUE(result);
    ASSERT_EQ(24, argv[2]); // Check different result value from with_env callback
    ASSERT_EQ(24, argv[3]);

    FreeFuncType(func_type);
    wasm_runtime_unload(module);
}

/******
 * Test Case: wasm_runtime_invoke_c_api_native_TrapWithMessage_ExceptionSet
 * Source: core/iwasm/common/wasm_runtime_common.c:7279-7296
 * Target Lines: 7279-7296 (trap handling with message)
 * Functional Purpose: Tests trap handling when callback returns trap with message
 * Call Path: Direct API call to wasm_runtime_invoke_c_api_native()
 * Coverage Goal: Exercise trap handling path with message processing
 ******/
TEST_F(EnhancedWasmRuntimeCommonCApiTest, wasm_runtime_invoke_c_api_native_TrapWithMessage_ExceptionSet) {
    // Load and instantiate module
    wasm_module_t module = wasm_runtime_load(simple_wasm, simple_wasm_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module);

    module_inst = wasm_runtime_instantiate(module, 8192, 0, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module_inst);

    // Create function type
    WASMFuncType *func_type = CreateTestFuncType(1, 1);
    ASSERT_NE(nullptr, func_type);

    // Prepare arguments
    uint32 argv[2] = {10, 0};

    // Call the function under test with trap-generating callback
    bool result = wasm_runtime_invoke_c_api_native(module_inst, (void*)mock_callback_with_trap,
                                                   func_type, 1, argv, false, nullptr);

    // Should return false due to trap
    ASSERT_FALSE(result);

    // Check that exception was set
    const char *exception = wasm_runtime_get_exception(module_inst);
    ASSERT_NE(nullptr, exception);
    ASSERT_TRUE(strstr(exception, "Test trap message") != nullptr);

    FreeFuncType(func_type);
    wasm_runtime_clear_exception(module_inst);
    wasm_runtime_unload(module);
}

/******
 * Test Case: wasm_runtime_invoke_c_api_native_TrapWithoutMessage_ExceptionSet
 * Source: core/iwasm/common/wasm_runtime_common.c:7291-7296
 * Target Lines: 7291-7296 (trap handling without message)
 * Functional Purpose: Tests trap handling when callback returns trap without message
 * Call Path: Direct API call to wasm_runtime_invoke_c_api_native()
 * Coverage Goal: Exercise trap handling path for unknown exceptions
 ******/
TEST_F(EnhancedWasmRuntimeCommonCApiTest, wasm_runtime_invoke_c_api_native_TrapWithoutMessage_ExceptionSet) {
    // Load and instantiate module
    wasm_module_t module = wasm_runtime_load(simple_wasm, simple_wasm_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module);

    module_inst = wasm_runtime_instantiate(module, 8192, 0, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module_inst);

    // Create function type
    WASMFuncType *func_type = CreateTestFuncType(1, 1);
    ASSERT_NE(nullptr, func_type);

    // Prepare arguments
    uint32 argv[2] = {10, 0};

    // Call the function under test with empty trap callback
    bool result = wasm_runtime_invoke_c_api_native(module_inst, (void*)mock_callback_with_empty_trap,
                                                   func_type, 1, argv, false, nullptr);

    // Should return false due to trap
    ASSERT_FALSE(result);

    // Check that unknown exception was set
    const char *exception = wasm_runtime_get_exception(module_inst);
    ASSERT_NE(nullptr, exception);
    ASSERT_TRUE(strstr(exception, "native function throw unknown exception") != nullptr);

    FreeFuncType(func_type);
    wasm_runtime_clear_exception(module_inst);
    wasm_runtime_unload(module);
}

/******
 * Test Case: wasm_runtime_invoke_c_api_native_UnsupportedParamType_Failure
 * Source: core/iwasm/common/wasm_runtime_common.c:7247-7249
 * Target Lines: 7247-7249 (argv_to_params failure path)
 * Functional Purpose: Tests failure path when argv_to_params encounters unsupported parameter type
 * Call Path: Direct API call -> argv_to_params() failure
 * Coverage Goal: Exercise parameter conversion error path
 ******/
TEST_F(EnhancedWasmRuntimeCommonCApiTest, wasm_runtime_invoke_c_api_native_UnsupportedParamType_Failure) {
    // Load and instantiate module
    wasm_module_t module = wasm_runtime_load(simple_wasm, simple_wasm_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module);

    module_inst = wasm_runtime_instantiate(module, 8192, 0, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module_inst);

    // Create function type with unsupported parameter type
    WASMFuncType *func_type = CreateTestFuncType(2, 1, true); // Include unsupported type
    ASSERT_NE(nullptr, func_type);

    // Prepare arguments
    uint32 argv[3] = {10, 20, 0};

    // Call the function under test
    bool result = wasm_runtime_invoke_c_api_native(module_inst, (void*)mock_callback_no_env,
                                                   func_type, 2, argv, false, nullptr);

    // Should return false due to unsupported param type
    ASSERT_FALSE(result);

    // Check that exception was set
    const char *exception = wasm_runtime_get_exception(module_inst);
    ASSERT_NE(nullptr, exception);
    ASSERT_TRUE(strstr(exception, "unsupported param type") != nullptr);

    FreeFuncType(func_type);
    wasm_runtime_clear_exception(module_inst);
    wasm_runtime_unload(module);
}

/******
 * Test Case: wasm_runtime_invoke_c_api_native_ResultsToArgvFailure_ExceptionSet
 * Source: core/iwasm/common/wasm_runtime_common.c:7299-7301
 * Target Lines: 7299-7301 (results_to_argv failure path)
 * Functional Purpose: Tests failure path when results_to_argv encounters unsupported result type
 * Call Path: Direct API call -> results_to_argv() failure
 * Coverage Goal: Exercise result conversion error path
 ******/
TEST_F(EnhancedWasmRuntimeCommonCApiTest, wasm_runtime_invoke_c_api_native_ResultsToArgvFailure_ExceptionSet) {
    // Load and instantiate module
    wasm_module_t module = wasm_runtime_load(simple_wasm, simple_wasm_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module);

    module_inst = wasm_runtime_instantiate(module, 8192, 0, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, module_inst);

    // Create function type with unsupported result type
    WASMFuncType *func_type = CreateTestFuncType(1, 1);
    ASSERT_NE(nullptr, func_type);

    // Set unsupported result type
    func_type->types[1] = 0xFF; // Unsupported result type

    // Prepare arguments
    uint32 argv[2] = {10, 0};

    // Call the function under test - results_to_argv will fail
    bool result = wasm_runtime_invoke_c_api_native(module_inst, (void*)mock_callback_no_env,
                                                   func_type, 1, argv, false, nullptr);

    // Should return false due to unsupported result type
    ASSERT_FALSE(result);

    // Check that exception was set
    const char *exception = wasm_runtime_get_exception(module_inst);
    ASSERT_NE(nullptr, exception);
    ASSERT_TRUE(strstr(exception, "unsupported result type") != nullptr);

    FreeFuncType(func_type);
    wasm_runtime_clear_exception(module_inst);
    wasm_runtime_unload(module);
}