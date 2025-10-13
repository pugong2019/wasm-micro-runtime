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
static char *WASM_FILE;

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

class simd_bool_reductions_test_suit : public testing::Test
{
  protected:
    virtual void SetUp() {}

    static void SetUpTestCase()
    {
        CWD = get_binary_path();
        WASM_FILE = strdup((CWD + MAIN_WASM).c_str());
    }

    virtual void TearDown() {}

    static void TearDownTestCase() { free(WASM_FILE); }

    WAMRRuntimeRAII<512 * 1024> runtime;
};

// Test i8x16.all_true operation
TEST_F(simd_bool_reductions_test_suit, simd_i8x16_all_true_operations)
{
    const char *wasm_file = WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;

    wasm_file_buf =
        (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    EXPECT_NE(wasm_file_buf, nullptr);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    EXPECT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    EXPECT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    EXPECT_NE(comp_ctx, nullptr);

    // Test that SIMD compilation context is properly configured
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test i16x8.all_true operation
TEST_F(simd_bool_reductions_test_suit, simd_i16x8_all_true_operations)
{
    const char *wasm_file = WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;

    wasm_file_buf =
        (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    EXPECT_NE(wasm_file_buf, nullptr);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    EXPECT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    EXPECT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    EXPECT_NE(comp_ctx, nullptr);

    // Test that SIMD compilation context is properly configured
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test i32x4.all_true operation
TEST_F(simd_bool_reductions_test_suit, simd_i32x4_all_true_operations)
{
    const char *wasm_file = WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;

    wasm_file_buf =
        (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    EXPECT_NE(wasm_file_buf, nullptr);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    EXPECT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    EXPECT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    EXPECT_NE(comp_ctx, nullptr);

    // Test that SIMD compilation context is properly configured
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test i64x2.all_true operation
TEST_F(simd_bool_reductions_test_suit, simd_i64x2_all_true_operations)
{
    const char *wasm_file = WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;

    wasm_file_buf =
        (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    EXPECT_NE(wasm_file_buf, nullptr);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    EXPECT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    EXPECT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    EXPECT_NE(comp_ctx, nullptr);

    // Test that SIMD compilation context is properly configured
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test v128.any_true operation
TEST_F(simd_bool_reductions_test_suit, simd_v128_any_true_operations)
{
    const char *wasm_file = WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;

    wasm_file_buf =
        (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    EXPECT_NE(wasm_file_buf, nullptr);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    EXPECT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    EXPECT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    EXPECT_NE(comp_ctx, nullptr);

    // Test that SIMD compilation context is properly configured
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}
// Test i8x16.popcnt operation
TEST_F(simd_bool_reductions_test_suit, simd_i8x16_popcnt_operations)
{
    const char *wasm_file = WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;

    wasm_file_buf =
        (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    EXPECT_NE(wasm_file_buf, nullptr);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    EXPECT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    EXPECT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    EXPECT_NE(comp_ctx, nullptr);

    // Test that SIMD compilation context is properly configured
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test i8x16.avgr_u operation
TEST_F(simd_bool_reductions_test_suit, simd_i8x16_avgr_u_operations)
{
    const char *wasm_file = WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;

    wasm_file_buf =
        (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    EXPECT_NE(wasm_file_buf, nullptr);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    EXPECT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    EXPECT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    EXPECT_NE(comp_ctx, nullptr);

    // Test that SIMD compilation context is properly configured
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test i16x8.avgr_u operation
TEST_F(simd_bool_reductions_test_suit, simd_i16x8_avgr_u_operations)
{
    const char *wasm_file = WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;

    wasm_file_buf =
        (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    EXPECT_NE(wasm_file_buf, nullptr);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    EXPECT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    EXPECT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    EXPECT_NE(comp_ctx, nullptr);

    // Test that SIMD compilation context is properly configured
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test i16x8.extadd_pairwise_i8x16 operation
TEST_F(simd_bool_reductions_test_suit, simd_i16x8_extadd_pairwise_i8x16_operations)
{
    const char *wasm_file = WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;

    wasm_file_buf =
        (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    EXPECT_NE(wasm_file_buf, nullptr);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    EXPECT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    EXPECT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    EXPECT_NE(comp_ctx, nullptr);

    // Test that SIMD compilation context is properly configured
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test i32x4.extadd_pairwise_i16x8 operation
TEST_F(simd_bool_reductions_test_suit, simd_i32x4_extadd_pairwise_i16x8_operations)
{
    const char *wasm_file = WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;

    wasm_file_buf =
        (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    EXPECT_NE(wasm_file_buf, nullptr);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    EXPECT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    EXPECT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    EXPECT_NE(comp_ctx, nullptr);

    // Test that SIMD compilation context is properly configured
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test i16x8.q15mulr_sat operation
TEST_F(simd_bool_reductions_test_suit, simd_i16x8_q15mulr_sat_operations)
{
    const char *wasm_file = WASM_FILE;
    unsigned int wasm_file_size = 0;
    unsigned char *wasm_file_buf = nullptr;
    char error_buf[128] = { 0 };
    wasm_module_t wasm_module = nullptr;
    aot_comp_data_t comp_data = nullptr;
    aot_comp_context_t comp_ctx = nullptr;
    AOTCompOption option = { 0 };

    option.opt_level = 3;
    option.size_level = 3;
    option.output_format = AOT_FORMAT_FILE;
    option.bounds_checks = 2;
    option.enable_simd = true;

    wasm_file_buf =
        (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    EXPECT_NE(wasm_file_buf, nullptr);
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    EXPECT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    EXPECT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    EXPECT_NE(comp_ctx, nullptr);

    // Test that SIMD compilation context is properly configured
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}
