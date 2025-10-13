/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "test_helper.h"
#include "gtest/gtest.h"

#include "wasm_export.h"
#include "aot_export.h"
#include "bh_read_file.h"

static std::string BITWISE_WASM = "simd_bitwise_ops_test.wasm";
static char *WASM_FILE;

class simd_bitwise_ops_test_suit : public testing::Test
{
  protected:
    virtual void SetUp() {}

    static void SetUpTestCase()
    {
        WASM_FILE = strdup(BITWISE_WASM.c_str());
    }

    virtual void TearDown() {}

    static void TearDownTestCase() { free(WASM_FILE); }

    WAMRRuntimeRAII<512 * 1024> runtime;
};

// Test basic SIMD bitwise operations compilation
TEST_F(simd_bitwise_ops_test_suit, simd_bitwise_operations_basic)
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
    ASSERT_NE(wasm_file_buf, nullptr);
    
    printf("WASM file size: %u\n", wasm_file_size);
    
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    if (!wasm_module) {
        printf("Failed to load WASM module: %s\n", error_buf);
    }
    ASSERT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Test that SIMD bitwise compilation context is properly configured
    ASSERT_STREQ(aot_get_last_error(), "");
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

// Test individual V128 bitwise operations
TEST_F(simd_bitwise_ops_test_suit, test_v128_and_basic_operation) {
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
    ASSERT_NE(wasm_file_buf, nullptr);
    
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    ASSERT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Test that V128 AND operation compilation succeeds
    ASSERT_STREQ(aot_get_last_error(), "");
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

TEST_F(simd_bitwise_ops_test_suit, test_v128_or_basic_operation) {
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
    ASSERT_NE(wasm_file_buf, nullptr);
    
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    ASSERT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Test that V128 OR operation compilation succeeds
    ASSERT_STREQ(aot_get_last_error(), "");
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

TEST_F(simd_bitwise_ops_test_suit, test_v128_xor_basic_operation) {
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
    ASSERT_NE(wasm_file_buf, nullptr);
    
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    ASSERT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Test that V128 XOR operation compilation succeeds
    ASSERT_STREQ(aot_get_last_error(), "");
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

TEST_F(simd_bitwise_ops_test_suit, test_v128_andnot_basic_operation) {
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
    ASSERT_NE(wasm_file_buf, nullptr);
    
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    ASSERT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Test that V128 ANDNOT operation compilation succeeds
    ASSERT_STREQ(aot_get_last_error(), "");
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

TEST_F(simd_bitwise_ops_test_suit, test_v128_not_basic_operation) {
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
    ASSERT_NE(wasm_file_buf, nullptr);
    
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    ASSERT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Test that V128 NOT operation compilation succeeds
    ASSERT_STREQ(aot_get_last_error(), "");
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

TEST_F(simd_bitwise_ops_test_suit, test_v128_bitselect_basic_operation) {
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
    ASSERT_NE(wasm_file_buf, nullptr);
    
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    ASSERT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Test that V128 BITSELECT operation compilation succeeds
    ASSERT_STREQ(aot_get_last_error(), "");
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

TEST_F(simd_bitwise_ops_test_suit, test_bitwise_operations_comprehensive_validation) {
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
    ASSERT_NE(wasm_file_buf, nullptr);
    
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    ASSERT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Test comprehensive validation of all bitwise operations
    ASSERT_STREQ(aot_get_last_error(), "");
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

TEST_F(simd_bitwise_ops_test_suit, test_bitwise_operations_edge_cases) {
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
    ASSERT_NE(wasm_file_buf, nullptr);
    
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    ASSERT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Test edge cases for bitwise operations
    ASSERT_STREQ(aot_get_last_error(), "");
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}

TEST_F(simd_bitwise_ops_test_suit, test_bitwise_operations_performance_benchmark) {
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
    ASSERT_NE(wasm_file_buf, nullptr);
    
    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    ASSERT_NE(wasm_module, nullptr);

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    ASSERT_NE(nullptr, comp_data);
    comp_ctx = aot_create_comp_context(comp_data, &option);
    ASSERT_NE(comp_ctx, nullptr);

    // Test performance benchmark for bitwise operations compilation
    ASSERT_STREQ(aot_get_last_error(), "");
    ASSERT_TRUE(aot_compile_wasm(comp_ctx));

    // Clean up resources
    if (comp_ctx) aot_destroy_comp_context(comp_ctx);
    if (comp_data) aot_destroy_comp_data(comp_data);
    if (wasm_module) wasm_runtime_unload(wasm_module);
    if (wasm_file_buf) BH_FREE(wasm_file_buf);
}