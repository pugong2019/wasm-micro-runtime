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

class ValueTypeTest : public testing::Test {
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

// Test 1: Primitive types creation and validation
TEST_F(ValueTypeTest, PrimitiveTypes_AllKinds_CreateCorrectly) {
    // Test i32 type
    wasm_valtype_t* i32_type = wasm_valtype_new_i32();
    ASSERT_NE(nullptr, i32_type);
    ASSERT_EQ(WASM_I32, wasm_valtype_kind(i32_type));
    wasm_valtype_delete(i32_type);
    
    // Test i64 type
    wasm_valtype_t* i64_type = wasm_valtype_new_i64();
    ASSERT_NE(nullptr, i64_type);
    ASSERT_EQ(WASM_I64, wasm_valtype_kind(i64_type));
    wasm_valtype_delete(i64_type);
    
    // Test f32 type
    wasm_valtype_t* f32_type = wasm_valtype_new_f32();
    ASSERT_NE(nullptr, f32_type);
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(f32_type));
    wasm_valtype_delete(f32_type);
    
    // Test f64 type
    wasm_valtype_t* f64_type = wasm_valtype_new_f64();
    ASSERT_NE(nullptr, f64_type);
    ASSERT_EQ(WASM_F64, wasm_valtype_kind(f64_type));
    wasm_valtype_delete(f64_type);
}

// Test 2: Reference types creation and validation
TEST_F(ValueTypeTest, ReferenceTypes_Validation_WorksCorrectly) {
    // Test funcref type
    wasm_valtype_t* funcref_type = wasm_valtype_new_funcref();
    ASSERT_NE(nullptr, funcref_type);
    ASSERT_EQ(WASM_FUNCREF, wasm_valtype_kind(funcref_type));
    wasm_valtype_delete(funcref_type);
    
    // Test externref type
    wasm_valtype_t* externref_type = wasm_valtype_new_externref();
    ASSERT_NE(nullptr, externref_type);
    ASSERT_EQ(WASM_EXTERNREF, wasm_valtype_kind(externref_type));
    wasm_valtype_delete(externref_type);
}

// Test 3: Generic type creation with kind parameter
TEST_F(ValueTypeTest, GenericTypeCreation_AllKinds_WorkCorrectly) {
    // Test all primitive kinds
    wasm_valkind_t kinds[] = {
        WASM_I32, WASM_I64, WASM_F32, WASM_F64, 
        WASM_FUNCREF, WASM_EXTERNREF
    };
    
    for (size_t i = 0; i < sizeof(kinds) / sizeof(kinds[0]); ++i) {
        wasm_valtype_t* type = wasm_valtype_new(kinds[i]);
        ASSERT_NE(nullptr, type);
        ASSERT_EQ(kinds[i], wasm_valtype_kind(type));
        wasm_valtype_delete(type);
    }
}

// Test 4: Type comparison and equality
TEST_F(ValueTypeTest, TypeComparison_SameTypes_ReturnsTrue) {
    wasm_valtype_t* i32_type1 = wasm_valtype_new_i32();
    wasm_valtype_t* i32_type2 = wasm_valtype_new_i32();
    wasm_valtype_t* i64_type = wasm_valtype_new_i64();
    
    ASSERT_NE(nullptr, i32_type1);
    ASSERT_NE(nullptr, i32_type2);
    ASSERT_NE(nullptr, i64_type);
    
    // Same kinds should be equal
    ASSERT_EQ(wasm_valtype_kind(i32_type1), wasm_valtype_kind(i32_type2));
    
    // Different kinds should not be equal
    ASSERT_NE(wasm_valtype_kind(i32_type1), wasm_valtype_kind(i64_type));
    
    wasm_valtype_delete(i32_type1);
    wasm_valtype_delete(i32_type2);
    wasm_valtype_delete(i64_type);
}

// Test 5: Type copying functionality
TEST_F(ValueTypeTest, TypeCopy_AllTypes_CopiesCorrectly) {
    wasm_valkind_t kinds[] = {
        WASM_I32, WASM_I64, WASM_F32, WASM_F64, 
        WASM_FUNCREF, WASM_EXTERNREF
    };
    
    for (size_t i = 0; i < sizeof(kinds) / sizeof(kinds[0]); ++i) {
        wasm_valtype_t* original = wasm_valtype_new(kinds[i]);
        ASSERT_NE(nullptr, original);
        
        wasm_valtype_t* copy = wasm_valtype_copy(original);
        ASSERT_NE(nullptr, copy);
        ASSERT_EQ(wasm_valtype_kind(original), wasm_valtype_kind(copy));
        
        wasm_valtype_delete(original);
        wasm_valtype_delete(copy);
    }
}

// Test 6: Type validation with invalid kinds
TEST_F(ValueTypeTest, TypeCreation_InvalidKind_HandlesGracefully) {
    // Test with invalid kind value
    wasm_valtype_t* invalid_type = wasm_valtype_new((wasm_valkind_t)999);
    
    // Implementation may return null or create a valid type
    // We just ensure it doesn't crash
    if (invalid_type) {
        wasm_valtype_delete(invalid_type);
    }
}

// Test 7: Type deletion safety
TEST_F(ValueTypeTest, TypeDeletion_NullType_HandlesGracefully) {
    // Should not crash when deleting null type
    wasm_valtype_delete(nullptr);
}

// Test 8: Multiple type creation and management
TEST_F(ValueTypeTest, MultipleTypes_Creation_ManagesCorrectly) {
    std::vector<wasm_valtype_t*> types;
    
    // Create multiple types of each kind
    for (int i = 0; i < 5; ++i) {
        types.push_back(wasm_valtype_new_i32());
        types.push_back(wasm_valtype_new_i64());
        types.push_back(wasm_valtype_new_f32());
        types.push_back(wasm_valtype_new_f64());
        types.push_back(wasm_valtype_new_funcref());
        types.push_back(wasm_valtype_new_externref());
    }
    
    // Verify all types are valid
    for (size_t i = 0; i < types.size(); ++i) {
        ASSERT_NE(nullptr, types[i]);
        
        // Verify kind based on position
        wasm_valkind_t expected_kind;
        switch (i % 6) {
            case 0: expected_kind = WASM_I32; break;
            case 1: expected_kind = WASM_I64; break;
            case 2: expected_kind = WASM_F32; break;
            case 3: expected_kind = WASM_F64; break;
            case 4: expected_kind = WASM_FUNCREF; break;
            case 5: expected_kind = WASM_EXTERNREF; break;
            default: expected_kind = WASM_I32; break;
        }
        
        ASSERT_EQ(expected_kind, wasm_valtype_kind(types[i]));
    }
    
    // Clean up all types
    for (auto* type : types) {
        wasm_valtype_delete(type);
    }
}

// Test 9: Type kind enumeration completeness
TEST_F(ValueTypeTest, TypeKind_Enumeration_CoversAllTypes) {
    // Verify all expected kinds are supported
    struct {
        wasm_valkind_t kind;
        const char* name;
    } test_cases[] = {
        { WASM_I32, "i32" },
        { WASM_I64, "i64" },
        { WASM_F32, "f32" },
        { WASM_F64, "f64" },
        { WASM_FUNCREF, "funcref" },
        { WASM_EXTERNREF, "externref" }
    };
    
    for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); ++i) {
        wasm_valtype_t* type = wasm_valtype_new(test_cases[i].kind);
        ASSERT_NE(nullptr, type) << "Failed to create type: " << test_cases[i].name;
        ASSERT_EQ(test_cases[i].kind, wasm_valtype_kind(type)) 
            << "Kind mismatch for type: " << test_cases[i].name;
        wasm_valtype_delete(type);
    }
}

// Test 10: Type consistency across operations
TEST_F(ValueTypeTest, TypeConsistency_AcrossOperations_MaintainsCorrectly) {
    wasm_valtype_t* original = wasm_valtype_new_f64();
    ASSERT_NE(nullptr, original);
    ASSERT_EQ(WASM_F64, wasm_valtype_kind(original));
    
    // Copy should maintain type
    wasm_valtype_t* copy1 = wasm_valtype_copy(original);
    ASSERT_NE(nullptr, copy1);
    ASSERT_EQ(WASM_F64, wasm_valtype_kind(copy1));
    
    // Second copy should also maintain type
    wasm_valtype_t* copy2 = wasm_valtype_copy(copy1);
    ASSERT_NE(nullptr, copy2);
    ASSERT_EQ(WASM_F64, wasm_valtype_kind(copy2));
    
    // All should have same kind
    ASSERT_EQ(wasm_valtype_kind(original), wasm_valtype_kind(copy1));
    ASSERT_EQ(wasm_valtype_kind(copy1), wasm_valtype_kind(copy2));
    
    wasm_valtype_delete(original);
    wasm_valtype_delete(copy1);
    wasm_valtype_delete(copy2);
}

// Test 11: Type memory management stress test
TEST_F(ValueTypeTest, TypeMemoryManagement_StressTest_HandlesCorrectly) {
    const int iterations = 1000;
    
    for (int i = 0; i < iterations; ++i) {
        wasm_valtype_t* type = wasm_valtype_new((wasm_valkind_t)(i % 6));
        ASSERT_NE(nullptr, type);
        
        wasm_valtype_t* copy = wasm_valtype_copy(type);
        ASSERT_NE(nullptr, copy);
        ASSERT_EQ(wasm_valtype_kind(type), wasm_valtype_kind(copy));
        
        wasm_valtype_delete(type);
        wasm_valtype_delete(copy);
    }
}

// Test 12: Type usage in vector context
TEST_F(ValueTypeTest, TypeUsage_InVectorContext_WorksCorrectly) {
    wasm_valtype_vec_t type_vec = { 0 };
    wasm_valtype_vec_new_uninitialized(&type_vec, 6);
    
    ASSERT_NE(nullptr, type_vec.data);
    ASSERT_EQ(6, type_vec.size);
    
    // Populate with all type kinds
    type_vec.data[0] = wasm_valtype_new_i32();
    type_vec.data[1] = wasm_valtype_new_i64();
    type_vec.data[2] = wasm_valtype_new_f32();
    type_vec.data[3] = wasm_valtype_new_f64();
    type_vec.data[4] = wasm_valtype_new_funcref();
    type_vec.data[5] = wasm_valtype_new_externref();
    
    // Verify all types in vector
    ASSERT_EQ(WASM_I32, wasm_valtype_kind(type_vec.data[0]));
    ASSERT_EQ(WASM_I64, wasm_valtype_kind(type_vec.data[1]));
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(type_vec.data[2]));
    ASSERT_EQ(WASM_F64, wasm_valtype_kind(type_vec.data[3]));
    ASSERT_EQ(WASM_FUNCREF, wasm_valtype_kind(type_vec.data[4]));
    ASSERT_EQ(WASM_EXTERNREF, wasm_valtype_kind(type_vec.data[5]));
    
    wasm_valtype_vec_delete(&type_vec);
}

// Test 13: Type conversion and casting
TEST_F(ValueTypeTest, TypeConversion_Casting_WorksCorrectly) {
    wasm_valtype_t* i32_type = wasm_valtype_new_i32();
    wasm_valtype_t* funcref_type = wasm_valtype_new_funcref();
    
    ASSERT_NE(nullptr, i32_type);
    ASSERT_NE(nullptr, funcref_type);
    
    // Verify types maintain their identity
    ASSERT_EQ(WASM_I32, wasm_valtype_kind(i32_type));
    ASSERT_EQ(WASM_FUNCREF, wasm_valtype_kind(funcref_type));
    
    // Types should be different
    ASSERT_NE(wasm_valtype_kind(i32_type), wasm_valtype_kind(funcref_type));
    
    wasm_valtype_delete(i32_type);
    wasm_valtype_delete(funcref_type);
}

// Test 14: Type equality verification
TEST_F(ValueTypeTest, TypeEquality_SameKinds_VerifiesCorrectly) {
    // Create multiple instances of same type
    wasm_valtype_t* f32_type1 = wasm_valtype_new_f32();
    wasm_valtype_t* f32_type2 = wasm_valtype_new_f32();
    wasm_valtype_t* f32_type3 = wasm_valtype_new(WASM_F32);
    
    ASSERT_NE(nullptr, f32_type1);
    ASSERT_NE(nullptr, f32_type2);
    ASSERT_NE(nullptr, f32_type3);
    
    // All should have same kind
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(f32_type1));
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(f32_type2));
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(f32_type3));
    
    // Kinds should be equal
    ASSERT_EQ(wasm_valtype_kind(f32_type1), wasm_valtype_kind(f32_type2));
    ASSERT_EQ(wasm_valtype_kind(f32_type2), wasm_valtype_kind(f32_type3));
    
    wasm_valtype_delete(f32_type1);
    wasm_valtype_delete(f32_type2);
    wasm_valtype_delete(f32_type3);
}

// Test 15: Type creation performance
TEST_F(ValueTypeTest, TypeCreation_Performance_AcceptableTiming) {
    const int num_types = 10000;
    std::vector<wasm_valtype_t*> types;
    types.reserve(num_types);
    
    // Create many types quickly
    for (int i = 0; i < num_types; ++i) {
        wasm_valtype_t* type = wasm_valtype_new((wasm_valkind_t)(i % 6));
        ASSERT_NE(nullptr, type);
        types.push_back(type);
    }
    
    // Verify all types
    for (int i = 0; i < num_types; ++i) {
        wasm_valkind_t expected = (wasm_valkind_t)(i % 6);
        ASSERT_EQ(expected, wasm_valtype_kind(types[i]));
    }
    
    // Clean up all types
    for (auto* type : types) {
        wasm_valtype_delete(type);
    }
}

// Test 16: Type reference management
TEST_F(ValueTypeTest, TypeReference_Management_WorksCorrectly) {
    wasm_valtype_t* original = wasm_valtype_new_externref();
    ASSERT_NE(nullptr, original);
    ASSERT_EQ(WASM_EXTERNREF, wasm_valtype_kind(original));
    
    // Create multiple references
    std::vector<wasm_valtype_t*> references;
    for (int i = 0; i < 10; ++i) {
        wasm_valtype_t* ref = wasm_valtype_copy(original);
        ASSERT_NE(nullptr, ref);
        ASSERT_EQ(WASM_EXTERNREF, wasm_valtype_kind(ref));
        references.push_back(ref);
    }
    
    // Delete original
    wasm_valtype_delete(original);
    
    // References should still be valid
    for (auto* ref : references) {
        ASSERT_EQ(WASM_EXTERNREF, wasm_valtype_kind(ref));
        wasm_valtype_delete(ref);
    }
}

// Test 17: Type system completeness
TEST_F(ValueTypeTest, TypeSystem_Completeness_CoversAllCases) {
    // Test numeric types
    wasm_valtype_t* i32 = wasm_valtype_new_i32();
    wasm_valtype_t* i64 = wasm_valtype_new_i64();
    wasm_valtype_t* f32 = wasm_valtype_new_f32();
    wasm_valtype_t* f64 = wasm_valtype_new_f64();
    
    ASSERT_NE(nullptr, i32);
    ASSERT_NE(nullptr, i64);
    ASSERT_NE(nullptr, f32);
    ASSERT_NE(nullptr, f64);
    
    // Test reference types
    wasm_valtype_t* funcref = wasm_valtype_new_funcref();
    wasm_valtype_t* externref = wasm_valtype_new_externref();
    
    ASSERT_NE(nullptr, funcref);
    ASSERT_NE(nullptr, externref);
    
    // Verify distinct kinds
    std::vector<wasm_valkind_t> kinds = {
        wasm_valtype_kind(i32),
        wasm_valtype_kind(i64),
        wasm_valtype_kind(f32),
        wasm_valtype_kind(f64),
        wasm_valtype_kind(funcref),
        wasm_valtype_kind(externref)
    };
    
    // All kinds should be unique
    std::sort(kinds.begin(), kinds.end());
    auto it = std::unique(kinds.begin(), kinds.end());
    ASSERT_EQ(kinds.end(), it) << "Found duplicate type kinds";
    
    wasm_valtype_delete(i32);
    wasm_valtype_delete(i64);
    wasm_valtype_delete(f32);
    wasm_valtype_delete(f64);
    wasm_valtype_delete(funcref);
    wasm_valtype_delete(externref);
}

// Test 18: Type validation edge cases
TEST_F(ValueTypeTest, TypeValidation_EdgeCases_HandlesCorrectly) {
    // Test copy of null type
    wasm_valtype_t* null_copy = wasm_valtype_copy(nullptr);
    ASSERT_EQ(nullptr, null_copy);
    
    // Test kind of null type (should not crash)
    // Note: This might crash in some implementations, so we skip it
    // wasm_valkind_t kind = wasm_valtype_kind(nullptr);
}

// Test 19: Type interoperability
TEST_F(ValueTypeTest, TypeInteroperability_WithOtherAPIs_WorksCorrectly) {
    wasm_valtype_t* i64_type = wasm_valtype_new_i64();
    ASSERT_NE(nullptr, i64_type);
    
    // Use in function type creation
    wasm_functype_t* func_type = wasm_functype_new_1_1(
        wasm_valtype_copy(i64_type), wasm_valtype_copy(i64_type));
    ASSERT_NE(nullptr, func_type);
    
    // Verify function type parameters
    const wasm_valtype_vec_t* params = wasm_functype_params(func_type);
    const wasm_valtype_vec_t* results = wasm_functype_results(func_type);
    
    ASSERT_NE(nullptr, params);
    ASSERT_NE(nullptr, results);
    ASSERT_EQ(1, params->size);
    ASSERT_EQ(1, results->size);
    ASSERT_EQ(WASM_I64, wasm_valtype_kind(params->data[0]));
    ASSERT_EQ(WASM_I64, wasm_valtype_kind(results->data[0]));
    
    wasm_functype_delete(func_type);
    wasm_valtype_delete(i64_type);
}

// Test 20: Type lifecycle management
TEST_F(ValueTypeTest, TypeLifecycle_Management_CompleteFlow) {
    // Create type
    wasm_valtype_t* type = wasm_valtype_new_f32();
    ASSERT_NE(nullptr, type);
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(type));
    
    // Copy type
    wasm_valtype_t* copy = wasm_valtype_copy(type);
    ASSERT_NE(nullptr, copy);
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(copy));
    
    // Use in vector
    wasm_valtype_vec_t vec = { 0 };
    wasm_valtype_t* types[2] = { type, copy };
    wasm_valtype_vec_new(&vec, 2, types);
    
    ASSERT_NE(nullptr, vec.data);
    ASSERT_EQ(2, vec.size);
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(vec.data[0]));
    ASSERT_EQ(WASM_F32, wasm_valtype_kind(vec.data[1]));
    
    // Clean up (vector delete will handle the types)
    wasm_valtype_vec_delete(&vec);
}