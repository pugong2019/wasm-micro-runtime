/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

#include "bh_platform.h"
#include "wasm_c_api.h"
#include "wasm_c_api_internal.h"

#ifndef own
#define own
#endif

class FunctionTypeTest : public testing::Test {
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

// Test 1: Function type with no parameters and no results
TEST_F(FunctionTypeTest, FunctionType_NoParamsNoResults_CreatesCorrectly) {
    wasm_functype_t* func_type = wasm_functype_new_0_0();
    ASSERT_NE(nullptr, func_type);
    
    const wasm_valtype_vec_t* params = wasm_functype_params(func_type);
    const wasm_valtype_vec_t* results = wasm_functype_results(func_type);
    
    ASSERT_NE(nullptr, params);
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(0, params->size);
    ASSERT_EQ(0, results->size);
    
    wasm_functype_delete(func_type);
}

// Test 2: Function type with one parameter, no results
TEST_F(FunctionTypeTest, FunctionType_OneParamNoResult_CreatesCorrectly) {
    wasm_functype_t* func_type = wasm_functype_new_1_0(wasm_valtype_new_i32());
    ASSERT_NE(nullptr, func_type);
    
    const wasm_valtype_vec_t* params = wasm_functype_params(func_type);
    const wasm_valtype_vec_t* results = wasm_functype_results(func_type);
    
    ASSERT_NE(nullptr, params);
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(1, params->size);
    ASSERT_EQ(0, results->size);
    ASSERT_EQ(WASM_I32, wasm_valtype_kind(params->data[0]));
    
    wasm_functype_delete(func_type);
}

// Test 3: Function type with no parameters, one result
TEST_F(FunctionTypeTest, FunctionType_NoParamOneResult_CreatesCorrectly) {
    wasm_functype_t* func_type = wasm_functype_new_0_1(wasm_valtype_new_f64());
    ASSERT_NE(nullptr, func_type);
    
    const wasm_valtype_vec_t* params = wasm_functype_params(func_type);
    const wasm_valtype_vec_t* results = wasm_functype_results(func_type);
    
    ASSERT_NE(nullptr, params);
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(0, params->size);
    ASSERT_EQ(1, results->size);
    ASSERT_EQ(WASM_F64, wasm_valtype_kind(results->data[0]));
    
    wasm_functype_delete(func_type);
}

// Test 4: Function type with one parameter and one result
TEST_F(FunctionTypeTest, FunctionType_OneParamOneResult_CreatesCorrectly) {
    wasm_functype_t* func_type = wasm_functype_new_1_1(
        wasm_valtype_new_i64(), wasm_valtype_new_f32());
    ASSERT_NE(nullptr, func_type);
    
    const wasm_valtype_vec_t* params = wasm_functype_params(func_type);
    const wasm_valtype_vec_t* results = wasm_functype_results(func_type);
    
    ASSERT_NE(nullptr, params);
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(1, params->size);
    ASSERT_EQ(1, results->size);
    ASSERT_EQ(WASM_I64, wasm_valtype_kind(params->data[0]));
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(results->data[0]));
    
    wasm_functype_delete(func_type);
}

// Test 5: Function type with two parameters and two results
TEST_F(FunctionTypeTest, FunctionType_TwoParamsTwoResults_CreatesCorrectly) {
    wasm_functype_t* func_type = wasm_functype_new_2_2(
        wasm_valtype_new_i32(), wasm_valtype_new_i64(),
        wasm_valtype_new_f32(), wasm_valtype_new_f64());
    ASSERT_NE(nullptr, func_type);
    
    const wasm_valtype_vec_t* params = wasm_functype_params(func_type);
    const wasm_valtype_vec_t* results = wasm_functype_results(func_type);
    
    ASSERT_NE(nullptr, params);
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(2, params->size);
    ASSERT_EQ(2, results->size);
    
    ASSERT_EQ(WASM_I32, wasm_valtype_kind(params->data[0]));
    ASSERT_EQ(WASM_I64, wasm_valtype_kind(params->data[1]));
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(results->data[0]));
    ASSERT_EQ(WASM_F64, wasm_valtype_kind(results->data[1]));
    
    wasm_functype_delete(func_type);
}

// Test 6: Function type with three parameters and three results
TEST_F(FunctionTypeTest, FunctionType_ThreeParamsThreeResults_CreatesCorrectly) {
    wasm_functype_t* func_type = wasm_functype_new_3_3(
        wasm_valtype_new_i32(), wasm_valtype_new_i64(), wasm_valtype_new_f32(),
        wasm_valtype_new_f64(), wasm_valtype_new_funcref(), wasm_valtype_new_externref());
    ASSERT_NE(nullptr, func_type);
    
    const wasm_valtype_vec_t* params = wasm_functype_params(func_type);
    const wasm_valtype_vec_t* results = wasm_functype_results(func_type);
    
    ASSERT_NE(nullptr, params);
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(3, params->size);
    ASSERT_EQ(3, results->size);
    
    ASSERT_EQ(WASM_I32, wasm_valtype_kind(params->data[0]));
    ASSERT_EQ(WASM_I64, wasm_valtype_kind(params->data[1]));
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(params->data[2]));
    ASSERT_EQ(WASM_F64, wasm_valtype_kind(results->data[0]));
    ASSERT_EQ(WASM_FUNCREF, wasm_valtype_kind(results->data[1]));
    ASSERT_EQ(WASM_EXTERNREF, wasm_valtype_kind(results->data[2]));
    
    wasm_functype_delete(func_type);
}

// Test 7: Function type with custom parameter and result vectors
TEST_F(FunctionTypeTest, FunctionType_CustomVectors_CreatesCorrectly) {
    // Create parameter vector
    wasm_valtype_vec_t params = { 0 };
    wasm_valtype_t* param_types[4] = {
        wasm_valtype_new_i32(),
        wasm_valtype_new_i64(),
        wasm_valtype_new_f32(),
        wasm_valtype_new_f64()
    };
    wasm_valtype_vec_new(&params, 4, param_types);
    
    // Create result vector
    wasm_valtype_vec_t results = { 0 };
    wasm_valtype_t* result_types[2] = {
        wasm_valtype_new_funcref(),
        wasm_valtype_new_externref()
    };
    wasm_valtype_vec_new(&results, 2, result_types);
    
    // Create function type
    wasm_functype_t* func_type = wasm_functype_new(&params, &results);
    ASSERT_NE(nullptr, func_type);
    
    const wasm_valtype_vec_t* retrieved_params = wasm_functype_params(func_type);
    const wasm_valtype_vec_t* retrieved_results = wasm_functype_results(func_type);
    
    ASSERT_NE(nullptr, retrieved_params);
    ASSERT_NE(nullptr, retrieved_results);
    ASSERT_EQ(4, retrieved_params->size);
    ASSERT_EQ(2, retrieved_results->size);
    
    ASSERT_EQ(WASM_I32, wasm_valtype_kind(retrieved_params->data[0]));
    ASSERT_EQ(WASM_I64, wasm_valtype_kind(retrieved_params->data[1]));
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(retrieved_params->data[2]));
    ASSERT_EQ(WASM_F64, wasm_valtype_kind(retrieved_params->data[3]));
    ASSERT_EQ(WASM_FUNCREF, wasm_valtype_kind(retrieved_results->data[0]));
    ASSERT_EQ(WASM_EXTERNREF, wasm_valtype_kind(retrieved_results->data[1]));
    
    wasm_functype_delete(func_type);
}

// Test 8: Function type copying
TEST_F(FunctionTypeTest, FunctionType_Copy_CreatesIndependentCopy) {
    wasm_functype_t* original = wasm_functype_new_2_1(
        wasm_valtype_new_i32(), wasm_valtype_new_f64(),
        wasm_valtype_new_i64());
    ASSERT_NE(nullptr, original);
    
    wasm_functype_t* copy = wasm_functype_copy(original);
    ASSERT_NE(nullptr, copy);
    
    // Verify both have same signature
    const wasm_valtype_vec_t* orig_params = wasm_functype_params(original);
    const wasm_valtype_vec_t* copy_params = wasm_functype_params(copy);
    const wasm_valtype_vec_t* orig_results = wasm_functype_results(original);
    const wasm_valtype_vec_t* copy_results = wasm_functype_results(copy);
    
    ASSERT_EQ(orig_params->size, copy_params->size);
    ASSERT_EQ(orig_results->size, copy_results->size);
    
    for (size_t i = 0; i < orig_params->size; ++i) {
        ASSERT_EQ(wasm_valtype_kind(orig_params->data[i]),
                  wasm_valtype_kind(copy_params->data[i]));
    }
    
    for (size_t i = 0; i < orig_results->size; ++i) {
        ASSERT_EQ(wasm_valtype_kind(orig_results->data[i]),
                  wasm_valtype_kind(copy_results->data[i]));
    }
    
    wasm_functype_delete(original);
    wasm_functype_delete(copy);
}

// Test 9: Function type with reference types
TEST_F(FunctionTypeTest, FunctionType_ReferenceTypes_HandlesCorrectly) {
    wasm_functype_t* func_type = wasm_functype_new_2_2(
        wasm_valtype_new_funcref(), wasm_valtype_new_externref(),
        wasm_valtype_new_externref(), wasm_valtype_new_funcref());
    ASSERT_NE(nullptr, func_type);
    
    const wasm_valtype_vec_t* params = wasm_functype_params(func_type);
    const wasm_valtype_vec_t* results = wasm_functype_results(func_type);
    
    ASSERT_EQ(WASM_FUNCREF, wasm_valtype_kind(params->data[0]));
    ASSERT_EQ(WASM_EXTERNREF, wasm_valtype_kind(params->data[1]));
    ASSERT_EQ(WASM_EXTERNREF, wasm_valtype_kind(results->data[0]));
    ASSERT_EQ(WASM_FUNCREF, wasm_valtype_kind(results->data[1]));
    
    wasm_functype_delete(func_type);
}

// Test 10: Function type with mixed numeric and reference types
TEST_F(FunctionTypeTest, FunctionType_MixedTypes_HandlesCorrectly) {
    wasm_functype_t* func_type = wasm_functype_new_3_3(
        wasm_valtype_new_i32(), wasm_valtype_new_funcref(), wasm_valtype_new_f64(),
        wasm_valtype_new_externref(), wasm_valtype_new_i64(), wasm_valtype_new_f32());
    ASSERT_NE(nullptr, func_type);
    
    const wasm_valtype_vec_t* params = wasm_functype_params(func_type);
    const wasm_valtype_vec_t* results = wasm_functype_results(func_type);
    
    ASSERT_EQ(WASM_I32, wasm_valtype_kind(params->data[0]));
    ASSERT_EQ(WASM_FUNCREF, wasm_valtype_kind(params->data[1]));
    ASSERT_EQ(WASM_F64, wasm_valtype_kind(params->data[2]));
    ASSERT_EQ(WASM_EXTERNREF, wasm_valtype_kind(results->data[0]));
    ASSERT_EQ(WASM_I64, wasm_valtype_kind(results->data[1]));
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(results->data[2]));
    
    wasm_functype_delete(func_type);
}

// Test 11: Function type parameter validation
TEST_F(FunctionTypeTest, FunctionType_ParameterValidation_WorksCorrectly) {
    // Test with null parameter vector
    wasm_valtype_vec_t results = { 0 };
    wasm_valtype_t* result_types[1] = { wasm_valtype_new_i32() };
    wasm_valtype_vec_new(&results, 1, result_types);
    
    wasm_functype_t* func_type = wasm_functype_new(nullptr, &results);
    // Implementation may handle this differently, just ensure no crash
    if (func_type) {
        wasm_functype_delete(func_type);
    }
}

// Test 12: Function type result validation
TEST_F(FunctionTypeTest, FunctionType_ResultValidation_WorksCorrectly) {
    // Test with null result vector
    wasm_valtype_vec_t params = { 0 };
    wasm_valtype_t* param_types[1] = { wasm_valtype_new_i32() };
    wasm_valtype_vec_new(&params, 1, param_types);
    
    wasm_functype_t* func_type = wasm_functype_new(&params, nullptr);
    // Implementation may handle this differently, just ensure no crash
    if (func_type) {
        wasm_functype_delete(func_type);
    }
}

// Test 13: Function type deletion safety
TEST_F(FunctionTypeTest, FunctionType_DeletionSafety_HandlesNull) {
    // Should not crash when deleting null function type
    wasm_functype_delete(nullptr);
}

// Test 14: Function type as external type
TEST_F(FunctionTypeTest, FunctionType_AsExternalType_ConvertsCorrectly) {
    wasm_functype_t* func_type = wasm_functype_new_1_1(
        wasm_valtype_new_i32(), wasm_valtype_new_i64());
    ASSERT_NE(nullptr, func_type);
    
    wasm_externtype_t* extern_type = wasm_functype_as_externtype(func_type);
    ASSERT_NE(nullptr, extern_type);
    ASSERT_EQ(WASM_EXTERN_FUNC, wasm_externtype_kind(extern_type));
    
    // Convert back
    wasm_functype_t* converted_back = wasm_externtype_as_functype(extern_type);
    ASSERT_NE(nullptr, converted_back);
    
    wasm_functype_delete(func_type);
}

// Test 15: Function type equality comparison
TEST_F(FunctionTypeTest, FunctionType_Equality_ComparesCorrectly) {
    wasm_functype_t* func_type1 = wasm_functype_new_2_1(
        wasm_valtype_new_i32(), wasm_valtype_new_f64(),
        wasm_valtype_new_i64());
    
    wasm_functype_t* func_type2 = wasm_functype_new_2_1(
        wasm_valtype_new_i32(), wasm_valtype_new_f64(),
        wasm_valtype_new_i64());
    
    wasm_functype_t* func_type3 = wasm_functype_new_2_1(
        wasm_valtype_new_f32(), wasm_valtype_new_f64(),
        wasm_valtype_new_i64());
    
    ASSERT_NE(nullptr, func_type1);
    ASSERT_NE(nullptr, func_type2);
    ASSERT_NE(nullptr, func_type3);
    
    // Same signatures should have matching parameters and results
    const wasm_valtype_vec_t* params1 = wasm_functype_params(func_type1);
    const wasm_valtype_vec_t* params2 = wasm_functype_params(func_type2);
    const wasm_valtype_vec_t* params3 = wasm_functype_params(func_type3);
    
    ASSERT_EQ(params1->size, params2->size);
    ASSERT_EQ(wasm_valtype_kind(params1->data[0]), wasm_valtype_kind(params2->data[0]));
    ASSERT_NE(wasm_valtype_kind(params1->data[0]), wasm_valtype_kind(params3->data[0]));
    
    wasm_functype_delete(func_type1);
    wasm_functype_delete(func_type2);
    wasm_functype_delete(func_type3);
}

// Test 16: Complex function signatures
TEST_F(FunctionTypeTest, FunctionType_ComplexSignatures_HandlesCorrectly) {
    // Create a complex function type with many parameters and results
    wasm_valtype_vec_t params = { 0 };
    wasm_valtype_t* param_types[8] = {
        wasm_valtype_new_i32(), wasm_valtype_new_i64(),
        wasm_valtype_new_f32(), wasm_valtype_new_f64(),
        wasm_valtype_new_funcref(), wasm_valtype_new_externref(),
        wasm_valtype_new_i32(), wasm_valtype_new_f32()
    };
    wasm_valtype_vec_new(&params, 8, param_types);
    
    wasm_valtype_vec_t results = { 0 };
    wasm_valtype_t* result_types[6] = {
        wasm_valtype_new_f64(), wasm_valtype_new_i64(),
        wasm_valtype_new_externref(), wasm_valtype_new_funcref(),
        wasm_valtype_new_f32(), wasm_valtype_new_i32()
    };
    wasm_valtype_vec_new(&results, 6, result_types);
    
    wasm_functype_t* func_type = wasm_functype_new(&params, &results);
    ASSERT_NE(nullptr, func_type);
    
    const wasm_valtype_vec_t* retrieved_params = wasm_functype_params(func_type);
    const wasm_valtype_vec_t* retrieved_results = wasm_functype_results(func_type);
    
    ASSERT_EQ(8, retrieved_params->size);
    ASSERT_EQ(6, retrieved_results->size);
    
    // Verify all parameter types
    wasm_valkind_t expected_params[] = {
        WASM_I32, WASM_I64, WASM_F32, WASM_F64,
        WASM_FUNCREF, WASM_EXTERNREF, WASM_I32, WASM_F32
    };
    
    for (size_t i = 0; i < 8; ++i) {
        ASSERT_EQ(expected_params[i], wasm_valtype_kind(retrieved_params->data[i]));
    }
    
    // Verify all result types
    wasm_valkind_t expected_results[] = {
        WASM_F64, WASM_I64, WASM_EXTERNREF,
        WASM_FUNCREF, WASM_F32, WASM_I32
    };
    
    for (size_t i = 0; i < 6; ++i) {
        ASSERT_EQ(expected_results[i], wasm_valtype_kind(retrieved_results->data[i]));
    }
    
    wasm_functype_delete(func_type);
}

// Test 17: Function type memory management
TEST_F(FunctionTypeTest, FunctionType_MemoryManagement_WorksCorrectly) {
    std::vector<wasm_functype_t*> func_types;
    
    // Create many function types
    for (int i = 0; i < 100; ++i) {
        wasm_valkind_t param_kind = (wasm_valkind_t)(i % 6);
        wasm_valkind_t result_kind = (wasm_valkind_t)((i + 1) % 6);
        
        wasm_functype_t* func_type = wasm_functype_new_1_1(
            wasm_valtype_new(param_kind),
            wasm_valtype_new(result_kind));
        
        ASSERT_NE(nullptr, func_type);
        func_types.push_back(func_type);
    }
    
    // Verify all function types
    for (size_t i = 0; i < func_types.size(); ++i) {
        const wasm_valtype_vec_t* params = wasm_functype_params(func_types[i]);
        const wasm_valtype_vec_t* results = wasm_functype_results(func_types[i]);
        
        ASSERT_EQ(1, params->size);
        ASSERT_EQ(1, results->size);
        
        wasm_valkind_t expected_param = (wasm_valkind_t)(i % 6);
        wasm_valkind_t expected_result = (wasm_valkind_t)((i + 1) % 6);
        
        ASSERT_EQ(expected_param, wasm_valtype_kind(params->data[0]));
        ASSERT_EQ(expected_result, wasm_valtype_kind(results->data[0]));
    }
    
    // Clean up all function types
    for (auto* func_type : func_types) {
        wasm_functype_delete(func_type);
    }
}

// Test 18: Function type with empty vectors
TEST_F(FunctionTypeTest, FunctionType_EmptyVectors_HandlesCorrectly) {
    wasm_valtype_vec_t empty_params = { 0 };
    wasm_valtype_vec_t empty_results = { 0 };
    
    wasm_functype_t* func_type = wasm_functype_new(&empty_params, &empty_results);
    ASSERT_NE(nullptr, func_type);
    
    const wasm_valtype_vec_t* params = wasm_functype_params(func_type);
    const wasm_valtype_vec_t* results = wasm_functype_results(func_type);
    
    ASSERT_NE(nullptr, params);
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(0, params->size);
    ASSERT_EQ(0, results->size);
    
    wasm_functype_delete(func_type);
}

// Test 19: Function type copy null handling
TEST_F(FunctionTypeTest, FunctionType_CopyNull_HandlesGracefully) {
    wasm_functype_t* null_copy = wasm_functype_copy(nullptr);
    ASSERT_EQ(nullptr, null_copy);
}

// Test 20: Function type lifecycle management
TEST_F(FunctionTypeTest, FunctionType_Lifecycle_CompleteFlow) {
    // Create function type
    wasm_functype_t* original = wasm_functype_new_2_2(
        wasm_valtype_new_i32(), wasm_valtype_new_f64(),
        wasm_valtype_new_i64(), wasm_valtype_new_f32());
    ASSERT_NE(nullptr, original);
    
    // Copy function type
    wasm_functype_t* copy = wasm_functype_copy(original);
    ASSERT_NE(nullptr, copy);
    
    // Convert to external type
    wasm_externtype_t* extern_type = wasm_functype_as_externtype(copy);
    ASSERT_NE(nullptr, extern_type);
    ASSERT_EQ(WASM_EXTERN_FUNC, wasm_externtype_kind(extern_type));
    
    // Convert back to function type
    wasm_functype_t* converted = wasm_externtype_as_functype(extern_type);
    ASSERT_NE(nullptr, converted);
    
    // Verify consistency across all conversions
    const wasm_valtype_vec_t* orig_params = wasm_functype_params(original);
    const wasm_valtype_vec_t* conv_params = wasm_functype_params(converted);
    
    ASSERT_EQ(orig_params->size, conv_params->size);
    for (size_t i = 0; i < orig_params->size; ++i) {
        ASSERT_EQ(wasm_valtype_kind(orig_params->data[i]),
                  wasm_valtype_kind(conv_params->data[i]));
    }
    
    // Clean up
    wasm_functype_delete(original);
    wasm_functype_delete(copy);
}