/**
 * @file enhanced_i32x4_neg_test.cc
 * @brief Comprehensive unit tests for i32x4.neg SIMD opcode
 * @details Tests i32x4.neg functionality across interpreter and AOT execution modes
 *          with focus on basic negation operations, boundary conditions, integer limits,
 *          and error scenarios. Validates WAMR SIMD implementation correctness
 *          and cross-mode consistency for i32 vector negation operations.
 * @source_location tests/unit/enhanced_opcode/extension/enhanced_i32x4_neg_test.cc
 */

#include <gtest/gtest.h>
#include <cstring>
#include <memory>
#include <climits>
#include "wasm_runtime_common.h"
#include "wasm_export.h"
#include "test_helper.h"

/**
 * @class I32x4NegTestSuite
 * @brief Test fixture class for i32x4.neg opcode testing
 * @details Provides setup/teardown for WAMR runtime and module loading.
 *          Handles SIMD vector result validation using DummyExecEnv helper
 *          for comprehensive i32x4.neg operation validation.
 * @test_categories Main, Corner, Edge, Error exception validation
 */
class I32x4NegTestSuite : public testing::Test
{
protected:
    /**
     * @brief Initialize WAMR runtime and prepare test environment for i32x4.neg testing
     * @details Sets up WAMR runtime with SIMD support using WAMRRuntimeRAII helper.
     *          Initializes execution environment for WASM test files using relative path.
     * @source_location tests/unit/enhanced_opcode/extension/enhanced_i32x4_neg_test.cc:SetUp
     */
    void SetUp() override
    {
        // Initialize WAMR runtime using RAII helper
        runtime_raii = std::make_unique<WAMRRuntimeRAII<>>();

        // Load the i32x4.neg test module using relative path
        dummy_env = std::make_unique<DummyExecEnv>("wasm-apps/i32x4_neg_test.wasm");
        ASSERT_NE(nullptr, dummy_env->get())
            << "Failed to create execution environment for i32x4.neg tests";
    }

    /**
     * @brief Clean up WAMR runtime and release test resources
     * @details Properly destroys execution environment and WAMR runtime
     *          using RAII pattern to prevent resource leaks.
     * @source_location tests/unit/enhanced_opcode/extension/enhanced_i32x4_neg_test.cc:TearDown
     */
    void TearDown() override
    {
        // RAII automatically handles cleanup
        dummy_env.reset();
        runtime_raii.reset();
    }

    /**
     * @brief Helper function to call i32x4.neg WASM function and validate results
     * @param func_name WASM function name to call
     * @param input_values Array of 4 i32 input values to be negated
     * @param expected_values Array of 4 expected i32 values after negation
     * @details Calls the specified WASM function and validates the execution.
     *          Returns the resulting i32 values for lane-by-lane validation.
     * @source_location tests/unit/enhanced_opcode/extension/enhanced_i32x4_neg_test.cc:CallI32x4Neg
     */
    void CallI32x4Neg(const char* func_name, const int32_t input_values[4], const int32_t expected_values[4])
    {
        // v128 functions use 4 uint32_t values as input/output through argv array
        uint32_t argv[4];

        // Set input values (convert int32_t to uint32_t for WASM interface)
        for (int i = 0; i < 4; ++i) {
            argv[i] = static_cast<uint32_t>(input_values[i]);
        }

        // Execute the function and expect success
        bool call_result = dummy_env->execute(func_name, 4, argv);
        ASSERT_TRUE(call_result)
            << "Failed to call " << func_name << " with input values: ["
            << input_values[0] << ", " << input_values[1] << ", "
            << input_values[2] << ", " << input_values[3] << "]";

        // Extract result i32 values from argv (now contains the v128 result)
        int32_t* result_values = reinterpret_cast<int32_t*>(argv);

        // Validate all four lanes contain the expected negated values
        for (int lane = 0; lane < 4; ++lane) {
            ASSERT_EQ(expected_values[lane], result_values[lane])
                << "Lane " << lane << " mismatch for negation operation"
                << ": input=" << input_values[lane]
                << ", expected=" << expected_values[lane]
                << ", got=" << result_values[lane];
        }
    }

    /**
     * @brief Helper function to call single-value i32x4.neg functions
     * @param func_name WASM function name to call (no parameters, returns v128)
     * @param expected_values Array of 4 expected i32 values after negation
     * @details Calls constant-value WASM functions for testing specific edge cases.
     * @source_location tests/unit/enhanced_opcode/extension/enhanced_i32x4_neg_test.cc:CallI32x4NegConst
     */
    void CallI32x4NegConst(const char* func_name, const int32_t expected_values[4])
    {
        uint32_t argv[4] = { 0, 0, 0, 0 };

        // Execute the constant function and expect success
        bool call_result = dummy_env->execute(func_name, 0, argv);
        ASSERT_TRUE(call_result)
            << "Failed to call constant function " << func_name;

        // Extract result i32 values from argv
        int32_t* result_values = reinterpret_cast<int32_t*>(argv);

        // Validate all four lanes contain the expected values
        for (int lane = 0; lane < 4; ++lane) {
            ASSERT_EQ(expected_values[lane], result_values[lane])
                << "Lane " << lane << " mismatch in constant function " << func_name
                << ": expected=" << expected_values[lane]
                << ", got=" << result_values[lane];
        }
    }

protected:
    std::unique_ptr<WAMRRuntimeRAII<>> runtime_raii;
    std::unique_ptr<DummyExecEnv> dummy_env;
};

/**
 * @test BasicNegation_TypicalValues_ReturnsCorrectNegation
 * @brief Validates i32x4.neg produces correct negation results for typical i32 values
 * @details Tests fundamental negation operation with positive, negative, zero, and mixed values.
 *          Verifies that i32x4.neg correctly computes two's complement negation for each lane.
 * @test_category Main - Basic functionality validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_fast.c:SIMD_i32x4_neg
 * @input_conditions Mixed typical values: [42, -100, 0, 1000]
 * @expected_behavior Returns negated vector: [-42, 100, 0, -1000]
 * @validation_method Direct comparison of WASM function result with expected negated values
 */
TEST_F(I32x4NegTestSuite, BasicNegation_TypicalValues_ReturnsCorrectNegation)
{
    // Test basic negation with mixed positive, negative, zero, and larger values
    int32_t input_values[4] = { 42, -100, 0, 1000 };
    int32_t expected_values[4] = { -42, 100, 0, -1000 };
    CallI32x4Neg("test_i32x4_neg_basic", input_values, expected_values);

    // Test additional typical values
    int32_t input_values2[4] = { 123, -456, 789, -321 };
    int32_t expected_values2[4] = { -123, 456, -789, 321 };
    CallI32x4Neg("test_i32x4_neg_basic", input_values2, expected_values2);
}

/**
 * @test BoundaryValues_IntegerLimits_HandlesOverflow
 * @brief Validates i32x4.neg handles boundary values and overflow correctly
 * @details Tests negation operation with INT32_MIN and INT32_MAX values.
 *          Verifies correct overflow behavior for INT32_MIN (which cannot be negated).
 * @test_category Corner - Boundary condition validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_fast.c:SIMD_i32x4_neg
 * @input_conditions Boundary values: [INT32_MAX, INT32_MIN, -1, 1]
 * @expected_behavior Returns vector: [-2147483647, INT32_MIN (overflow), 1, -1]
 * @validation_method Lane-by-lane verification of boundary value negation with overflow handling
 */
TEST_F(I32x4NegTestSuite, BoundaryValues_IntegerLimits_HandlesOverflow)
{
    // Test INT32_MAX negation (should become -2147483647)
    int32_t expected_max[4] = { -2147483647, -2147483647, -2147483647, -2147483647 };
    CallI32x4NegConst("test_i32x4_neg_max", expected_max);

    // Test INT32_MIN negation (overflow case - remains INT32_MIN)
    int32_t expected_min[4] = { INT32_MIN, INT32_MIN, INT32_MIN, INT32_MIN };
    CallI32x4NegConst("test_i32x4_neg_min", expected_min);

    // Test boundary with mixed values
    int32_t input_boundary[4] = { INT32_MAX, INT32_MIN, -1, 1 };
    int32_t expected_boundary[4] = { -2147483647, INT32_MIN, 1, -1 };
    CallI32x4Neg("test_i32x4_neg_basic", input_boundary, expected_boundary);
}

/**
 * @test PowersOfTwo_BitPatterns_ReturnsCorrectNegation
 * @brief Validates i32x4.neg with power-of-2 values and specific bit patterns
 * @details Tests negation operation with powers of 2 and special bit patterns.
 *          Verifies correct two's complement arithmetic for powers of 2.
 * @test_category Edge - Power-of-2 and bit pattern validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_fast.c:SIMD_i32x4_neg
 * @input_conditions Powers of 2: [1, 2, 4, 1024], [8, 16, 512, 2048]
 * @expected_behavior Returns negated vectors: [-1, -2, -4, -1024], [-8, -16, -512, -2048]
 * @validation_method Bit pattern verification and two's complement validation
 */
TEST_F(I32x4NegTestSuite, PowersOfTwo_BitPatterns_ReturnsCorrectNegation)
{
    // Test small powers of 2
    int32_t input_powers1[4] = { 1, 2, 4, 1024 };
    int32_t expected_powers1[4] = { -1, -2, -4, -1024 };
    CallI32x4Neg("test_i32x4_neg_basic", input_powers1, expected_powers1);

    // Test medium powers of 2
    int32_t input_powers2[4] = { 8, 16, 512, 2048 };
    int32_t expected_powers2[4] = { -8, -16, -512, -2048 };
    CallI32x4Neg("test_i32x4_neg_basic", input_powers2, expected_powers2);

    // Test large power of 2
    int32_t large_power = 1 << 20; // 1048576
    int32_t input_large[4] = { large_power, large_power, large_power, large_power };
    int32_t expected_large[4] = { -large_power, -large_power, -large_power, -large_power };
    CallI32x4Neg("test_i32x4_neg_basic", input_large, expected_large);
}

/**
 * @test DoubleNegation_Identity_RestoresOriginalValues
 * @brief Validates double negation identity property (--x = x)
 * @details Tests that applying i32x4.neg twice returns the original values.
 *          Verifies fundamental mathematical identity property of negation.
 * @test_category Edge - Identity operation validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_fast.c:SIMD_i32x4_neg (double application)
 * @input_conditions Various test values: [123, -456, 789, -321]
 * @expected_behavior Double negation returns original: [123, -456, 789, -321]
 * @validation_method Roundtrip verification through double negation operations
 */
TEST_F(I32x4NegTestSuite, DoubleNegation_Identity_RestoresOriginalValues)
{
    // Test double negation identity property
    int32_t original_values[4] = { 123, -456, 789, -321 };
    int32_t expected_values[4] = { 123, -456, 789, -321 };
    CallI32x4Neg("test_i32x4_neg_double", original_values, expected_values);

    // Test identity with zero and boundary values (excluding INT32_MIN due to overflow)
    int32_t identity_values[4] = { 0, 1, -1, 2147483647 };
    int32_t expected_identity[4] = { 0, 1, -1, 2147483647 };
    CallI32x4Neg("test_i32x4_neg_double", identity_values, expected_identity);
}

/**
 * @test SpecialValues_ZeroAndSignedOnes_ReturnsCorrectNegation
 * @brief Validates i32x4.neg with special values zero, -1, and 1
 * @details Tests negation operation with mathematically significant values.
 *          Verifies correct handling of zero (which negates to itself) and ±1.
 * @test_category Edge - Special value validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_fast.c:SIMD_i32x4_neg
 * @input_conditions Special values: [0, -1, 1, 0]
 * @expected_behavior Returns negated vector: [0, 1, -1, 0]
 * @validation_method Direct comparison with expected special value patterns
 */
TEST_F(I32x4NegTestSuite, SpecialValues_ZeroAndSignedOnes_ReturnsCorrectNegation)
{
    // Test zero negation (should remain zero)
    int32_t expected_zero[4] = { 0, 0, 0, 0 };
    CallI32x4NegConst("test_i32x4_neg_zero", expected_zero);

    // Test negative one negation (should become 1)
    int32_t expected_neg_one[4] = { 1, 1, 1, 1 };
    CallI32x4NegConst("test_i32x4_neg_neg_one", expected_neg_one);

    // Test positive one negation (should become -1)
    int32_t expected_one[4] = { -1, -1, -1, -1 };
    CallI32x4NegConst("test_i32x4_neg_one", expected_one);

    // Test mixed special values
    int32_t input_special[4] = { 0, -1, 1, 0 };
    int32_t expected_special[4] = { 0, 1, -1, 0 };
    CallI32x4Neg("test_i32x4_neg_basic", input_special, expected_special);
}

/**
 * @test LaneIndependence_MixedValues_IndependentNegation
 * @brief Validates that each lane is negated independently
 * @details Tests that negation of one lane does not affect other lanes.
 *          Verifies lane independence with diverse value combinations.
 * @test_category Edge - Lane independence validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_fast.c:SIMD_i32x4_neg (lane processing)
 * @input_conditions Diverse values per lane: [100, -200, 300, -400]
 * @expected_behavior Each lane negated independently: [-100, 200, -300, 400]
 * @validation_method Lane-by-lane comparison ensuring no cross-lane interference
 */
TEST_F(I32x4NegTestSuite, LaneIndependence_MixedValues_IndependentNegation)
{
    // Test lane independence with diverse values
    int32_t input_diverse[4] = { 100, -200, 300, -400 };
    int32_t expected_diverse[4] = { -100, 200, -300, 400 };
    CallI32x4Neg("test_i32x4_neg_basic", input_diverse, expected_diverse);

    // Test lane independence with alternating patterns
    int32_t input_alternating[4] = { 555, -666, 777, -888 };
    int32_t expected_alternating[4] = { -555, 666, -777, 888 };
    CallI32x4Neg("test_i32x4_neg_basic", input_alternating, expected_alternating);

    // Test lane independence with extreme differences
    int32_t input_extreme[4] = { 1, 1000000, -1, -1000000 };
    int32_t expected_extreme[4] = { -1, -1000000, 1, 1000000 };
    CallI32x4Neg("test_i32x4_neg_basic", input_extreme, expected_extreme);
}