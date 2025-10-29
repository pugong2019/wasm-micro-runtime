/*
 * Copyright (C) 2021 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "test_helper.h"
#include "gtest/gtest.h"
#include "platform_common.h"
#include "wasm_runtime_common.h"
#include "bh_read_file.h"
#include "wasm_runtime.h"
#include "bh_platform.h"
#include "wasm_export.h"
#include "../interpreter/wasm.h"

// Fast-JIT specific includes
#include "jit_frontend.h"
#include "jit_compiler.h"

// Enhanced test fixture for jit_frontend.c functions
class EnhancedJitFrontendTest : public testing::Test {
protected:
    void SetUp() override {
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_Pool;
        init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
        init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);

        ASSERT_TRUE(wasm_runtime_full_init(&init_args));

        cleanup = true;

        // Initialize a minimal WASMModule for testing
        test_module = (WASMModule*)wasm_runtime_malloc(sizeof(WASMModule));
        ASSERT_NE(nullptr, test_module);
        memset(test_module, 0, sizeof(WASMModule));

        // Set up basic module properties
        test_module->global_data_size = 128;
        test_module->import_table_count = 0;
        test_module->table_count = 0;
        test_module->import_memory_count = 0;
        test_module->memory_count = 1;  // Default to 1 memory for testing
    }

    void TearDown() override {
        if (test_module) {
            if (test_module->import_tables) {
                wasm_runtime_free(test_module->import_tables);
            }
            if (test_module->tables) {
                wasm_runtime_free(test_module->tables);
            }
            wasm_runtime_free(test_module);
            test_module = nullptr;
        }

        if (cleanup) {
            wasm_runtime_destroy();
            cleanup = false;
        }
    }

    void CreateImportTables(uint32 count) {
        test_module->import_table_count = count;
        if (count > 0) {
            test_module->import_tables = (WASMImport*)wasm_runtime_malloc(
                sizeof(WASMImport) * count);
            ASSERT_NE(nullptr, test_module->import_tables);

            for (uint32 i = 0; i < count; i++) {
                // Initialize import table structure
                test_module->import_tables[i].kind = IMPORT_KIND_TABLE;
                test_module->import_tables[i].u.table.table_type.init_size = 10 + i;
                test_module->import_tables[i].u.table.table_type.max_size = 20 + i;
                test_module->import_tables[i].u.table.table_type.possible_grow = true;
            }
        }
    }

    void CreateLocalTables(uint32 count) {
        test_module->table_count = count;
        if (count > 0) {
            test_module->tables = (WASMTable*)wasm_runtime_malloc(
                sizeof(WASMTable) * count);
            ASSERT_NE(nullptr, test_module->tables);

            for (uint32 i = 0; i < count; i++) {
                test_module->tables[i].table_type.init_size = 15 + i;
                test_module->tables[i].table_type.max_size = 25 + i;
                test_module->tables[i].table_type.possible_grow = true;
            }
        }
    }

public:
    char global_heap_buf[512 * 1024];
    RuntimeInitArgs init_args;
    bool cleanup = true;
    WASMModule *test_module = nullptr;
};

/******
 * Test Case: jit_frontend_get_table_inst_offset_NoTables_ReturnsBaseOffset
 * Source: core/iwasm/fast-jit/jit_frontend.c:68-113
 * Target Lines: 68-73 (function entry, initialization), 90-92 (early return path)
 * Functional Purpose: Validates that jit_frontend_get_table_inst_offset() correctly
 *                     handles modules with no tables and returns the base offset
 *                     calculated from get_first_table_inst_offset().
 * Call Path: jit_frontend_get_table_inst_offset() [Direct function call]
 * Coverage Goal: Exercise function initialization and early return path when tbl_idx == 0
 ******/
TEST_F(EnhancedJitFrontendTest, jit_frontend_get_table_inst_offset_NoTables_ReturnsBaseOffset)
{
    // Test with no import tables and no local tables
    test_module->import_table_count = 0;
    test_module->table_count = 0;

    uint32 result = jit_frontend_get_table_inst_offset(test_module, 0);

    // Should return the base offset from get_first_table_inst_offset()
    // This exercises lines 68-73 (initialization) and 90-92 (early return)
    ASSERT_GT(result, 0);  // Should be positive value representing memory offset
}

/******
 * Test Case: jit_frontend_get_table_inst_offset_SingleImportTable_ProcessesLoop
 * Source: core/iwasm/fast-jit/jit_frontend.c:68-113
 * Target Lines: 74-88 (import table processing loop), 78-85 (MULTI_MODULE conditional)
 * Functional Purpose: Validates that jit_frontend_get_table_inst_offset() correctly
 *                     processes single import table through the while loop and
 *                     calculates offset with WASM_ENABLE_MULTI_MODULE enabled.
 * Call Path: jit_frontend_get_table_inst_offset() [Direct function call]
 * Coverage Goal: Exercise import table loop processing and MULTI_MODULE branch
 ******/
TEST_F(EnhancedJitFrontendTest, jit_frontend_get_table_inst_offset_SingleImportTable_ProcessesLoop)
{
    CreateImportTables(1);
    test_module->table_count = 0;

    uint32 result = jit_frontend_get_table_inst_offset(test_module, 0);

    // Should process the import table and calculate correct offset
    // This exercises lines 74-88 with MULTI_MODULE enabled (lines 78-85)
    ASSERT_GT(result, test_module->global_data_size);
}

/******
 * Test Case: jit_frontend_get_table_inst_offset_MultipleImportTables_ExitsEarly
 * Source: core/iwasm/fast-jit/jit_frontend.c:68-113
 * Target Lines: 74-88 (import table loop), 90-92 (early exit when i == tbl_idx)
 * Functional Purpose: Validates that jit_frontend_get_table_inst_offset() correctly
 *                     processes multiple import tables and exits early when the
 *                     requested table index matches an import table.
 * Call Path: jit_frontend_get_table_inst_offset() [Direct function call]
 * Coverage Goal: Exercise multi-iteration import table loop and early exit path
 ******/
TEST_F(EnhancedJitFrontendTest, jit_frontend_get_table_inst_offset_MultipleImportTables_ExitsEarly)
{
    CreateImportTables(3);
    test_module->table_count = 0;

    // Request table index 1 (second import table)
    uint32 result = jit_frontend_get_table_inst_offset(test_module, 1);

    // Should process first import table, then return when i == tbl_idx
    // This exercises the import table loop and early return on line 90-92
    ASSERT_GT(result, test_module->global_data_size);
}

/******
 * Test Case: jit_frontend_get_table_inst_offset_LocalTablesOnly_ProcessesSecondLoop
 * Source: core/iwasm/fast-jit/jit_frontend.c:68-113
 * Target Lines: 94-111 (local table processing loop), 100-108 (MULTI_MODULE conditional)
 * Functional Purpose: Validates that jit_frontend_get_table_inst_offset() correctly
 *                     processes local tables when no import tables exist, exercising
 *                     the second while loop and MULTI_MODULE conditional branches.
 * Call Path: jit_frontend_get_table_inst_offset() [Direct function call]
 * Coverage Goal: Exercise local table loop processing and MULTI_MODULE branch
 ******/
TEST_F(EnhancedJitFrontendTest, jit_frontend_get_table_inst_offset_LocalTablesOnly_ProcessesSecondLoop)
{
    test_module->import_table_count = 0;
    CreateLocalTables(2);

    // Request table index 1 (second local table)
    uint32 result = jit_frontend_get_table_inst_offset(test_module, 1);

    // Should process local tables and calculate offset
    // This exercises lines 94-111 with MULTI_MODULE enabled (lines 100-108)
    ASSERT_GT(result, test_module->global_data_size);
}

/******
 * Test Case: jit_frontend_get_table_inst_offset_MixedTables_ProcessesBothLoops
 * Source: core/iwasm/fast-jit/jit_frontend.c:68-113
 * Target Lines: 74-88 (import tables), 94-111 (local tables), 113 (final return)
 * Functional Purpose: Validates that jit_frontend_get_table_inst_offset() correctly
 *                     handles mixed import and local tables, processing through both
 *                     loops to reach a local table index.
 * Call Path: jit_frontend_get_table_inst_offset() [Direct function call]
 * Coverage Goal: Exercise both table processing loops and final return statement
 ******/
TEST_F(EnhancedJitFrontendTest, jit_frontend_get_table_inst_offset_MixedTables_ProcessesBothLoops)
{
    CreateImportTables(2);
    CreateLocalTables(2);

    // Request table index 3 (second local table after 2 import tables)
    uint32 result = jit_frontend_get_table_inst_offset(test_module, 3);

    // Should process all import tables, then process local tables
    // This exercises both loops and the final return on line 113
    ASSERT_GT(result, test_module->global_data_size);
}

/******
 * Test Case: jit_frontend_get_table_inst_offset_ExactImportMatch_ReturnsEarly
 * Source: core/iwasm/fast-jit/jit_frontend.c:68-113
 * Target Lines: 74-88 (import table loop), 90-92 (exact match early return)
 * Functional Purpose: Validates that jit_frontend_get_table_inst_offset() correctly
 *                     returns immediately when tbl_idx exactly matches the number
 *                     of import tables processed (i == tbl_idx condition).
 * Call Path: jit_frontend_get_table_inst_offset() [Direct function call]
 * Coverage Goal: Exercise exact match condition for early return
 ******/
TEST_F(EnhancedJitFrontendTest, jit_frontend_get_table_inst_offset_ExactImportMatch_ReturnsEarly)
{
    CreateImportTables(2);
    CreateLocalTables(1);

    // Request table index 2 (exactly matches import_table_count)
    uint32 result = jit_frontend_get_table_inst_offset(test_module, 2);

    // Should process all import tables and return without processing local tables
    // This exercises the exact match condition on lines 90-92
    ASSERT_GT(result, test_module->global_data_size);
}

/******
 * Test Case: jit_frontend_get_table_inst_offset_LargeTableIndex_ReturnsOffset
 * Source: core/iwasm/fast-jit/jit_frontend.c:68-113
 * Target Lines: 94-111 (local table processing), 113 (final return)
 * Functional Purpose: Validates that jit_frontend_get_table_inst_offset() correctly
 *                     handles large table indices that exceed both import and local
 *                     table counts, ensuring proper offset calculation.
 * Call Path: jit_frontend_get_table_inst_offset() [Direct function call]
 * Coverage Goal: Exercise boundary condition handling and final return path
 ******/
TEST_F(EnhancedJitFrontendTest, jit_frontend_get_table_inst_offset_LargeTableIndex_ReturnsOffset)
{
    CreateImportTables(1);
    CreateLocalTables(1);

    // Request table index 5 (exceeds available tables)
    uint32 result = jit_frontend_get_table_inst_offset(test_module, 5);

    // Should process all available tables and return final calculated offset
    // This exercises the final return on line 113 with boundary conditions
    ASSERT_GT(result, test_module->global_data_size);
}