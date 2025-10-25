/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <gtest/gtest.h>
#include <limits.h>
#include <cmath>
#include "wasm_c_api.h"
#include "wasm_c_api_internal.h"
#include "wasm_runtime_common.h"

// Enhanced test fixture for wasm_c_api.c coverage improvement
class EnhancedWasmCApiTestTableSet : public testing::Test
{
protected:
    void SetUp() override
    {
        // Initialize runtime - wasm_runtime_init takes no parameters
        bool init_result = wasm_runtime_init();
        ASSERT_TRUE(init_result);
        runtime_initialized = true;
    }

    void TearDown() override
    {
        if (runtime_initialized) {
            wasm_runtime_destroy();
        }
    }

    bool runtime_initialized = false;
};

/******
 * Test Case: wasm_table_set_NullTable_ReturnsFalse
 * Source: core/iwasm/common/wasm_c_api.c:4070-4072
 * Target Lines: 4070-4072 (null table validation)
 * Functional Purpose: Validates that wasm_table_set correctly rejects null table
 *                     parameter and returns false without attempting any operations.
 * Coverage Goal: Exercise null table parameter validation path
 ******/
TEST_F(EnhancedWasmCApiTestTableSet, wasm_table_set_NullTable_ReturnsFalse)
{
    // Test null table - this exercises lines 4070-4071
    bool result = wasm_table_set(nullptr, 0, nullptr);
    ASSERT_FALSE(result);
}

/******
 * Test Case: wasm_table_set_NullInstCommRt_ReturnsFalse
 * Source: core/iwasm/common/wasm_c_api.c:4070-4072
 * Target Lines: 4070-4072 (null inst_comm_rt validation)
 * Functional Purpose: Validates that wasm_table_set correctly rejects table with
 *                     null inst_comm_rt and returns false.
 * Coverage Goal: Exercise inst_comm_rt null validation path
 ******/
TEST_F(EnhancedWasmCApiTestTableSet, wasm_table_set_NullInstCommRt_ReturnsFalse)
{
    // Test table without inst_comm_rt - this exercises lines 4070-4071
    wasm_table_t* invalid_table = (wasm_table_t*)wasm_runtime_malloc(sizeof(wasm_table_t));
    ASSERT_NE(nullptr, invalid_table);
    memset(invalid_table, 0, sizeof(wasm_table_t));
    invalid_table->inst_comm_rt = nullptr;

    bool result = wasm_table_set(invalid_table, 0, nullptr);
    ASSERT_FALSE(result);

    wasm_runtime_free(invalid_table);
}

/******
 * Test Case: wasm_table_set_InvalidModuleTypeConfig_ReturnsFalse
 * Source: core/iwasm/common/wasm_c_api.c:4119-4121
 * Target Lines: 4119-4121 (null p_ref_idx validation)
 * Functional Purpose: Validates that wasm_table_set handles the edge case where
 *                     neither interpreter nor AOT paths set p_ref_idx, indicating
 *                     wrong module filetype and compilation flag combination.
 * Coverage Goal: Exercise error path for invalid module type configuration
 ******/
TEST_F(EnhancedWasmCApiTestTableSet, wasm_table_set_InvalidModuleTypeConfig_ReturnsFalse)
{
    // This test verifies the error condition at lines 4119-4121
    // This occurs when there's a mismatch between module type and compilation flags

    wasm_table_t* malformed_table = (wasm_table_t*)wasm_runtime_malloc(sizeof(wasm_table_t));
    ASSERT_NE(nullptr, malformed_table);
    memset(malformed_table, 0, sizeof(wasm_table_t));

    // Create a mock instance with invalid module type
    WASMModuleInstanceCommon* mock_inst = (WASMModuleInstanceCommon*)wasm_runtime_malloc(sizeof(WASMModuleInstanceCommon));
    ASSERT_NE(nullptr, mock_inst);
    memset(mock_inst, 0, sizeof(WASMModuleInstanceCommon));
    mock_inst->module_type = (uint8)255; // Invalid module type to trigger p_ref_idx == NULL

    malformed_table->inst_comm_rt = mock_inst;
    malformed_table->table_idx_rt = 0;

    // Create a mock table type to pass initial validation
    wasm_tabletype_t* table_type = (wasm_tabletype_t*)wasm_runtime_malloc(sizeof(wasm_tabletype_t));
    ASSERT_NE(nullptr, table_type);
    memset(table_type, 0, sizeof(wasm_tabletype_t));

    wasm_valtype_t* val_type = (wasm_valtype_t*)wasm_runtime_malloc(sizeof(wasm_valtype_t));
    ASSERT_NE(nullptr, val_type);
    val_type->kind = WASM_FUNCREF;

    table_type->val_type = val_type;
    malformed_table->type = table_type;

    // This should fail because p_ref_idx will remain NULL due to invalid module_type
    bool result = wasm_table_set(malformed_table, 0, nullptr);
    ASSERT_FALSE(result);

    wasm_runtime_free(val_type);
    wasm_runtime_free(table_type);
    wasm_runtime_free(mock_inst);
    wasm_runtime_free(malformed_table);
}

// =============================================================================
// NEW TESTS: AOT Export Processing Coverage (Lines 4743-4831)
// =============================================================================

// Enhanced test fixture for aot_process_export coverage
class EnhancedWasmCApiTestAotExport : public testing::Test
{
protected:
    void SetUp() override
    {
        // Initialize runtime
        bool init_result = wasm_runtime_init();
        ASSERT_TRUE(init_result);
        runtime_initialized = true;

        // Create engine and store
        engine = wasm_engine_new();
        ASSERT_NE(nullptr, engine);
        store = wasm_store_new(engine);
        ASSERT_NE(nullptr, store);

        // Use proven working WASM bytecode from existing tests

        // WASM module with function export (working from test_module_operations.cc)
        wasm_func_export_only = {
            0x00, 0x61, 0x73, 0x6d,  // WASM magic number
            0x01, 0x00, 0x00, 0x00,  // Version 1
            0x01, 0x04, 0x01, 0x60,  // Type section: 1 function type
            0x00, 0x00,              // Function type: no params, no results
            0x03, 0x02, 0x01, 0x00,  // Function section: 1 function of type 0
            0x07, 0x07, 0x01, 0x03,  // Export section: 1 export
            0x66, 0x6f, 0x6f, 0x00,  // Export name "foo", function index 0
            0x00,
            0x0a, 0x04, 0x01, 0x02,  // Code section: 1 function body
            0x00, 0x0b               // Function body: end
        };

        // Simplified WASM module with global export
        wasm_global_export_only = {
            0x00, 0x61, 0x73, 0x6d,  // WASM magic number
            0x01, 0x00, 0x00, 0x00,  // Version 1
            0x06, 0x06, 0x01, 0x7f,  // Global section: 1 global (i32, mutable)
            0x01, 0x41, 0x2a, 0x0b,  // Global: mutable i32 with initial value 42
            0x07, 0x0a, 0x01, 0x06,  // Export section: 1 export, 10 bytes
            0x67, 0x6c, 0x6f, 0x62,  // Export name "glob"
            0x61, 0x6c, 0x03, 0x00   // Export type: global, index 0
        };

        // Empty WASM module (no exports) for testing edge cases
        empty_wasm_module = {
            0x00, 0x61, 0x73, 0x6d,  // WASM magic number
            0x01, 0x00, 0x00, 0x00   // Version 1 only
        };

        // WASM module with memory export
        wasm_memory_export_only = {
            0x00, 0x61, 0x73, 0x6d,  // WASM magic number
            0x01, 0x00, 0x00, 0x00,  // Version 1
            0x05, 0x03, 0x01, 0x00,  // Memory section: 1 memory
            0x01,                    // Memory: min 1 page
            0x07, 0x09, 0x01, 0x05,  // Export section: 1 export, 9 bytes
            0x6d, 0x65, 0x6d, 0x6f,  // Export name "memo"
            0x72, 0x02, 0x00         // Export type: memory, index 0
        };
    }

    void TearDown() override
    {
        if (store) wasm_store_delete(store);
        if (engine) wasm_engine_delete(engine);
        if (runtime_initialized) {
            wasm_runtime_destroy();
        }
    }

    bool runtime_initialized = false;
    wasm_engine_t* engine = nullptr;
    wasm_store_t* store = nullptr;
    std::vector<uint8_t> wasm_func_export_only;
    std::vector<uint8_t> wasm_global_export_only;
    std::vector<uint8_t> wasm_memory_export_only;
    std::vector<uint8_t> empty_wasm_module;
};

/******
 * Test Case: aot_process_export_GlobalExport_ProcessesGlobalExportPath
 * Source: core/iwasm/common/wasm_c_api.c:4771-4782
 * Target Lines: 4771-4782 (EXPORT_KIND_GLOBAL case)
 * Functional Purpose: Validates that aot_process_export correctly processes
 *                     global exports by calling wasm_global_new_internal and
 *                     converting to external representation.
 * Coverage Goal: Exercise EXPORT_KIND_GLOBAL processing path specifically
 ******/
TEST_F(EnhancedWasmCApiTestAotExport, aot_process_export_GlobalExport_ProcessesGlobalExportPath)
{
    wasm_byte_vec_t wasm_bytes;
    wasm_byte_vec_new(&wasm_bytes, wasm_global_export_only.size(),
                      reinterpret_cast<const wasm_byte_t*>(wasm_global_export_only.data()));

    wasm_module_t* module = wasm_module_new(store, &wasm_bytes);
    ASSERT_NE(nullptr, module);

    // This triggers aot_process_export which should hit EXPORT_KIND_GLOBAL case (lines 4771-4782)
    wasm_instance_t* instance = wasm_instance_new(store, module, nullptr, nullptr);
    ASSERT_NE(nullptr, instance);

    wasm_extern_vec_t exports;
    wasm_instance_exports(instance, &exports);

    // Should have 1 global export
    ASSERT_EQ(1u, exports.size);

    wasm_extern_t* global_extern = exports.data[0];
    ASSERT_NE(nullptr, global_extern);
    ASSERT_EQ(WASM_EXTERN_GLOBAL, wasm_extern_kind(global_extern));

    // Verify the global export was created successfully (line 4780: wasm_global_as_extern)
    wasm_global_t* global = wasm_extern_as_global(global_extern);
    ASSERT_NE(nullptr, global);

    wasm_extern_vec_delete(&exports);
    wasm_instance_delete(instance);
    wasm_module_delete(module);
    wasm_byte_vec_delete(&wasm_bytes);
}

/******
 * Test Case: aot_process_export_MemoryExport_ProcessesMemoryExportPath
 * Source: core/iwasm/common/wasm_c_api.c:4795-4806
 * Target Lines: 4795-4806 (EXPORT_KIND_MEMORY case)
 * Functional Purpose: Validates that aot_process_export correctly processes
 *                     memory exports by calling wasm_memory_new_internal and
 *                     converting to external representation.
 * Coverage Goal: Exercise EXPORT_KIND_MEMORY processing path specifically
 ******/
TEST_F(EnhancedWasmCApiTestAotExport, aot_process_export_MemoryExport_ProcessesMemoryExportPath)
{
    wasm_byte_vec_t wasm_bytes;
    wasm_byte_vec_new(&wasm_bytes, wasm_memory_export_only.size(),
                      reinterpret_cast<const wasm_byte_t*>(wasm_memory_export_only.data()));

    wasm_module_t* module = wasm_module_new(store, &wasm_bytes);
    ASSERT_NE(nullptr, module);

    // This triggers aot_process_export which should hit EXPORT_KIND_MEMORY case (lines 4795-4806)
    wasm_instance_t* instance = wasm_instance_new(store, module, nullptr, nullptr);
    ASSERT_NE(nullptr, instance);

    wasm_extern_vec_t exports;
    wasm_instance_exports(instance, &exports);

    // Should have 1 memory export
    ASSERT_EQ(1u, exports.size);

    wasm_extern_t* memory_extern = exports.data[0];
    ASSERT_NE(nullptr, memory_extern);
    ASSERT_EQ(WASM_EXTERN_MEMORY, wasm_extern_kind(memory_extern));

    // Verify the memory export was created successfully (line 4804: wasm_memory_as_extern)
    wasm_memory_t* memory = wasm_extern_as_memory(memory_extern);
    ASSERT_NE(nullptr, memory);

    wasm_extern_vec_delete(&exports);
    wasm_instance_delete(instance);
    wasm_module_delete(module);
    wasm_byte_vec_delete(&wasm_bytes);
}

/******
 * Test Case: aot_process_export_SingleFunctionExport_ProcessesFuncExportPath
 * Source: core/iwasm/common/wasm_c_api.c:4759-4770
 * Target Lines: 4759-4770 (EXPORT_KIND_FUNC case)
 * Functional Purpose: Validates that aot_process_export correctly processes
 *                     function exports by calling wasm_func_new_internal and
 *                     converting to external representation.
 * Coverage Goal: Exercise EXPORT_KIND_FUNC processing path specifically
 ******/
TEST_F(EnhancedWasmCApiTestAotExport, aot_process_export_SingleFunctionExport_ProcessesFuncExportPath)
{
    wasm_byte_vec_t wasm_bytes;
    wasm_byte_vec_new(&wasm_bytes, wasm_func_export_only.size(),
                      reinterpret_cast<const wasm_byte_t*>(wasm_func_export_only.data()));

    wasm_module_t* module = wasm_module_new(store, &wasm_bytes);
    ASSERT_NE(nullptr, module);

    // This triggers aot_process_export which should hit EXPORT_KIND_FUNC case (lines 4759-4770)
    wasm_instance_t* instance = wasm_instance_new(store, module, nullptr, nullptr);
    ASSERT_NE(nullptr, instance);

    wasm_extern_vec_t exports;
    wasm_instance_exports(instance, &exports);

    // Should have 1 function export
    ASSERT_EQ(1u, exports.size);

    wasm_extern_t* func_extern = exports.data[0];
    ASSERT_NE(nullptr, func_extern);
    ASSERT_EQ(WASM_EXTERN_FUNC, wasm_extern_kind(func_extern));

    // Verify the function export was created successfully (line 4768: wasm_func_as_extern)
    wasm_func_t* func = wasm_extern_as_func(func_extern);
    ASSERT_NE(nullptr, func);

    wasm_extern_vec_delete(&exports);
    wasm_instance_delete(instance);
    wasm_module_delete(module);
    wasm_byte_vec_delete(&wasm_bytes);
}

/******
 * Test Case: aot_process_export_EmptyExportList_ReturnsSuccessWithoutProcessing
 * Source: core/iwasm/common/wasm_c_api.c:4755-4827
 * Target Lines: 4755 (loop condition with zero exports), 4827 (success return)
 * Functional Purpose: Validates that aot_process_export correctly handles
 *                     modules with no exports by skipping the export loop
 *                     and returning success immediately.
 * Coverage Goal: Exercise empty export list edge case
 ******/
TEST_F(EnhancedWasmCApiTestAotExport, aot_process_export_EmptyExportList_ReturnsSuccessWithoutProcessing)
{
    wasm_byte_vec_t wasm_bytes;
    wasm_byte_vec_new(&wasm_bytes, empty_wasm_module.size(),
                      reinterpret_cast<const wasm_byte_t*>(empty_wasm_module.data()));

    wasm_module_t* module = wasm_module_new(store, &wasm_bytes);
    ASSERT_NE(nullptr, module);

    // This should trigger aot_process_export with zero exports (exercise line 4755 condition)
    wasm_instance_t* instance = wasm_instance_new(store, module, nullptr, nullptr);
    ASSERT_NE(nullptr, instance);

    wasm_extern_vec_t exports;
    wasm_instance_exports(instance, &exports);

    // Should have no exports - confirms loop was skipped and success path (line 4827) was taken
    ASSERT_EQ(0u, exports.size);

    wasm_extern_vec_delete(&exports);
    wasm_instance_delete(instance);
    wasm_module_delete(module);
    wasm_byte_vec_delete(&wasm_bytes);
}