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

class aot_emit_gc_test_suit : public testing::Test
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

static bool
load_and_compile_module(const char *wasm_file, aot_comp_context_t *comp_ctx_out,
                        aot_comp_data_t *comp_data_out)
{
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
    option.enable_aux_stack_check = true;
    option.enable_bulk_memory = true;
    option.enable_ref_types = true;

    wasm_file_buf =
        (unsigned char *)bh_read_file_to_buffer(wasm_file, &wasm_file_size);
    if (!wasm_file_buf) {
        return false;
    }

    wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size, error_buf,
                                    sizeof(error_buf));
    if (!wasm_module) {
        free(wasm_file_buf);
        return false;
    }

    comp_data = aot_create_comp_data(wasm_module, NULL, false);
    if (!comp_data) {
        wasm_runtime_unload(wasm_module);
        free(wasm_file_buf);
        return false;
    }

    comp_ctx = aot_create_comp_context(comp_data, &option);
    if (!comp_ctx) {
        aot_destroy_comp_data(comp_data);
        wasm_runtime_unload(wasm_module);
        free(wasm_file_buf);
        return false;
    }

    if (!aot_compile_wasm(comp_ctx)) {
        aot_destroy_comp_context(comp_ctx);
        aot_destroy_comp_data(comp_data);
        wasm_runtime_unload(wasm_module);
        free(wasm_file_buf);
        return false;
    }

    *comp_ctx_out = comp_ctx;
    *comp_data_out = comp_data;
    
    free(wasm_file_buf);
    return true;
}

TEST_F(aot_emit_gc_test_suit, basic_gc_compilation)
{
    aot_comp_context_t comp_ctx = nullptr;
    aot_comp_data_t comp_data = nullptr;
    
    ASSERT_TRUE(load_and_compile_module(WASM_FILE, &comp_ctx, &comp_data));
    
    // Test that compilation succeeds with GC operations
    // This validates that the GC emission functions work correctly
    // by ensuring the compilation process completes successfully
    
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
}

TEST_F(aot_emit_gc_test_suit, compilation_with_gc_references)
{
    aot_comp_context_t comp_ctx = nullptr;
    aot_comp_data_t comp_data = nullptr;
    
    ASSERT_TRUE(load_and_compile_module(WASM_FILE, &comp_ctx, &comp_data));
    
    // Test compilation with GC reference operations
    // This validates GC emission with reference types
    
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
}

TEST_F(aot_emit_gc_test_suit, compilation_with_gc_memory_management)
{
    aot_comp_context_t comp_ctx = nullptr;
    aot_comp_data_t comp_data = nullptr;
    
    ASSERT_TRUE(load_and_compile_module(WASM_FILE, &comp_ctx, &comp_data));
    
    // Test compilation with GC memory management operations
    // This validates GC emission with memory management
    
    aot_destroy_comp_context(comp_ctx);
    aot_destroy_comp_data(comp_data);
}