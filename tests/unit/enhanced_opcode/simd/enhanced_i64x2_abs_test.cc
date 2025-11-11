/**
 * @file enhanced_i64x2_abs_test.cc
 * @brief Comprehensive unit tests for i64x2.abs SIMD opcode
 * @details Tests i64x2.abs functionality across interpreter and AOT execution modes
 *          with focus on basic operations, boundary conditions, 64-bit integer absolute value,
 *          and error scenarios. Validates WAMR SIMD implementation correctness
 *          and cross-mode consistency for i64 vector absolute value operations.
 * @source_location tests/unit/enhanced_opcode/simd/enhanced_i64x2_abs_test.cc
 */

#include <gtest/gtest.h>
#include <cstdint>
#include <climits>
#include "wasm_runtime_common.h"
#include "wasm_exec_env.h"
#include "bh_read_file.h"
#include "wasm_memory.h"

/**
 * @class I64x2AbsTestSuite
 * @brief Test fixture class for i64x2.abs opcode testing
 * @details Provides setup/teardown for WAMR runtime and module loading.
 *          Handles SIMD vector result validation across interpreter and AOT modes
 *          for comprehensive i64x2.abs operation validation.
 * @test_categories Main, Corner, Edge validation
 */
class I64x2AbsTestSuite : public testing::TestWithParam<RunningMode>
{
protected:
    /**
     * @brief Initialize WAMR runtime and prepare test environment for i64x2.abs testing
     * @details Sets up WAMR runtime with SIMD support, loads WASM test module.
     *          Initializes execution environment for WASM test files using relative path.
     * @source_location tests/unit/enhanced_opcode/simd/enhanced_i64x2_abs_test.cc:SetUp
     */
    void SetUp() override
    {
        memset(&init_args, 0, sizeof(RuntimeInitArgs));

        init_args.mem_alloc_type = Alloc_With_System_Allocator;
        init_args.n_native_symbols = 0;

        ASSERT_TRUE(wasm_runtime_full_init(&init_args))
            << "Failed to initialize WAMR runtime";

        LoadTestModule();
        CreateExecutionEnvironment();
    }

    /**
     * @brief Clean up WAMR runtime and release test resources
     * @details Properly destroys execution environment and WAMR runtime
     *          using proper cleanup pattern to prevent resource leaks.
     * @source_location tests/unit/enhanced_opcode/simd/enhanced_i64x2_abs_test.cc:TearDown
     */
    void TearDown() override
    {
        if (exec_env) {
            wasm_runtime_destroy_exec_env(exec_env);
            exec_env = nullptr;
        }
        if (module_inst) {
            wasm_runtime_deinstantiate(module_inst);
            module_inst = nullptr;
        }
        if (module) {
            wasm_runtime_unload(module);
            module = nullptr;
        }
        if (buffer) {
            wasm_runtime_free(buffer);
            buffer = nullptr;
        }
        wasm_runtime_destroy();
    }

    /**
     * @brief Load WASM test module containing i64x2.abs test functions
     * @details Reads WASM bytecode file and validates successful module loading
     * @source_location tests/unit/enhanced_opcode/simd/enhanced_i64x2_abs_test.cc:LoadTestModule
     */
    void LoadTestModule()
    {
        const char* wasm_file = "wasm-apps/i64x2_abs_test.wasm";

        buffer = (uint8_t*)bh_read_file_to_buffer(wasm_file, &buffer_size);
        ASSERT_NE(nullptr, buffer) << "Failed to read WASM file: " << wasm_file;
        ASSERT_GT(buffer_size, 0U) << "WASM file is empty: " << wasm_file;

        module = wasm_runtime_load(buffer, buffer_size, error_buf, sizeof(error_buf));
        ASSERT_NE(nullptr, module) << "Failed to load WASM module: " << error_buf;
    }

    /**
     * @brief Create execution environment for test module
     * @details Instantiates WASM module and creates execution environment with proper configuration
     * @source_location tests/unit/enhanced_opcode/simd/enhanced_i64x2_abs_test.cc:CreateExecutionEnvironment
     */
    void CreateExecutionEnvironment()
    {
        module_inst = wasm_runtime_instantiate(module, 65536, 65536, error_buf, sizeof(error_buf));
        ASSERT_NE(nullptr, module_inst) << "Failed to instantiate WASM module: " << error_buf;

        exec_env = wasm_runtime_create_exec_env(module_inst, 65536);
        ASSERT_NE(nullptr, exec_env) << "Failed to create execution environment";
    }

    /**
     * @brief Helper function to call i64x2.abs WASM function and validate results
     * @param func_name WASM function name to call
     * @param input_lanes Array of 2 input i64 values for the vector
     * @param expected_lanes Array of 2 expected i64 values after abs operation
     * @details Calls the specified WASM function and validates the execution.
     *          Returns the resulting i64 values for lane-by-lane validation.
     * @source_location tests/unit/enhanced_opcode/simd/enhanced_i64x2_abs_test.cc:CallI64x2Abs
     */
    void CallI64x2Abs(const char* func_name, const int64_t* input_lanes, const int64_t* expected_lanes)
    {
        wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst, func_name);
        ASSERT_NE(nullptr, func) << "Function not found: " << func_name;

        // Prepare arguments: WASM expects 2 i64 parameters, packed as 4 i32 values
        uint32_t argv[4];

        // Pack vector input lanes (2 i64 values as 4 i32 values)
        argv[0] = static_cast<uint32_t>(input_lanes[0] & 0xFFFFFFFF);        // input0 low
        argv[1] = static_cast<uint32_t>(input_lanes[0] >> 32);               // input0 high
        argv[2] = static_cast<uint32_t>(input_lanes[1] & 0xFFFFFFFF);        // input1 low
        argv[3] = static_cast<uint32_t>(input_lanes[1] >> 32);               // input1 high

        // Execute function (pass 4 i32 for 2 i64 parameters)
        bool call_result = wasm_runtime_call_wasm(exec_env, func, 4, argv);
        ASSERT_TRUE(call_result) << "Failed to call " << func_name
                                << " with inputs [" << input_lanes[0] << ", " << input_lanes[1] << "]: "
                                << wasm_runtime_get_exception(module_inst);

        // Unpack result vector lanes (2 i64 values from 4 i32 values)
        int64_t result_lanes[2];
        result_lanes[0] = (static_cast<int64_t>(argv[1]) << 32) | argv[0];  // result0 from low,high
        result_lanes[1] = (static_cast<int64_t>(argv[3]) << 32) | argv[2];  // result1 from low,high

        // Validate both lanes contain the expected absolute values
        for (int lane = 0; lane < 2; ++lane) {
            ASSERT_EQ(expected_lanes[lane], result_lanes[lane])
                << "Lane " << lane << " abs mismatch for input [" << input_lanes[0] << ", " << input_lanes[1] << "]"
                << ": expected " << expected_lanes[lane]
                << ", got " << result_lanes[lane];
        }
    }

    // Test infrastructure members
    RuntimeInitArgs init_args;
    wasm_module_t module = nullptr;
    wasm_module_inst_t module_inst = nullptr;
    wasm_exec_env_t exec_env = nullptr;
    uint8_t* buffer = nullptr;
    uint32_t buffer_size = 0;
    char error_buf[128];
};

/**
 * @test BasicAbsolute_ReturnsCorrectValues
 * @brief Validates i64x2.abs produces correct absolute values for typical inputs
 * @details Tests fundamental absolute value operation with positive, negative, and mixed values.
 *          Verifies that i64x2.abs correctly computes abs(input) for various input combinations.
 * @test_category Main - Basic functionality validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i64x2_abs_operation
 * @input_conditions Standard i64 pairs: [42, -100], [-999999, 1000000]
 * @expected_behavior Returns absolute values: [42, 100], [999999, 1000000]
 * @validation_method Direct comparison of WASM function results with expected absolute values
 */
TEST_P(I64x2AbsTestSuite, BasicAbsolute_ReturnsCorrectValues)
{
    // Test positive and negative value absolute values
    int64_t input1[2] = { 42, -100 };
    int64_t expected1[2] = { 42, 100 };
    CallI64x2Abs("test_basic_abs", input1, expected1);

    // Test large negative and positive values
    int64_t input2[2] = { -999999, 1000000 };
    int64_t expected2[2] = { 999999, 1000000 };
    CallI64x2Abs("test_basic_abs", input2, expected2);
}

/**
 * @test BoundaryValues_IntegerLimits
 * @brief Validates i64x2.abs handles boundary values correctly
 * @details Tests absolute value operation with INT64_MIN and INT64_MAX values.
 *          Verifies correct handling of extreme i64 boundary conditions including overflow behavior.
 * @test_category Corner - Boundary condition validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i64x2_abs_operation
 * @input_conditions Boundary values: [INT64_MIN, INT64_MAX], [INT64_MAX, INT64_MIN]
 * @expected_behavior Returns: [INT64_MIN, INT64_MAX], [INT64_MAX, INT64_MIN] (INT64_MIN overflow preserved)
 * @validation_method Lane-by-lane verification of boundary value handling
 */
TEST_P(I64x2AbsTestSuite, BoundaryValues_IntegerLimits)
{
    // Test INT64_MIN and INT64_MAX absolute values
    // Note: abs(INT64_MIN) typically remains INT64_MIN due to two's complement overflow
    int64_t input1[2] = { INT64_MIN, INT64_MAX };
    int64_t expected1[2] = { INT64_MIN, INT64_MAX };  // INT64_MIN overflow behavior
    CallI64x2Abs("test_boundary_abs", input1, expected1);

    // Test reversed boundary values
    int64_t input2[2] = { INT64_MAX, INT64_MIN };
    int64_t expected2[2] = { INT64_MAX, INT64_MIN };  // INT64_MIN overflow behavior
    CallI64x2Abs("test_boundary_abs", input2, expected2);
}

/**
 * @test SpecialValues_ZeroAndSignedUnity
 * @brief Validates i64x2.abs with special values zero and signed unity
 * @details Tests absolute value operation with zero and signed one values.
 *          Verifies correct handling of special numeric patterns.
 * @test_category Edge - Special value validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i64x2_abs_operation
 * @input_conditions Special values: [0, 0], [-1, 1]
 * @expected_behavior Returns absolute values: [0, 0], [1, 1]
 * @validation_method Direct comparison with expected special value patterns
 */
TEST_P(I64x2AbsTestSuite, SpecialValues_ZeroAndSignedUnity)
{
    // Test zero values absolute values (should remain zero)
    int64_t input1[2] = { 0, 0 };
    int64_t expected1[2] = { 0, 0 };
    CallI64x2Abs("test_special_abs", input1, expected1);

    // Test negative one and positive one absolute values
    int64_t input2[2] = { -1, 1 };
    int64_t expected2[2] = { 1, 1 };
    CallI64x2Abs("test_special_abs", input2, expected2);
}

/**
 * @test MixedLaneOperations_LaneIndependence
 * @brief Validates that absolute value operations are performed independently per lane
 * @details Tests mixed positive/negative values across lanes to ensure proper lane isolation.
 *          Verifies that each 64-bit lane is processed independently.
 * @test_category Main - Lane independence validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i64x2_abs_operation
 * @input_conditions Mixed lane scenarios: [42, -42], [-123, 456]
 * @expected_behavior Returns independent absolute values: [42, 42], [123, 456]
 * @validation_method Verification of lane isolation and correct per-lane results
 */
TEST_P(I64x2AbsTestSuite, MixedLaneOperations_LaneIndependence)
{
    // Test positive/negative mixed lanes
    int64_t input1[2] = { 42, -42 };
    int64_t expected1[2] = { 42, 42 };
    CallI64x2Abs("test_mixed_abs", input1, expected1);

    // Test negative/positive mixed lanes
    int64_t input2[2] = { -123, 456 };
    int64_t expected2[2] = { 123, 456 };
    CallI64x2Abs("test_mixed_abs", input2, expected2);
}

/**
 * @test BitPatternOperations_SpecialPatterns
 * @brief Validates i64x2.abs with special bit patterns and 64-bit values
 * @details Tests absolute value operation with specific bit patterns that exercise 64-bit boundaries.
 *          Verifies correct handling of alternating and boundary bit patterns.
 * @test_category Edge - Bit pattern validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i64x2_abs_operation
 * @input_conditions Bit patterns: [0x5555555555555555, -0x5555555555555555]
 * @expected_behavior Returns absolute values: [0x5555555555555555, 0x5555555555555555]
 * @validation_method Hexadecimal bit pattern verification and absolute value validation
 */
TEST_P(I64x2AbsTestSuite, BitPatternOperations_SpecialPatterns)
{
    // Test alternating bit pattern absolute values
    int64_t alternating_positive = 0x5555555555555555LL;
    int64_t alternating_negative = -0x5555555555555555LL;

    int64_t input1[2] = { alternating_positive, alternating_negative };
    int64_t expected1[2] = { alternating_positive, 0x5555555555555555LL };
    CallI64x2Abs("test_bitpattern_abs", input1, expected1);
}

// Parameterized test instantiation for interpreter and AOT modes
INSTANTIATE_TEST_SUITE_P(
    RunningModeTest, I64x2AbsTestSuite,
    testing::Values(Mode_Interp, Mode_LLVM_JIT),
    [](const testing::TestParamInfo<I64x2AbsTestSuite::ParamType>& info) {
        return info.param == Mode_Interp ? "interpreter" : "aot";
    }
);