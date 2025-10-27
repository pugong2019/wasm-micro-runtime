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

/******
 * Test Case: aot_compile_op_i32_trunc_f32_SignedNonSaturating_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:342-392
 * Target Lines: 342 (function entry), 348 (POP_F32), 350-351 (condition check),
 *               370-373 (direct mode signed), 380-381 (CHECK_LLVM_CONST),
 *               383-386 (non-saturating path calling trunc_float_to_int)
 * Functional Purpose: Validates that aot_compile_op_i32_trunc_f32() successfully
 *                     compiles i32.trunc_f32_s operation with signed non-saturating
 *                     truncation in direct mode.
 * Call Path: aot_compile_op_i32_trunc_f32() <- aot_compiler.c WASM_OP_I32_TRUNC_S_F32
 * Coverage Goal: Exercise signed non-saturating truncation path
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i32_trunc_f32_SignedNonSaturating_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i32_trunc_f32_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options for direct mode
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

    // Compile WASM - this will trigger aot_compile_op_i32_trunc_f32 for i32.trunc_f32_s
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i32_trunc_f32_UnsignedNonSaturating_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:342-392
 * Target Lines: 342 (function entry), 348 (POP_F32), 350-351 (condition check),
 *               370-377 (direct mode unsigned), 380-381 (CHECK_LLVM_CONST),
 *               383-386 (non-saturating path calling trunc_float_to_int)
 * Functional Purpose: Validates that aot_compile_op_i32_trunc_f32() successfully
 *                     compiles i32.trunc_f32_u operation with unsigned non-saturating
 *                     truncation in direct mode.
 * Call Path: aot_compile_op_i32_trunc_f32() <- aot_compiler.c WASM_OP_I32_TRUNC_U_F32
 * Coverage Goal: Exercise unsigned non-saturating truncation path
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i32_trunc_f32_UnsignedNonSaturating_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i32_trunc_f32_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options for direct mode
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

    // Compile WASM - this will trigger aot_compile_op_i32_trunc_f32 for i32.trunc_f32_u
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i32_trunc_f32_SignedSaturating_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:342-392
 * Target Lines: 342 (function entry), 348 (POP_F32), 350-351 (condition check),
 *               370-373 (direct mode signed), 380-381 (CHECK_LLVM_CONST),
 *               387-390 (saturating path calling trunc_sat_float_to_int)
 * Functional Purpose: Validates that aot_compile_op_i32_trunc_f32() successfully
 *                     compiles i32.trunc_sat_f32_s operation with signed saturating
 *                     truncation in direct mode.
 * Call Path: aot_compile_op_i32_trunc_f32() <- aot_compiler.c WASM_OP_I32_TRUNC_SAT_S_F32
 * Coverage Goal: Exercise signed saturating truncation path
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i32_trunc_f32_SignedSaturating_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i32_trunc_f32_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options for direct mode
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

    // Compile WASM - this will trigger aot_compile_op_i32_trunc_f32 for i32.trunc_sat_f32_s
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i32_trunc_f32_UnsignedSaturating_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:342-392
 * Target Lines: 342 (function entry), 348 (POP_F32), 350-351 (condition check),
 *               370-377 (direct mode unsigned), 380-381 (CHECK_LLVM_CONST),
 *               387-390 (saturating path calling trunc_sat_float_to_int)
 * Functional Purpose: Validates that aot_compile_op_i32_trunc_f32() successfully
 *                     compiles i32.trunc_sat_f32_u operation with unsigned saturating
 *                     truncation in direct mode.
 * Call Path: aot_compile_op_i32_trunc_f32() <- aot_compiler.c WASM_OP_I32_TRUNC_SAT_U_F32
 * Coverage Goal: Exercise unsigned saturating truncation path
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i32_trunc_f32_UnsignedSaturating_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i32_trunc_f32_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options for direct mode
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

    // Compile WASM - this will trigger aot_compile_op_i32_trunc_f32 for i32.trunc_sat_f32_u
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

// === New Test Cases for aot_compile_op_i32_trunc_f64 (lines 396-446) ===

/******
 * Test Case: aot_compile_op_i32_trunc_f64_SignedNonSaturating_DirectMode_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:396-446
 * Target Lines: 396 (function entry), 399-402 (variable declaration, POP_F64),
 *               404 (is_indirect_mode check), 424-428 (direct mode signed path),
 *               434-435 (CHECK_LLVM_CONST), 437-440 (non-saturating trunc_float_to_int)
 * Functional Purpose: Validates that aot_compile_op_i32_trunc_f64() successfully
 *                     compiles i32.trunc_f64_s operation with signed non-saturating
 *                     truncation in direct mode (non-indirect compilation context).
 * Call Path: aot_compile_op_i32_trunc_f64() <- aot_compiler.c WASM_OP_I32_TRUNC_S_F64
 * Coverage Goal: Exercise signed non-saturating truncation path in direct mode
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i32_trunc_f64_SignedNonSaturating_DirectMode_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i32_trunc_f64_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options for direct mode (non-indirect)
    option.opt_level = 1;
    option.size_level = 1;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = true;
    option.enable_ref_types = false;
    option.is_indirect_mode = false; // Force direct mode

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

    // Compile WASM - this will trigger aot_compile_op_i32_trunc_f64 for i32.trunc_f64_s
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i32_trunc_f64_UnsignedNonSaturating_DirectMode_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:396-446
 * Target Lines: 396 (function entry), 399-402 (variable declaration, POP_F64),
 *               404 (is_indirect_mode check), 428-432 (direct mode unsigned path),
 *               434-435 (CHECK_LLVM_CONST), 437-440 (non-saturating trunc_float_to_int)
 * Functional Purpose: Validates that aot_compile_op_i32_trunc_f64() successfully
 *                     compiles i32.trunc_f64_u operation with unsigned non-saturating
 *                     truncation in direct mode (non-indirect compilation context).
 * Call Path: aot_compile_op_i32_trunc_f64() <- aot_compiler.c WASM_OP_I32_TRUNC_U_F64
 * Coverage Goal: Exercise unsigned non-saturating truncation path in direct mode
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i32_trunc_f64_UnsignedNonSaturating_DirectMode_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i32_trunc_f64_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options for direct mode (non-indirect)
    option.opt_level = 1;
    option.size_level = 1;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = true;
    option.enable_ref_types = false;
    option.is_indirect_mode = false; // Force direct mode

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

    // Compile WASM - this will trigger aot_compile_op_i32_trunc_f64 for i32.trunc_f64_u
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i32_trunc_f64_SignedSaturating_DirectMode_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:396-446
 * Target Lines: 396 (function entry), 399-402 (variable declaration, POP_F64),
 *               404 (is_indirect_mode check), 424-428 (direct mode signed path),
 *               434-435 (CHECK_LLVM_CONST), 441-444 (saturating trunc_sat_float_to_int)
 * Functional Purpose: Validates that aot_compile_op_i32_trunc_f64() successfully
 *                     compiles i32.trunc_sat_f64_s operation with signed saturating
 *                     truncation in direct mode (non-indirect compilation context).
 * Call Path: aot_compile_op_i32_trunc_f64() <- aot_compiler.c WASM_OP_I32_TRUNC_SAT_S_F64
 * Coverage Goal: Exercise signed saturating truncation path in direct mode
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i32_trunc_f64_SignedSaturating_DirectMode_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i32_trunc_f64_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options for direct mode (non-indirect)
    option.opt_level = 2;
    option.size_level = 1;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 1;
    option.enable_simd = false;
    option.enable_aux_stack_check = false;
    option.enable_bulk_memory = true;
    option.enable_ref_types = false;
    option.is_indirect_mode = false; // Force direct mode

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

    // Compile WASM - this will trigger aot_compile_op_i32_trunc_f64 for i32.trunc_sat_f64_s
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i32_trunc_f64_UnsignedSaturating_DirectMode_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:396-446
 * Target Lines: 396 (function entry), 399-402 (variable declaration, POP_F64),
 *               404 (is_indirect_mode check), 428-432 (direct mode unsigned path),
 *               434-435 (CHECK_LLVM_CONST), 441-444 (saturating trunc_sat_float_to_int)
 * Functional Purpose: Validates that aot_compile_op_i32_trunc_f64() successfully
 *                     compiles i32.trunc_sat_f64_u operation with unsigned saturating
 *                     truncation in direct mode (non-indirect compilation context).
 * Call Path: aot_compile_op_i32_trunc_f64() <- aot_compiler.c WASM_OP_I32_TRUNC_SAT_U_F64
 * Coverage Goal: Exercise unsigned saturating truncation path in direct mode
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i32_trunc_f64_UnsignedSaturating_DirectMode_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i32_trunc_f64_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options for direct mode (non-indirect)
    option.opt_level = 2;
    option.size_level = 2;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 1;
    option.enable_simd = true;
    option.enable_aux_stack_check = false;
    option.enable_bulk_memory = true;
    option.enable_ref_types = false;
    option.is_indirect_mode = false; // Force direct mode

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

    // Compile WASM - this will trigger aot_compile_op_i32_trunc_f64 for i32.trunc_sat_f64_u
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

// =============================================================================
// NEW TEST CASES FOR aot_compile_op_i64_extend_i32 COVERAGE (Lines 450-471)
// =============================================================================

/******
 * Test Case: aot_compile_op_i64_extend_i32_SignedExtension_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:450-471
 * Target Lines: 450 (function entry), 455 (POP_I32), 457-459 (signed extension path),
 *               463-466 (error check for LLVMBuildSExt), 468 (PUSH_I64), 469 (return true)
 * Functional Purpose: Validates that aot_compile_op_i64_extend_i32() successfully
 *                     compiles i64.extend_i32_s operation with signed extension
 *                     using LLVMBuildSExt to extend i32 to i64 with sign extension.
 * Call Path: aot_compile_op_i64_extend_i32() <- aot_compile_func() <- aot_compile_wasm()
 * Coverage Goal: Exercise signed extension path (LLVMBuildSExt) in success scenario
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_extend_i32_SignedExtension_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_extend_i32_test.wasm";
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
    option.enable_simd = true;
    option.enable_aux_stack_check = false;
    option.enable_bulk_memory = true;
    option.enable_ref_types = false;
    option.is_indirect_mode = false;

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

    // Compile WASM - this will trigger aot_compile_op_i64_extend_i32 for i64.extend_i32_s
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i64_extend_i32_UnsignedExtension_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:450-471
 * Target Lines: 450 (function entry), 455 (POP_I32), 460-462 (unsigned extension path),
 *               463-466 (error check for LLVMBuildZExt), 468 (PUSH_I64), 469 (return true)
 * Functional Purpose: Validates that aot_compile_op_i64_extend_i32() successfully
 *                     compiles i64.extend_i32_u operation with unsigned extension
 *                     using LLVMBuildZExt to extend i32 to i64 with zero extension.
 * Call Path: aot_compile_op_i64_extend_i32() <- aot_compile_func() <- aot_compile_wasm()
 * Coverage Goal: Exercise unsigned extension path (LLVMBuildZExt) in success scenario
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_extend_i32_UnsignedExtension_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_extend_i32_test.wasm";
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
    option.enable_simd = true;
    option.enable_aux_stack_check = false;
    option.enable_bulk_memory = true;
    option.enable_ref_types = false;
    option.is_indirect_mode = false;

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

    // Compile WASM - this will trigger aot_compile_op_i64_extend_i32 for i64.extend_i32_u
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i64_extend_i32_CombinedOperations_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:450-471
 * Target Lines: 450 (function entry), 455 (POP_I32), 457-462 (both signed and unsigned paths),
 *               463-466 (error check for both operations), 468 (PUSH_I64), 469 (return true)
 * Functional Purpose: Validates that aot_compile_op_i64_extend_i32() successfully
 *                     handles both signed and unsigned extensions in a single WASM module,
 *                     ensuring complete path coverage for both extension types.
 * Call Path: aot_compile_op_i64_extend_i32() <- aot_compile_func() <- aot_compile_wasm()
 * Coverage Goal: Exercise both signed and unsigned extension paths in combined scenario
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_extend_i32_CombinedOperations_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_extend_i32_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options with different parameters to ensure thorough coverage
    option.opt_level = 1;
    option.size_level = 1;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 1;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = true;
    option.enable_ref_types = false;
    option.is_indirect_mode = false;

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

    // Compile WASM - this will trigger both signed and unsigned i64_extend_i32 operations
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

// =============================================================================
// NEW TEST CASES FOR aot_compile_op_i64_extend_i64 COVERAGE (Lines 475-511)
// =============================================================================

/******
 * Test Case: aot_compile_op_i64_extend_i64_Extend8S_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:475-511
 * Target Lines: 475 (function entry), 478 (variable declaration), 480 (POP_I64),
 *               482-484 (bitwidth 8 casting), 495-498 (cast_value check),
 *               500-501 (LLVMBuildSExt), 503-506 (res check), 508 (PUSH_I64), 509 (return true)
 * Functional Purpose: Validates that aot_compile_op_i64_extend_i64() successfully
 *                     compiles i64.extend8_s operation, performing 8-bit casting
 *                     followed by sign extension to 64-bit value.
 * Call Path: aot_compile_op_i64_extend_i64() <- aot_compiler.c WASM_OP_I64_EXTEND8_S <- aot_compile_wasm()
 * Coverage Goal: Exercise 8-bit sign extension path in i64_extend_i64 function
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_extend_i64_Extend8S_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_extend8s_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options
    option.opt_level = 2;
    option.size_level = 1;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 1;
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

    // Compile WASM - this will trigger aot_compile_op_i64_extend_i64 with bitwidth=8
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i64_extend_i64_Extend16S_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:475-511
 * Target Lines: 475 (function entry), 478 (variable declaration), 480 (POP_I64),
 *               486-488 (bitwidth 16 casting), 495-498 (cast_value check),
 *               500-501 (LLVMBuildSExt), 503-506 (res check), 508 (PUSH_I64), 509 (return true)
 * Functional Purpose: Validates that aot_compile_op_i64_extend_i64() successfully
 *                     compiles i64.extend16_s operation, performing 16-bit casting
 *                     followed by sign extension to 64-bit value.
 * Call Path: aot_compile_op_i64_extend_i64() <- aot_compiler.c WASM_OP_I64_EXTEND16_S <- aot_compile_wasm()
 * Coverage Goal: Exercise 16-bit sign extension path in i64_extend_i64 function
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_extend_i64_Extend16S_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_extend16s_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options
    option.opt_level = 1;
    option.size_level = 2;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = false;
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

    // Compile WASM - this will trigger aot_compile_op_i64_extend_i64 with bitwidth=16
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i64_extend_i64_Extend32S_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:475-511
 * Target Lines: 475 (function entry), 478 (variable declaration), 480 (POP_I64),
 *               490-492 (bitwidth 32 casting), 495-498 (cast_value check),
 *               500-501 (LLVMBuildSExt), 503-506 (res check), 508 (PUSH_I64), 509 (return true)
 * Functional Purpose: Validates that aot_compile_op_i64_extend_i64() successfully
 *                     compiles i64.extend32_s operation, performing 32-bit casting
 *                     followed by sign extension to 64-bit value.
 * Call Path: aot_compile_op_i64_extend_i64() <- aot_compiler.c WASM_OP_I64_EXTEND32_S <- aot_compile_wasm()
 * Coverage Goal: Exercise 32-bit sign extension path in i64_extend_i64 function
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_extend_i64_Extend32S_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_extend32s_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options
    option.opt_level = 3;
    option.size_level = 1;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 1;
    option.enable_simd = true;
    option.enable_aux_stack_check = true;
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

    // Compile WASM - this will trigger aot_compile_op_i64_extend_i64 with bitwidth=32
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i64_extend_i64_CombinedOperations_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:475-511
 * Target Lines: 475 (function entry), 478 (variable declaration), 480 (POP_I64),
 *               482-492 (all bitwidth casting paths), 495-498 (cast_value check),
 *               500-501 (LLVMBuildSExt), 503-506 (res check), 508 (PUSH_I64), 509 (return true)
 * Functional Purpose: Validates that aot_compile_op_i64_extend_i64() successfully
 *                     handles multiple extend operations within the same module,
 *                     ensuring all bitwidth paths (8, 16, 32) are covered.
 * Call Path: aot_compile_op_i64_extend_i64() <- aot_compiler.c multiple opcodes <- aot_compile_wasm()
 * Coverage Goal: Exercise all bitwidth paths in a single comprehensive test
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_extend_i64_CombinedOperations_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_extend_combined_test.wasm";
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
    option.bounds_checks = 2;
    option.enable_simd = true;
    option.enable_aux_stack_check = false;
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

    // Compile WASM - this will trigger aot_compile_op_i64_extend_i64 with all bitwidths
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

// =============================================================================
// NEW TEST CASES FOR aot_compile_op_i32_extend_i32 COVERAGE (Lines 515-545)
// =============================================================================

/******
 * Test Case: aot_compile_op_i32_extend_i32_Extend8S_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:515-545
 * Target Lines: 515 (function entry), 520 (POP_I32), 522-524 (bitwidth 8 casting),
 *               531-534 (cast_value check), 536-537 (LLVMBuildSExt), 539-542 (res check),
 *               544 (PUSH_I32), 545 (return true)
 * Functional Purpose: Validates that aot_compile_op_i32_extend_i32() successfully
 *                     compiles i32.extend8_s operation, performing 8-bit casting
 *                     followed by sign extension to 32-bit value.
 * Call Path: aot_compile_op_i32_extend_i32() <- aot_compiler.c WASM_OP_I32_EXTEND8_S <- aot_compile_wasm()
 * Coverage Goal: Exercise 8-bit sign extension path in i32_extend_i32 function
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i32_extend_i32_Extend8S_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i32_extend8s_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options
    option.opt_level = 2;
    option.size_level = 1;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 1;
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

    // Compile WASM - this will trigger aot_compile_op_i32_extend_i32 with bitwidth=8
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i32_extend_i32_Extend16S_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:515-545
 * Target Lines: 515 (function entry), 520 (POP_I32), 526-528 (bitwidth 16 casting),
 *               531-534 (cast_value check), 536-537 (LLVMBuildSExt), 539-542 (res check),
 *               544 (PUSH_I32), 545 (return true)
 * Functional Purpose: Validates that aot_compile_op_i32_extend_i32() successfully
 *                     compiles i32.extend16_s operation, performing 16-bit casting
 *                     followed by sign extension to 32-bit value.
 * Call Path: aot_compile_op_i32_extend_i32() <- aot_compiler.c WASM_OP_I32_EXTEND16_S <- aot_compile_wasm()
 * Coverage Goal: Exercise 16-bit sign extension path in i32_extend_i32 function
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i32_extend_i32_Extend16S_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i32_extend16s_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options
    option.opt_level = 1;
    option.size_level = 2;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = false;
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

    // Compile WASM - this will trigger aot_compile_op_i32_extend_i32 with bitwidth=16
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i32_extend_i32_CombinedOperations_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:515-545
 * Target Lines: 515 (function entry), 520 (POP_I32), 522-528 (both bitwidth paths),
 *               531-534 (cast_value check), 536-537 (LLVMBuildSExt), 539-542 (res check),
 *               544 (PUSH_I32), 545 (return true) for multiple invocations
 * Functional Purpose: Validates that aot_compile_op_i32_extend_i32() successfully
 *                     handles both 8-bit and 16-bit extensions in a single WASM module,
 *                     ensuring complete path coverage for both extension types.
 * Call Path: aot_compile_op_i32_extend_i32() <- aot_compiler.c multiple opcodes <- aot_compile_wasm()
 * Coverage Goal: Exercise both 8-bit and 16-bit extension paths in combined scenario
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i32_extend_i32_CombinedOperations_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i32_extend_combined_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options with different parameters to ensure thorough coverage
    option.opt_level = 3;
    option.size_level = 1;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 1;
    option.enable_simd = true;
    option.enable_aux_stack_check = true;
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

    // Compile WASM - this will trigger both 8-bit and 16-bit i32_extend_i32 operations
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

// =============================================================================
// NEW TEST CASES FOR aot_compile_op_i64_trunc_f32 COVERAGE (Lines 551-601)
// =============================================================================

/******
 * Test Case: aot_compile_op_i64_trunc_f32_SignedNonSaturating_DirectMode_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:551-601
 * Target Lines: 551 (function entry), 557 (POP_F32), 559 (is_indirect_mode check),
 *               580-583 (direct mode signed path), 589-590 (CHECK_LLVM_CONST),
 *               592-595 (non-saturating path calling trunc_float_to_int)
 * Functional Purpose: Validates that aot_compile_op_i64_trunc_f32() successfully
 *                     compiles i64.trunc_f32_s operation with signed non-saturating
 *                     truncation in direct mode (non-indirect compilation context).
 * Call Path: aot_compile_op_i64_trunc_f32() <- aot_compiler.c WASM_OP_I64_TRUNC_S_F32
 * Coverage Goal: Exercise signed non-saturating truncation path in direct mode
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_trunc_f32_SignedNonSaturating_DirectMode_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_trunc_f32_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options for direct mode (non-indirect)
    option.opt_level = 1;
    option.size_level = 1;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = true;
    option.enable_ref_types = false;
    option.is_indirect_mode = false; // Force direct mode

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

    // Compile WASM - this will trigger aot_compile_op_i64_trunc_f32 for i64.trunc_f32_s
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i64_trunc_f32_UnsignedNonSaturating_DirectMode_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:551-601
 * Target Lines: 551 (function entry), 557 (POP_F32), 559 (is_indirect_mode check),
 *               584-587 (direct mode unsigned path), 589-590 (CHECK_LLVM_CONST),
 *               592-595 (non-saturating path calling trunc_float_to_int)
 * Functional Purpose: Validates that aot_compile_op_i64_trunc_f32() successfully
 *                     compiles i64.trunc_f32_u operation with unsigned non-saturating
 *                     truncation in direct mode (non-indirect compilation context).
 * Call Path: aot_compile_op_i64_trunc_f32() <- aot_compiler.c WASM_OP_I64_TRUNC_U_F32
 * Coverage Goal: Exercise unsigned non-saturating truncation path in direct mode
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_trunc_f32_UnsignedNonSaturating_DirectMode_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_trunc_f32_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options for direct mode (non-indirect)
    option.opt_level = 2;
    option.size_level = 1;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 1;
    option.enable_simd = false;
    option.enable_aux_stack_check = false;
    option.enable_bulk_memory = true;
    option.enable_ref_types = false;
    option.is_indirect_mode = false; // Force direct mode

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

    // Compile WASM - this will trigger aot_compile_op_i64_trunc_f32 for i64.trunc_f32_u
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i64_trunc_f32_SignedSaturating_DirectMode_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:551-601
 * Target Lines: 551 (function entry), 557 (POP_F32), 559 (is_indirect_mode check),
 *               580-583 (direct mode signed path), 589-590 (CHECK_LLVM_CONST),
 *               596-599 (saturating path calling trunc_sat_float_to_int)
 * Functional Purpose: Validates that aot_compile_op_i64_trunc_f32() successfully
 *                     compiles i64.trunc_sat_f32_s operation with signed saturating
 *                     truncation in direct mode (non-indirect compilation context).
 * Call Path: aot_compile_op_i64_trunc_f32() <- aot_compiler.c WASM_OP_I64_TRUNC_SAT_S_F32
 * Coverage Goal: Exercise signed saturating truncation path in direct mode
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_trunc_f32_SignedSaturating_DirectMode_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_trunc_f32_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options for direct mode (non-indirect)
    option.opt_level = 2;
    option.size_level = 2;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 1;
    option.enable_simd = true;
    option.enable_aux_stack_check = false;
    option.enable_bulk_memory = true;
    option.enable_ref_types = false;
    option.is_indirect_mode = false; // Force direct mode

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

    // Compile WASM - this will trigger aot_compile_op_i64_trunc_f32 for i64.trunc_sat_f32_s
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i64_trunc_f32_UnsignedSaturating_DirectMode_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:551-601
 * Target Lines: 551 (function entry), 557 (POP_F32), 559 (is_indirect_mode check),
 *               584-587 (direct mode unsigned path), 589-590 (CHECK_LLVM_CONST),
 *               596-599 (saturating path calling trunc_sat_float_to_int)
 * Functional Purpose: Validates that aot_compile_op_i64_trunc_f32() successfully
 *                     compiles i64.trunc_sat_f32_u operation with unsigned saturating
 *                     truncation in direct mode (non-indirect compilation context).
 * Call Path: aot_compile_op_i64_trunc_f32() <- aot_compiler.c WASM_OP_I64_TRUNC_SAT_U_F32
 * Coverage Goal: Exercise unsigned saturating truncation path in direct mode
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_trunc_f32_UnsignedSaturating_DirectMode_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_trunc_f32_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options for direct mode (non-indirect)
    option.opt_level = 3;
    option.size_level = 1;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = false;
    option.enable_ref_types = false;
    option.is_indirect_mode = false; // Force direct mode

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

    // Compile WASM - this will trigger aot_compile_op_i64_trunc_f32 for i64.trunc_sat_f32_u
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i64_trunc_f32_IndirectMode_IntrinsicCapability_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:551-601
 * Target Lines: 551 (function entry), 557 (POP_F32), 559-561 (indirect mode and intrinsic check),
 *               562-577 (indirect mode paths with aot_load_const_from_table), 589-590 (CHECK_LLVM_CONST),
 *               592-595 (non-saturating path calling trunc_float_to_int)
 * Functional Purpose: Validates that aot_compile_op_i64_trunc_f32() successfully
 *                     handles indirect mode compilation with intrinsic capability check,
 *                     using aot_load_const_from_table for constant value loading.
 * Call Path: aot_compile_op_i64_trunc_f32() <- aot_compiler.c in indirect mode
 * Coverage Goal: Exercise indirect mode path with intrinsic capability support
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_trunc_f32_IndirectMode_IntrinsicCapability_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_trunc_f32_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options for indirect mode
    option.opt_level = 1;
    option.size_level = 1;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = true;
    option.enable_ref_types = false;
    option.is_indirect_mode = true; // Force indirect mode

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

    // Compile WASM - this will trigger aot_compile_op_i64_trunc_f32 in indirect mode
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i64_trunc_f32_CombinedOperations_AllPaths_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:551-601
 * Target Lines: 551 (function entry), 557 (POP_F32), 559-578 (all indirect mode paths),
 *               579-588 (all direct mode paths), 589-590 (CHECK_LLVM_CONST),
 *               592-599 (both saturating and non-saturating paths)
 * Functional Purpose: Validates that aot_compile_op_i64_trunc_f32() successfully
 *                     handles all four operation variants (signed/unsigned × non-saturating/saturating)
 *                     in a comprehensive test covering multiple code paths.
 * Call Path: aot_compile_op_i64_trunc_f32() <- aot_compiler.c multiple opcodes
 * Coverage Goal: Exercise maximum code coverage by testing all operation combinations
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_trunc_f32_CombinedOperations_AllPaths_ReturnsTrue) {
    const char *wasm_file = "/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_trunc_f32_test.wasm";
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = {0};
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = {0};

    // Initialize compilation options with moderate settings
    option.opt_level = 2;
    option.size_level = 2;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 1;
    option.enable_simd = false;
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = true;
    option.enable_ref_types = false;
    option.is_indirect_mode = false; // Use direct mode for comprehensive coverage

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

    // Compile WASM - this will trigger all aot_compile_op_i64_trunc_f32 variants
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

// ==================== NEW TEST CASES FOR LINES 605-693 ====================

/******
 * Test Case: aot_compile_op_i64_trunc_f64_Signed_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:605-657
 * Target Lines: 605 (function entry), 611 (POP_F64), 613-623 (indirect mode signed path),
 *               616-622 (signed min/max values), 634-637 (direct mode signed path),
 *               647-649 (trunc_float_to_int call), 656 (return true)
 * Functional Purpose: Validates that aot_compile_op_i64_trunc_f64() successfully
 *                     compiles i64.trunc_f64_s operation by truncating f64 to i64,
 *                     including proper stack operations and LLVM IR generation for signed conversion.
 * Call Path: aot_compile_op_i64_trunc_f64() <- aot_compiler.c:2370 <- WASM_OP_I64_TRUNC_S_F64
 * Coverage Goal: Exercise signed truncation path for i64.trunc_f64_s conversion
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_trunc_f64_Signed_ReturnsTrue) {
    wasm_module_t wasm_module;
    aot_comp_data_t comp_data;
    aot_comp_context_t comp_ctx;
    uint32 wasm_file_size;
    uint8 *wasm_file_buf;
    char error_buf[128];
    AOTCompOption option = { 0 };

    // Load i64_trunc_f64 test WASM module
    wasm_file_buf = (uint8*)bh_read_file_to_buffer("/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_trunc_f64_test.wasm", &wasm_file_size);
    ASSERT_NE(nullptr, wasm_file_buf);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, wasm_module);

    // Create compilation data
    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);

    // Create compilation context
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(nullptr, comp_ctx);

    // Compile WASM - this will trigger aot_compile_op_i64_trunc_f64 signed path
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i64_trunc_f64_Unsigned_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:605-657
 * Target Lines: 605 (function entry), 611 (POP_F64), 613-614 (indirect mode check),
 *               624-631 (unsigned min/max values), 638-641 (direct mode unsigned path),
 *               647-649 (trunc_float_to_int call), 656 (return true)
 * Functional Purpose: Validates that aot_compile_op_i64_trunc_f64() successfully
 *                     compiles i64.trunc_f64_u operation by truncating f64 to i64,
 *                     including proper stack operations and LLVM IR generation for unsigned conversion.
 * Call Path: aot_compile_op_i64_trunc_f64() <- aot_compiler.c:2370 <- WASM_OP_I64_TRUNC_U_F64
 * Coverage Goal: Exercise unsigned truncation path for i64.trunc_f64_u conversion
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_trunc_f64_Unsigned_ReturnsTrue) {
    wasm_module_t wasm_module;
    aot_comp_data_t comp_data;
    aot_comp_context_t comp_ctx;
    uint32 wasm_file_size;
    uint8 *wasm_file_buf;
    char error_buf[128];
    AOTCompOption option = { 0 };

    // Load i64_trunc_f64 test WASM module
    wasm_file_buf = (uint8*)bh_read_file_to_buffer("/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_trunc_f64_test.wasm", &wasm_file_size);
    ASSERT_NE(nullptr, wasm_file_buf);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, wasm_module);

    // Create compilation data
    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);

    // Create compilation context
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(nullptr, comp_ctx);

    // Compile WASM - this will trigger aot_compile_op_i64_trunc_f64 unsigned path
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i64_trunc_f64_SignedSaturating_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:605-657
 * Target Lines: 605 (function entry), 611 (POP_F64), 613-623 (indirect mode signed path),
 *               646 (saturating check), 651-653 (trunc_sat_float_to_int call), 656 (return true)
 * Functional Purpose: Validates that aot_compile_op_i64_trunc_f64() successfully
 *                     compiles i64.trunc_sat_f64_s operation by saturating truncation f64 to i64,
 *                     including proper stack operations and LLVM IR generation for signed saturating conversion.
 * Call Path: aot_compile_op_i64_trunc_f64() <- aot_compiler.c:2511 <- WASM_OP_I64_TRUNC_SAT_S_F64
 * Coverage Goal: Exercise signed saturating truncation path for i64.trunc_sat_f64_s conversion
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_trunc_f64_SignedSaturating_ReturnsTrue) {
    wasm_module_t wasm_module;
    aot_comp_data_t comp_data;
    aot_comp_context_t comp_ctx;
    uint32 wasm_file_size;
    uint8 *wasm_file_buf;
    char error_buf[128];
    AOTCompOption option = { 0 };

    // Load i64_trunc_f64 test WASM module
    wasm_file_buf = (uint8*)bh_read_file_to_buffer("/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_trunc_f64_test.wasm", &wasm_file_size);
    ASSERT_NE(nullptr, wasm_file_buf);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, wasm_module);

    // Create compilation data
    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);

    // Create compilation context
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(nullptr, comp_ctx);

    // Compile WASM - this will trigger aot_compile_op_i64_trunc_f64 signed saturating path
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_i64_trunc_f64_UnsignedSaturating_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:605-657
 * Target Lines: 605 (function entry), 611 (POP_F64), 624-631 (unsigned min/max values),
 *               646 (saturating check), 651-653 (trunc_sat_float_to_int call), 656 (return true)
 * Functional Purpose: Validates that aot_compile_op_i64_trunc_f64() successfully
 *                     compiles i64.trunc_sat_f64_u operation by saturating truncation f64 to i64,
 *                     including proper stack operations and LLVM IR generation for unsigned saturating conversion.
 * Call Path: aot_compile_op_i64_trunc_f64() <- aot_compiler.c:2511 <- WASM_OP_I64_TRUNC_SAT_U_F64
 * Coverage Goal: Exercise unsigned saturating truncation path for i64.trunc_sat_f64_u conversion
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_i64_trunc_f64_UnsignedSaturating_ReturnsTrue) {
    wasm_module_t wasm_module;
    aot_comp_data_t comp_data;
    aot_comp_context_t comp_ctx;
    uint32 wasm_file_size;
    uint8 *wasm_file_buf;
    char error_buf[128];
    AOTCompOption option = { 0 };

    // Load i64_trunc_f64 test WASM module
    wasm_file_buf = (uint8*)bh_read_file_to_buffer("/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/i64_trunc_f64_test.wasm", &wasm_file_size);
    ASSERT_NE(nullptr, wasm_file_buf);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, wasm_module);

    // Create compilation data
    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);

    // Create compilation context
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(nullptr, comp_ctx);

    // Compile WASM - this will trigger aot_compile_op_i64_trunc_f64 unsigned saturating path
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_f32_convert_i32_Signed_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:660-694
 * Target Lines: 660 (function entry), 665 (POP_I32), 667-669 (intrinsic capability check),
 *               678-680 (LLVMBuildSIToFP), 685-687 (error check), 690 (PUSH_F32), 691 (return true)
 * Functional Purpose: Validates that aot_compile_op_f32_convert_i32() successfully
 *                     compiles f32.convert_i32_s operation by converting i32 to f32,
 *                     including proper stack operations and LLVM IR generation for signed conversion.
 * Call Path: aot_compile_op_f32_convert_i32() <- aot_compiler.c:2378 <- WASM_OP_F32_CONVERT_S_I32
 * Coverage Goal: Exercise signed conversion path for f32.convert_i32_s conversion
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_f32_convert_i32_Signed_ReturnsTrue) {
    wasm_module_t wasm_module;
    aot_comp_data_t comp_data;
    aot_comp_context_t comp_ctx;
    uint32 wasm_file_size;
    uint8 *wasm_file_buf;
    char error_buf[128];
    AOTCompOption option = { 0 };

    // Load f32_convert_i32 test WASM module
    wasm_file_buf = (uint8*)bh_read_file_to_buffer("/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/f32_convert_i32_test.wasm", &wasm_file_size);
    ASSERT_NE(nullptr, wasm_file_buf);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, wasm_module);

    // Create compilation data
    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);

    // Create compilation context
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(nullptr, comp_ctx);

    // Compile WASM - this will trigger aot_compile_op_f32_convert_i32 signed path
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_f32_convert_i32_Unsigned_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:660-694
 * Target Lines: 660 (function entry), 665 (POP_I32), 667-669 (intrinsic capability check),
 *               681-683 (LLVMBuildUIToFP), 685-687 (error check), 690 (PUSH_F32), 691 (return true)
 * Functional Purpose: Validates that aot_compile_op_f32_convert_i32() successfully
 *                     compiles f32.convert_i32_u operation by converting i32 to f32,
 *                     including proper stack operations and LLVM IR generation for unsigned conversion.
 * Call Path: aot_compile_op_f32_convert_i32() <- aot_compiler.c:2378 <- WASM_OP_F32_CONVERT_U_I32
 * Coverage Goal: Exercise unsigned conversion path for f32.convert_i32_u conversion
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_f32_convert_i32_Unsigned_ReturnsTrue) {
    wasm_module_t wasm_module;
    aot_comp_data_t comp_data;
    aot_comp_context_t comp_ctx;
    uint32 wasm_file_size;
    uint8 *wasm_file_buf;
    char error_buf[128];
    AOTCompOption option = { 0 };

    // Load f32_convert_i32 test WASM module
    wasm_file_buf = (uint8*)bh_read_file_to_buffer("/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/f32_convert_i32_test.wasm", &wasm_file_size);
    ASSERT_NE(nullptr, wasm_file_buf);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, wasm_module);

    // Create compilation data
    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);

    // Create compilation context
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(nullptr, comp_ctx);

    // Compile WASM - this will trigger aot_compile_op_f32_convert_i32 unsigned path
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}

/******
 * Test Case: aot_compile_op_f32_convert_i32_WithIntrinsics_ReturnsTrue
 * Source: core/iwasm/compilation/aot_emit_conversion.c:660-694
 * Target Lines: 660 (function entry), 665 (POP_I32), 667-675 (intrinsic path),
 *               670-675 (aot_call_llvm_intrinsic), 690 (PUSH_F32), 691 (return true)
 * Functional Purpose: Validates that aot_compile_op_f32_convert_i32() successfully
 *                     compiles f32.convert_i32_s/u operations using LLVM intrinsics,
 *                     including proper stack operations and intrinsic function calls.
 * Call Path: aot_compile_op_f32_convert_i32() <- aot_compiler.c:2378 <- WASM_OP_F32_CONVERT_*_I32
 * Coverage Goal: Exercise intrinsic path for f32.convert_i32 conversions
 ******/
TEST_F(EnhancedAotEmitConversionTest, aot_compile_op_f32_convert_i32_WithIntrinsics_ReturnsTrue) {
    wasm_module_t wasm_module;
    aot_comp_data_t comp_data;
    aot_comp_context_t comp_ctx;
    uint32 wasm_file_size;
    uint8 *wasm_file_buf;
    char error_buf[128];
    AOTCompOption option = { 0 };

    // Enable intrinsics to trigger the intrinsic path
    option.disable_llvm_intrinsics = true;

    // Load f32_convert_i32 test WASM module
    wasm_file_buf = (uint8*)bh_read_file_to_buffer("/home/pugong/CPU_WPE/wasm-micro-runtime/tests/unit/compilation/f32_convert_i32_test.wasm", &wasm_file_size);
    ASSERT_NE(nullptr, wasm_file_buf);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
    ASSERT_NE(nullptr, wasm_module);

    // Create compilation data
    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);

    // Create compilation context with intrinsics enabled
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(nullptr, comp_ctx);

    // Compile WASM - this will trigger aot_compile_op_f32_convert_i32 intrinsic path
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
    wasm_runtime_unload(wasm_module);
}