/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "../common/test_helper.h"
#include "gtest/gtest.h"

#include "bh_read_file.h"
#include "aot_emit_const.h"
#include "aot_compiler.h"

static std::string CWD;
static std::string MAIN_WASM = "/main.wasm";
static std::string F32_CONST_WASM = "/f32_const_test.wasm";
static char *WASM_FILE;
static char *F32_CONST_WASM_FILE;

static std::string
get_binary_path()
{
    char cwd[1024];
    memset(cwd, 0, 1024);

    if (readlink("/proc/self/exe", cwd, 1024) <= 0) {
    }

    char *path_end = strrchr(cwd, '/');
    if (path_end != NULL) {
        *path_end = '\0';
    }

    return std::string(cwd);
}

// MANDATORY: Enhanced test fixture following existing patterns
// Use source file name in fixture class (EnhancedAotEmitConstTest)
class EnhancedAotEmitConstTest : public testing::Test {
protected:
    void SetUp() override {
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_Pool;
        init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
        init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);

        bool ret = wasm_runtime_full_init(&init_args);
        ASSERT_TRUE(ret);
    }

    void TearDown() override {
        wasm_runtime_destroy();
    }

    static void SetUpTestCase()
    {
        CWD = get_binary_path();
        WASM_FILE = strdup((CWD + MAIN_WASM).c_str());
        F32_CONST_WASM_FILE = strdup((CWD + F32_CONST_WASM).c_str());
    }

    static void TearDownTestCase() {
        free(WASM_FILE);
        free(F32_CONST_WASM_FILE);
    }

public:
    char global_heap_buf[512 * 1024];
    RuntimeInitArgs init_args;
};

/******
 * Test Case: aot_compile_op_i64_const_CoverageTest_Success
 * Source: core/iwasm/compilation/aot_emit_const.c:38-62
 * Target Lines: Exercise i64 constant compilation through normal AOT compilation flow
 * Functional Purpose: Validates i64 constant compilation through the standard WAMR AOT flow.
 *                     This achieves coverage by compiling WASM files that contain i64 constants.
 * Call Path: aot_compile_op_i64_const() <- aot_compiler.c:2105 (during aot_compile_wasm)
 * Coverage Goal: Exercise i64 constant compilation paths through natural compilation flow
 ******/
TEST_F(EnhancedAotEmitConstTest, aot_compile_op_i64_const_CoverageTest_Success)
{
    const char *wasm_file = WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;

    struct AOTCompData *comp_data = nullptr;
    struct AOTCompContext *comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = true;
    option.enable_ref_types = true;

    wasm_file_buf = (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    ASSERT_NE(nullptr, wasm_file_buf);

    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, wasm_module);

    comp_data = aot_create_comp_data((WASMModule *)wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);

    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(nullptr, comp_ctx);

    // The main test - compile the WASM module which should exercise i64 constant compilation
    // This will internally call aot_compile_op_i64_const for any i64 constants in the WASM
    bool compile_result = aot_compile_wasm(comp_ctx);
    ASSERT_TRUE(compile_result);

    // Verify the compilation was successful and context is valid
    ASSERT_NE(nullptr, comp_ctx->func_ctxes);
    ASSERT_GT(comp_data->func_count, 0);

    // Cleanup
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
    if (wasm_file_buf)
        BH_FREE(wasm_file_buf);
}

/******
 * Test Case: aot_compile_op_f32_const_IndirectMode_Success
 * Source: core/iwasm/compilation/aot_emit_const.c:66-113
 * Target Lines: 71-80 (indirect mode with intrinsic capability)
 * Functional Purpose: Validates f32 constant compilation in indirect mode when intrinsic
 *                     capability is available, exercising the aot_load_const_from_table path.
 * Call Path: aot_compile_op_f32_const() <- aot_compiler.c:2113 (during WASM_OP_F32_CONST)
 * Coverage Goal: Exercise indirect mode constant loading path for f32 constants
 ******/
TEST_F(EnhancedAotEmitConstTest, aot_compile_op_f32_const_IndirectMode_Success)
{
    const char *wasm_file = F32_CONST_WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;

    struct AOTCompData *comp_data = nullptr;
    struct AOTCompContext *comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    // Enable indirect mode to trigger lines 71-80
    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = true;
    option.enable_ref_types = true;
    option.is_indirect_mode = true;  // Key: enable indirect mode

    wasm_file_buf = (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    ASSERT_NE(nullptr, wasm_file_buf);

    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, wasm_module);

    comp_data = aot_create_comp_data((WASMModule *)wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);

    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(nullptr, comp_ctx);

    // Verify indirect mode is enabled
    ASSERT_TRUE(comp_ctx->is_indirect_mode);

    // Compile the WASM module which should exercise f32 constant compilation
    // This will internally call aot_compile_op_f32_const for f32 constants in indirect mode
    bool compile_result = aot_compile_wasm(comp_ctx);
    ASSERT_TRUE(compile_result);

    // Verify the compilation was successful and context is valid
    ASSERT_NE(nullptr, comp_ctx->func_ctxes);
    ASSERT_GT(comp_data->func_count, 0);

    // Cleanup
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
    if (wasm_file_buf)
        BH_FREE(wasm_file_buf);
}

/******
 * Test Case: aot_compile_op_f32_const_NormalFloat_Success
 * Source: core/iwasm/compilation/aot_emit_const.c:66-113
 * Target Lines: 82-85 (normal float constant, non-NaN path)
 * Functional Purpose: Validates f32 constant compilation for normal float values that are
 *                     not NaN, exercising the F32_CONST and CHECK_LLVM_CONST code path.
 * Call Path: aot_compile_op_f32_const() <- aot_compiler.c:2113 (during WASM_OP_F32_CONST)
 * Coverage Goal: Exercise normal float constant compilation path (non-NaN, non-indirect)
 ******/
TEST_F(EnhancedAotEmitConstTest, aot_compile_op_f32_const_NormalFloat_Success)
{
    const char *wasm_file = F32_CONST_WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;

    struct AOTCompData *comp_data = nullptr;
    struct AOTCompContext *comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    // Use standard mode (not indirect) to trigger lines 82-85
    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = true;
    option.enable_ref_types = true;
    option.is_indirect_mode = false;  // Key: disable indirect mode

    wasm_file_buf = (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    ASSERT_NE(nullptr, wasm_file_buf);

    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, wasm_module);

    comp_data = aot_create_comp_data((WASMModule *)wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);

    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(nullptr, comp_ctx);

    // Verify indirect mode is disabled
    ASSERT_FALSE(comp_ctx->is_indirect_mode);

    // Compile the WASM module which should exercise f32 constant compilation
    // This will internally call aot_compile_op_f32_const for normal f32 constants
    bool compile_result = aot_compile_wasm(comp_ctx);
    ASSERT_TRUE(compile_result);

    // Verify the compilation was successful and context is valid
    ASSERT_NE(nullptr, comp_ctx->func_ctxes);
    ASSERT_GT(comp_data->func_count, 0);

    // Cleanup
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
    if (wasm_file_buf)
        BH_FREE(wasm_file_buf);
}
/******
 * Test Case: aot_compile_op_f32_const_NaNHandling_Success
 * Source: core/iwasm/compilation/aot_emit_const.c:66-113
 * Target Lines: 87-111 (NaN float constant handling with complex memory operations)
 * Functional Purpose: Validates f32 NaN constant compilation which requires complex LLVM
 *                     memory operations including LLVMBuildAlloca, LLVMBuildStore,
 *                     LLVMBuildBitCast, and LLVMBuildLoad2 for proper NaN representation.
 * Call Path: aot_compile_op_f32_const() <- aot_compiler.c:2113 (during WASM_OP_F32_CONST with NaN)
 * Coverage Goal: Exercise NaN handling path with complex memory operations (lines 87-111)
 ******/
TEST_F(EnhancedAotEmitConstTest, aot_compile_op_f32_const_NaNHandling_Success)
{
    const char *wasm_file = F32_CONST_WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;

    struct AOTCompData *comp_data = nullptr;
    struct AOTCompContext *comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    // Use standard mode (not indirect) to ensure NaN path is triggered
    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = true;
    option.enable_ref_types = true;
    option.is_indirect_mode = false;  // Key: disable indirect mode for NaN path

    wasm_file_buf = (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    ASSERT_NE(nullptr, wasm_file_buf);

    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, wasm_module);

    comp_data = aot_create_comp_data((WASMModule *)wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);

    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(nullptr, comp_ctx);

    // Verify indirect mode is disabled
    ASSERT_FALSE(comp_ctx->is_indirect_mode);

    // Compile the WASM module containing NaN constants which should exercise
    // the complex memory handling path in aot_compile_op_f32_const (lines 87-111)
    bool compile_result = aot_compile_wasm(comp_ctx);
    ASSERT_TRUE(compile_result);

    // Verify the compilation was successful and context is valid
    ASSERT_NE(nullptr, comp_ctx->func_ctxes);
    ASSERT_GT(comp_data->func_count, 0);

    // Additional validation that LLVM builder context is properly set up
    // since NaN handling requires complex LLVM memory operations
    ASSERT_NE(nullptr, comp_ctx->builder);
    ASSERT_NE(nullptr, comp_ctx->context);

    // Cleanup
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
    if (wasm_file_buf)
        BH_FREE(wasm_file_buf);
}
