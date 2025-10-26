/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <limits.h>
#include <cstring>
#include "gtest/gtest.h"
#include "wasm_runtime_common.h"
#include "wasm_runtime.h"
#include "wasm.h"
#include "bh_platform.h"

// Enhanced test fixture for wasm_runtime.c functions
class EnhancedWasmRuntimeTest : public testing::Test {
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

    // Helper method to create a mock WASMModule with import functions
    WASMModule* CreateMockModuleWithImports(uint32 import_count, bool linked_state = false) {
        // Allocate memory for the module
        WASMModule *module = (WASMModule*)wasm_runtime_malloc(sizeof(WASMModule));
        EXPECT_NE(nullptr, module);
        if (!module) return nullptr;

        memset(module, 0, sizeof(WASMModule));

        module->import_function_count = import_count;

        if (import_count > 0) {
            // Allocate memory for import functions
            size_t import_size = sizeof(WASMImport) * import_count;
            module->import_functions = (WASMImport*)wasm_runtime_malloc(import_size);
            EXPECT_NE(nullptr, module->import_functions);
            if (!module->import_functions) {
                wasm_runtime_free(module);
                return nullptr;
            }

            memset(module->import_functions, 0, import_size);

            // Initialize import functions with mock data
            for (uint32 i = 0; i < import_count; i++) {
                WASMImport *import = &module->import_functions[i];
                import->kind = IMPORT_KIND_FUNC;

                // Initialize the function import
                WASMFunctionImport *func_import = &import->u.function;
                func_import->func_ptr_linked = linked_state ? (void*)0x1 : NULL;
                func_import->module_name = (char*)"test_module";
                func_import->field_name = (char*)"test_function";
#if WASM_ENABLE_MULTI_MODULE != 0
                func_import->import_func_linked = NULL;
#endif
            }
        }

        return module;
    }

    void DestroyMockModule(WASMModule *module) {
        if (module) {
            if (module->import_functions) {
                wasm_runtime_free(module->import_functions);
            }
            wasm_runtime_free(module);
        }
    }

public:
    char global_heap_buf[512 * 1024];
    RuntimeInitArgs init_args;
};

/******
 * Test Case: wasm_resolve_symbols_NoImportFunctions_ReturnsTrue
 * Source: core/iwasm/interpreter/wasm_runtime.c:87-103
 * Target Lines: 87 (function entry), 89 (ret = true), 90 (idx declaration), 91 (for loop condition false), 103 (return ret)
 * Functional Purpose: Validates that wasm_resolve_symbols() correctly handles modules
 *                     with no import functions by returning true without iterating.
 * Call Path: wasm_resolve_symbols() [PUBLIC API]
 * Coverage Goal: Exercise empty import function list path
 ******/
TEST_F(EnhancedWasmRuntimeTest, wasm_resolve_symbols_NoImportFunctions_ReturnsTrue) {
    WASMModule *module = CreateMockModuleWithImports(0);
    ASSERT_NE(nullptr, module);

    // Test the function with no import functions
    bool result = wasm_resolve_symbols(module);
    ASSERT_TRUE(result);

    DestroyMockModule(module);
}

/******
 * Test Case: wasm_resolve_symbols_AllFunctionsLinked_ReturnsTrue
 * Source: core/iwasm/interpreter/wasm_runtime.c:87-103
 * Target Lines: 87, 89-93 (loop iteration), 99 (condition false due to linked=true), 103
 * Functional Purpose: Validates that wasm_resolve_symbols() correctly handles modules
 *                     where all import functions are already linked (func_ptr_linked=true).
 * Call Path: wasm_resolve_symbols() [PUBLIC API]
 * Coverage Goal: Exercise path where all functions are already resolved
 ******/
TEST_F(EnhancedWasmRuntimeTest, wasm_resolve_symbols_AllFunctionsLinked_ReturnsTrue) {
    WASMModule *module = CreateMockModuleWithImports(3, true);  // 3 imports, all linked
    ASSERT_NE(nullptr, module);

    // Test the function with all functions already linked
    bool result = wasm_resolve_symbols(module);
    ASSERT_TRUE(result);

    DestroyMockModule(module);
}

/******
 * Test Case: wasm_resolve_symbols_UnlinkedFunctionsFailResolve_ReturnsFalse
 * Source: core/iwasm/interpreter/wasm_runtime.c:87-103
 * Target Lines: 87, 89-93, 99-100 (wasm_resolve_import_func fails, ret = false), 103
 * Functional Purpose: Validates that wasm_resolve_symbols() correctly handles modules
 *                     with unlinked import functions that fail to resolve by returning false.
 * Call Path: wasm_resolve_symbols() -> wasm_resolve_import_func() [FAIL]
 * Coverage Goal: Exercise failure path when import resolution fails
 ******/
TEST_F(EnhancedWasmRuntimeTest, wasm_resolve_symbols_UnlinkedFunctionsFailResolve_ReturnsFalse) {
    WASMModule *module = CreateMockModuleWithImports(2, false);  // 2 imports, not linked
    ASSERT_NE(nullptr, module);

    // Make sure functions are not linked
    for (uint32 i = 0; i < module->import_function_count; i++) {
        WASMFunctionImport *import = &module->import_functions[i].u.function;
        import->func_ptr_linked = NULL;
#if WASM_ENABLE_MULTI_MODULE != 0
        import->import_func_linked = NULL;
#endif
    }

    // Test the function with unlinked functions that will fail to resolve
    bool result = wasm_resolve_symbols(module);
    ASSERT_FALSE(result);

    DestroyMockModule(module);
}

/******
 * Test Case: wasm_resolve_symbols_MixedLinkedState_ReturnsFalse
 * Source: core/iwasm/interpreter/wasm_runtime.c:87-103
 * Target Lines: 87, 89-93 (multiple iterations), 99-100 (mixed results), 103
 * Functional Purpose: Validates that wasm_resolve_symbols() correctly handles modules
 *                     with mixed import function states (some linked, some not).
 * Call Path: wasm_resolve_symbols() -> wasm_resolve_import_func() [MIXED]
 * Coverage Goal: Exercise path with mixed resolution states
 ******/
TEST_F(EnhancedWasmRuntimeTest, wasm_resolve_symbols_MixedLinkedState_ReturnsFalse) {
    WASMModule *module = CreateMockModuleWithImports(4, false);
    ASSERT_NE(nullptr, module);

    // Set first two functions as linked
    module->import_functions[0].u.function.func_ptr_linked = (void*)0x1;
    module->import_functions[1].u.function.func_ptr_linked = (void*)0x1;

    // Leave last two functions unlinked (will fail to resolve)
    module->import_functions[2].u.function.func_ptr_linked = NULL;
    module->import_functions[3].u.function.func_ptr_linked = NULL;

    // Test should return false because some functions fail to resolve
    bool result = wasm_resolve_symbols(module);
    ASSERT_FALSE(result);

    DestroyMockModule(module);
}

#if WASM_ENABLE_MULTI_MODULE != 0
/******
 * Test Case: wasm_resolve_symbols_MultiModuleLinked_ReturnsTrue
 * Source: core/iwasm/interpreter/wasm_runtime.c:87-103
 * Target Lines: 87, 89-98 (multi-module path), 99 (condition false), 103
 * Functional Purpose: Validates that wasm_resolve_symbols() correctly handles the
 *                     WASM_ENABLE_MULTI_MODULE path where import_func_linked is true.
 * Call Path: wasm_resolve_symbols() [MULTI_MODULE enabled]
 * Coverage Goal: Exercise multi-module conditional compilation path
 ******/
TEST_F(EnhancedWasmRuntimeTest, wasm_resolve_symbols_MultiModuleLinked_ReturnsTrue) {
    WASMModule *module = CreateMockModuleWithImports(2, false);
    ASSERT_NE(nullptr, module);

    // Set functions as not func_ptr_linked but import_func_linked
    for (uint32 i = 0; i < module->import_function_count; i++) {
        WASMFunctionImport *import = &module->import_functions[i].u.function;
        import->func_ptr_linked = NULL;
        import->import_func_linked = (WASMFunction*)0x1;  // This should make linked = true
    }

    bool result = wasm_resolve_symbols(module);
    ASSERT_TRUE(result);

    DestroyMockModule(module);
}
#endif

/******
 * Test Case: wasm_resolve_symbols_SingleIteration_ReturnsTrue
 * Source: core/iwasm/interpreter/wasm_runtime.c:87-103
 * Target Lines: 87, 89-93 (single iteration), 99 (condition false), 103
 * Functional Purpose: Validates that wasm_resolve_symbols() correctly handles a module
 *                     with exactly one import function that is already linked.
 * Call Path: wasm_resolve_symbols() [PUBLIC API]
 * Coverage Goal: Exercise single function iteration path
 ******/
TEST_F(EnhancedWasmRuntimeTest, wasm_resolve_symbols_SingleIteration_ReturnsTrue) {
    WASMModule *module = CreateMockModuleWithImports(1, true);
    ASSERT_NE(nullptr, module);

    bool result = wasm_resolve_symbols(module);
    ASSERT_TRUE(result);

    DestroyMockModule(module);
}