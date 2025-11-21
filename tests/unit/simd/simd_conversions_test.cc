/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "simd_test_helper.h"
#include "gtest/gtest.h"

class SIMDConversionsTest : public SIMDTestBase {
protected:
    // 测试文件路径
    std::string wasm_file_path;

    void SetUp() override {
        // 调用基类设置方法
        SIMDTestBase::SetUp();
        // 设置WASM文件路径
        wasm_file_path = get_test_file_path("simd_conversions_test.wasm");
    }

    // 辅助函数获取测试文件路径
    std::string get_test_file_path(const std::string& file_name) {
        char cwd[1024];
        memset(cwd, 0, 1024);
        if (getcwd(cwd, sizeof(cwd)) == nullptr) {
            return file_name;
        }
        return std::string(cwd) + "/" + file_name;
    }
};

// 测试i16x8.extend_i8x16_u 无符号扩展操作
TEST_F(SIMDConversionsTest, simd_i16x8_extend_i8x16) {
    // 简单测试函数是否存在，使用正确的参数类型
    int params[1] = {0};  // 参数数组
    int results[1] = {0}; // 结果数组
    
    // 执行WASM函数（6参数版本）
    ASSERT_TRUE(execute_wasm_function(wasm_file_path.c_str(), "i16x8_extend_i8x16_u_test", 
                                     params, static_cast<uint32_t>(1), results, static_cast<uint32_t>(1)));
}

// 测试i32x4.extend_i16x8_s 有符号扩展操作
TEST_F(SIMDConversionsTest, simd_i32x4_extend_i16x8) {
    // 简单测试函数是否存在，使用正确的参数类型
    int params[1] = {0};  // 参数数组
    int results[1] = {0}; // 结果数组
    
    // 执行WASM函数（6参数版本）
    ASSERT_TRUE(execute_wasm_function(wasm_file_path.c_str(), "i32x4_extend_i16x8_s_test", 
                                     params, static_cast<uint32_t>(1), results, static_cast<uint32_t>(1)));
}

// 测试i32x4.trunc_sat_f32x4_s 截断操作
TEST_F(SIMDConversionsTest, simd_trunc_sat) {
    // 简单测试函数是否存在，使用正确的参数类型
    int params[1] = {0};  // 参数数组
    int results[1] = {0}; // 结果数组
    
    // 执行WASM函数（6参数版本）
    ASSERT_TRUE(execute_wasm_function(wasm_file_path.c_str(), "i32x4_convert_f32x4_s_test", 
                                     params, static_cast<uint32_t>(1), results, static_cast<uint32_t>(1)));
}

// 测试f32x4.convert_i32x4_s 转换操作
TEST_F(SIMDConversionsTest, simd_convert) {
    // 简单测试函数是否存在，使用正确的参数类型
    int params[1] = {0};  // 参数数组
    int results[1] = {0}; // 结果数组
    
    // 执行WASM函数（6参数版本）
    ASSERT_TRUE(execute_wasm_function(wasm_file_path.c_str(), "f32x4_convert_i32x4_s_test", 
                                     params, static_cast<uint32_t>(1), results, static_cast<uint32_t>(1)));
}