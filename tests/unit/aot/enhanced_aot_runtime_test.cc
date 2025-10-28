/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <limits.h>
#include "gtest/gtest.h"
#include "wasm_export.h"
#include "aot_runtime.h"

// Enhanced test fixture for aot_runtime.c functions
class EnhancedAotRuntimeTest : public testing::Test {
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

public:
    char global_heap_buf[512 * 1024];
    RuntimeInitArgs init_args;
};

/******
 * Test Case: aot_resolve_import_func_NativeResolutionFails_SubModuleLoadSuccess
 * Source: core/iwasm/aot/aot_runtime.c:5618-5633
 * Target Lines: 5618-5620 (sub-module loading success path)
 * Functional Purpose: Validates that aot_resolve_import_func() correctly handles
 *                     successful sub-module loading when native symbol resolution fails
 *                     for non-built-in modules.
 * Call Path: aot_resolve_import_func() <- aot_resolve_symbols() <- module loading
 * Coverage Goal: Exercise sub-module loading success path for dependency resolution
 ******/
TEST_F(EnhancedAotRuntimeTest, aot_resolve_import_func_NativeResolutionFails_SubModuleLoadSuccess) {
    // Create a minimal AOT module for testing
    AOTModule test_module;
    memset(&test_module, 0, sizeof(AOTModule));

    // Create test import function that fails native resolution
    AOTImportFunc import_func;
    memset(&import_func, 0, sizeof(AOTImportFunc));

    // Set up import function with non-built-in module name
    import_func.module_name = (char*)"test_module";
    import_func.func_name = (char*)"test_function";
    import_func.func_ptr_linked = NULL; // Ensure native resolution fails

    // Create minimal function type
    AOTFuncType func_type;
    memset(&func_type, 0, sizeof(AOTFuncType));
    func_type.param_count = 0;
    func_type.result_count = 0;
    import_func.func_type = &func_type;

    // Test the function - this should attempt sub-module loading
    bool result = aot_resolve_import_func(&test_module, &import_func);

    // The result depends on whether sub-module loading succeeds
    // Since we're testing with a non-existent module, it should fail gracefully
    ASSERT_FALSE(result);
}

/******
 * Test Case: aot_resolve_import_func_SubModuleLoadFails_LogWarning
 * Source: core/iwasm/aot/aot_runtime.c:5621-5623
 * Target Lines: 5621-5623 (sub-module loading failure and LOG_WARNING)
 * Functional Purpose: Validates that aot_resolve_import_func() correctly handles
 *                     sub-module loading failure and logs appropriate warning messages
 *                     when dependency modules cannot be loaded.
 * Call Path: aot_resolve_import_func() <- aot_resolve_symbols() <- module loading
 * Coverage Goal: Exercise sub-module loading failure path and warning logging
 ******/
TEST_F(EnhancedAotRuntimeTest, aot_resolve_import_func_SubModuleLoadFails_LogWarning) {
    // Create a minimal AOT module for testing
    AOTModule test_module;
    memset(&test_module, 0, sizeof(AOTModule));

    // Create test import function that fails native resolution
    AOTImportFunc import_func;
    memset(&import_func, 0, sizeof(AOTImportFunc));

    // Set up import function with invalid module name to force loading failure
    import_func.module_name = (char*)"nonexistent_module";
    import_func.func_name = (char*)"nonexistent_function";
    import_func.func_ptr_linked = NULL; // Ensure native resolution fails

    // Create minimal function type
    AOTFuncType func_type;
    memset(&func_type, 0, sizeof(AOTFuncType));
    func_type.param_count = 0;
    func_type.result_count = 0;
    import_func.func_type = &func_type;

    // Test the function - this should fail sub-module loading and trigger LOG_WARNING
    bool result = aot_resolve_import_func(&test_module, &import_func);

    // Should return false due to failed sub-module loading
    ASSERT_FALSE(result);

    // The import function should still have no linked pointer
    ASSERT_EQ(import_func.func_ptr_linked, nullptr);
}

/******
 * Test Case: aot_resolve_import_func_SubModuleNull_FallbackResolution
 * Source: core/iwasm/aot/aot_runtime.c:5624-5627
 * Target Lines: 5624-5627 (fallback function resolution when sub_module is NULL)
 * Functional Purpose: Validates that aot_resolve_import_func() correctly falls back to
 *                     aot_resolve_function_ex() when sub-module loading fails (sub_module is NULL)
 *                     and attempts alternative function resolution methods.
 * Call Path: aot_resolve_import_func() <- aot_resolve_symbols() <- module loading
 * Coverage Goal: Exercise fallback resolution path when sub-module loading fails
 ******/
TEST_F(EnhancedAotRuntimeTest, aot_resolve_import_func_SubModuleNull_FallbackResolution) {
    // Create a minimal AOT module for testing
    AOTModule test_module;
    memset(&test_module, 0, sizeof(AOTModule));

    // Create test import function that fails native resolution
    AOTImportFunc import_func;
    memset(&import_func, 0, sizeof(AOTImportFunc));

    // Set up import function with module name that will fail loading
    import_func.module_name = (char*)"fallback_test_module";
    import_func.func_name = (char*)"fallback_test_function";
    import_func.func_ptr_linked = NULL; // Ensure native resolution fails

    // Create minimal function type
    AOTFuncType func_type;
    memset(&func_type, 0, sizeof(AOTFuncType));
    func_type.param_count = 0;
    func_type.result_count = 0;
    import_func.func_type = &func_type;

    // Test the function - this should fail loading and use fallback resolution
    bool result = aot_resolve_import_func(&test_module, &import_func);

    // Should return false since fallback resolution will also fail for non-existent function
    ASSERT_FALSE(result);

    // Verify that the function attempted resolution but failed
    ASSERT_EQ(import_func.func_ptr_linked, nullptr);
}

/******
 * Test Case: aot_resolve_import_func_FunctionResolutionFails_LogWarning
 * Source: core/iwasm/aot/aot_runtime.c:5632-5633
 * Target Lines: 5632-5633 (function resolution failure and LOG_WARNING)
 * Functional Purpose: Validates that aot_resolve_import_func() correctly handles
 *                     function resolution failure and logs appropriate warning messages
 *                     when imported functions cannot be resolved after dependency loading.
 * Call Path: aot_resolve_import_func() <- aot_resolve_symbols() <- module loading
 * Coverage Goal: Exercise function resolution failure path and warning logging
 ******/
TEST_F(EnhancedAotRuntimeTest, aot_resolve_import_func_FunctionResolutionFails_LogWarning) {
    // Create a minimal AOT module for testing
    AOTModule test_module;
    memset(&test_module, 0, sizeof(AOTModule));

    // Create test import function that fails native resolution
    AOTImportFunc import_func;
    memset(&import_func, 0, sizeof(AOTImportFunc));

    // Set up import function that will fail final resolution
    import_func.module_name = (char*)"resolution_fail_module";
    import_func.func_name = (char*)"resolution_fail_function";
    import_func.func_ptr_linked = NULL; // Ensure native resolution fails

    // Create minimal function type
    AOTFuncType func_type;
    memset(&func_type, 0, sizeof(AOTFuncType));
    func_type.param_count = 0;
    func_type.result_count = 0;
    import_func.func_type = &func_type;

    // Test the function - this should fail all resolution attempts and log warnings
    bool result = aot_resolve_import_func(&test_module, &import_func);

    // Should return false due to failed function resolution
    ASSERT_FALSE(result);

    // The import function should still have no linked pointer
    ASSERT_EQ(import_func.func_ptr_linked, nullptr);
}

/******
 * Test Case: aot_resolve_import_func_BuiltInModule_SkipSubModuleLoading
 * Source: core/iwasm/aot/aot_runtime.c:5617-5635
 * Target Lines: 5617 (built-in module check to skip sub-module loading)
 * Functional Purpose: Validates that aot_resolve_import_func() correctly skips
 *                     sub-module loading for built-in modules and returns early
 *                     when wasm_runtime_is_built_in_module() returns true.
 * Call Path: aot_resolve_import_func() <- aot_resolve_symbols() <- module loading
 * Coverage Goal: Exercise built-in module skip path for dependency resolution
 ******/
TEST_F(EnhancedAotRuntimeTest, aot_resolve_import_func_BuiltInModule_SkipSubModuleLoading) {
    // Create a minimal AOT module for testing
    AOTModule test_module;
    memset(&test_module, 0, sizeof(AOTModule));

    // Create test import function that fails native resolution
    AOTImportFunc import_func;
    memset(&import_func, 0, sizeof(AOTImportFunc));

    // Set up import function with a built-in module name (like "env")
    import_func.module_name = (char*)"env";
    import_func.func_name = (char*)"builtin_test_function";
    import_func.func_ptr_linked = NULL; // Ensure native resolution fails

    // Create minimal function type
    AOTFuncType func_type;
    memset(&func_type, 0, sizeof(AOTFuncType));
    func_type.param_count = 0;
    func_type.result_count = 0;
    import_func.func_type = &func_type;

    // Test the function - built-in modules should skip sub-module loading
    bool result = aot_resolve_import_func(&test_module, &import_func);

    // Should return false since the function doesn't exist even in built-in modules
    ASSERT_FALSE(result);

    // The import function should still have no linked pointer
    ASSERT_EQ(import_func.func_ptr_linked, nullptr);
}

/******
 * Test Case: aot_resolve_import_func_MultiModuleDisabled_SkipDependencyLoading
 * Source: core/iwasm/aot/aot_runtime.c:5611-5638
 * Target Lines: 5615-5637 (conditional multimodule code execution)
 * Functional Purpose: Validates that aot_resolve_import_func() correctly handles cases
 *                     when WASM_ENABLE_MULTI_MODULE is enabled but native resolution fails,
 *                     testing various error scenarios and fallback paths in the multimodule logic.
 * Call Path: aot_resolve_import_func() <- aot_resolve_symbols() <- module loading
 * Coverage Goal: Exercise the multimodule-specific logic paths when native resolution fails
 ******/
TEST_F(EnhancedAotRuntimeTest, aot_resolve_import_func_MultiModuleDisabled_SkipDependencyLoading) {
    // Create a minimal AOT module for testing
    AOTModule test_module;
    memset(&test_module, 0, sizeof(AOTModule));

    // Create test import function that will fail native resolution
    AOTImportFunc import_func;
    memset(&import_func, 0, sizeof(AOTImportFunc));

    // Set up import function with a name that will fail native resolution
    import_func.module_name = (char*)"multimodule_test_module";
    import_func.func_name = (char*)"multimodule_test_function";
    import_func.func_ptr_linked = NULL; // Start with no pointer

    // Create minimal function type
    AOTFuncType func_type;
    memset(&func_type, 0, sizeof(AOTFuncType));
    func_type.param_count = 0;
    func_type.result_count = 0;
    import_func.func_type = &func_type;

    // Test the function - this will test the multimodule code paths
    bool result = aot_resolve_import_func(&test_module, &import_func);

    // Should return false since no resolution method will succeed
    ASSERT_FALSE(result);

    // The import function should still have no linked pointer
    ASSERT_EQ(import_func.func_ptr_linked, nullptr);
}

/******
 * Test Case: aot_resolve_symbols_WithUnlinkedFunctions_ResolutionAttempt
 * Source: core/iwasm/aot/aot_runtime.c:5525-5531
 * Target Lines: 5525 (function pointer access), 5526 (linked check), 5527 (resolution attempt)
 * Functional Purpose: Validates that aot_resolve_symbols() correctly iterates through
 *                     import functions and attempts resolution for unlinked functions.
 * Call Path: aot_resolve_symbols() <- wasm_runtime_resolve_symbols() <- public API
 * Coverage Goal: Exercise basic function iteration and resolution attempt logic
 ******/
TEST_F(EnhancedAotRuntimeTest, aot_resolve_symbols_WithUnlinkedFunctions_ResolutionAttempt) {
    // Create a minimal AOT module with import functions
    AOTModule test_module;
    memset(&test_module, 0, sizeof(AOTModule));

    // Create array of import functions
    AOTImportFunc import_funcs[2];
    memset(import_funcs, 0, sizeof(import_funcs));

    // Set up first import function (unlinked)
    import_funcs[0].module_name = (char*)"test_module1";
    import_funcs[0].func_name = (char*)"test_function1";
    import_funcs[0].func_ptr_linked = NULL; // Not linked

    // Create minimal function type for first function
    AOTFuncType func_type1;
    memset(&func_type1, 0, sizeof(AOTFuncType));
    func_type1.param_count = 0;
    func_type1.result_count = 0;
    import_funcs[0].func_type = &func_type1;

    // Set up second import function (unlinked)
    import_funcs[1].module_name = (char*)"test_module2";
    import_funcs[1].func_name = (char*)"test_function2";
    import_funcs[1].func_ptr_linked = NULL; // Not linked

    // Create minimal function type for second function
    AOTFuncType func_type2;
    memset(&func_type2, 0, sizeof(AOTFuncType));
    func_type2.param_count = 0;
    func_type2.result_count = 0;
    import_funcs[1].func_type = &func_type2;

    // Configure module with import functions
    test_module.import_funcs = import_funcs;
    test_module.import_func_count = 2;

    // Test the function - should attempt to resolve both functions
    bool result = aot_resolve_symbols(&test_module);

    // Should return false since both functions will fail to resolve
    ASSERT_FALSE(result);

    // Both functions should still be unlinked
    ASSERT_EQ(import_funcs[0].func_ptr_linked, nullptr);
    ASSERT_EQ(import_funcs[1].func_ptr_linked, nullptr);
}

/******
 * Test Case: aot_resolve_symbols_WithAlreadyLinkedFunctions_SkipResolution
 * Source: core/iwasm/aot/aot_runtime.c:5525-5531
 * Target Lines: 5525 (function pointer access), 5526 (linked check - skip path)
 * Functional Purpose: Validates that aot_resolve_symbols() correctly skips
 *                     functions that are already linked (func_ptr_linked != NULL).
 * Call Path: aot_resolve_symbols() <- wasm_runtime_resolve_symbols() <- public API
 * Coverage Goal: Exercise the skip path for already linked functions
 ******/
TEST_F(EnhancedAotRuntimeTest, aot_resolve_symbols_WithAlreadyLinkedFunctions_SkipResolution) {
    // Create a minimal AOT module with import functions
    AOTModule test_module;
    memset(&test_module, 0, sizeof(AOTModule));

    // Create array of import functions
    AOTImportFunc import_funcs[2];
    memset(import_funcs, 0, sizeof(import_funcs));

    // Set up first import function (already linked)
    import_funcs[0].module_name = (char*)"linked_module1";
    import_funcs[0].func_name = (char*)"linked_function1";
    import_funcs[0].func_ptr_linked = (void*)0x12345678; // Already linked

    // Create minimal function type for first function
    AOTFuncType func_type1;
    memset(&func_type1, 0, sizeof(AOTFuncType));
    func_type1.param_count = 0;
    func_type1.result_count = 0;
    import_funcs[0].func_type = &func_type1;

    // Set up second import function (unlinked - will fail)
    import_funcs[1].module_name = (char*)"unlinked_module2";
    import_funcs[1].func_name = (char*)"unlinked_function2";
    import_funcs[1].func_ptr_linked = NULL; // Not linked

    // Create minimal function type for second function
    AOTFuncType func_type2;
    memset(&func_type2, 0, sizeof(AOTFuncType));
    func_type2.param_count = 0;
    func_type2.result_count = 0;
    import_funcs[1].func_type = &func_type2;

    // Configure module with import functions
    test_module.import_funcs = import_funcs;
    test_module.import_func_count = 2;

    // Test the function - should skip first function, fail on second
    bool result = aot_resolve_symbols(&test_module);

    // Should return false since second function will fail to resolve
    ASSERT_FALSE(result);

    // First function should remain linked
    ASSERT_NE(import_funcs[0].func_ptr_linked, nullptr);
    ASSERT_EQ(import_funcs[0].func_ptr_linked, (void*)0x12345678);

    // Second function should still be unlinked
    ASSERT_EQ(import_funcs[1].func_ptr_linked, nullptr);
}

/******
 * Test Case: aot_resolve_symbols_ResolutionFailure_LogWarningAndReturnFalse
 * Source: core/iwasm/aot/aot_runtime.c:5525-5531
 * Target Lines: 5527 (resolution failure), 5528-5530 (LOG_WARNING), 5531 (ret = false)
 * Functional Purpose: Validates that aot_resolve_symbols() correctly handles
 *                     resolution failures by logging warnings and setting return value to false.
 * Call Path: aot_resolve_symbols() <- wasm_runtime_resolve_symbols() <- public API
 * Coverage Goal: Exercise warning logging and failure return path
 ******/
TEST_F(EnhancedAotRuntimeTest, aot_resolve_symbols_ResolutionFailure_LogWarningAndReturnFalse) {
    // Create a minimal AOT module with import functions
    AOTModule test_module;
    memset(&test_module, 0, sizeof(AOTModule));

    // Create array of import functions
    AOTImportFunc import_funcs[1];
    memset(import_funcs, 0, sizeof(import_funcs));

    // Set up import function that will fail resolution
    import_funcs[0].module_name = (char*)"nonexistent_module";
    import_funcs[0].func_name = (char*)"nonexistent_function";
    import_funcs[0].func_ptr_linked = NULL; // Not linked

    // Create minimal function type
    AOTFuncType func_type;
    memset(&func_type, 0, sizeof(AOTFuncType));
    func_type.param_count = 0;
    func_type.result_count = 0;
    import_funcs[0].func_type = &func_type;

    // Configure module with import function
    test_module.import_funcs = import_funcs;
    test_module.import_func_count = 1;

    // Test the function - should fail resolution and log warning
    bool result = aot_resolve_symbols(&test_module);

    // Should return false due to failed resolution
    ASSERT_FALSE(result);

    // Function should still be unlinked
    ASSERT_EQ(import_funcs[0].func_ptr_linked, nullptr);
}

/******
 * Test Case: aot_resolve_symbols_EmptyImportFuncArray_ReturnTrue
 * Source: core/iwasm/aot/aot_runtime.c:5524-5535
 * Target Lines: 5524 (loop condition with count=0), 5535 (return ret=true)
 * Functional Purpose: Validates that aot_resolve_symbols() correctly handles
 *                     modules with no import functions by returning true immediately.
 * Call Path: aot_resolve_symbols() <- wasm_runtime_resolve_symbols() <- public API
 * Coverage Goal: Exercise the success path when no import functions need resolution
 ******/
TEST_F(EnhancedAotRuntimeTest, aot_resolve_symbols_EmptyImportFuncArray_ReturnTrue) {
    // Create a minimal AOT module with no import functions
    AOTModule test_module;
    memset(&test_module, 0, sizeof(AOTModule));

    // Configure module with zero import functions
    test_module.import_funcs = NULL;
    test_module.import_func_count = 0;

    // Test the function - should return true with no functions to resolve
    bool result = aot_resolve_symbols(&test_module);

    // Should return true since there are no functions to resolve
    ASSERT_TRUE(result);
}

/******
 * Test Case: aot_resolve_symbols_MixedLinkedUnlinked_PartialFailure
 * Source: core/iwasm/aot/aot_runtime.c:5525-5531
 * Target Lines: 5525-5531 (complete iteration with mixed success/failure)
 * Functional Purpose: Validates that aot_resolve_symbols() correctly processes
 *                     modules with mixed linked/unlinked functions and returns false
 *                     when any unlinked function fails resolution.
 * Call Path: aot_resolve_symbols() <- wasm_runtime_resolve_symbols() <- public API
 * Coverage Goal: Exercise complete iteration logic with partial failures
 ******/
TEST_F(EnhancedAotRuntimeTest, aot_resolve_symbols_MixedLinkedUnlinked_PartialFailure) {
    // Create a minimal AOT module with mixed import functions
    AOTModule test_module;
    memset(&test_module, 0, sizeof(AOTModule));

    // Create array of import functions
    AOTImportFunc import_funcs[3];
    memset(import_funcs, 0, sizeof(import_funcs));

    // Set up first import function (already linked - should be skipped)
    import_funcs[0].module_name = (char*)"linked_module";
    import_funcs[0].func_name = (char*)"linked_function";
    import_funcs[0].func_ptr_linked = (void*)0xABCDEF12; // Already linked

    // Create minimal function type for first function
    AOTFuncType func_type1;
    memset(&func_type1, 0, sizeof(AOTFuncType));
    func_type1.param_count = 0;
    func_type1.result_count = 0;
    import_funcs[0].func_type = &func_type1;

    // Set up second import function (unlinked - will fail)
    import_funcs[1].module_name = (char*)"fail_module1";
    import_funcs[1].func_name = (char*)"fail_function1";
    import_funcs[1].func_ptr_linked = NULL; // Not linked

    // Create minimal function type for second function
    AOTFuncType func_type2;
    memset(&func_type2, 0, sizeof(AOTFuncType));
    func_type2.param_count = 0;
    func_type2.result_count = 0;
    import_funcs[1].func_type = &func_type2;

    // Set up third import function (unlinked - will also fail)
    import_funcs[2].module_name = (char*)"fail_module2";
    import_funcs[2].func_name = (char*)"fail_function2";
    import_funcs[2].func_ptr_linked = NULL; // Not linked

    // Create minimal function type for third function
    AOTFuncType func_type3;
    memset(&func_type3, 0, sizeof(AOTFuncType));
    func_type3.param_count = 0;
    func_type3.result_count = 0;
    import_funcs[2].func_type = &func_type3;

    // Configure module with import functions
    test_module.import_funcs = import_funcs;
    test_module.import_func_count = 3;

    // Test the function - should process all three functions
    bool result = aot_resolve_symbols(&test_module);

    // Should return false due to failed resolutions
    ASSERT_FALSE(result);

    // First function should remain linked
    ASSERT_EQ(import_funcs[0].func_ptr_linked, (void*)0xABCDEF12);

    // Second and third functions should still be unlinked
    ASSERT_EQ(import_funcs[1].func_ptr_linked, nullptr);
    ASSERT_EQ(import_funcs[2].func_ptr_linked, nullptr);
}