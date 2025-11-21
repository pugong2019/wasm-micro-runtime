/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "simd_test_helper.h"
#include "gtest/gtest.h"

#include "wasm_export.h"
#include "aot_export.h"

class simd_floating_point_test_suit : public SIMDTestBase
{
  protected:
    virtual void SetUp() override {
        // 调用基类的SetUp方法初始化WAMR运行时
        SIMDTestBase::SetUp();
    }

    virtual void TearDown() override {
        // 调用基类的TearDown方法清理WAMR运行时
        SIMDTestBase::TearDown();
    }
};

// Test 32-bit floating point arithmetic operations
TEST_F(simd_floating_point_test_suit, simd_f32x4_arith_operations)
{
    // 使用基类提供的WASM文件路径和执行方法
    const char* wasm_file = getWASMFilename("simd_floating_point_test.wasm");
    
    // 测试f32x4_add操作
    {   
        // 准备参数 - 两个包含4个float的向量
        float vec1[4] = {1.0f, 2.0f, 3.0f, 4.0f};
        float vec2[4] = {5.0f, 6.0f, 7.0f, 8.0f};
        
        // 期望结果 - 两个向量的元素级加法
        float expected[4] = {6.0f, 8.0f, 10.0f, 12.0f};
        
        // 执行WASM函数
        float result[4] = {0};
        ASSERT_TRUE(execute_wasm_function(wasm_file, "f32x4_add_test", 
                                         (uint8_t*)vec1, sizeof(vec1),
                                         (uint8_t*)vec2, sizeof(vec2),
                                         (uint8_t*)result, sizeof(result)));
        
        // 验证结果
        for (int i = 0; i < 4; i++) {
            ASSERT_FLOAT_EQ(expected[i], result[i]);
        }
    }
    
    // 测试f32x4_sub操作
    {   
        float vec1[4] = {10.0f, 8.0f, 6.0f, 4.0f};
        float vec2[4] = {5.0f, 3.0f, 2.0f, 1.0f};
        float expected[4] = {5.0f, 5.0f, 4.0f, 3.0f};
        float result[4] = {0};
        
        ASSERT_TRUE(execute_wasm_function(wasm_file, "f32x4_sub_test", 
                                         (uint8_t*)vec1, sizeof(vec1),
                                         (uint8_t*)vec2, sizeof(vec2),
                                         (uint8_t*)result, sizeof(result)));
        
        for (int i = 0; i < 4; i++) {
            ASSERT_FLOAT_EQ(expected[i], result[i]);
        }
    }
}

// Test 64-bit floating point arithmetic operations
TEST_F(simd_floating_point_test_suit, simd_f64x2_arith_operations)
{
    const char* wasm_file = getWASMFilename("simd_floating_point_test.wasm");
    
    // 测试f64x2_add操作
    {   
        double vec1[2] = {1.5, 2.5};
        double vec2[2] = {3.5, 4.5};
        double expected[2] = {5.0, 7.0};
        double result[2] = {0};
        
        ASSERT_TRUE(execute_wasm_function(wasm_file, "f64x2_add_test", 
                                         (uint8_t*)vec1, sizeof(vec1),
                                         (uint8_t*)vec2, sizeof(vec2),
                                         (uint8_t*)result, sizeof(result)));
        
        for (int i = 0; i < 2; i++) {
            ASSERT_DOUBLE_EQ(expected[i], result[i]);
        }
    }
    
    // 测试f64x2_sub操作
    {   
        double vec1[2] = {10.5, 8.5};
        double vec2[2] = {3.5, 2.5};
        double expected[2] = {7.0, 6.0};
        double result[2] = {0};
        
        ASSERT_TRUE(execute_wasm_function(wasm_file, "f64x2_sub_test", 
                                         (uint8_t*)vec1, sizeof(vec1),
                                         (uint8_t*)vec2, sizeof(vec2),
                                         (uint8_t*)result, sizeof(result)));
        
        for (int i = 0; i < 2; i++) {
            ASSERT_DOUBLE_EQ(expected[i], result[i]);
        }
    }
}

// Test 32-bit floating point comparison operations
TEST_F(simd_floating_point_test_suit, simd_f32x4_compare_operations)
{
    const char* wasm_file = getWASMFilename("simd_floating_point_test.wasm");
    
    // 测试f32x4_eq操作 (相等比较)
    {   
        float vec1[4] = {1.0f, 2.0f, 3.0f, 4.0f};
        float vec2[4] = {1.0f, 2.0f, 5.0f, 4.0f};
        uint32_t expected[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0x0, 0xFFFFFFFF}; // 全1表示true，全0表示false
        uint32_t result[4] = {0};
        
        ASSERT_TRUE(execute_wasm_function(wasm_file, "f32x4_eq_test", 
                                         (uint8_t*)vec1, sizeof(vec1),
                                         (uint8_t*)vec2, sizeof(vec2),
                                         (uint8_t*)result, sizeof(result)));
        
        for (int i = 0; i < 4; i++) {
            ASSERT_EQ(expected[i], result[i]);
        }
    }
    
    // 测试f32x4_lt操作 (小于比较)
    {   
        float vec1[4] = {1.0f, 3.0f, 5.0f, 7.0f};
        float vec2[4] = {2.0f, 2.0f, 6.0f, 6.0f};
        uint32_t expected[4] = {0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x0}; // 全1表示true，全0表示false
        uint32_t result[4] = {0};
        
        ASSERT_TRUE(execute_wasm_function(wasm_file, "f32x4_lt_test", 
                                         (uint8_t*)vec1, sizeof(vec1),
                                         (uint8_t*)vec2, sizeof(vec2),
                                         (uint8_t*)result, sizeof(result)));
        
        for (int i = 0; i < 4; i++) {
            ASSERT_EQ(expected[i], result[i]);
        }
    }
}

// Test 64-bit floating point comparison operations
TEST_F(simd_floating_point_test_suit, simd_f64x2_compare_operations)
{
    const char* wasm_file = getWASMFilename("simd_floating_point_test.wasm");
    
    // 测试f64x2_eq操作 (相等比较)
    {
        double vec1[2] = {1.5, 2.5};
        double vec2[2] = {1.5, 3.5};
        uint32_t expected[2] = {0xFFFFFFFF, 0x0}; // 全1表示true，全0表示false
        uint32_t result[2] = {0};
        
        ASSERT_TRUE(execute_wasm_function(wasm_file, "f64x2_eq_test", 
                                        (uint8_t*)vec1, sizeof(vec1),
                                        (uint8_t*)vec2, sizeof(vec2),
                                        (uint8_t*)result, sizeof(result)));
        
        for (int i = 0; i < 2; i++) {
            ASSERT_EQ(expected[i], result[i]);
        }
    }
    
    // 测试f64x2_lt操作 (小于比较)
    {
        double vec1[2] = {2.5, 5.5};
        double vec2[2] = {3.5, 4.5};
        uint32_t expected[2] = {0xFFFFFFFF, 0x0}; // 全1表示true，全0表示false
        uint32_t result[2] = {0};
        
        ASSERT_TRUE(execute_wasm_function(wasm_file, "f64x2_lt_test", 
                                        (uint8_t*)vec1, sizeof(vec1),
                                        (uint8_t*)vec2, sizeof(vec2),
                                        (uint8_t*)result, sizeof(result)));
        
        for (int i = 0; i < 2; i++) {
            ASSERT_EQ(expected[i], result[i]);
        }
    }
}

// 添加其他测试方法，覆盖更多的浮点运算操作
TEST_F(simd_floating_point_test_suit, simd_f32x4_abs_operations)
{
    const char* wasm_file = getWASMFilename("simd_floating_point_test.wasm");
    
    // 测试f32x4_abs操作 (绝对值)
    {
        float vec[4] = {-1.0f, 2.0f, -3.0f, 4.0f};
        float expected[4] = {1.0f, 2.0f, 3.0f, 4.0f};
        float result[4] = {0};
        
        ASSERT_TRUE(execute_wasm_function(wasm_file, "f32x4_abs_test", 
                                        (uint8_t*)vec, sizeof(vec),
                                        (uint8_t*)result, sizeof(result)));
        
        for (int i = 0; i < 4; i++) {
            ASSERT_FLOAT_EQ(expected[i], result[i]);
        }
    }
}

TEST_F(simd_floating_point_test_suit, simd_f64x2_abs_operations)
{
    const char* wasm_file = getWASMFilename("simd_floating_point_test.wasm");
    
    // 测试f64x2_abs操作 (绝对值)
    {
        double vec[2] = {-1.5, 2.5};
        double expected[2] = {1.5, 2.5};
        double result[2] = {0};
        
        ASSERT_TRUE(execute_wasm_function(wasm_file, "f64x2_abs_test", 
                                        (uint8_t*)vec, sizeof(vec),
                                        (uint8_t*)result, sizeof(result)));
        
        for (int i = 0; i < 2; i++) {
            ASSERT_DOUBLE_EQ(expected[i], result[i]);
        }
    }
}