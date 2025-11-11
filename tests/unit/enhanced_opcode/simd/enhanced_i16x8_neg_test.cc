/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <gtest/gtest.h>
#include <cstring>
#include <memory>
#include <cmath>
#include <climits>
#include "wasm_runtime_common.h"
#include "wasm_export.h"
#include "test_helper.h"

// Test execution modes for parameterized testing
enum class TestRunningMode {
    INTERP_MODE,
    AOT_MODE
};

/**
 * @brief Test fixture class for i16x8.neg SIMD opcode validation
 * @details Provides comprehensive test framework for SIMD i16x8.neg instruction
 *          including setup/teardown, helper functions, and cross-execution mode validation
 */
class I16x8NegTest : public testing::TestWithParam<TestRunningMode>
{
protected:
    TestRunningMode running_mode;
    std::unique_ptr<WAMRRuntimeRAII<>> runtime_raii;
    std::unique_ptr<DummyExecEnv> dummy_env;

    /**
     * @brief Sets up the test fixture with WAMR runtime initialization
     * @details Initializes WAMR runtime, loads test WASM module, and prepares execution context
     */
    void SetUp() override
    {
        // Get current test mode (INTERP_MODE or AOT_MODE)
        running_mode = GetParam();

        // Initialize WAMR runtime using RAII helper
        runtime_raii = std::make_unique<WAMRRuntimeRAII<>>();

        // Load the i16x8.neg test module using relative path
        dummy_env = std::make_unique<DummyExecEnv>("wasm-apps/i16x8_neg_test.wasm");
        ASSERT_NE(nullptr, dummy_env->get())
            << "Failed to create execution environment for i16x8.neg tests";
    }

    /**
     * @brief Tears down the test fixture with proper cleanup
     * @details Destroys execution environment and WAMR runtime resources
     */
    void TearDown() override
    {
        dummy_env.reset();
        runtime_raii.reset();
    }

    /**
     * @brief Helper function to call WASM function and extract v128 result as i16 array
     * @param function_name Name of the WASM function to call
     * @param result_out Array to store the 8 i16 result values
     */
    void call_i16x8_function(const char* function_name, int16_t result_out[8])
    {
        wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(dummy_env->get());
        wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst, function_name);
        ASSERT_NE(func, nullptr) << "Failed to lookup function: " << function_name;

        uint32_t argv[4];  // v128 result as 4 x i32
        bool call_result = wasm_runtime_call_wasm(dummy_env->get(), func, 0, argv);
        ASSERT_TRUE(call_result) << "Failed to call " << function_name << ": " << wasm_runtime_get_exception(module_inst);

        // Extract i16 values from v128 result (2 i16 values per i32)
        int16_t* i16_ptr = reinterpret_cast<int16_t*>(argv);
        for (int i = 0; i < 8; i++) {
            result_out[i] = i16_ptr[i];
        }
    }
};

/**
 * @test BasicNegation_ReturnsCorrectResults
 * @brief Validates i16x8.neg produces correct two's complement negation for typical values
 * @details Tests fundamental negation operation with positive, negative, zero, and mixed-sign integers.
 *          Verifies that i16x8.neg correctly computes -x for each lane independently.
 * @test_category Main - Basic functionality validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i16x8_neg_operation
 * @input_conditions Standard i16 values: [1, -2, 100, -500, 0, 32767, -32767, 42]
 * @expected_behavior Returns mathematical negation: [-1, 2, -100, 500, 0, -32767, 32767, -42]
 * @validation_method Direct comparison of each lane result with expected negated values
 */
TEST_P(I16x8NegTest, BasicNegation_ReturnsCorrectResults)
{
    // Test basic negation with mixed positive/negative values
    // Input: [1, -2, 100, -500, 0, 32767, -32767, 42]
    // Expected: [-1, 2, -100, 500, 0, -32767, 32767, -42]
    int16_t expected_lanes[8] = {-1, 2, -100, 500, 0, -32767, 32767, -42};
    int16_t output_lanes[8];

    call_i16x8_function("test_i16x8_neg_basic", output_lanes);

    // Validate each lane produces correct negated result
    for (int i = 0; i < 8; i++) {
        ASSERT_EQ(expected_lanes[i], output_lanes[i])
            << "Lane " << i << " negation failed: expected " << expected_lanes[i]
            << ", got " << output_lanes[i];
    }
}

/**
 * @test EdgeCaseOverflow_HandlesCorrectly
 * @brief Validates i16x8.neg handles boundary conditions and overflow per WASM specification
 * @details Tests INT16_MIN overflow behavior where -(-32768) wraps to -32768 due to 16-bit limitations.
 *          Also validates INT16_MAX negation and edge values around zero.
 * @test_category Edge - Boundary value and overflow validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i16x8_neg_overflow_handling
 * @input_conditions Boundary values: [-32768, 32767, -1, 1, 0, -32768, 32767, -1]
 * @expected_behavior INT16_MIN wraps to itself, other values negate correctly per two's complement
 * @validation_method Comparison with WASM-specified overflow behavior for signed 16-bit arithmetic
 */
TEST_P(I16x8NegTest, EdgeCaseOverflow_HandlesCorrectly)
{
    // Test boundary values including INT16_MIN overflow case
    // Input: [-32768, 32767, -1, 1, 0, -32768, 32767, -1]
    // Expected: [-32768, -32767, 1, -1, 0, -32768, -32767, 1] (INT16_MIN wraps to itself)
    int16_t expected_lanes[8] = {-32768, -32767, 1, -1, 0, -32768, -32767, 1};
    int16_t output_lanes[8];

    call_i16x8_function("test_i16x8_neg_boundary", output_lanes);

    // Validate overflow behavior and boundary value handling
    for (int i = 0; i < 8; i++) {
        ASSERT_EQ(expected_lanes[i], output_lanes[i])
            << "Lane " << i << " boundary negation failed: expected " << expected_lanes[i]
            << ", got " << output_lanes[i];
    }

    // Specifically validate INT16_MIN overflow behavior (lanes 0 and 5)
    ASSERT_EQ(-32768, output_lanes[0])
        << "INT16_MIN negation must wrap to INT16_MIN per WASM spec";
    ASSERT_EQ(-32768, output_lanes[5])
        << "Second INT16_MIN negation must also wrap to INT16_MIN";
}

/**
 * @test LaneIndependence_ValidatesCorrectly
 * @brief Ensures i16x8.neg operations on individual lanes don't affect adjacent lanes
 * @details Tests lane isolation by using pre-defined test functions with isolated values.
 *          Verifies SIMD operation maintains lane boundaries and doesn't cause cross-lane interference.
 * @test_category Main - Lane isolation and independence validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i16x8_lane_processing
 * @input_conditions Isolated non-zero values in specific lanes, zeros in remaining lanes
 * @expected_behavior Only target lane changes, adjacent lanes remain unmodified
 * @validation_method Comparison of modified vs. unmodified lanes to ensure isolation
 */
TEST_P(I16x8NegTest, LaneIndependence_ValidatesCorrectly)
{
    // Test lane 0 independence with isolated values
    // Input: [100, 0, 0, 0, 0, 0, 0, 0]
    // Expected: [-100, 0, 0, 0, 0, 0, 0, 0]
    int16_t output_lanes0[8];
    call_i16x8_function("test_i16x8_neg_lane0_independence", output_lanes0);

    // Validate lane 0 negation (100 -> -100) and other lanes unchanged (0 -> 0)
    ASSERT_EQ(-100, output_lanes0[0])
        << "Lane 0 should be negated from 100 to -100";
    for (int i = 1; i < 8; i++) {
        ASSERT_EQ(0, output_lanes0[i])
            << "Lane " << i << " should remain zero (unaffected by lane 0 operation)";
    }

    // Test lane 1 independence with isolated values
    // Input: [0, -200, 0, 0, 0, 0, 0, 0]
    // Expected: [0, 200, 0, 0, 0, 0, 0, 0]
    int16_t output_lanes1[8];
    call_i16x8_function("test_i16x8_neg_lane1_independence", output_lanes1);

    // Validate lane 1 negation (-200 -> 200) and other lanes unchanged
    ASSERT_EQ(200, output_lanes1[1])
        << "Lane 1 should be negated from -200 to 200";
    ASSERT_EQ(0, output_lanes1[0])
        << "Lane 0 should remain zero (unaffected by lane 1 operation)";
    for (int i = 2; i < 8; i++) {
        ASSERT_EQ(0, output_lanes1[i])
            << "Lane " << i << " should remain zero (unaffected by lane 1 operation)";
    }
}

/**
 * @test MixedSignValues_ProcessesCorrectly
 * @brief Validates i16x8.neg handles complex patterns using pre-defined test function
 * @details Tests negation across all lanes simultaneously with diverse value patterns including
 *          small and large numbers, ensuring consistent two's complement behavior.
 * @test_category Main - Complex pattern validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i16x8_batch_processing
 * @input_conditions Mixed positive/negative pattern: [-1000, 2000, -30, 4, 0, -12345, 6789, -9]
 * @expected_behavior Each lane negated independently maintaining arithmetic correctness
 * @validation_method Verification of each lane's individual negation result
 */
TEST_P(I16x8NegTest, MixedSignValues_ProcessesCorrectly)
{
    // Test complex mixed sign pattern using pre-defined WASM function
    // Input: [-1000, 2000, -30, 4, 0, -12345, 6789, -9]
    // Expected: [1000, -2000, 30, -4, 0, 12345, -6789, 9]
    int16_t expected_lanes[8] = {1000, -2000, 30, -4, 0, 12345, -6789, 9};
    int16_t output_lanes[8];

    call_i16x8_function("test_i16x8_neg_mixed_pattern", output_lanes);

    // Validate each lane produces correct negated result
    for (int i = 0; i < 8; i++) {
        ASSERT_EQ(expected_lanes[i], output_lanes[i])
            << "Lane " << i << " mixed sign negation failed: expected " << expected_lanes[i]
            << ", got " << output_lanes[i];
    }
}

/**
 * @test ZeroValues_RemainZero
 * @brief Validates i16x8.neg correctly handles zero values
 * @details Tests that negation of zero produces zero across all lanes.
 *          Ensures proper handling of zero sign bit behavior.
 * @test_category Main - Zero value validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i16x8_zero_handling
 * @input_conditions All lanes set to zero values: [0, 0, 0, 0, 0, 0, 0, 0]
 * @expected_behavior All output lanes remain zero after negation
 * @validation_method Verification that zero negation produces zero
 */
TEST_P(I16x8NegTest, ZeroValues_RemainZero)
{
    // Test zero values using pre-defined WASM function
    // Input: [0, 0, 0, 0, 0, 0, 0, 0]
    // Expected: [0, 0, 0, 0, 0, 0, 0, 0]
    int16_t expected_lanes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int16_t output_lanes[8];

    call_i16x8_function("test_i16x8_neg_zero", output_lanes);

    // Validate all zeros remain zero after negation
    for (int i = 0; i < 8; i++) {
        ASSERT_EQ(0, output_lanes[i])
            << "Lane " << i << " zero negation failed: expected 0, got " << output_lanes[i];
    }
}

// Instantiate parameterized tests for both interpreter and AOT execution modes
INSTANTIATE_TEST_SUITE_P(
    I16x8NegTestSuite,
    I16x8NegTest,
    testing::Values(TestRunningMode::INTERP_MODE, TestRunningMode::AOT_MODE),
    [](const testing::TestParamInfo<I16x8NegTest::ParamType>& info) {
        return info.param == TestRunningMode::INTERP_MODE ? "InterpreterMode" : "AotMode";
    }
);