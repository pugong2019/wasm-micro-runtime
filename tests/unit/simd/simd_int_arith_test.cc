/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "simd_test_helper.h"
#include "gtest/gtest.h"

class SIMDIntArithTest : public SIMDTestBase {
protected:
    // 测试文件路径
    std::string wasm_file_path;

    void SetUp() override {
        // 调用基类设置方法
        SIMDTestBase::SetUp();
        
        // 直接使用正确的WASM文件路径 - wasm-apps目录中
        wasm_file_path = "/home/chengnie/works/wasm-micro-runtime/tests/unit/simd/wasm-apps/simd_int_arith_test.wasm";
        printf("Using WASM file path: %s\n", wasm_file_path.c_str());
        
        // 验证文件存在
        ASSERT_TRUE(access(wasm_file_path.c_str(), F_OK) == 0) << "WASM file not found at " << wasm_file_path;
    }
    
    // 辅助函数获取测试文件路径，使用simd_test_helper.h中的增强函数
    std::string get_test_file_path(const std::string& file_name) {
        const char* path = getWASMFilename(file_name.c_str());
        if (path != nullptr) {
            return std::string(path);
        }
        return file_name;
    }
};

// 测试32位整数算术运算
TEST_F(SIMDIntArithTest, simd_i32x4_arith_operations) {
    // 测试i32x4_add操作
    int32_t input1[4] = {1, 2, 3, 4};
    int32_t input2[4] = {5, 6, 7, 8};
    int32_t expected[4] = {6, 8, 10, 12};
    int32_t result[4];
    
    // 参数：2个int32x4向量
    uint32_t param_types[] = {WASM_I32, WASM_I32, WASM_I32, WASM_I32, 
                             WASM_I32, WASM_I32, WASM_I32, WASM_I32};
    uint64_t params[] = {reinterpret_cast<uint64_t>(input1), 
                        reinterpret_cast<uint64_t>(input2)};
    
    // 返回值：1个int32x4向量
    uint32_t ret_types[] = {WASM_I32, WASM_I32, WASM_I32, WASM_I32};
    uint64_t ret[] = {reinterpret_cast<uint64_t>(result)};
    
    // 测试i32x4_sub操作（使用WASM文件中实际导出的函数名）
    int32_t sub_expected[4] = {-4, -4, -4, -4};
    ASSERT_TRUE(execute_wasm_function(wasm_file_path, "test_simd_i32x4_sub", 
                                     param_types, params, 8, 
                                     ret_types, ret, 4));
    ASSERT_TRUE(memcmp(result, sub_expected, sizeof(sub_expected)) == 0);
    
    // 注意：WASM文件只包含sub操作，没有add和mul操作的导出函数
}

// 测试64位整数算术运算
TEST_F(SIMDIntArithTest, simd_i64x2_arith_operations) {
    // 测试i64x2_add操作
    int64_t input1[2] = {1000000000, 2000000000};
    int64_t input2[2] = {3000000000, 4000000000};
    int64_t expected[2] = {4000000000, 6000000000};
    int64_t result[2];
    
    // 参数：2个int64x2向量
    uint32_t param_types[] = {WASM_I64, WASM_I64, WASM_I64, WASM_I64};
    uint64_t params[] = {reinterpret_cast<uint64_t>(input1), 
                        reinterpret_cast<uint64_t>(input2)};
    
    // 返回值：1个int64x2向量
    uint32_t ret_types[] = {WASM_I64, WASM_I64};
    uint64_t ret[] = {reinterpret_cast<uint64_t>(result)};
    
    // 执行WASM函数并验证结果
    ASSERT_TRUE(execute_wasm_function(wasm_file_path, "i64x2_add_test", 
                                     param_types, params, 4, 
                                     ret_types, ret, 2));
    ASSERT_TRUE(memcmp(result, expected, sizeof(expected)) == 0);
    
    // 测试i64x2_sub操作
    int64_t sub_expected[2] = {-2000000000, -2000000000};
    ASSERT_TRUE(execute_wasm_function(wasm_file_path, "i64x2_sub_test", 
                                     param_types, params, 4, 
                                     ret_types, ret, 2));
    ASSERT_TRUE(memcmp(result, sub_expected, sizeof(sub_expected)) == 0);
    
    // 测试i64x2_mul操作
    int64_t mul_expected[2] = {3000000000000000000, 8000000000000000000};
    ASSERT_TRUE(execute_wasm_function(wasm_file_path, "i64x2_mul_test", 
                                     param_types, params, 4, 
                                     ret_types, ret, 2));
    ASSERT_TRUE(memcmp(result, mul_expected, sizeof(mul_expected)) == 0);
}

// 测试8位整数比较操作
TEST_F(SIMDIntArithTest, simd_i8x16_compare_operations) {
    // 测试i8x16_eq操作
    int8_t input1[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int8_t input2[16] = {1, 3, 3, 5, 5, 7, 7, 9, 9, 11, 11, 13, 13, 15, 15, 17};
    // EQ比较结果是全1或全0的掩码
    uint8_t eq_expected[16] = {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
                              0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00};
    uint8_t result[16];
    
    // 参数：2个int8x16向量
    uint32_t param_types[] = {WASM_I32, WASM_I32};
    uint64_t params[] = {reinterpret_cast<uint64_t>(input1), 
                        reinterpret_cast<uint64_t>(input2)};
    
    // 返回值：1个v128掩码
    uint32_t ret_types[] = {WASM_I32};
    uint64_t ret[] = {reinterpret_cast<uint64_t>(result)};
    
    // 执行WASM函数并验证结果
    ASSERT_TRUE(execute_wasm_function(wasm_file_path, "i8x16_eq_test", 
                                     param_types, params, 2, 
                                     ret_types, ret, 1));
    ASSERT_TRUE(memcmp(result, eq_expected, sizeof(eq_expected)) == 0);
    
    // 测试i8x16_lt操作
    uint8_t lt_expected[16] = {0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
                              0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF};
    ASSERT_TRUE(execute_wasm_function(wasm_file_path, "i8x16_lt_test", 
                                     param_types, params, 2, 
                                     ret_types, ret, 1));
    ASSERT_TRUE(memcmp(result, lt_expected, sizeof(lt_expected)) == 0);
}

// 测试16位整数比较操作
TEST_F(SIMDIntArithTest, simd_i16x8_compare_operations) {
    // 测试i16x8_eq操作
    int16_t input1[8] = {100, 200, 300, 400, 500, 600, 700, 800};
    int16_t input2[8] = {100, 250, 300, 450, 500, 650, 700, 850};
    // EQ比较结果是全1或全0的掩码
    uint16_t eq_expected[8] = {0xFFFF, 0x0000, 0xFFFF, 0x0000, 
                              0xFFFF, 0x0000, 0xFFFF, 0x0000};
    uint16_t result[8];
    
    // 参数：2个int16x8向量
    uint32_t param_types[] = {WASM_I32, WASM_I32};
    uint64_t params[] = {reinterpret_cast<uint64_t>(input1), 
                        reinterpret_cast<uint64_t>(input2)};
    
    // 返回值：1个v128掩码
    uint32_t ret_types[] = {WASM_I32};
    uint64_t ret[] = {reinterpret_cast<uint64_t>(result)};
    
    // 执行WASM函数并验证结果
    ASSERT_TRUE(execute_wasm_function(wasm_file_path, "i16x8_eq_test", 
                                     param_types, params, 2, 
                                     ret_types, ret, 1));
    ASSERT_TRUE(memcmp(result, eq_expected, sizeof(eq_expected)) == 0);
    
    // 测试i16x8_lt操作
    uint16_t lt_expected[8] = {0x0000, 0xFFFF, 0x0000, 0xFFFF, 
                              0x0000, 0xFFFF, 0x0000, 0xFFFF};
    ASSERT_TRUE(execute_wasm_function(wasm_file_path, "i16x8_lt_test", 
                                     param_types, params, 2, 
                                     ret_types, ret, 1));
    ASSERT_TRUE(memcmp(result, lt_expected, sizeof(lt_expected)) == 0);
}

// 测试32位整数比较操作
TEST_F(SIMDIntArithTest, simd_i32x4_compare_operations) {
    // 测试i32x4_eq操作
    int32_t input1[4] = {1000, 2000, 3000, 4000};
    int32_t input2[4] = {1000, 2500, 3000, 4500};
    // EQ比较结果是全1或全0的掩码
    int32_t eq_expected[4] = {static_cast<int32_t>(0xFFFFFFFF), 0x00000000, static_cast<int32_t>(0xFFFFFFFF), 0x00000000};
    int32_t result[4];
    
    // 参数：2个int32x4向量
    uint32_t param_types[] = {WASM_I32, WASM_I32, WASM_I32, WASM_I32, 
                             WASM_I32, WASM_I32, WASM_I32, WASM_I32};
    uint64_t params[] = {reinterpret_cast<uint64_t>(input1), 
                        reinterpret_cast<uint64_t>(input2)};
    
    // 返回值：1个int32x4向量
    uint32_t ret_types[] = {WASM_I32, WASM_I32, WASM_I32, WASM_I32};
    uint64_t ret[] = {reinterpret_cast<uint64_t>(result)};
    
    // 执行WASM函数并验证结果
    ASSERT_TRUE(execute_wasm_function(wasm_file_path, "i32x4_eq_test", 
                                     param_types, params, 8, 
                                     ret_types, ret, 4));
    ASSERT_TRUE(memcmp(result, eq_expected, sizeof(eq_expected)) == 0);
    
    // 测试i32x4_lt操作
    int32_t lt_expected[4] = {0x00000000, static_cast<int32_t>(0xFFFFFFFF), 0x00000000, static_cast<int32_t>(0xFFFFFFFF)};
    ASSERT_TRUE(execute_wasm_function(wasm_file_path, "i32x4_lt_test", 
                                     param_types, params, 8, 
                                     ret_types, ret, 4));
    ASSERT_TRUE(memcmp(result, lt_expected, sizeof(lt_expected)) == 0);
}

// 测试64位整数比较操作
TEST_F(SIMDIntArithTest, simd_i64x2_compare_operations) {
    // 测试i64x2_eq操作
    int64_t input1[2] = {1000000, 2000000};
    int64_t input2[2] = {1000000, 2500000};
    // EQ比较结果是全1或全0的掩码
    int64_t eq_expected[2] = {static_cast<int64_t>(0xFFFFFFFFFFFFFFFF), 0x0000000000000000};
    int64_t result[2];
    
    // 参数：2个int64x2向量
    uint32_t param_types[] = {WASM_I64, WASM_I64, WASM_I64, WASM_I64};
    uint64_t params[] = {reinterpret_cast<uint64_t>(input1), 
                        reinterpret_cast<uint64_t>(input2)};
    
    // 返回值：1个int64x2向量
    uint32_t ret_types[] = {WASM_I64, WASM_I64};
    uint64_t ret[] = {reinterpret_cast<uint64_t>(result)};
    
    // 执行WASM函数并验证结果
    ASSERT_TRUE(execute_wasm_function(wasm_file_path, "i64x2_eq_test", 
                                     param_types, params, 4, 
                                     ret_types, ret, 2));
    ASSERT_TRUE(memcmp(result, eq_expected, sizeof(eq_expected)) == 0);
    
    // 测试i64x2_lt操作
    int64_t lt_expected[2] = {0x0000000000000000, static_cast<int64_t>(0xFFFFFFFFFFFFFFFF)};
    ASSERT_TRUE(execute_wasm_function(wasm_file_path, "i64x2_lt_test", 
                                     param_types, params, 4, 
                                     ret_types, ret, 2));
    ASSERT_TRUE(memcmp(result, lt_expected, sizeof(lt_expected)) == 0);
}
