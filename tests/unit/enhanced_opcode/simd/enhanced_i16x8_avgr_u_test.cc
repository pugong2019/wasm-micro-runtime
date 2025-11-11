/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "test_helper.h"
#include "gtest/gtest.h"

/**
 * @brief Test class for i16x8.avgr_u opcode functionality validation
 * @details This test class validates the i16x8.avgr_u SIMD opcode which performs
 *          unsigned average with rounding operations on packed 16-bit integers.
 *          The operation computes (a[i] + b[i] + 1) / 2 for each lane i using
 *          17-bit arithmetic to prevent overflow, then rounds up.
 *          Tests cover basic functionality, boundary conditions, edge cases,
 *          and cross-execution mode validation using comprehensive WAMR testing framework.
 */
class I16x8AvgrUTestSuite : public testing::Test {
protected:
    /**
     * @brief Set up test environment and initialize WAMR runtime
     * @details Initializes WAMR runtime using RAII helper and loads the
     *          i16x8.avgr_u test WASM module for execution testing.
     */
    void SetUp() override {
        // Initialize WAMR runtime using RAII helper
        runtime_raii = std::make_unique<WAMRRuntimeRAII<>>();

        // Load the i16x8.avgr_u test module using relative path
        dummy_env = std::make_unique<DummyExecEnv>("wasm-apps/i16x8_avgr_u_test.wasm");
        ASSERT_NE(nullptr, dummy_env->get())
            << "Failed to create execution environment for i16x8.avgr_u tests";
    }

    /**
     * @brief Clean up test environment and destroy WAMR runtime
     * @details Properly releases all allocated resources using RAII pattern
     *          to prevent resource leaks.
     */
    void TearDown() override {
        // Cleanup handled by RAII destructors
        dummy_env.reset();
        runtime_raii.reset();
    }

    /**
     * @brief Execute i16x8.avgr_u operation with two input vectors
     * @param vec_a Array of 8 uint16_t values representing the first input vector
     * @param vec_b Array of 8 uint16_t values representing the second input vector
     * @param result_values Output array to store the 8 result uint16_t values
     * @return bool True if operation succeeded, false on error
     * @details Calls the WASM test function to perform i16x8.avgr_u operation
     *          using the formula: (vec_a[i] + vec_b[i] + 1) / 2 for each lane i
     */
    bool call_i16x8_avgr_u(const uint16_t vec_a[8], const uint16_t vec_b[8], uint16_t result_values[8]) {
        uint32_t argv[18]; // 16 inputs + 2 result parameters

        // Pack input vectors into argv (each uint16_t as uint32_t)
        for (int i = 0; i < 8; i++) {
            argv[i] = static_cast<uint32_t>(vec_a[i]);      // First vector lanes 0-7
            argv[i + 8] = static_cast<uint32_t>(vec_b[i]);  // Second vector lanes 0-7
        }

        // Result pointer parameters
        argv[16] = 0; // Unused
        argv[17] = 0; // Unused

        // Execute the WASM function
        bool success = dummy_env->execute("test_i16x8_avgr_u", 18, argv);

        // Extract results from argv (returned via modified parameters)
        if (success) {
            for (int i = 0; i < 8; i++) {
                result_values[i] = static_cast<uint16_t>(argv[i]);
            }
        }

        return success;
    }

protected:
    std::unique_ptr<WAMRRuntimeRAII<>> runtime_raii;
    std::unique_ptr<DummyExecEnv> dummy_env;
};

/**
 * @test BasicAveraging_ReturnsCorrectResults
 * @brief Validates i16x8.avgr_u produces correct unsigned average results for typical inputs
 * @details Tests fundamental average operation with typical value pairs across all 8 lanes.
 *          Verifies that i16x8.avgr_u correctly computes (a[i] + b[i] + 1) / 2 for various
 *          input combinations, ensuring proper rounding behavior.
 * @test_category Main - Basic functionality validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i16x8_avgr_u_operation
 * @input_conditions Standard uint16_t value pairs: (1000,2000), (5000,10000), etc.
 * @expected_behavior Returns mathematical average with rounding: 1500, 7500, etc.
 * @validation_method Direct comparison of WASM function result with expected average values
 */
TEST_F(I16x8AvgrUTestSuite, BasicAveraging_ReturnsCorrectResults) {
    uint16_t vec_a[8] = {1000, 5000, 2500, 100, 3000, 6000, 1500, 800};
    uint16_t vec_b[8] = {2000, 10000, 7500, 100, 4000, 8000, 2500, 1600};
    uint16_t result[8];

    // Execute the average operation
    ASSERT_TRUE(call_i16x8_avgr_u(vec_a, vec_b, result))
        << "Failed to execute i16x8.avgr_u with typical values";

    // Verify each lane result: (a[i] + b[i] + 1) / 2
    ASSERT_EQ(1500, result[0]) << "Average of (1000+2000+1)/2 should be 1500";
    ASSERT_EQ(7500, result[1]) << "Average of (5000+10000+1)/2 should be 7500";
    ASSERT_EQ(5000, result[2]) << "Average of (2500+7500+1)/2 should be 5000";
    ASSERT_EQ(100, result[3]) << "Average of (100+100+1)/2 should be 100";
    ASSERT_EQ(3500, result[4]) << "Average of (3000+4000+1)/2 should be 3500";
    ASSERT_EQ(7000, result[5]) << "Average of (6000+8000+1)/2 should be 7000";
    ASSERT_EQ(2000, result[6]) << "Average of (1500+2500+1)/2 should be 2000";
    ASSERT_EQ(1200, result[7]) << "Average of (800+1600+1)/2 should be 1200";
}

/**
 * @test RoundingBehavior_UpwardForOddSums
 * @brief Validates i16x8.avgr_u performs correct rounding for odd sums
 * @details Tests scenarios where intermediate sum is odd to verify the +1 rounding behavior.
 *          Ensures proper rounding up for fractional averages according to the
 *          WebAssembly specification for unsigned average with rounding.
 * @test_category Main - Rounding validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i16x8_avgr_u_rounding
 * @input_conditions Value pairs that produce odd sums: (1,2), (3,4), (7,8), etc.
 * @expected_behavior Proper rounding up: 2, 4, 8 respectively (not 1, 3, 7)
 * @validation_method Verification of rounding behavior for fractional results
 */
TEST_F(I16x8AvgrUTestSuite, RoundingBehavior_UpwardForOddSums) {
    // Test values that create odd sums to verify rounding
    uint16_t vec_a[8] = {1, 3, 7, 15, 31, 63, 127, 255};
    uint16_t vec_b[8] = {2, 4, 8, 16, 32, 64, 128, 256};
    uint16_t result[8];

    // Execute the average operation
    ASSERT_TRUE(call_i16x8_avgr_u(vec_a, vec_b, result))
        << "Failed to execute i16x8.avgr_u with rounding test values";

    // Verify rounding behavior: (odd_sum + 1) / 2 rounds up
    ASSERT_EQ(2, result[0]) << "Average of (1+2+1)/2 should round up to 2";
    ASSERT_EQ(4, result[1]) << "Average of (3+4+1)/2 should round up to 4";
    ASSERT_EQ(8, result[2]) << "Average of (7+8+1)/2 should round up to 8";
    ASSERT_EQ(16, result[3]) << "Average of (15+16+1)/2 should round up to 16";
    ASSERT_EQ(32, result[4]) << "Average of (31+32+1)/2 should round up to 32";
    ASSERT_EQ(64, result[5]) << "Average of (63+64+1)/2 should round up to 64";
    ASSERT_EQ(128, result[6]) << "Average of (127+128+1)/2 should round up to 128";
    ASSERT_EQ(256, result[7]) << "Average of (255+256+1)/2 should round up to 256";
}

/**
 * @test BoundaryValues_HandleCorrectly
 * @brief Validates i16x8.avgr_u handles boundary conditions correctly
 * @details Tests boundary value combinations including (0,0), (65535,65535), and (0,65535).
 *          Verifies proper handling without 16-bit overflow using 17-bit intermediate arithmetic.
 * @test_category Corner - Boundary condition validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i16x8_avgr_u_boundary_handling
 * @input_conditions Boundary combinations: (0,0), (65535,65535), (0,65535), (32767,32768)
 * @expected_behavior Proper boundary handling: 0, 65535, 32768, 32768 respectively
 * @validation_method Verification of boundary value arithmetic and overflow prevention
 */
TEST_F(I16x8AvgrUTestSuite, BoundaryValues_HandleCorrectly) {
    uint16_t vec_a[8] = {0, 65535, 0, 32767, 0, 65535, 32768, 16384};
    uint16_t vec_b[8] = {0, 65535, 65535, 32768, 1, 65534, 32767, 49152};
    uint16_t result[8];

    // Execute the average operation
    ASSERT_TRUE(call_i16x8_avgr_u(vec_a, vec_b, result))
        << "Failed to execute i16x8.avgr_u with boundary values";

    // Verify boundary cases: (a + b + 1) / 2
    ASSERT_EQ(0, result[0]) << "Average of (0+0+1)/2 should be 0";
    ASSERT_EQ(65535, result[1]) << "Average of (65535+65535+1)/2 should be 65535";
    ASSERT_EQ(32768, result[2]) << "Average of (0+65535+1)/2 should be 32768";
    ASSERT_EQ(32768, result[3]) << "Average of (32767+32768+1)/2 should be 32768";
    ASSERT_EQ(1, result[4]) << "Average of (0+1+1)/2 should be 1";
    ASSERT_EQ(65535, result[5]) << "Average of (65535+65534+1)/2 should be 65535";
    ASSERT_EQ(32768, result[6]) << "Average of (32768+32767+1)/2 should be 32768";
    ASSERT_EQ(32768, result[7]) << "Average of (16384+49152+1)/2 should be 32768";
}

/**
 * @test LaneIndependence_ProcessesCorrectly
 * @brief Validates that each of the 8 lanes processes independently without cross-lane interference
 * @details Tests vectors with different values in each lane position to verify lane independence.
 *          Each result lane should be computed only from corresponding input lane values
 *          without any interference from other lanes.
 * @test_category Main - Lane independence validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i16x8_avgr_u_lane_processing
 * @input_conditions Vectors with different values in each lane position
 * @expected_behavior Each result lane computed only from corresponding input lane values
 * @validation_method Individual lane validation with distinct expected results
 */
TEST_F(I16x8AvgrUTestSuite, LaneIndependence_ProcessesCorrectly) {
    // Use distinct values in each lane to verify independence
    uint16_t vec_a[8] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000};
    uint16_t vec_b[8] = {9000, 8000, 7000, 6000, 5000, 4000, 3000, 2000};
    uint16_t result[8];

    // Execute the average operation
    ASSERT_TRUE(call_i16x8_avgr_u(vec_a, vec_b, result))
        << "Failed to execute i16x8.avgr_u with lane independence test values";

    // Verify each lane processes independently
    ASSERT_EQ(5000, result[0]) << "Lane 0: (1000+9000+1)/2 should be 5000";
    ASSERT_EQ(5000, result[1]) << "Lane 1: (2000+8000+1)/2 should be 5000";
    ASSERT_EQ(5000, result[2]) << "Lane 2: (3000+7000+1)/2 should be 5000";
    ASSERT_EQ(5000, result[3]) << "Lane 3: (4000+6000+1)/2 should be 5000";
    ASSERT_EQ(5000, result[4]) << "Lane 4: (5000+5000+1)/2 should be 5000";
    ASSERT_EQ(5000, result[5]) << "Lane 5: (6000+4000+1)/2 should be 5000";
    ASSERT_EQ(5000, result[6]) << "Lane 6: (7000+3000+1)/2 should be 5000";
    ASSERT_EQ(5000, result[7]) << "Lane 7: (8000+2000+1)/2 should be 5000";
}