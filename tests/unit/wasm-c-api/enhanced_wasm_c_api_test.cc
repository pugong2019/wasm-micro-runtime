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