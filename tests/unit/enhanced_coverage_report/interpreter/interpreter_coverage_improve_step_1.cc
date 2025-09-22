/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <gtest/gtest.h>
#include "wasm_export.h"
#include "bh_read_file.h"
#include "test_helper.h"

// Helper functions for i64 parameter handling
static inline uint64_t
get_u64_from_addr(uint32_t *addr)
{
    union {
        uint64_t val;
        uint32_t parts[2];
    } u;
    u.parts[0] = addr[0];
    u.parts[1] = addr[1];
    return u.val;
}

static inline void
put_u64_to_addr(uint32_t *addr, uint64_t value)
{
    uint32_t *addr_u32 = (uint32_t *)(addr);
    union {
        uint64_t val;
        uint32_t parts[2];
    } u;
    u.val = value;
    addr_u32[0] = u.parts[0];
    addr_u32[1] = u.parts[1];
}

/**
 * Interpreter Coverage Improvement - Step 1: Arithmetic and Bitwise Operations
 * 
 * Target Functions (10 functions, ~140 lines coverage):
 * - clz32, clz64 (Count Leading Zeros)
 * - ctz32, ctz64 (Count Trailing Zeros)  
 * - rotl32, rotr32, rotl64, rotr64 (Rotate Left/Right)
 * - popcount32, popcount64 (Population Count)
 * 
 * Expected Coverage Improvement: +1.5% (140+ lines)
 */

class ArithmeticBitwiseTest : public testing::Test
{
protected:
    void SetUp() override
    {
        runtime = std::make_unique<WAMRRuntimeRAII<512 * 1024>>();
        
        // Load the arithmetic_bitwise.wasm module using DummyExecEnv
        dummy_env = std::make_unique<DummyExecEnv>("arithmetic_bitwise.wasm");
        ASSERT_NE(dummy_env->get(), nullptr) << "Failed to create execution environment";
    }
    
    void TearDown() override
    {
        dummy_env.reset();
        runtime.reset();
    }
    
    // Helper function to call WASM functions with i32 parameter and return
    uint32_t call_wasm_i32_i32(const char* func_name, uint32_t param)
    {
        uint32_t wasm_argv[1] = { param };
        bool success = dummy_env->execute(func_name, 1, wasm_argv);
        EXPECT_TRUE(success) << "Function call failed: " << dummy_env->get_exception();
        
        return wasm_argv[0];
    }
    
    // Helper function to call WASM functions with i64 parameter and return
    uint64_t call_wasm_i64_i64(const char* func_name, uint64_t param)
    {
        uint32_t wasm_argv[2];
        put_u64_to_addr(wasm_argv, param);
        bool success = dummy_env->execute(func_name, 2, wasm_argv);
        EXPECT_TRUE(success) << "Function call failed: " << dummy_env->get_exception();
        
        return get_u64_from_addr(wasm_argv);
    }
    
    // Helper function to call WASM functions with two i32 parameters
    uint32_t call_wasm_i32_i32_i32(const char* func_name, uint32_t param1, uint32_t param2)
    {
        uint32_t wasm_argv[2] = { param1, param2 };
        bool success = dummy_env->execute(func_name, 2, wasm_argv);
        EXPECT_TRUE(success) << "Function call failed: " << dummy_env->get_exception();
        
        return wasm_argv[0];
    }
    
    // Helper function to call WASM functions with two i64 parameters
    uint64_t call_wasm_i64_i64_i64(const char* func_name, uint64_t param1, uint64_t param2)
    {
        uint32_t wasm_argv[4];
        put_u64_to_addr(wasm_argv, param1);
        put_u64_to_addr(wasm_argv + 2, param2);
        bool success = dummy_env->execute(func_name, 4, wasm_argv);
        EXPECT_TRUE(success) << "Function call failed: " << dummy_env->get_exception();
        
        return get_u64_from_addr(wasm_argv);
    }
    
    std::unique_ptr<WAMRRuntimeRAII<512 * 1024>> runtime;
    std::unique_ptr<DummyExecEnv> dummy_env;
};

// Count Leading Zeros (32-bit) Tests
TEST_F(ArithmeticBitwiseTest, Clz32_ZeroInput_Returns32)
{
    // Test clz32 with zero input - should return 32
    uint32_t result = call_wasm_i32_i32("test_clz32", 0);
    ASSERT_EQ(32, result);
    
    // Test edge case function specifically
    result = call_wasm_i32_i32("test_clz32_zero", 0);
    ASSERT_EQ(32, result);
}

TEST_F(ArithmeticBitwiseTest, Clz32_SingleBitPatterns_ReturnsCorrectCount)
{
    // Test single bit patterns
    ASSERT_EQ(31, call_wasm_i32_i32("test_clz32", 1));        // 0x00000001
    ASSERT_EQ(30, call_wasm_i32_i32("test_clz32", 2));        // 0x00000002
    ASSERT_EQ(29, call_wasm_i32_i32("test_clz32", 4));        // 0x00000004
    ASSERT_EQ(28, call_wasm_i32_i32("test_clz32", 8));        // 0x00000008
    ASSERT_EQ(0, call_wasm_i32_i32("test_clz32", 0x80000000)); // MSB set
}

TEST_F(ArithmeticBitwiseTest, Clz32_BoundaryValues_ReturnsCorrectCount)
{
    // Test boundary values
    ASSERT_EQ(0, call_wasm_i32_i32("test_clz32", 0xFFFFFFFF)); // All bits set
    ASSERT_EQ(1, call_wasm_i32_i32("test_clz32", 0x7FFFFFFF)); // All except MSB
    ASSERT_EQ(16, call_wasm_i32_i32("test_clz32", 0x0000FFFF)); // Lower 16 bits
}

// Count Leading Zeros (64-bit) Tests
TEST_F(ArithmeticBitwiseTest, Clz64_ZeroInput_Returns64)
{
    // Test clz64 with zero input - should return 64
    uint64_t result = call_wasm_i64_i64("test_clz64", 0);
    ASSERT_EQ(64, result);
    
    // Test edge case function specifically
    result = call_wasm_i64_i64("test_clz64_zero", 0);
    ASSERT_EQ(64, result);
}

TEST_F(ArithmeticBitwiseTest, Clz64_SingleBitPatterns_ReturnsCorrectCount)
{
    // Test single bit patterns
    ASSERT_EQ(63, call_wasm_i64_i64("test_clz64", 1));                    // 0x0000000000000001
    ASSERT_EQ(62, call_wasm_i64_i64("test_clz64", 2));                    // 0x0000000000000002
    ASSERT_EQ(32, call_wasm_i64_i64("test_clz64", 0x0000000080000000ULL)); // Bit 31 set
    ASSERT_EQ(0, call_wasm_i64_i64("test_clz64", 0x8000000000000000ULL));  // MSB set
}

TEST_F(ArithmeticBitwiseTest, Clz64_BoundaryValues_ReturnsCorrectCount)
{
    // Test boundary values
    ASSERT_EQ(0, call_wasm_i64_i64("test_clz64", 0xFFFFFFFFFFFFFFFFULL)); // All bits set
    ASSERT_EQ(1, call_wasm_i64_i64("test_clz64", 0x7FFFFFFFFFFFFFFFULL)); // All except MSB
    ASSERT_EQ(32, call_wasm_i64_i64("test_clz64", 0x00000000FFFFFFFFULL)); // Lower 32 bits
}

// Count Trailing Zeros (32-bit) Tests
TEST_F(ArithmeticBitwiseTest, Ctz32_ZeroInput_Returns32)
{
    // Test ctz32 with zero input - should return 32
    uint32_t result = call_wasm_i32_i32("test_ctz32", 0);
    ASSERT_EQ(32, result);
    
    // Test edge case function specifically
    result = call_wasm_i32_i32("test_ctz32_zero", 0);
    ASSERT_EQ(32, result);
}

TEST_F(ArithmeticBitwiseTest, Ctz32_SingleBitPatterns_ReturnsCorrectCount)
{
    // Test single bit patterns
    ASSERT_EQ(0, call_wasm_i32_i32("test_ctz32", 1));        // 0x00000001
    ASSERT_EQ(1, call_wasm_i32_i32("test_ctz32", 2));        // 0x00000002
    ASSERT_EQ(2, call_wasm_i32_i32("test_ctz32", 4));        // 0x00000004
    ASSERT_EQ(3, call_wasm_i32_i32("test_ctz32", 8));        // 0x00000008
    ASSERT_EQ(31, call_wasm_i32_i32("test_ctz32", 0x80000000)); // MSB set
}

TEST_F(ArithmeticBitwiseTest, Ctz32_BoundaryValues_ReturnsCorrectCount)
{
    // Test boundary values
    ASSERT_EQ(0, call_wasm_i32_i32("test_ctz32", 0xFFFFFFFF)); // All bits set
    ASSERT_EQ(1, call_wasm_i32_i32("test_ctz32", 0xFFFFFFFE)); // All except LSB
    ASSERT_EQ(16, call_wasm_i32_i32("test_ctz32", 0xFFFF0000)); // Upper 16 bits
}

// Count Trailing Zeros (64-bit) Tests
TEST_F(ArithmeticBitwiseTest, Ctz64_ZeroInput_Returns64)
{
    // Test ctz64 with zero input - should return 64
    uint64_t result = call_wasm_i64_i64("test_ctz64", 0);
    ASSERT_EQ(64, result);
    
    // Test edge case function specifically
    result = call_wasm_i64_i64("test_ctz64_zero", 0);
    ASSERT_EQ(64, result);
}

TEST_F(ArithmeticBitwiseTest, Ctz64_SingleBitPatterns_ReturnsCorrectCount)
{
    // Test single bit patterns
    ASSERT_EQ(0, call_wasm_i64_i64("test_ctz64", 1));                    // 0x0000000000000001
    ASSERT_EQ(1, call_wasm_i64_i64("test_ctz64", 2));                    // 0x0000000000000002
    ASSERT_EQ(32, call_wasm_i64_i64("test_ctz64", 0x0000000100000000ULL)); // Bit 32 set
    ASSERT_EQ(63, call_wasm_i64_i64("test_ctz64", 0x8000000000000000ULL)); // MSB set
}

TEST_F(ArithmeticBitwiseTest, Ctz64_BoundaryValues_ReturnsCorrectCount)
{
    // Test boundary values
    ASSERT_EQ(0, call_wasm_i64_i64("test_ctz64", 0xFFFFFFFFFFFFFFFFULL)); // All bits set
    ASSERT_EQ(1, call_wasm_i64_i64("test_ctz64", 0xFFFFFFFFFFFFFFFEULL)); // All except LSB
    ASSERT_EQ(32, call_wasm_i64_i64("test_ctz64", 0xFFFFFFFF00000000ULL)); // Upper 32 bits
}

// Rotate Left (32-bit) Tests
TEST_F(ArithmeticBitwiseTest, Rotl32_BasicRotation_ReturnsCorrectValue)
{
    // Test basic rotations
    ASSERT_EQ(0x2, call_wasm_i32_i32_i32("test_rotl32", 0x1, 1));      // 1 << 1
    ASSERT_EQ(0x4, call_wasm_i32_i32_i32("test_rotl32", 0x1, 2));      // 1 << 2
    ASSERT_EQ(0x80000000, call_wasm_i32_i32_i32("test_rotl32", 0x1, 31)); // 1 << 31
    
    // Test rotation with pattern - 0xABCD rotated left by 4 bits = 0xABCD0
    ASSERT_EQ(0xABCD0, call_wasm_i32_i32_i32("test_rotl32", 0xABCD, 4)); // Rotate 0xABCD left by 4
}

TEST_F(ArithmeticBitwiseTest, Rotl32_EdgeCases_ReturnsCorrectValue)
{
    // Test zero rotation
    ASSERT_EQ(0xABCD, call_wasm_i32_i32_i32("test_rotl32", 0xABCD, 0));
    
    // Test full rotation (32 bits) - should return original value
    uint32_t result = call_wasm_i32_i32("test_rotl32_full", 0xABCD1234);
    ASSERT_EQ(0xABCD1234, result);
    
    // Test rotation by 33 (equivalent to rotation by 1)
    ASSERT_EQ(0x2, call_wasm_i32_i32_i32("test_rotl32", 0x1, 33));
}

// Rotate Right (32-bit) Tests
TEST_F(ArithmeticBitwiseTest, Rotr32_BasicRotation_ReturnsCorrectValue)
{
    // Test basic rotations
    ASSERT_EQ(0x80000000, call_wasm_i32_i32_i32("test_rotr32", 0x1, 1));   // 1 >> 1 (with wrap)
    ASSERT_EQ(0x40000000, call_wasm_i32_i32_i32("test_rotr32", 0x1, 2));   // 1 >> 2 (with wrap)
    ASSERT_EQ(0x2, call_wasm_i32_i32_i32("test_rotr32", 0x1, 31));         // 1 >> 31 (with wrap)
    
    // Test rotation with pattern - 0xABCD right by 4 = 0xDABC000C (high bits wrap to low)
    ASSERT_EQ(0xDABC000C, call_wasm_i32_i32_i32("test_rotr32", 0xABCD, 4)); // Rotate 0xABCD right by 4
}

TEST_F(ArithmeticBitwiseTest, Rotr32_EdgeCases_ReturnsCorrectValue)
{
    // Test zero rotation
    ASSERT_EQ(0xABCD, call_wasm_i32_i32_i32("test_rotr32", 0xABCD, 0));
    
    // Test full rotation (32 bits) - should return original value
    uint32_t result = call_wasm_i32_i32("test_rotr32_full", 0xABCD1234);
    ASSERT_EQ(0xABCD1234, result);
    
    // Test rotation by 33 (equivalent to rotation by 1)
    ASSERT_EQ(0x80000000, call_wasm_i32_i32_i32("test_rotr32", 0x1, 33));
}

// Rotate Left (64-bit) Tests
TEST_F(ArithmeticBitwiseTest, Rotl64_BasicRotation_ReturnsCorrectValue)
{
    // Test basic rotations
    ASSERT_EQ(0x2ULL, call_wasm_i64_i64_i64("test_rotl64", 0x1ULL, 1));      // 1 << 1
    ASSERT_EQ(0x4ULL, call_wasm_i64_i64_i64("test_rotl64", 0x1ULL, 2));      // 1 << 2
    ASSERT_EQ(0x8000000000000000ULL, call_wasm_i64_i64_i64("test_rotl64", 0x1ULL, 63)); // 1 << 63
    
    // Test rotation with pattern - 0xABCD left by 4 = 0xABCD0
    ASSERT_EQ(0xABCD0ULL, call_wasm_i64_i64_i64("test_rotl64", 0xABCDULL, 4)); // Rotate 0xABCD left by 4
}

TEST_F(ArithmeticBitwiseTest, Rotl64_EdgeCases_ReturnsCorrectValue)
{
    // Test zero rotation
    ASSERT_EQ(0xABCD1234ULL, call_wasm_i64_i64_i64("test_rotl64", 0xABCD1234ULL, 0));
    
    // Test full rotation (64 bits) - should return original value
    uint64_t result = call_wasm_i64_i64("test_rotl64_full", 0xABCD123456789ABCULL);
    ASSERT_EQ(0xABCD123456789ABCULL, result);
    
    // Test rotation by 65 (equivalent to rotation by 1)
    ASSERT_EQ(0x2ULL, call_wasm_i64_i64_i64("test_rotl64", 0x1ULL, 65));
}

// Rotate Right (64-bit) Tests
TEST_F(ArithmeticBitwiseTest, Rotr64_BasicRotation_ReturnsCorrectValue)
{
    // Test basic rotations
    ASSERT_EQ(0x8000000000000000ULL, call_wasm_i64_i64_i64("test_rotr64", 0x1ULL, 1));   // 1 >> 1 (with wrap)
    ASSERT_EQ(0x4000000000000000ULL, call_wasm_i64_i64_i64("test_rotr64", 0x1ULL, 2));   // 1 >> 2 (with wrap)
    ASSERT_EQ(0x2ULL, call_wasm_i64_i64_i64("test_rotr64", 0x1ULL, 63));                 // 1 >> 63 (with wrap)
    
    // Test rotation with pattern
    ASSERT_EQ(0xDABC000000000000ULL, call_wasm_i64_i64_i64("test_rotr64", 0xABCDULL, 4)); // Rotate 0xABCD right by 4
}

TEST_F(ArithmeticBitwiseTest, Rotr64_EdgeCases_ReturnsCorrectValue)
{
    // Test zero rotation
    ASSERT_EQ(0xABCD1234ULL, call_wasm_i64_i64_i64("test_rotr64", 0xABCD1234ULL, 0));
    
    // Test full rotation (64 bits) - should return original value
    uint64_t result = call_wasm_i64_i64("test_rotr64_full", 0xABCD123456789ABCULL);
    ASSERT_EQ(0xABCD123456789ABCULL, result);
    
    // Test rotation by 65 (equivalent to rotation by 1)
    ASSERT_EQ(0x8000000000000000ULL, call_wasm_i64_i64_i64("test_rotr64", 0x1ULL, 65));
}

// Population Count (32-bit) Tests
TEST_F(ArithmeticBitwiseTest, Popcount32_VariousPatterns_ReturnsCorrectCount)
{
    // Test zero
    ASSERT_EQ(0, call_wasm_i32_i32("test_popcount32", 0));
    
    // Test single bits
    ASSERT_EQ(1, call_wasm_i32_i32("test_popcount32", 1));
    ASSERT_EQ(1, call_wasm_i32_i32("test_popcount32", 2));
    ASSERT_EQ(1, call_wasm_i32_i32("test_popcount32", 4));
    ASSERT_EQ(1, call_wasm_i32_i32("test_popcount32", 0x80000000));
    
    // Test multiple bits
    ASSERT_EQ(2, call_wasm_i32_i32("test_popcount32", 3));      // 0b11
    ASSERT_EQ(4, call_wasm_i32_i32("test_popcount32", 15));     // 0b1111
    ASSERT_EQ(8, call_wasm_i32_i32("test_popcount32", 0xFF));   // 0b11111111
    
    // Test all bits set
    uint32_t result = call_wasm_i32_i32("test_popcount32_all_bits", 0);
    ASSERT_EQ(32, result);
    ASSERT_EQ(32, call_wasm_i32_i32("test_popcount32", 0xFFFFFFFF));
}

// Population Count (64-bit) Tests
TEST_F(ArithmeticBitwiseTest, Popcount64_VariousPatterns_ReturnsCorrectCount)
{
    // Test zero
    ASSERT_EQ(0, call_wasm_i64_i64("test_popcount64", 0));
    
    // Test single bits
    ASSERT_EQ(1, call_wasm_i64_i64("test_popcount64", 1));
    ASSERT_EQ(1, call_wasm_i64_i64("test_popcount64", 2));
    ASSERT_EQ(1, call_wasm_i64_i64("test_popcount64", 4));
    ASSERT_EQ(1, call_wasm_i64_i64("test_popcount64", 0x8000000000000000ULL));
    
    // Test multiple bits
    ASSERT_EQ(2, call_wasm_i64_i64("test_popcount64", 3));           // 0b11
    ASSERT_EQ(4, call_wasm_i64_i64("test_popcount64", 15));          // 0b1111
    ASSERT_EQ(8, call_wasm_i64_i64("test_popcount64", 0xFF));        // 0b11111111
    ASSERT_EQ(16, call_wasm_i64_i64("test_popcount64", 0xFFFF));     // 16 bits set
    ASSERT_EQ(32, call_wasm_i64_i64("test_popcount64", 0xFFFFFFFFULL)); // 32 bits set
    
    // Test all bits set
    uint64_t result = call_wasm_i64_i64("test_popcount64_all_bits", 0);
    ASSERT_EQ(64, result);
    ASSERT_EQ(64, call_wasm_i64_i64("test_popcount64", 0xFFFFFFFFFFFFFFFFULL));
}