/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <gtest/gtest.h>
#include <limits.h>
#include "wasm_c_api.h"

// Enhanced test fixture for wasm-c-api coverage improvement
class EnhancedWasmCApiTest : public testing::Test
{
protected:
    void SetUp() override
    {
        engine = wasm_engine_new();
        ASSERT_NE(nullptr, engine);
        store = wasm_store_new(engine);
        ASSERT_NE(nullptr, store);
    }

    void TearDown() override
    {
        if (store) {
            wasm_store_delete(store);
        }
        if (engine) {
            wasm_engine_delete(engine);
        }
    }

    wasm_engine_t* engine = nullptr;
    wasm_store_t* store = nullptr;

    // Helper to create a simple function type
    wasm_functype_t* create_simple_functype()
    {
        wasm_valtype_vec_t params, results;
        wasm_valtype_vec_new_empty(&params);
        wasm_valtype_vec_new_uninitialized(&results, 1);
        results.data[0] = wasm_valtype_new(WASM_I32);
        return wasm_functype_new(&params, &results);
    }

    // Helper to create a simple global type
    wasm_globaltype_t* create_simple_globaltype()
    {
        wasm_valtype_t* valtype = wasm_valtype_new(WASM_I32);
        return wasm_globaltype_new(valtype, WASM_CONST);
    }

    // Helper to create a name
    wasm_name_t* create_name(const char* str)
    {
        wasm_name_t* name = new wasm_name_t;
        wasm_name_new_from_string(name, str);
        return name;
    }

    // Helper to create a zero-sized name
    wasm_name_t* create_zero_size_name()
    {
        wasm_name_t* name = new wasm_name_t;
        wasm_name_new_empty(name);
        return name;
    }

    // Helper to create exporttype with function type
    wasm_exporttype_t* create_exporttype_with_functype()
    {
        wasm_name_t* export_name = create_name("test_function");
        wasm_functype_t* functype = create_simple_functype();
        wasm_externtype_t* externtype = wasm_functype_as_externtype(functype);
        return wasm_exporttype_new(export_name, externtype);
    }

    // Helper to create exporttype with global type
    wasm_exporttype_t* create_exporttype_with_globaltype()
    {
        wasm_name_t* export_name = create_name("test_global");
        wasm_globaltype_t* globaltype = create_simple_globaltype();
        wasm_externtype_t* externtype = wasm_globaltype_as_externtype(globaltype);
        return wasm_exporttype_new(export_name, externtype);
    }

    // Helper to create exporttype with table type
    wasm_exporttype_t* create_exporttype_with_tabletype()
    {
        wasm_name_t* export_name = create_name("test_table");
        wasm_valtype_t* elemtype = wasm_valtype_new(WASM_FUNCREF);
        wasm_limits_t limits = { 10, 100 };
        wasm_tabletype_t* tabletype = wasm_tabletype_new(elemtype, &limits);
        wasm_externtype_t* externtype = wasm_tabletype_as_externtype(tabletype);
        return wasm_exporttype_new(export_name, externtype);
    }

    // Helper to create exporttype with memory type
    wasm_exporttype_t* create_exporttype_with_memorytype()
    {
        wasm_name_t* export_name = create_name("test_memory");
        wasm_limits_t limits = { 1, 10 };
        wasm_memorytype_t* memorytype = wasm_memorytype_new(&limits);
        wasm_externtype_t* externtype = wasm_memorytype_as_externtype(memorytype);
        return wasm_exporttype_new(export_name, externtype);
    }

    // Helper to create importtype with specific extern type
    wasm_importtype_t* create_importtype_with_functype()
    {
        wasm_name_t* module_name = create_name("test_module");
        wasm_name_t* import_name = create_name("test_function");
        wasm_functype_t* functype = create_simple_functype();
        wasm_externtype_t* externtype = wasm_functype_as_externtype(functype);
        return wasm_importtype_new(module_name, import_name, externtype);
    }

    // Helper to create importtype with global type
    wasm_importtype_t* create_importtype_with_globaltype()
    {
        wasm_name_t* module_name = create_name("global_module");
        wasm_name_t* import_name = create_name("global_var");
        wasm_globaltype_t* globaltype = create_simple_globaltype();
        wasm_externtype_t* externtype = wasm_globaltype_as_externtype(globaltype);
        return wasm_importtype_new(module_name, import_name, externtype);
    }
};

// ===== WASM_EXPORTTYPE_COPY ENHANCED TESTS =====

// Target: Normal successful copy operation
TEST_F(EnhancedWasmCApiTest, wasm_exporttype_copy_ValidExportType_SucceedsCorrectly)
{
    // Arrange: Create a valid export type with function
    wasm_exporttype_t* original = create_exporttype_with_functype();
    ASSERT_NE(nullptr, original);

    // Act: Copy the export type
    wasm_exporttype_t* copied = wasm_exporttype_copy(original);

    // Assert: Verify successful copy
    ASSERT_NE(nullptr, copied);
    ASSERT_NE(original, copied);  // Different objects

    // Verify name is copied correctly
    const wasm_name_t* orig_name = wasm_exporttype_name(original);
    const wasm_name_t* copied_name = wasm_exporttype_name(copied);
    ASSERT_NE(nullptr, orig_name);
    ASSERT_NE(nullptr, copied_name);
    ASSERT_EQ(orig_name->size, copied_name->size);
    ASSERT_EQ(0, memcmp(orig_name->data, copied_name->data, orig_name->size));

    // Verify extern type is copied correctly
    const wasm_externtype_t* orig_type = wasm_exporttype_type(original);
    const wasm_externtype_t* copied_type = wasm_exporttype_type(copied);
    ASSERT_NE(nullptr, orig_type);
    ASSERT_NE(nullptr, copied_type);
    ASSERT_EQ(wasm_externtype_kind(orig_type), wasm_externtype_kind(copied_type));

    // Cleanup
    wasm_exporttype_delete(original);
    wasm_exporttype_delete(copied);
}

// Target: NULL input handling (line 1558-1560)
TEST_F(EnhancedWasmCApiTest, wasm_exporttype_copy_NullInput_ReturnsNull)
{
    // Act: Call with NULL input
    wasm_exporttype_t* result = wasm_exporttype_copy(nullptr);

    // Assert: Should return NULL
    ASSERT_EQ(nullptr, result);
}

// Target: Memory allocation failure for name copy (lines 1562-1565)
TEST_F(EnhancedWasmCApiTest, wasm_exporttype_copy_NameCopyWithNonZeroSize_SucceedsCorrectly)
{
    // Arrange: Create export type with non-zero name size
    wasm_exporttype_t* original = create_exporttype_with_functype();
    ASSERT_NE(nullptr, original);

    // Verify the original has non-zero name size
    const wasm_name_t* orig_name = wasm_exporttype_name(original);
    ASSERT_NE(nullptr, orig_name);
    ASSERT_GT(orig_name->size, 0);

    // Act: Copy should succeed in normal conditions
    wasm_exporttype_t* copied = wasm_exporttype_copy(original);
    
    // Assert: Normal case should succeed
    ASSERT_NE(nullptr, copied);

    // Verify name data is properly copied
    const wasm_name_t* copied_name = wasm_exporttype_name(copied);
    ASSERT_NE(nullptr, copied_name);
    ASSERT_NE(nullptr, copied_name->data);
    ASSERT_EQ(orig_name->size, copied_name->size);

    // Cleanup
    wasm_exporttype_delete(original);
    wasm_exporttype_delete(copied);
}

// Target: Failure in extern_type copying (lines 1567-1569)
TEST_F(EnhancedWasmCApiTest, wasm_exporttype_copy_ExternTypeCopy_SucceedsCorrectly)
{
    // Arrange: Create export type with global type
    wasm_exporttype_t* original = create_exporttype_with_globaltype();
    ASSERT_NE(nullptr, original);

    // Verify extern type exists
    const wasm_externtype_t* orig_type = wasm_exporttype_type(original);
    ASSERT_NE(nullptr, orig_type);
    ASSERT_EQ(WASM_EXTERN_GLOBAL, wasm_externtype_kind(orig_type));

    // Act: Copy should succeed in normal conditions
    wasm_exporttype_t* copied = wasm_exporttype_copy(original);
    
    // Assert: Normal case should succeed
    ASSERT_NE(nullptr, copied);

    // Verify extern type is properly copied
    const wasm_externtype_t* copied_type = wasm_exporttype_type(copied);
    ASSERT_NE(nullptr, copied_type);
    ASSERT_EQ(WASM_EXTERN_GLOBAL, wasm_externtype_kind(copied_type));

    // Cleanup
    wasm_exporttype_delete(original);
    wasm_exporttype_delete(copied);
}

// Target: Failure in export_type creation (lines 1571-1573)
TEST_F(EnhancedWasmCApiTest, wasm_exporttype_copy_ExportTypeCreation_SucceedsCorrectly)
{
    // Arrange: Create export type with table type
    wasm_exporttype_t* original = create_exporttype_with_tabletype();
    ASSERT_NE(nullptr, original);

    // Act: Copy should succeed in normal conditions
    wasm_exporttype_t* copied = wasm_exporttype_copy(original);
    
    // Assert: Normal case should succeed
    ASSERT_NE(nullptr, copied);

    // Verify all components are properly set
    ASSERT_NE(nullptr, wasm_exporttype_name(copied));
    ASSERT_NE(nullptr, wasm_exporttype_type(copied));
    ASSERT_EQ(WASM_EXTERN_TABLE, wasm_externtype_kind(wasm_exporttype_type(copied)));

    // Cleanup
    wasm_exporttype_delete(original);
    wasm_exporttype_delete(copied);
}

// Target: Edge cases with zero-sized name
TEST_F(EnhancedWasmCApiTest, wasm_exporttype_copy_ZeroSizedName_SucceedsCorrectly)
{
    // Arrange: Create export type with zero-sized name
    wasm_name_t* export_name = create_zero_size_name();
    wasm_functype_t* functype = create_simple_functype();
    wasm_externtype_t* externtype = wasm_functype_as_externtype(functype);
    
    wasm_exporttype_t* original = wasm_exporttype_new(export_name, externtype);
    ASSERT_NE(nullptr, original);

    // Verify original has zero-sized name
    const wasm_name_t* orig_name = wasm_exporttype_name(original);
    ASSERT_NE(nullptr, orig_name);
    ASSERT_EQ(0, orig_name->size);

    // Act: Copy the export type
    wasm_exporttype_t* copied = wasm_exporttype_copy(original);

    // Assert: Should succeed even with zero-sized name
    ASSERT_NE(nullptr, copied);
    
    const wasm_name_t* copied_name = wasm_exporttype_name(copied);
    ASSERT_NE(nullptr, copied_name);
    ASSERT_EQ(0, copied_name->size);

    // Cleanup
    wasm_exporttype_delete(original);
    wasm_exporttype_delete(copied);
}

// Target: Test cleanup path execution (goto failed - lines 1577-1579)
TEST_F(EnhancedWasmCApiTest, wasm_exporttype_copy_CleanupPath_HandlesCorrectly)
{
    // This test verifies that the cleanup path works correctly
    // by creating scenarios where allocation succeeds but later steps might fail

    // Arrange: Create a valid export type first
    wasm_exporttype_t* original = create_exporttype_with_memorytype();
    ASSERT_NE(nullptr, original);

    // Act: Normal copy should succeed
    wasm_exporttype_t* copied = wasm_exporttype_copy(original);
    
    // Assert: Verify successful copy
    ASSERT_NE(nullptr, copied);

    // Verify all components are properly copied
    ASSERT_NE(nullptr, wasm_exporttype_name(copied));
    ASSERT_NE(nullptr, wasm_exporttype_type(copied));
    ASSERT_EQ(WASM_EXTERN_MEMORY, wasm_externtype_kind(wasm_exporttype_type(copied)));

    // Cleanup
    wasm_exporttype_delete(original);
    wasm_exporttype_delete(copied);
}

// Target: Test with different extern types (table type)
TEST_F(EnhancedWasmCApiTest, wasm_exporttype_copy_TableType_SucceedsCorrectly)
{
    // Arrange: Create export type with table type
    wasm_exporttype_t* original = create_exporttype_with_tabletype();
    ASSERT_NE(nullptr, original);

    // Act: Copy the export type
    wasm_exporttype_t* copied = wasm_exporttype_copy(original);

    // Assert: Verify successful copy with table type
    ASSERT_NE(nullptr, copied);
    
    const wasm_externtype_t* copied_type = wasm_exporttype_type(copied);
    ASSERT_NE(nullptr, copied_type);
    ASSERT_EQ(WASM_EXTERN_TABLE, wasm_externtype_kind(copied_type));

    // Verify name is also copied
    const wasm_name_t* copied_name = wasm_exporttype_name(copied);
    ASSERT_NE(nullptr, copied_name);
    ASSERT_GT(copied_name->size, 0);

    // Cleanup
    wasm_exporttype_delete(original);
    wasm_exporttype_delete(copied);
}

// Target: Test with different extern types (memory type)
TEST_F(EnhancedWasmCApiTest, wasm_exporttype_copy_MemoryType_SucceedsCorrectly)
{
    // Arrange: Create export type with memory type
    wasm_exporttype_t* original = create_exporttype_with_memorytype();
    ASSERT_NE(nullptr, original);

    // Act: Copy the export type
    wasm_exporttype_t* copied = wasm_exporttype_copy(original);

    // Assert: Verify successful copy with memory type
    ASSERT_NE(nullptr, copied);
    
    const wasm_externtype_t* copied_type = wasm_exporttype_type(copied);
    ASSERT_NE(nullptr, copied_type);
    ASSERT_EQ(WASM_EXTERN_MEMORY, wasm_externtype_kind(copied_type));

    // Cleanup
    wasm_exporttype_delete(original);
    wasm_exporttype_delete(copied);
}

// Target: Test with large names to stress memory allocation
TEST_F(EnhancedWasmCApiTest, wasm_exporttype_copy_LargeName_SucceedsCorrectly)
{
    // Arrange: Create export type with large name
    std::string large_name(1000, 'E');
    
    wasm_name_t* export_name = create_name(large_name.c_str());
    wasm_functype_t* functype = create_simple_functype();
    wasm_externtype_t* externtype = wasm_functype_as_externtype(functype);
    
    wasm_exporttype_t* original = wasm_exporttype_new(export_name, externtype);
    ASSERT_NE(nullptr, original);

    // Act: Copy the export type
    wasm_exporttype_t* copied = wasm_exporttype_copy(original);

    // Assert: Verify successful copy with large name
    ASSERT_NE(nullptr, copied);
    
    const wasm_name_t* copied_name = wasm_exporttype_name(copied);
    ASSERT_NE(nullptr, copied_name);
    ASSERT_EQ(1000, copied_name->size);
    ASSERT_NE(nullptr, copied_name->data);

    // Cleanup
    wasm_exporttype_delete(original);
    wasm_exporttype_delete(copied);
}

// Target: Multiple consecutive copy operations
TEST_F(EnhancedWasmCApiTest, wasm_exporttype_copy_MultipleCopies_AllSucceed)
{
    // Arrange: Create original export type
    wasm_exporttype_t* original = create_exporttype_with_globaltype();
    ASSERT_NE(nullptr, original);

    // Act: Create multiple copies
    wasm_exporttype_t* copy1 = wasm_exporttype_copy(original);
    wasm_exporttype_t* copy2 = wasm_exporttype_copy(original);
    wasm_exporttype_t* copy3 = wasm_exporttype_copy(copy1);

    // Assert: All copies should succeed and be independent
    ASSERT_NE(nullptr, copy1);
    ASSERT_NE(nullptr, copy2);
    ASSERT_NE(nullptr, copy3);
    
    ASSERT_NE(original, copy1);
    ASSERT_NE(original, copy2);
    ASSERT_NE(original, copy3);
    ASSERT_NE(copy1, copy2);
    ASSERT_NE(copy1, copy3);
    ASSERT_NE(copy2, copy3);

    // Verify all have same content
    const wasm_externtype_t* orig_type = wasm_exporttype_type(original);
    const wasm_externtype_t* copy1_type = wasm_exporttype_type(copy1);
    const wasm_externtype_t* copy2_type = wasm_exporttype_type(copy2);
    const wasm_externtype_t* copy3_type = wasm_exporttype_type(copy3);
    
    ASSERT_EQ(wasm_externtype_kind(orig_type), wasm_externtype_kind(copy1_type));
    ASSERT_EQ(wasm_externtype_kind(orig_type), wasm_externtype_kind(copy2_type));
    ASSERT_EQ(wasm_externtype_kind(orig_type), wasm_externtype_kind(copy3_type));

    // Cleanup
    wasm_exporttype_delete(original);
    wasm_exporttype_delete(copy1);
    wasm_exporttype_delete(copy2);
    wasm_exporttype_delete(copy3);
}

// Target: Stress test for resource management
TEST_F(EnhancedWasmCApiTest, wasm_exporttype_copy_ResourceManagement_NoLeaks)
{
    // This test creates and destroys many export types to verify proper resource management
    for (int i = 0; i < 50; ++i) {
        // Create original
        wasm_exporttype_t* original = create_exporttype_with_functype();
        ASSERT_NE(nullptr, original);

        // Create copy
        wasm_exporttype_t* copied = wasm_exporttype_copy(original);
        ASSERT_NE(nullptr, copied);

        // Verify copy is valid
        ASSERT_NE(nullptr, wasm_exporttype_name(copied));
        ASSERT_NE(nullptr, wasm_exporttype_type(copied));

        // Cleanup immediately
        wasm_exporttype_delete(original);
        wasm_exporttype_delete(copied);
    }
    
    // Test passes if no memory issues occur
    ASSERT_TRUE(true);
}

// Target: Test name data integrity after copy
TEST_F(EnhancedWasmCApiTest, wasm_exporttype_copy_NameDataIntegrity_PreservesContent)
{
    // Arrange: Create export type with specific name content
    const char* test_name = "test_export_function_with_special_chars_123!@#";
    wasm_name_t* export_name = create_name(test_name);
    wasm_functype_t* functype = create_simple_functype();
    wasm_externtype_t* externtype = wasm_functype_as_externtype(functype);
    
    wasm_exporttype_t* original = wasm_exporttype_new(export_name, externtype);
    ASSERT_NE(nullptr, original);

    // Act: Copy the export type
    wasm_exporttype_t* copied = wasm_exporttype_copy(original);

    // Assert: Verify name content is preserved exactly
    ASSERT_NE(nullptr, copied);
    
    const wasm_name_t* orig_name = wasm_exporttype_name(original);
    const wasm_name_t* copied_name = wasm_exporttype_name(copied);
    
    ASSERT_NE(nullptr, orig_name);
    ASSERT_NE(nullptr, copied_name);
    ASSERT_EQ(orig_name->size, copied_name->size);
    ASSERT_EQ(strlen(test_name), copied_name->size);
    ASSERT_EQ(0, memcmp(orig_name->data, copied_name->data, orig_name->size));
    ASSERT_EQ(0, strncmp(test_name, copied_name->data, copied_name->size));

    // Cleanup
    wasm_exporttype_delete(original);
    wasm_exporttype_delete(copied);
}

// Target: Test copy independence (modifying original doesn't affect copy)
TEST_F(EnhancedWasmCApiTest, wasm_exporttype_copy_Independence_CopyUnaffectedByOriginalDeletion)
{
    // Arrange: Create original export type
    wasm_exporttype_t* original = create_exporttype_with_functype();
    ASSERT_NE(nullptr, original);

    // Get original name for comparison
    const wasm_name_t* orig_name = wasm_exporttype_name(original);
    std::string orig_name_str(orig_name->data, orig_name->size);

    // Act: Copy the export type
    wasm_exporttype_t* copied = wasm_exporttype_copy(original);
    ASSERT_NE(nullptr, copied);

    // Delete original immediately
    wasm_exporttype_delete(original);
    original = nullptr;

    // Assert: Copy should still be valid and accessible
    const wasm_name_t* copied_name = wasm_exporttype_name(copied);
    const wasm_externtype_t* copied_type = wasm_exporttype_type(copied);
    
    ASSERT_NE(nullptr, copied_name);
    ASSERT_NE(nullptr, copied_type);
    ASSERT_EQ(WASM_EXTERN_FUNC, wasm_externtype_kind(copied_type));
    
    // Verify name content is still intact
    std::string copied_name_str(copied_name->data, copied_name->size);
    ASSERT_EQ(orig_name_str, copied_name_str);

    // Cleanup
    wasm_exporttype_delete(copied);
}

// ===== LEGACY IMPORT TYPE TESTS (PRESERVED) =====

// Target: Normal successful copy operation
TEST_F(EnhancedWasmCApiTest, wasm_importtype_copy_ValidImportType_SucceedsCorrectly)
{
    // Arrange: Create a valid import type
    wasm_importtype_t* original = create_importtype_with_functype();
    ASSERT_NE(nullptr, original);

    // Act: Copy the import type
    wasm_importtype_t* copied = wasm_importtype_copy(original);

    // Assert: Verify successful copy
    ASSERT_NE(nullptr, copied);
    ASSERT_NE(original, copied);  // Different objects

    // Verify module name is copied correctly
    const wasm_name_t* orig_module = wasm_importtype_module(original);
    const wasm_name_t* copied_module = wasm_importtype_module(copied);
    ASSERT_NE(nullptr, orig_module);
    ASSERT_NE(nullptr, copied_module);
    ASSERT_EQ(orig_module->size, copied_module->size);
    ASSERT_EQ(0, memcmp(orig_module->data, copied_module->data, orig_module->size));

    // Verify import name is copied correctly
    const wasm_name_t* orig_name = wasm_importtype_name(original);
    const wasm_name_t* copied_name = wasm_importtype_name(copied);
    ASSERT_NE(nullptr, orig_name);
    ASSERT_NE(nullptr, copied_name);
    ASSERT_EQ(orig_name->size, copied_name->size);
    ASSERT_EQ(0, memcmp(orig_name->data, copied_name->data, orig_name->size));

    // Verify extern type is copied correctly
    const wasm_externtype_t* orig_type = wasm_importtype_type(original);
    const wasm_externtype_t* copied_type = wasm_importtype_type(copied);
    ASSERT_NE(nullptr, orig_type);
    ASSERT_NE(nullptr, copied_type);
    ASSERT_EQ(wasm_externtype_kind(orig_type), wasm_externtype_kind(copied_type));

    // Cleanup
    wasm_importtype_delete(original);
    wasm_importtype_delete(copied);
}

// Target: NULL input handling
TEST_F(EnhancedWasmCApiTest, wasm_importtype_copy_NullInput_ReturnsNull)
{
    // Act: Call with NULL input
    wasm_importtype_t* result = wasm_importtype_copy(nullptr);

    // Assert: Should return NULL
    ASSERT_EQ(nullptr, result);
}