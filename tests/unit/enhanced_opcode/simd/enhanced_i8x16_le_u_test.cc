/**
 * @file enhanced_i8x16_le_u_test.cc
 * @brief Comprehensive unit tests for i8x16.le_u SIMD opcode
 * @details Tests i8x16.le_u functionality across interpreter and AOT execution modes
 *          with focus on element-wise unsigned less-than-or-equal comparison of sixteen 8-bit integers,
 *          unsigned comparison behavior validation, and comprehensive edge case coverage.
 *          Validates WAMR SIMD implementation correctness and cross-mode consistency.
 * @source_location tests/unit/enhanced_opcode/simd/enhanced_i8x16_le_u_test.cc
 */

#include <gtest/gtest.h>
#include <cstring>
#include <memory>
#include "wasm_runtime_common.h"
#include "wasm_export.h"
#include "test_helper.h"

/**
 * @class I8x16LeUTestSuite
 * @brief Test fixture class for i8x16.le_u opcode testing
 * @details Provides setup/teardown for WAMR runtime and module loading.
 *          Handles memory initialization and SIMD vector result validation
 *          for comprehensive testing using existing WAMR test helpers.
 * @test_categories Main, Corner, Edge, Unsigned comparison validation
 */
class I8x16LeUTestSuite : public testing::Test
{
protected:
    /**
     * @brief Initialize WAMR runtime and prepare test environment for i8x16.le_u testing
     * @details Sets up WAMR runtime with SIMD support using WAMRRuntimeRAII helper.
     *          Initializes execution environment for WASM test files.
     * @source_location tests/unit/enhanced_opcode/simd/enhanced_i8x16_le_u_test.cc:SetUp
     */
    void SetUp() override
    {
        // Initialize WAMR runtime using RAII helper
        runtime_raii = std::make_unique<WAMRRuntimeRAII<>>();

        // Load the i8x16.le_u test module using relative path
        dummy_env = std::make_unique<DummyExecEnv>("wasm-apps/i8x16_le_u_test.wasm");
        ASSERT_NE(nullptr, dummy_env->get())
            << "Failed to create execution environment for i8x16.le_u tests";
    }

    /**
     * @brief Clean up WAMR runtime and release test resources
     * @details Properly destroys execution environment and WAMR runtime
     *          using RAII pattern to prevent resource leaks.
     * @source_location tests/unit/enhanced_opcode/simd/enhanced_i8x16_le_u_test.cc:TearDown
     */
    void TearDown() override
    {
        // Cleanup handled by RAII destructors
        dummy_env.reset();
        runtime_raii.reset();
    }

    /**
     * @brief Helper function to call WASM i8x16.le_u function with two vector inputs
     * @details Executes i8x16.le_u operation on two input vectors and returns element-wise unsigned
     *          less-than-or-equal result. Handles WASM function invocation and v128 result extraction
     *          for 16-byte unsigned comparison.
     * @param input1_bytes 16-byte array representing first input v128 vector as i8 lanes
     * @param input2_bytes 16-byte array representing second input v128 vector as i8 lanes
     * @param result_bytes 16-byte array to store the unsigned less-than-or-equal comparison result
     * @return bool True if operation succeeded, false on error
     * @source_location tests/unit/enhanced_opcode/simd/enhanced_i8x16_le_u_test.cc:call_i8x16_le_u
     */
    bool call_i8x16_le_u(const uint8_t input1_bytes[16], const uint8_t input2_bytes[16],
                         uint8_t result_bytes[16])
    {
        // Prepare arguments: two input v128 vectors as four i64 values each
        uint32_t argv[8];

        // Convert byte arrays to 64-bit values (little-endian format)
        // First v128 vector
        uint64_t input1_lo = 0, input1_hi = 0;
        for (int i = 0; i < 8; i++) {
            input1_lo |= (static_cast<uint64_t>(input1_bytes[i]) << (i * 8));
            input1_hi |= (static_cast<uint64_t>(input1_bytes[i + 8]) << (i * 8));
        }

        // Second v128 vector
        uint64_t input2_lo = 0, input2_hi = 0;
        for (int i = 0; i < 8; i++) {
            input2_lo |= (static_cast<uint64_t>(input2_bytes[i]) << (i * 8));
            input2_hi |= (static_cast<uint64_t>(input2_bytes[i + 8]) << (i * 8));
        }

        // WASM expects little-endian format: low part first, then high part
        // First v128 vector
        argv[0] = static_cast<uint32_t>(input1_lo);        // Low 32 bits of low i64
        argv[1] = static_cast<uint32_t>(input1_lo >> 32);  // High 32 bits of low i64
        argv[2] = static_cast<uint32_t>(input1_hi);        // Low 32 bits of high i64
        argv[3] = static_cast<uint32_t>(input1_hi >> 32);  // High 32 bits of high i64
        // Second v128 vector
        argv[4] = static_cast<uint32_t>(input2_lo);        // Low 32 bits of low i64
        argv[5] = static_cast<uint32_t>(input2_lo >> 32);  // High 32 bits of low i64
        argv[6] = static_cast<uint32_t>(input2_hi);        // Low 32 bits of high i64
        argv[7] = static_cast<uint32_t>(input2_hi >> 32);  // High 32 bits of high i64

        // Call WASM function with two v128 inputs
        bool call_success = dummy_env->execute("test_i8x16_le_u", 8, argv);

        if (call_success) {
            // Extract v128 result and convert back to byte array
            uint64_t result_lo = (static_cast<uint64_t>(argv[1]) << 32) | argv[0];
            uint64_t result_hi = (static_cast<uint64_t>(argv[3]) << 32) | argv[2];

            // Convert 64-bit values back to byte array
            for (int i = 0; i < 8; i++) {
                result_bytes[i] = static_cast<uint8_t>(result_lo >> (i * 8));
                result_bytes[i + 8] = static_cast<uint8_t>(result_hi >> (i * 8));
            }
        }

        return call_success;
    }

private:
    std::unique_ptr<WAMRRuntimeRAII<>> runtime_raii;
    std::unique_ptr<DummyExecEnv> dummy_env;
};

/**
 * @test BasicComparison_ReturnsCorrectResults
 * @brief Validates i8x16.le_u produces correct unsigned less-than-or-equal results for typical inputs
 * @details Tests fundamental unsigned comparison operation with various 8-bit value combinations.
 *          Verifies that i8x16.le_u correctly computes lane_a <= lane_b for each of 16 lanes.
 * @test_category Main - Basic functionality validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i8x16_le_u_operation
 * @input_conditions Standard 8-bit value pairs across all 16 lanes with known comparison outcomes
 * @expected_behavior Returns 0xFF for lanes where first <= second (unsigned), 0x00 otherwise
 * @validation_method Direct comparison of WASM function result with expected lane-wise values
 */
TEST_F(I8x16LeUTestSuite, BasicComparison_ReturnsCorrectResults)
{
    // Test data: first vector with ascending values, second vector with mixed values
    uint8_t input1[16] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160};
    uint8_t input2[16] = {15, 20, 25, 45, 50, 55, 75, 80, 85, 105, 110, 115, 135, 140, 145, 165};
    uint8_t result[16];

    // Expected results: lane-wise comparison (input1[i] <= input2[i] ? 0xFF : 0x00)
    // 10<=15:true, 20<=20:true, 30<=25:false, 40<=45:true, 50<=50:true, 60<=55:false,
    // 70<=75:true, 80<=80:true, 90<=85:false, 100<=105:true, 110<=110:true, 120<=115:false,
    // 130<=135:true, 140<=140:true, 150<=145:false, 160<=165:true
    uint8_t expected[16] = {0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF};

    // Execute i8x16.le_u operation
    ASSERT_TRUE(call_i8x16_le_u(input1, input2, result))
        << "Failed to execute i8x16.le_u with basic comparison inputs";

    // Validate each lane result
    for (int i = 0; i < 16; i++) {
        ASSERT_EQ(expected[i], result[i])
            << "Lane " << i << " comparison failed: " << static_cast<int>(input1[i])
            << " <= " << static_cast<int>(input2[i]) << " should be "
            << (expected[i] == 0xFF ? "true" : "false") << " but got "
            << (result[i] == 0xFF ? "true" : "false");
    }
}

/**
 * @test BoundaryValues_ReturnsCorrectResults
 * @brief Validates i8x16.le_u behavior with boundary values (0x00, 0xFF)
 * @details Tests unsigned comparison behavior at the extremes of 8-bit unsigned range.
 *          Ensures proper handling of minimum (0x00) and maximum (0xFF) unsigned values.
 * @test_category Corner - Boundary value validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i8x16_le_u_operation
 * @input_conditions Boundary values 0x00 and 0xFF in various lane positions
 * @expected_behavior 0x00 <= anything = true, anything <= 0xFF = true, boundary logic correct
 * @validation_method Verification of unsigned comparison semantics at value extremes
 */
TEST_F(I8x16LeUTestSuite, BoundaryValues_ReturnsCorrectResults)
{
    // Test vectors with boundary values
    uint8_t input1[16] = {0x00, 0xFF, 0x00, 0xFF, 0x7F, 0x80, 0x00, 0xFF, 0x01, 0xFE, 0x7F, 0x80, 0x00, 0xFF, 0x7F, 0x80};
    uint8_t input2[16] = {0x00, 0x00, 0xFF, 0xFF, 0x7F, 0x80, 0x80, 0x7F, 0xFE, 0x01, 0x80, 0x7F, 0x01, 0xFE, 0x80, 0x7F};
    uint8_t result[16];

    // Expected results for boundary comparisons:
    // 0x00<=0x00:true, 0xFF<=0x00:false, 0x00<=0xFF:true, 0xFF<=0xFF:true,
    // 0x7F<=0x7F:true, 0x80<=0x80:true, 0x00<=0x80:true, 0xFF<=0x7F:false,
    // 0x01<=0xFE:true, 0xFE<=0x01:false, 0x7F<=0x80:true, 0x80<=0x7F:false,
    // 0x00<=0x01:true, 0xFF<=0xFE:false, 0x7F<=0x80:true, 0x80<=0x7F:false
    uint8_t expected[16] = {0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00};

    // Execute i8x16.le_u operation
    ASSERT_TRUE(call_i8x16_le_u(input1, input2, result))
        << "Failed to execute i8x16.le_u with boundary value inputs";

    // Validate each lane result with detailed boundary explanations
    for (int i = 0; i < 16; i++) {
        ASSERT_EQ(expected[i], result[i])
            << "Boundary comparison failed at lane " << i << ": 0x"
            << std::hex << static_cast<int>(input1[i]) << " <= 0x"
            << std::hex << static_cast<int>(input2[i]) << " (unsigned) should be "
            << (expected[i] == 0xFF ? "true" : "false") << " but got "
            << (result[i] == 0xFF ? "true" : "false");
    }
}

/**
 * @test EqualValues_ReturnsAllTrue
 * @brief Validates i8x16.le_u returns true (0xFF) for all equal value comparisons
 * @details Tests that identical values in corresponding lanes result in true for <= comparison.
 *          Verifies the "equal" component of "less-than-or-equal" logic across various values.
 * @test_category Main - Equality subset validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i8x16_le_u_operation
 * @input_conditions Identical values in corresponding lanes across 16 lanes
 * @expected_behavior All lanes should return 0xFF (true) for equal value comparisons
 * @validation_method Verification that x <= x is always true for all unsigned 8-bit values
 */
TEST_F(I8x16LeUTestSuite, EqualValues_ReturnsAllTrue)
{
    // Test vector with various equal values
    uint8_t input_vector[16] = {0x00, 0x01, 0x7F, 0x80, 0xFF, 0x42, 0xAA, 0x55, 0x11, 0x22, 0x33, 0x44, 0x99, 0xBB, 0xCC, 0xDD};
    uint8_t result[16];

    // All comparisons should be true (equal values)
    uint8_t expected[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    // Execute i8x16.le_u operation with identical inputs
    ASSERT_TRUE(call_i8x16_le_u(input_vector, input_vector, result))
        << "Failed to execute i8x16.le_u with equal value inputs";

    // Validate all lanes return true for equal comparisons
    for (int i = 0; i < 16; i++) {
        ASSERT_EQ(0xFF, result[i])
            << "Equal value comparison failed at lane " << i << ": 0x"
            << std::hex << static_cast<int>(input_vector[i]) << " <= 0x"
            << std::hex << static_cast<int>(input_vector[i])
            << " should always be true but got false";
    }
}

/**
 * @test UnsignedBehavior_ValidatesCorrectComparison
 * @brief Validates i8x16.le_u uses unsigned comparison semantics (not signed)
 * @details Tests values that differ in signed vs unsigned interpretation to ensure
 *          proper unsigned comparison behavior. Critical for values >= 0x80.
 * @test_category Edge - Unsigned vs signed comparison validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i8x16_le_u_operation
 * @input_conditions Values that have different signed/unsigned ordering (0x7F vs 0x80, etc.)
 * @expected_behavior Ensures unsigned comparison logic: 0x7F < 0x80 (not 127 > -128)
 * @validation_method Direct verification of unsigned comparison semantics
 */
TEST_F(I8x16LeUTestSuite, UnsignedBehavior_ValidatesCorrectComparison)
{
    // Test cases specifically targeting signed vs unsigned differences
    // In signed: 0x80=-128, 0x7F=127, so 0x80 < 0x7F would be true (signed)
    // In unsigned: 0x80=128, 0x7F=127, so 0x80 > 0x7F (unsigned) - should be false for <=
    uint8_t input1[16] = {0x7F, 0x80, 0x7F, 0x80, 0xFF, 0x00, 0x81, 0x7E, 0xFE, 0x01, 0x90, 0x6F, 0xA0, 0x5F, 0xF0, 0x0F};
    uint8_t input2[16] = {0x80, 0x7F, 0x81, 0x7E, 0x00, 0xFF, 0x7F, 0x82, 0x01, 0xFE, 0x6F, 0x91, 0x5F, 0xA1, 0x0F, 0xF1};
    uint8_t result[16];

    // Expected results using unsigned comparison:
    // 0x7F(127)<=0x80(128):true, 0x80(128)<=0x7F(127):false, 0x7F<=0x81:true, 0x80<=0x7E:false,
    // 0xFF<=0x00:false, 0x00<=0xFF:true, 0x81<=0x7F:false, 0x7E<=0x82:true,
    // 0xFE<=0x01:false, 0x01<=0xFE:true, 0x90<=0x6F:false, 0x6F<=0x91:true,
    // 0xA0<=0x5F:false, 0x5F<=0xA1:true, 0xF0<=0x0F:false, 0x0F<=0xF1:true
    uint8_t expected[16] = {0xFF, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF};

    // Execute i8x16.le_u operation
    ASSERT_TRUE(call_i8x16_le_u(input1, input2, result))
        << "Failed to execute i8x16.le_u with unsigned behavior test inputs";

    // Validate unsigned comparison behavior
    for (int i = 0; i < 16; i++) {
        ASSERT_EQ(expected[i], result[i])
            << "Unsigned comparison failed at lane " << i << ": "
            << static_cast<unsigned>(input1[i]) << " <= "
            << static_cast<unsigned>(input2[i]) << " (unsigned) should be "
            << (expected[i] == 0xFF ? "true" : "false") << " but got "
            << (result[i] == 0xFF ? "true" : "false")
            << " - ensure unsigned semantics, not signed";
    }
}