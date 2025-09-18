/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <gtest/gtest.h>
#include <cstring>
#include <vector>
#include <algorithm>

#include "bh_platform.h"
#include "wasm_c_api.h"
#include "wasm_c_api_internal.h"

#ifndef own
#define own
#endif

class VectorTest : public testing::Test {
protected:
    void SetUp() override {
        bh_log_set_verbose_level(5);
        engine = wasm_engine_new();
        ASSERT_NE(nullptr, engine);
        store = wasm_store_new(engine);
        ASSERT_NE(nullptr, store);
    }
    
    void TearDown() override {
        if (store) {
            wasm_store_delete(store);
            store = nullptr;
        }
        if (engine) {
            wasm_engine_delete(engine);
            engine = nullptr;
        }
    }
    
    wasm_engine_t* engine = nullptr;
    wasm_store_t* store = nullptr;
};

// Test 1: Byte vector zero size handling
TEST_F(VectorTest, ByteVec_ZeroSize_HandlesCorrectly) {
    wasm_byte_vec_t byte_vec = { 0 };
    wasm_byte_vec_new_uninitialized(&byte_vec, 0);
    
    ASSERT_EQ(nullptr, byte_vec.data);
    ASSERT_EQ(0, byte_vec.size);
    
    wasm_byte_vec_delete(&byte_vec);
    ASSERT_EQ(nullptr, byte_vec.data);
    ASSERT_EQ(0, byte_vec.size);
}

// Test 2: Byte vector normal size allocation
TEST_F(VectorTest, ByteVec_NormalSize_AllocatesCorrectly) {
    wasm_byte_vec_t byte_vec = { 0 };
    wasm_byte_vec_new_uninitialized(&byte_vec, 100);
    
    ASSERT_NE(nullptr, byte_vec.data);
    ASSERT_EQ(100, byte_vec.size);
    
    // Test data accessibility
    byte_vec.data[0] = 'H';
    byte_vec.data[1] = 'e';
    byte_vec.data[2] = 'l';
    byte_vec.data[3] = 'l';
    byte_vec.data[4] = 'o';
    byte_vec.data[5] = '\0';
    
    ASSERT_STREQ("Hello", (char*)byte_vec.data);
    
    wasm_byte_vec_delete(&byte_vec);
    ASSERT_EQ(nullptr, byte_vec.data);
    ASSERT_EQ(0, byte_vec.size);
}

// Test 3: Byte vector large size allocation
TEST_F(VectorTest, ByteVec_LargeSize_AllocatesSuccessfully) {
    wasm_byte_vec_t byte_vec = { 0 };
    const size_t large_size = 1024 * 1024; // 1MB
    wasm_byte_vec_new_uninitialized(&byte_vec, large_size);
    
    ASSERT_NE(nullptr, byte_vec.data);
    ASSERT_EQ(large_size, byte_vec.size);
    
    // Test boundary access
    byte_vec.data[0] = 0xAA;
    byte_vec.data[large_size - 1] = 0xBB;
    
    ASSERT_EQ(0xAA, byte_vec.data[0]);
    ASSERT_EQ(0xBB, byte_vec.data[large_size - 1]);
    
    wasm_byte_vec_delete(&byte_vec);
}

// Test 4: Byte vector initialization with data
TEST_F(VectorTest, ByteVec_InitWithData_CopiesCorrectly) {
    const char* test_data = "Test vector data initialization";
    const size_t data_len = strlen(test_data);
    
    wasm_byte_vec_t byte_vec = { 0 };
    wasm_byte_vec_new(&byte_vec, data_len, (wasm_byte_t*)test_data);
    
    ASSERT_NE(nullptr, byte_vec.data);
    ASSERT_EQ(data_len, byte_vec.size);
    ASSERT_EQ(0, memcmp(byte_vec.data, test_data, data_len));
    
    wasm_byte_vec_delete(&byte_vec);
}

// Test 5: Byte vector copy operation
TEST_F(VectorTest, ByteVec_Copy_CreatesIndependentCopy) {
    wasm_byte_vec_t original = { 0 };
    const char* test_data = "Original data";
    wasm_byte_vec_new(&original, strlen(test_data), (wasm_byte_t*)test_data);
    
    wasm_byte_vec_t copy = { 0 };
    wasm_byte_vec_copy(&copy, &original);
    
    ASSERT_NE(nullptr, copy.data);
    ASSERT_EQ(original.size, copy.size);
    ASSERT_NE(original.data, copy.data); // Different memory
    ASSERT_EQ(0, memcmp(original.data, copy.data, original.size));
    
    // Modify original to verify independence
    original.data[0] = 'X';
    ASSERT_NE(original.data[0], copy.data[0]);
    
    wasm_byte_vec_delete(&original);
    wasm_byte_vec_delete(&copy);
}

// Test 6: Valtype vector zero size
TEST_F(VectorTest, ValtypeVec_ZeroSize_HandlesCorrectly) {
    wasm_valtype_vec_t valtype_vec = { 0 };
    wasm_valtype_vec_new_uninitialized(&valtype_vec, 0);
    
    ASSERT_EQ(nullptr, valtype_vec.data);
    ASSERT_EQ(0, valtype_vec.size);
    
    wasm_valtype_vec_delete(&valtype_vec);
}

// Test 7: Valtype vector normal allocation
TEST_F(VectorTest, ValtypeVec_NormalSize_AllocatesCorrectly) {
    wasm_valtype_vec_t valtype_vec = { 0 };
    wasm_valtype_vec_new_uninitialized(&valtype_vec, 5);
    
    ASSERT_NE(nullptr, valtype_vec.data);
    ASSERT_EQ(5, valtype_vec.size);
    
    // Populate with different types
    valtype_vec.data[0] = wasm_valtype_new_i32();
    valtype_vec.data[1] = wasm_valtype_new_i64();
    valtype_vec.data[2] = wasm_valtype_new_f32();
    valtype_vec.data[3] = wasm_valtype_new_f64();
    valtype_vec.data[4] = wasm_valtype_new_funcref();
    
    ASSERT_EQ(WASM_I32, wasm_valtype_kind(valtype_vec.data[0]));
    ASSERT_EQ(WASM_I64, wasm_valtype_kind(valtype_vec.data[1]));
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(valtype_vec.data[2]));
    ASSERT_EQ(WASM_F64, wasm_valtype_kind(valtype_vec.data[3]));
    ASSERT_EQ(WASM_FUNCREF, wasm_valtype_kind(valtype_vec.data[4]));
    
    wasm_valtype_vec_delete(&valtype_vec);
}

// Test 8: Valtype vector initialization with data
TEST_F(VectorTest, ValtypeVec_InitWithData_CopiesCorrectly) {
    wasm_valtype_t* types[3] = {
        wasm_valtype_new_i32(),
        wasm_valtype_new_f64(),
        wasm_valtype_new_externref()
    };
    
    wasm_valtype_vec_t valtype_vec = { 0 };
    wasm_valtype_vec_new(&valtype_vec, 3, types);
    
    ASSERT_NE(nullptr, valtype_vec.data);
    ASSERT_EQ(3, valtype_vec.size);
    ASSERT_EQ(WASM_I32, wasm_valtype_kind(valtype_vec.data[0]));
    ASSERT_EQ(WASM_F64, wasm_valtype_kind(valtype_vec.data[1]));
    ASSERT_EQ(WASM_EXTERNREF, wasm_valtype_kind(valtype_vec.data[2]));
    
    wasm_valtype_vec_delete(&valtype_vec);
}

// Test 9: Valtype vector copy operation
TEST_F(VectorTest, ValtypeVec_Copy_CreatesIndependentCopy) {
    wasm_valtype_t* types[2] = {
        wasm_valtype_new_i64(),
        wasm_valtype_new_funcref()
    };
    
    wasm_valtype_vec_t original = { 0 };
    wasm_valtype_vec_new(&original, 2, types);
    
    wasm_valtype_vec_t copy = { 0 };
    wasm_valtype_vec_copy(&copy, &original);
    
    ASSERT_NE(nullptr, copy.data);
    ASSERT_EQ(original.size, copy.size);
    ASSERT_NE(original.data, copy.data);
    ASSERT_EQ(WASM_I64, wasm_valtype_kind(copy.data[0]));
    ASSERT_EQ(WASM_FUNCREF, wasm_valtype_kind(copy.data[1]));
    
    wasm_valtype_vec_delete(&original);
    wasm_valtype_vec_delete(&copy);
}

// Test 10: Functype vector operations
TEST_F(VectorTest, FunctypeVec_Operations_WorkCorrectly) {
    wasm_functype_vec_t functype_vec = { 0 };
    wasm_functype_vec_new_uninitialized(&functype_vec, 2);
    
    ASSERT_NE(nullptr, functype_vec.data);
    ASSERT_EQ(2, functype_vec.size);
    
    // Create function types
    functype_vec.data[0] = wasm_functype_new_0_0();
    functype_vec.data[1] = wasm_functype_new_1_1(
        wasm_valtype_new_i32(), wasm_valtype_new_i64());
    
    ASSERT_NE(nullptr, functype_vec.data[0]);
    ASSERT_NE(nullptr, functype_vec.data[1]);
    
    wasm_functype_vec_delete(&functype_vec);
}

// Test 11: Globaltype vector operations
TEST_F(VectorTest, GlobaltypeVec_Operations_WorkCorrectly) {
    wasm_globaltype_vec_t globaltype_vec = { 0 };
    wasm_globaltype_vec_new_uninitialized(&globaltype_vec, 3);
    
    ASSERT_NE(nullptr, globaltype_vec.data);
    ASSERT_EQ(3, globaltype_vec.size);
    
    // Create global types
    globaltype_vec.data[0] = wasm_globaltype_new(wasm_valtype_new_i32(), WASM_CONST);
    globaltype_vec.data[1] = wasm_globaltype_new(wasm_valtype_new_f64(), WASM_VAR);
    globaltype_vec.data[2] = wasm_globaltype_new(wasm_valtype_new_externref(), WASM_CONST);
    
    ASSERT_NE(nullptr, globaltype_vec.data[0]);
    ASSERT_NE(nullptr, globaltype_vec.data[1]);
    ASSERT_NE(nullptr, globaltype_vec.data[2]);
    
    ASSERT_EQ(WASM_CONST, wasm_globaltype_mutability(globaltype_vec.data[0]));
    ASSERT_EQ(WASM_VAR, wasm_globaltype_mutability(globaltype_vec.data[1]));
    ASSERT_EQ(WASM_CONST, wasm_globaltype_mutability(globaltype_vec.data[2]));
    
    wasm_globaltype_vec_delete(&globaltype_vec);
}

// Test 12: Tabletype vector operations
TEST_F(VectorTest, TabletypeVec_Operations_WorkCorrectly) {
    wasm_tabletype_vec_t tabletype_vec = { 0 };
    wasm_tabletype_vec_new_uninitialized(&tabletype_vec, 2);
    
    ASSERT_NE(nullptr, tabletype_vec.data);
    ASSERT_EQ(2, tabletype_vec.size);
    
    // Create table types with limits
    wasm_limits_t limits1 = { 10, 100 };
    wasm_limits_t limits2 = { 5, UINT32_MAX };
    
    tabletype_vec.data[0] = wasm_tabletype_new(wasm_valtype_new_funcref(), &limits1);
    tabletype_vec.data[1] = wasm_tabletype_new(wasm_valtype_new_externref(), &limits2);
    
    ASSERT_NE(nullptr, tabletype_vec.data[0]);
    ASSERT_NE(nullptr, tabletype_vec.data[1]);
    
    const wasm_limits_t* retrieved_limits1 = wasm_tabletype_limits(tabletype_vec.data[0]);
    const wasm_limits_t* retrieved_limits2 = wasm_tabletype_limits(tabletype_vec.data[1]);
    
    ASSERT_EQ(10, retrieved_limits1->min);
    ASSERT_EQ(100, retrieved_limits1->max);
    ASSERT_EQ(5, retrieved_limits2->min);
    ASSERT_EQ(UINT32_MAX, retrieved_limits2->max);
    
    wasm_tabletype_vec_delete(&tabletype_vec);
}

// Test 13: Memorytype vector operations
TEST_F(VectorTest, MemorytypeVec_Operations_WorkCorrectly) {
    wasm_memorytype_vec_t memorytype_vec = { 0 };
    wasm_memorytype_vec_new_uninitialized(&memorytype_vec, 2);
    
    ASSERT_NE(nullptr, memorytype_vec.data);
    ASSERT_EQ(2, memorytype_vec.size);
    
    // Create memory types with different limits
    wasm_limits_t limits1 = { 1, 10 };      // 64KB to 640KB
    wasm_limits_t limits2 = { 2, UINT32_MAX }; // 128KB to max
    
    memorytype_vec.data[0] = wasm_memorytype_new(&limits1);
    memorytype_vec.data[1] = wasm_memorytype_new(&limits2);
    
    ASSERT_NE(nullptr, memorytype_vec.data[0]);
    ASSERT_NE(nullptr, memorytype_vec.data[1]);
    
    const wasm_limits_t* retrieved_limits1 = wasm_memorytype_limits(memorytype_vec.data[0]);
    const wasm_limits_t* retrieved_limits2 = wasm_memorytype_limits(memorytype_vec.data[1]);
    
    ASSERT_EQ(1, retrieved_limits1->min);
    ASSERT_EQ(10, retrieved_limits1->max);
    ASSERT_EQ(2, retrieved_limits2->min);
    ASSERT_EQ(UINT32_MAX, retrieved_limits2->max);
    
    wasm_memorytype_vec_delete(&memorytype_vec);
}

// Test 14: Externtype vector operations
TEST_F(VectorTest, ExterntypeVec_Operations_WorkCorrectly) {
    wasm_externtype_vec_t externtype_vec = { 0 };
    wasm_externtype_vec_new_uninitialized(&externtype_vec, 3);
    
    ASSERT_NE(nullptr, externtype_vec.data);
    ASSERT_EQ(3, externtype_vec.size);
    
    // Create external types
    wasm_functype_t* functype = wasm_functype_new_0_1(wasm_valtype_new_i32());
    wasm_globaltype_t* globaltype = wasm_globaltype_new(wasm_valtype_new_f64(), WASM_CONST);
    wasm_limits_t limits = { 1, 100 };
    wasm_memorytype_t* memorytype = wasm_memorytype_new(&limits);
    
    externtype_vec.data[0] = wasm_functype_as_externtype(functype);
    externtype_vec.data[1] = wasm_globaltype_as_externtype(globaltype);
    externtype_vec.data[2] = wasm_memorytype_as_externtype(memorytype);
    
    ASSERT_NE(nullptr, externtype_vec.data[0]);
    ASSERT_NE(nullptr, externtype_vec.data[1]);
    ASSERT_NE(nullptr, externtype_vec.data[2]);
    
    ASSERT_EQ(WASM_EXTERN_FUNC, wasm_externtype_kind(externtype_vec.data[0]));
    ASSERT_EQ(WASM_EXTERN_GLOBAL, wasm_externtype_kind(externtype_vec.data[1]));
    ASSERT_EQ(WASM_EXTERN_MEMORY, wasm_externtype_kind(externtype_vec.data[2]));
    
    wasm_externtype_vec_delete(&externtype_vec);
}

// Test 15: Importtype vector operations
TEST_F(VectorTest, ImporttypeVec_Operations_WorkCorrectly) {
    wasm_importtype_vec_t importtype_vec = { 0 };
    wasm_importtype_vec_new_uninitialized(&importtype_vec, 2);
    
    ASSERT_NE(nullptr, importtype_vec.data);
    ASSERT_EQ(2, importtype_vec.size);
    
    // Create import types
    wasm_byte_vec_t module_name1, name1;
    wasm_byte_vec_new(&module_name1, 4, (wasm_byte_t*)"math");
    wasm_byte_vec_new(&name1, 3, (wasm_byte_t*)"add");
    
    wasm_functype_t* functype = wasm_functype_new_2_1(
        wasm_valtype_new_i32(), wasm_valtype_new_i32(), wasm_valtype_new_i32());
    
    importtype_vec.data[0] = wasm_importtype_new(
        &module_name1, &name1, wasm_functype_as_externtype(functype));
    
    wasm_byte_vec_t module_name2, name2;
    wasm_byte_vec_new(&module_name2, 3, (wasm_byte_t*)"env");
    wasm_byte_vec_new(&name2, 6, (wasm_byte_t*)"global");
    
    wasm_globaltype_t* globaltype = wasm_globaltype_new(wasm_valtype_new_f64(), WASM_CONST);
    
    importtype_vec.data[1] = wasm_importtype_new(
        &module_name2, &name2, wasm_globaltype_as_externtype(globaltype));
    
    ASSERT_NE(nullptr, importtype_vec.data[0]);
    ASSERT_NE(nullptr, importtype_vec.data[1]);
    
    wasm_importtype_vec_delete(&importtype_vec);
}

// Test 16: Exporttype vector operations
TEST_F(VectorTest, ExporttypeVec_Operations_WorkCorrectly) {
    wasm_exporttype_vec_t exporttype_vec = { 0 };
    wasm_exporttype_vec_new_uninitialized(&exporttype_vec, 2);
    
    ASSERT_NE(nullptr, exporttype_vec.data);
    ASSERT_EQ(2, exporttype_vec.size);
    
    // Create export types
    wasm_byte_vec_t name1, name2;
    wasm_byte_vec_new(&name1, 8, (wasm_byte_t*)"exported");
    wasm_byte_vec_new(&name2, 6, (wasm_byte_t*)"memory");
    
    wasm_functype_t* functype = wasm_functype_new_0_0();
    wasm_limits_t limits = { 1, 10 };
    wasm_memorytype_t* memorytype = wasm_memorytype_new(&limits);
    
    exporttype_vec.data[0] = wasm_exporttype_new(
        &name1, wasm_functype_as_externtype(functype));
    exporttype_vec.data[1] = wasm_exporttype_new(
        &name2, wasm_memorytype_as_externtype(memorytype));
    
    ASSERT_NE(nullptr, exporttype_vec.data[0]);
    ASSERT_NE(nullptr, exporttype_vec.data[1]);
    
    wasm_exporttype_vec_delete(&exporttype_vec);
}

// Test 17: Vector boundary conditions
TEST_F(VectorTest, Vector_BoundaryConditions_HandleCorrectly) {
    // Test maximum reasonable size
    wasm_byte_vec_t large_vec = { 0 };
    const size_t max_size = 10 * 1024 * 1024; // 10MB
    wasm_byte_vec_new_uninitialized(&large_vec, max_size);
    
    ASSERT_NE(nullptr, large_vec.data);
    ASSERT_EQ(max_size, large_vec.size);
    
    // Test boundary access
    large_vec.data[0] = 0xFF;
    large_vec.data[max_size - 1] = 0xAA;
    
    ASSERT_EQ(0xFF, large_vec.data[0]);
    ASSERT_EQ(0xAA, large_vec.data[max_size - 1]);
    
    wasm_byte_vec_delete(&large_vec);
}

// Test 18: Vector null handling
TEST_F(VectorTest, Vector_NullHandling_WorksCorrectly) {
    wasm_byte_vec_t byte_vec = { 0 };
    
    // Test delete on uninitialized vector
    wasm_byte_vec_delete(&byte_vec);
    ASSERT_EQ(nullptr, byte_vec.data);
    ASSERT_EQ(0, byte_vec.size);
    
    // Test delete on null data
    byte_vec.data = nullptr;
    byte_vec.size = 0;
    wasm_byte_vec_delete(&byte_vec);
}

// Test 19: Vector copy with empty source
TEST_F(VectorTest, Vector_CopyEmpty_HandlesCorrectly) {
    wasm_byte_vec_t empty_vec = { 0 };
    wasm_byte_vec_t copy_vec = { 0 };
    
    wasm_byte_vec_copy(&copy_vec, &empty_vec);
    
    ASSERT_EQ(nullptr, copy_vec.data);
    ASSERT_EQ(0, copy_vec.size);
    
    wasm_byte_vec_delete(&copy_vec);
}

// Test 20: Vector memory integrity
TEST_F(VectorTest, Vector_MemoryIntegrity_MaintainsCorrectly) {
    std::vector<wasm_byte_vec_t> vectors;
    
    // Create multiple vectors
    for (int i = 0; i < 10; ++i) {
        wasm_byte_vec_t vec = { 0 };
        wasm_byte_vec_new_uninitialized(&vec, 100 + i * 10);
        
        ASSERT_NE(nullptr, vec.data);
        ASSERT_EQ(100 + i * 10, vec.size);
        
        // Fill with pattern
        for (size_t j = 0; j < vec.size; ++j) {
            vec.data[j] = (wasm_byte_t)(i * 10 + j % 10);
        }
        
        vectors.push_back(vec);
    }
    
    // Verify all vectors maintain their data
    for (size_t i = 0; i < vectors.size(); ++i) {
        for (size_t j = 0; j < vectors[i].size; ++j) {
            wasm_byte_t expected = (wasm_byte_t)(i * 10 + j % 10);
            ASSERT_EQ(expected, vectors[i].data[j]);
        }
    }
    
    // Clean up
    for (auto& vec : vectors) {
        wasm_byte_vec_delete(&vec);
    }
}