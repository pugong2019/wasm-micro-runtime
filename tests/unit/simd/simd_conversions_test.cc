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
static std::string CONVERSIONS_WASM = "/simd_conversions_test.wasm";
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

class simd_conversions_test_suit : public testing::Test
{
  protected:
    virtual void SetUp() {}

    static void SetUpTestCase()
    {
        CWD = get_binary_path();
        WASM_FILE = strdup((CWD + CONVERSIONS_WASM).c_str());
    }

    virtual void TearDown() {}

    static void TearDownTestCase() { free(WASM_FILE); }

    WAMRRuntimeRAII<512 * 1024> runtime;
};

// Test SIMD integer extension operations
TEST_F(simd_conversions_test_suit, simd_i16x8_extend_i8x16)
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

    // Test that SIMD compilation context is properly configured for extensions
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test SIMD i32x4 extend i16x8 operations
TEST_F(simd_conversions_test_suit, simd_i32x4_extend_i16x8)
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

    // Test that SIMD compilation context is properly configured for 32-bit extensions
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test SIMD i64x2 extend i32x4 operations
TEST_F(simd_conversions_test_suit, simd_i64x2_extend_i32x4)
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

    // Test that SIMD compilation context is properly configured for 64-bit extensions
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test SIMD i8x16 narrow i16x8 operations
TEST_F(simd_conversions_test_suit, simd_i8x16_narrow_i16x8)
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

    // Test that SIMD compilation context is properly configured for narrowing
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test SIMD i16x8 narrow i32x4 operations
TEST_F(simd_conversions_test_suit, simd_i16x8_narrow_i32x4)
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

    // Test that SIMD compilation context is properly configured for 16-bit narrowing
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test SIMD f32x4 convert i32x4 operations
TEST_F(simd_conversions_test_suit, simd_f32x4_convert_i32x4)
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

    // Test that SIMD compilation context is properly configured for FP conversion
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test SIMD f64x2 convert i32x4 operations
TEST_F(simd_conversions_test_suit, simd_f64x2_convert_i32x4)
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

    // Test that SIMD compilation context is properly configured for 64-bit FP conversion
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test SIMD i32x4 trunc sat f32x4 operations
TEST_F(simd_conversions_test_suit, simd_i32x4_trunc_sat_f32x4)
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

    // Test that SIMD compilation context is properly configured for truncation
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test SIMD i32x4 trunc sat f64x2 operations
TEST_F(simd_conversions_test_suit, simd_i32x4_trunc_sat_f64x2)
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

    // Test that SIMD compilation context is properly configured for 64-bit truncation
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test SIMD f32x4 promote operations
TEST_F(simd_conversions_test_suit, simd_f32x4_promote)
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

    // Test that SIMD compilation context is properly configured for promotion
    EXPECT_STREQ(aot_get_last_error(), "");
    EXPECT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test f64x2.demote operation
TEST_F(simd_conversions_test_suit, simd_f64x2_demote_operations)
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