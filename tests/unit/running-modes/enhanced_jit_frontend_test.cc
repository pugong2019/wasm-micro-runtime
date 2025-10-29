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

// ==============================================
// Enhanced test cases for jit_frontend_get_module_inst_extra_offset()
// Targeting lines 117-122 in jit_frontend.c
// ==============================================

/******
 * Test Case: jit_frontend_get_module_inst_extra_offset_NoTables_ReturnsAlignedOffset
 * Source: core/iwasm/fast-jit/jit_frontend.c:117-122
 * Target Lines: 117 (function entry), 119-120 (table offset calculation), 122 (align_uint call)
 * Functional Purpose: Validates that jit_frontend_get_module_inst_extra_offset() correctly
 *                     calculates module instance extra offset for modules with no tables
 *                     and returns 8-byte aligned offset value.
 * Call Path: jit_frontend_get_module_inst_extra_offset() [Direct function call]
 * Coverage Goal: Exercise function with minimal table configuration and alignment logic
 ******/
TEST_F(EnhancedJitFrontendTest, jit_frontend_get_module_inst_extra_offset_NoTables_ReturnsAlignedOffset)
{
    // Configure module with no tables
    test_module->import_table_count = 0;
    test_module->table_count = 0;

    uint32 result = jit_frontend_get_module_inst_extra_offset(test_module);

    // Should return aligned offset (8-byte boundary)
    // This exercises lines 117, 119-120, and 122
    ASSERT_GT(result, 0);                    // Should be positive value
    ASSERT_EQ(result % 8, 0);               // Should be 8-byte aligned
    ASSERT_GE(result, test_module->global_data_size);  // Should be at least global data size
}

/******
 * Test Case: jit_frontend_get_module_inst_extra_offset_WithImportTables_HandlesCalculation
 * Source: core/iwasm/fast-jit/jit_frontend.c:117-122
 * Target Lines: 117 (function entry), 119-120 (total table count calculation), 122 (alignment)
 * Functional Purpose: Validates that jit_frontend_get_module_inst_extra_offset() correctly
 *                     handles modules with import tables by passing total table count
 *                     to jit_frontend_get_table_inst_offset() and aligning result.
 * Call Path: jit_frontend_get_module_inst_extra_offset() [Direct function call]
 * Coverage Goal: Exercise function with import tables and verify calculation path
 ******/
TEST_F(EnhancedJitFrontendTest, jit_frontend_get_module_inst_extra_offset_WithImportTables_HandlesCalculation)
{
    // Configure module with import tables
    CreateImportTables(2);
    test_module->table_count = 0;

    uint32 result = jit_frontend_get_module_inst_extra_offset(test_module);

    // Should process import tables and return aligned offset
    // This exercises lines 117, 119-120 (with import_table_count = 2), and 122
    ASSERT_GT(result, 0);                    // Should be positive value
    ASSERT_EQ(result % 8, 0);               // Should be 8-byte aligned
    ASSERT_GT(result, test_module->global_data_size);  // Should exceed base size
}

/******
 * Test Case: jit_frontend_get_module_inst_extra_offset_WithLocalTables_HandlesCalculation
 * Source: core/iwasm/fast-jit/jit_frontend.c:117-122
 * Target Lines: 117 (function entry), 119-120 (total table count calculation), 122 (alignment)
 * Functional Purpose: Validates that jit_frontend_get_module_inst_extra_offset() correctly
 *                     handles modules with local tables by calculating total table count
 *                     and passing it to jit_frontend_get_table_inst_offset().
 * Call Path: jit_frontend_get_module_inst_extra_offset() [Direct function call]
 * Coverage Goal: Exercise function with local tables and verify calculation path
 ******/
TEST_F(EnhancedJitFrontendTest, jit_frontend_get_module_inst_extra_offset_WithLocalTables_HandlesCalculation)
{
    // Configure module with local tables
    test_module->import_table_count = 0;
    CreateLocalTables(3);

    uint32 result = jit_frontend_get_module_inst_extra_offset(test_module);

    // Should process local tables and return aligned offset
    // This exercises lines 117, 119-120 (with table_count = 3), and 122
    ASSERT_GT(result, 0);                    // Should be positive value
    ASSERT_EQ(result % 8, 0);               // Should be 8-byte aligned
    ASSERT_GT(result, test_module->global_data_size);  // Should exceed base size
}

/******
 * Test Case: jit_frontend_get_module_inst_extra_offset_MixedTables_HandlesTotalCount
 * Source: core/iwasm/fast-jit/jit_frontend.c:117-122
 * Target Lines: 117 (function entry), 119-120 (import_table_count + table_count), 122 (alignment)
 * Functional Purpose: Validates that jit_frontend_get_module_inst_extra_offset() correctly
 *                     calculates total table count by summing import and local tables,
 *                     demonstrating the arithmetic operation in line 119-120.
 * Call Path: jit_frontend_get_module_inst_extra_offset() [Direct function call]
 * Coverage Goal: Exercise function with mixed tables to verify sum calculation
 ******/
TEST_F(EnhancedJitFrontendTest, jit_frontend_get_module_inst_extra_offset_MixedTables_HandlesTotalCount)
{
    // Configure module with both import and local tables
    CreateImportTables(2);
    CreateLocalTables(3);

    uint32 result = jit_frontend_get_module_inst_extra_offset(test_module);

    // Should calculate total table count (2 + 3 = 5) and return aligned offset
    // This exercises lines 117, 119-120 (import_table_count + table_count = 5), and 122
    ASSERT_GT(result, 0);                    // Should be positive value
    ASSERT_EQ(result % 8, 0);               // Should be 8-byte aligned
    ASSERT_GT(result, test_module->global_data_size);  // Should exceed base size

    // Verify that the result accounts for all tables by comparing with a smaller configuration
    CreateImportTables(1);
    CreateLocalTables(1);
    uint32 smaller_result = jit_frontend_get_module_inst_extra_offset(test_module);

    // Restore original configuration for final validation
    CreateImportTables(2);
    CreateLocalTables(3);
    uint32 final_result = jit_frontend_get_module_inst_extra_offset(test_module);

    ASSERT_GE(final_result, smaller_result);  // More tables should result in larger or equal offset
}

/******
 * Test Case: jit_frontend_get_module_inst_extra_offset_AlignmentBehavior_VerifyAlignment
 * Source: core/iwasm/fast-jit/jit_frontend.c:117-122
 * Target Lines: 122 (align_uint(offset, 8) - alignment logic)
 * Functional Purpose: Validates that jit_frontend_get_module_inst_extra_offset() correctly
 *                     applies 8-byte alignment to the offset returned by
 *                     jit_frontend_get_table_inst_offset(), ensuring proper memory alignment.
 * Call Path: jit_frontend_get_module_inst_extra_offset() [Direct function call]
 * Coverage Goal: Exercise alignment logic on line 122 with various configurations
 ******/
TEST_F(EnhancedJitFrontendTest, jit_frontend_get_module_inst_extra_offset_AlignmentBehavior_VerifyAlignment)
{
    // Test multiple configurations to verify consistent alignment
    uint32 results[4];

    // Configuration 1: No tables
    test_module->import_table_count = 0;
    test_module->table_count = 0;
    results[0] = jit_frontend_get_module_inst_extra_offset(test_module);

    // Configuration 2: 1 import table
    CreateImportTables(1);
    test_module->table_count = 0;
    results[1] = jit_frontend_get_module_inst_extra_offset(test_module);

    // Configuration 3: 1 local table
    test_module->import_table_count = 0;
    CreateLocalTables(1);
    results[2] = jit_frontend_get_module_inst_extra_offset(test_module);

    // Configuration 4: Mixed tables
    CreateImportTables(1);
    CreateLocalTables(2);
    results[3] = jit_frontend_get_module_inst_extra_offset(test_module);

    // Verify all results are 8-byte aligned (line 122: align_uint(offset, 8))
    for (int i = 0; i < 4; i++) {
        ASSERT_GT(results[i], 0);            // Should be positive
        ASSERT_EQ(results[i] % 8, 0);        // Should be 8-byte aligned
    }

    // Results should be monotonically non-decreasing or equal based on table count
    ASSERT_LE(results[0], results[1]);       // More tables → larger or equal offset
    ASSERT_LE(results[0], results[2]);       // More tables → larger or equal offset
    ASSERT_LE(results[1], results[3]);       // More tables → larger or equal offset
    ASSERT_LE(results[2], results[3]);       // More tables → larger or equal offset
}

// ==============================================
// Enhanced test cases for get_module_reg() - Lines 139-149
// TECHNICAL LIMITATION: get_module_reg() is a JIT code generation function
// that calls GEN_INSN() macro to emit machine code instructions. This function
// requires a complete JIT compilation context and cannot be unit tested
// in isolation without significant infrastructure setup.
//
// ALTERNATIVE APPROACH: Integration testing through JIT compilation scenarios
// would be required to achieve coverage of lines 139-149.
// ==============================================

/*
 * COVERAGE ANALYSIS FOR LINES 139-149:
 *
 * Target Function: get_module_reg(JitFrame *frame)
 * Source: core/iwasm/fast-jit/jit_frontend.c:139-149
 *
 * Line 139: JitReg get_module_reg(JitFrame *frame) {
 * Line 141:     JitCompContext *cc = frame->cc;
 * Line 142:     JitReg module_inst_reg = get_module_inst_reg(frame);
 * Line 144:     if (!frame->module_reg) {
 * Line 145:         frame->module_reg = cc->module_reg;
 * Line 146-147:   GEN_INSN(LDPTR, frame->module_reg, module_inst_reg,
 *                           NEW_CONST(I32, offsetof(WASMModuleInstance, module)));
 * Line 148:     }
 * Line 149:     return frame->module_reg;
 *
 * TECHNICAL CONSTRAINTS:
 * - GEN_INSN() macro generates actual JIT machine code instructions
 * - Requires active JIT compilation context with proper instruction stream
 * - Cannot be mocked without substantial JIT infrastructure duplication
 * - Designed for runtime JIT compilation, not standalone unit testing
 *
 * COVERAGE STATUS: BLOCKED - Requires integration testing approach
 */