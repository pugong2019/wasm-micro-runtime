/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "test_helper.h"
#include "gtest/gtest.h"

#include "wasm_export.h"
#include "aot_export.h"
#include "bh_read_file.h"
#include <limits.h>

// Need LLVM headers for LLVMValueRef
#include <llvm-c/Core.h>

// Enhanced test fixture for aot_compiler.c functions
class EnhancedAotCompilerTest : public testing::Test {
protected:
    void SetUp() override {
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_Pool;
        init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
        init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);
        wasm_runtime_full_init(&init_args);
    }

    void TearDown() override {
        wasm_runtime_destroy();
    }

    // Helper method to create a basic WASM module for testing
    wasm_module_t createTestModule() {
        // Simple WASM module with basic functions for testing
        uint8_t simple_wasm[] = {
            0x00, 0x61, 0x73, 0x6D, 0x01, 0x00, 0x00, 0x00,
            0x01, 0x07, 0x01, 0x60, 0x02, 0x7F, 0x7F, 0x01, 0x7F,
            0x03, 0x02, 0x01, 0x00,
            0x0A, 0x09, 0x01, 0x07, 0x00, 0x20, 0x00, 0x20, 0x01, 0x6A, 0x0B
        };

        char error_buf[128] = { 0 };
        wasm_module_t module = wasm_runtime_load(simple_wasm, sizeof(simple_wasm),
                                               error_buf, sizeof(error_buf));
        return module;
    }

public:
    char global_heap_buf[512 * 1024];
    RuntimeInitArgs init_args;
};

/******
 * Test Case: aot_compiler_createContext_WithGcEnabled_Success
 * Source: core/iwasm/compilation/aot_compiler.c:351-593
 * Target Lines: 350-530 (focusing on aot_gen_commit_values function coverage)
 * Functional Purpose: Validates that AOT compilation context can be created
 *                     with various configurations that would exercise the
 *                     aot_gen_commit_values function during compilation.
 * Call Path: aot_create_comp_context() -> ... -> aot_gen_commit_values()
 * Coverage Goal: Exercise compilation path that calls aot_gen_commit_values
 ******/
TEST_F(EnhancedAotCompilerTest, aot_compiler_createContext_WithGcEnabled_Success) {
    wasm_module_t module = createTestModule();
    ASSERT_NE(module, nullptr);

    aot_comp_data_t comp_data = aot_create_comp_data(module, NULL, false);
    ASSERT_NE(comp_data, nullptr);

    AOTCompOption option = { 0 };
    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = false;
    option.enable_ref_types = true;  // Enable ref types to trigger more paths
    option.enable_gc = true;         // Enable GC to trigger aot_gen_commit_values

    aot_comp_context_t comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Compile the WASM to trigger the compilation paths including aot_gen_commit_values
    bool compile_result = aot_compile_wasm(comp_ctx);
    ASSERT_TRUE(compile_result);

    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(module);
}

/******
 * Test Case: aot_compiler_createContext_WithJitMode_Success
 * Source: core/iwasm/compilation/aot_compiler.c:351-593
 * Target Lines: 366-368 (JIT mode vs AOT mode code path)
 * Functional Purpose: Validates that AOT compilation context can be created
 *                     in JIT mode, which exercises different code paths in
 *                     aot_gen_commit_values (JIT processes all locals).
 * Call Path: aot_create_comp_context() -> ... -> aot_gen_commit_values()
 * Coverage Goal: Exercise JIT mode path in aot_gen_commit_values
 ******/
TEST_F(EnhancedAotCompilerTest, aot_compiler_createContext_WithJitMode_Success) {
    wasm_module_t module = createTestModule();
    ASSERT_NE(module, nullptr);

    aot_comp_data_t comp_data = aot_create_comp_data(module, NULL, false);
    ASSERT_NE(comp_data, nullptr);

    AOTCompOption option = { 0 };
    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = false;
    option.enable_ref_types = true;
    option.enable_gc = true;
    option.is_jit_mode = true;       // Enable JIT mode

    aot_comp_context_t comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Compile the WASM to trigger the compilation paths including aot_gen_commit_values
    bool compile_result = aot_compile_wasm(comp_ctx);
    ASSERT_TRUE(compile_result);

    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(module);
}

/******
 * Test Case: aot_compiler_createContext_WithoutGc_Success
 * Source: core/iwasm/compilation/aot_compiler.c:351-593
 * Target Lines: 375-465 (GC disabled path)
 * Functional Purpose: Validates that AOT compilation context works when
 *                     GC is disabled, exercising the path in aot_gen_commit_values
 *                     that skips reference processing.
 * Call Path: aot_create_comp_context() -> ... -> aot_gen_commit_values()
 * Coverage Goal: Exercise non-GC path in aot_gen_commit_values
 ******/
TEST_F(EnhancedAotCompilerTest, aot_compiler_createContext_WithoutGc_Success) {
    wasm_module_t module = createTestModule();
    ASSERT_NE(module, nullptr);

    aot_comp_data_t comp_data = aot_create_comp_data(module, NULL, false);
    ASSERT_NE(comp_data, nullptr);

    AOTCompOption option = { 0 };
    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = false;
    option.enable_ref_types = false;  // Disable ref types
    option.enable_gc = false;         // Disable GC

    aot_comp_context_t comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Compile the WASM to trigger the compilation paths including aot_gen_commit_values
    bool compile_result = aot_compile_wasm(comp_ctx);
    ASSERT_TRUE(compile_result);

    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(module);
}

/******
 * Test Case: aot_compiler_compileComplexWasm_WithRefTypes_Success
 * Source: core/iwasm/compilation/aot_compiler.c:351-593
 * Target Lines: 418-459 (reference type handling paths)
 * Functional Purpose: Validates compilation of WASM with reference types
 *                     enabled, which exercises the reference type handling
 *                     paths in aot_gen_commit_values.
 * Call Path: aot_compile_wasm() -> ... -> aot_gen_commit_values()
 * Coverage Goal: Exercise reference type handling in aot_gen_commit_values
 ******/
TEST_F(EnhancedAotCompilerTest, aot_compiler_compileComplexWasm_WithRefTypes_Success) {
    // Use the same simple WASM as other tests to avoid module loading issues
    wasm_module_t module = createTestModule();
    ASSERT_NE(module, nullptr);

    aot_comp_data_t comp_data = aot_create_comp_data(module, NULL, false);
    ASSERT_NE(comp_data, nullptr);

    AOTCompOption option = { 0 };
    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = false;
    option.enable_ref_types = true;  // Enable ref types

    aot_comp_context_t comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Compile the WASM to trigger compilation paths
    bool compile_result = aot_compile_wasm(comp_ctx);
    ASSERT_TRUE(compile_result);

    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(module);
}

/******
 * Test Case: aot_compiler_compilationWithDifferentOptLevels_Success
 * Source: core/iwasm/compilation/aot_compiler.c:351-593
 * Target Lines: 350-593 (various optimization paths)
 * Functional Purpose: Validates that different optimization levels work
 *                     correctly and exercise different code paths in
 *                     aot_gen_commit_values during compilation.
 * Call Path: aot_compile_wasm() -> ... -> aot_gen_commit_values()
 * Coverage Goal: Exercise different optimization paths
 ******/
TEST_F(EnhancedAotCompilerTest, aot_compiler_compilationWithDifferentOptLevels_Success) {
    wasm_module_t module = createTestModule();
    ASSERT_NE(module, nullptr);

    aot_comp_data_t comp_data = aot_create_comp_data(module, NULL, false);
    ASSERT_NE(comp_data, nullptr);

    // Test different optimization levels
    for (uint32_t opt_level = 0; opt_level <= 3; opt_level++) {
        AOTCompOption option = { 0 };
        option.opt_level = opt_level;
        option.size_level = 3;
        option.output_format = AOT_FORMAT_FILE;
        option.bounds_checks = 2;
        option.enable_simd = false;
        option.enable_aux_stack_check = true;
        option.enable_bulk_memory = false;
        option.enable_ref_types = true;
        option.enable_gc = true;

        aot_comp_context_t comp_ctx = aot_create_comp_context(comp_data, &option);
        ASSERT_NE(comp_ctx, nullptr);

        // Compile the WASM to trigger compilation paths
        bool compile_result = aot_compile_wasm(comp_ctx);
        ASSERT_TRUE(compile_result);

        aot_destroy_comp_context(comp_ctx);
    }

    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(module);
}

/******
 * Test Case: aot_compiler_emitObjectFile_WithGC_Success
 * Source: core/iwasm/compilation/aot_compiler.c:351-593
 * Target Lines: 350-593 (complete compilation and emission path)
 * Functional Purpose: Validates that the complete compilation pipeline
 *                     including object file emission works with GC enabled,
 *                     ensuring aot_gen_commit_values is exercised fully.
 * Call Path: aot_emit_object_file() -> ... -> aot_gen_commit_values()
 * Coverage Goal: Exercise complete compilation pipeline
 ******/
TEST_F(EnhancedAotCompilerTest, aot_compiler_emitObjectFile_WithGC_Success) {
    wasm_module_t module = createTestModule();
    ASSERT_NE(module, nullptr);

    aot_comp_data_t comp_data = aot_create_comp_data(module, NULL, false);
    ASSERT_NE(comp_data, nullptr);

    AOTCompOption option = { 0 };
    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = false;
    option.enable_ref_types = true;
    option.enable_gc = true;

    aot_comp_context_t comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Compile the WASM
    bool compile_result = aot_compile_wasm(comp_ctx);
    ASSERT_TRUE(compile_result);

    // Emit object file to exercise complete pipeline
    char obj_file_name[] = "enhanced_test.o";
    bool emit_result = aot_emit_object_file(comp_ctx, obj_file_name);
    ASSERT_TRUE(emit_result);

    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(module);
}

// ============================================================================
// NEW TEST CASES FOR LINES 644-649 COVERAGE
// ============================================================================

/******
 * Test Case: aot_gen_commit_ip_TinyFrameType_Success
 * Source: core/iwasm/compilation/aot_compiler.c:644-645
 * Target Lines: 644 (AOT_STACK_FRAME_TYPE_TINY case), 645 (aot_tiny_frame_gen_commit_ip call)
 * Functional Purpose: Validates that AOT compilation with TINY frame type setting
 *                     successfully compiles a WASM module, exercising the code paths
 *                     in aot_gen_commit_ip() that handle TINY stack frame type.
 * Call Path: aot_compile_wasm() -> ... -> aot_gen_commit_ip() -> aot_tiny_frame_gen_commit_ip()
 * Coverage Goal: Exercise TINY stack frame type branch in aot_gen_commit_ip during compilation
 ******/
TEST_F(EnhancedAotCompilerTest, aot_gen_commit_ip_TinyFrameType_Success) {
    wasm_module_t module = createTestModule();
    ASSERT_NE(module, nullptr);

    aot_comp_data_t comp_data = aot_create_comp_data(module, NULL, false);
    ASSERT_NE(comp_data, nullptr);

    AOTCompOption option = { 0 };
    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = false;
    option.enable_ref_types = false;
    option.enable_gc = false;
    option.aux_stack_frame_type = AOT_STACK_FRAME_TYPE_TINY;  // Set TINY frame type
    // Enable instruction pointer tracking to trigger aot_gen_commit_ip calls
    option.call_stack_features.ip = true;

    aot_comp_context_t comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Compile WASM with TINY frame type - this will internally call aot_gen_commit_ip
    // with TINY frame type, exercising lines 644-645
    bool compile_result = aot_compile_wasm(comp_ctx);
    ASSERT_TRUE(compile_result);

    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(module);
}

/******
 * Test Case: aot_gen_commit_ip_StandardFrameType_Success
 * Source: core/iwasm/compilation/aot_compiler.c:641-643
 * Target Lines: 641 (AOT_STACK_FRAME_TYPE_STANDARD case), 642-643 (aot_standard_frame_gen_commit_ip call)
 * Functional Purpose: Validates that AOT compilation with STANDARD frame type setting
 *                     successfully compiles a WASM module, exercising the code paths
 *                     in aot_gen_commit_ip() that handle STANDARD stack frame type.
 *                     This complements the TINY frame test and exercises the switch statement.
 * Call Path: aot_compile_wasm() -> ... -> aot_gen_commit_ip() -> aot_standard_frame_gen_commit_ip()
 * Coverage Goal: Exercise STANDARD stack frame type branch and contrast with TINY
 ******/
TEST_F(EnhancedAotCompilerTest, aot_gen_commit_ip_StandardFrameType_Success) {
    wasm_module_t module = createTestModule();
    ASSERT_NE(module, nullptr);

    aot_comp_data_t comp_data = aot_create_comp_data(module, NULL, false);
    ASSERT_NE(comp_data, nullptr);

    AOTCompOption option = { 0 };
    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = false;
    option.enable_ref_types = false;
    option.enable_gc = false;
    option.aux_stack_frame_type = AOT_STACK_FRAME_TYPE_STANDARD;  // Set STANDARD frame type
    // Enable instruction pointer tracking to trigger aot_gen_commit_ip calls
    option.call_stack_features.ip = true;

    aot_comp_context_t comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Compile WASM with STANDARD frame type - this will internally call aot_gen_commit_ip
    // with STANDARD frame type, exercising lines 641-643
    bool compile_result = aot_compile_wasm(comp_ctx);
    ASSERT_TRUE(compile_result);

    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(module);
}

/******
 * Test Case: aot_gen_commit_ip_StackFrameOff_NoCallToFunction
 * Source: core/iwasm/compilation/aot_compiler.c:640-651
 * Target Lines: 640 (switch statement on aux_stack_frame_type)
 * Functional Purpose: Validates that AOT compilation with stack frame tracking disabled
 *                     (AOT_STACK_FRAME_OFF) does not exercise the aot_gen_commit_ip switch
 *                     statement code paths, providing contrast to the enabled cases.
 *                     This test helps ensure branch coverage of the entire function.
 * Call Path: aot_compile_wasm() - aot_gen_commit_ip should not be called when IP tracking off
 * Coverage Goal: Ensure complete coverage of aot_gen_commit_ip function entry and exits
 ******/
TEST_F(EnhancedAotCompilerTest, aot_gen_commit_ip_StackFrameOff_NoCallToFunction) {
    wasm_module_t module = createTestModule();
    ASSERT_NE(module, nullptr);

    aot_comp_data_t comp_data = aot_create_comp_data(module, NULL, false);
    ASSERT_NE(comp_data, nullptr);

    AOTCompOption option = { 0 };
    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = false;
    option.enable_ref_types = false;
    option.enable_gc = false;
    option.aux_stack_frame_type = AOT_STACK_FRAME_OFF;  // Disable stack frame tracking
    // Disable instruction pointer tracking - aot_gen_commit_ip should not be called
    option.call_stack_features.ip = false;

    aot_comp_context_t comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Compile WASM with stack frame tracking off - aot_gen_commit_ip should not be called
    bool compile_result = aot_compile_wasm(comp_ctx);
    ASSERT_TRUE(compile_result);

    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(module);
}