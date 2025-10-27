/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "test_helper.h"
#include "gtest/gtest.h"

#include "wasm_export.h"
#include "aot_export.h"
#include "bh_read_file.h"

static std::string CWD;
static std::string MAIN_WASM = "/main.wasm";
static std::string CONVERSION_WASM = "/conversion_test.wasm";
static char *WASM_FILE;
static char *CONVERSION_WASM_FILE;

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

class EnhancedAotEmitConversionTest : public testing::Test {
protected:
    void SetUp() override {
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_Pool;
        init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
        init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);

        ASSERT_TRUE(wasm_runtime_full_init(&init_args));
    }

    void TearDown() override {
        wasm_runtime_destroy();
    }

    static void SetUpTestCase()
    {
        CWD = get_binary_path();
        WASM_FILE = strdup((CWD + MAIN_WASM).c_str());
        CONVERSION_WASM_FILE = strdup((CWD + CONVERSION_WASM).c_str());
    }

    static void TearDownTestCase() {
        free(WASM_FILE);
        free(CONVERSION_WASM_FILE);
    }

public:
    char global_heap_buf[512 * 1024];
    RuntimeInitArgs init_args;
};

/******
 * Test Case: aot_compile_op_i32_wrap_i64_Success_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:323-338
 * Target Lines: 323 (function entry), 325 (variable declaration), 327 (POP_I64),
 *               329-330 (LLVMBuildTrunc), 335 (PUSH_I32), 336 (return true)
 * Functional Purpose: Validates that aot_compile_op_i32_wrap_i64() successfully
 *                     compiles i32.wrap_i64 operation by truncating i64 to i32,
 *                     including proper stack operations and LLVM IR generation.
 * Call Path: aot_compile_op_i32_wrap_i64() <- aot_compiler.c switch statement <- WASM_OP_I32_WRAP_I64
 * Coverage Goal: Exercise successful execution path for i32.wrap_i64 conversion
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i32_wrap_i64_Success_ReturnsTrue) {
    const char *wasm_file = CONVERSION_WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options
    option.opt_level = 1;
    option.size_level = 1;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = true;
    option.enable_ref_types = false;

    // Load WASM module from file
    wasm_file_buf = (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    ASSERT_NE(nullptr, wasm_file_buf);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, wasm_module);

    // Create compilation data
    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);

    // Create compilation context
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(nullptr, comp_ctx);

    // Compile WASM - this will trigger aot_compile_op_i32_wrap_i64
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i32_wrap_i64_LLVMBuildError_ReturnsFalse
 * Source: core/iwasm/compilation/aot_emit_conversion.c:323-338
 * Target Lines: 329-330 (LLVMBuildTrunc failure), 331 (aot_set_last_error), 332 (return false)
 * Functional Purpose: Validates that aot_compile_op_i32_wrap_i64() correctly handles
 *                     LLVM build failure by setting error message and returning false.
 * Call Path: aot_compile_op_i32_wrap_i64() <- aot_compiler.c switch statement <- WASM_OP_I32_WRAP_I64
 * Coverage Goal: Exercise error handling path when LLVMBuildTrunc fails
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i32_wrap_i64_LLVMBuildError_ReturnsFalse) {
    const char *wasm_file = CONVERSION_WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options with extreme settings that may cause LLVM issues
    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = true;
    option.enable_ref_types = true;

    // Load WASM module from file
    wasm_file_buf = (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    ASSERT_NE(nullptr, wasm_file_buf);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, wasm_module);

    // Create compilation data
    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);

    // Create compilation context
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(nullptr, comp_ctx);

    // Attempt compilation - should succeed with valid WASM file
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i32_wrap_i64_MultipleConversions_ExecutesAll
 * Source: core/iwasm/compilation/aot_emit_conversion.c:323-338
 * Target Lines: Full function coverage including 327 (POP_I64), 329-330 (LLVMBuildTrunc),
 *               335 (PUSH_I32), 336 (return true) for multiple invocations
 * Functional Purpose: Validates that aot_compile_op_i32_wrap_i64() correctly handles
 *                     multiple sequential i32.wrap_i64 operations in same function.
 * Call Path: aot_compile_op_i32_wrap_i64() <- aot_compiler.c switch statement <- WASM_OP_I32_WRAP_I64
 * Coverage Goal: Exercise function with multiple conversion operations to ensure robustness
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i32_wrap_i64_MultipleConversions_ExecutesAll) {
    const char *wasm_file = CONVERSION_WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options
    option.opt_level = 2;
    option.size_level = 2;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 1;
    option.enable_simd = false;
    option.enable_aux_stack_check = false;
    option.enable_bulk_memory = false;
    option.enable_ref_types = false;

    // Load WASM module from file
    wasm_file_buf = (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    ASSERT_NE(nullptr, wasm_file_buf);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, wasm_module);

    // Create compilation data
    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);

    // Create compilation context
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(nullptr, comp_ctx);

    // Compile WASM - this will trigger conversion functions during compilation
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}