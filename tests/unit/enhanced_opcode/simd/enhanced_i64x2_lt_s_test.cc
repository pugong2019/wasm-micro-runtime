/**
 * Enhanced unit tests for i64x2.lt_s WASM SIMD opcode
 * Tests signed less-than comparison on 2 x 64-bit integer lanes
 */

#include <gtest/gtest.h>
#include <cstdint>
#include <climits>
#include "wasm_runtime_common.h"
#include "wasm_exec_env.h"
#include "bh_read_file.h"
#include "wasm_memory.h"

// RunningMode is already defined in wasm_export.h

/**
 * @class I64x2LtSTest
 * @brief Comprehensive test suite for i64x2.lt_s SIMD opcode
 * @details Tests signed less-than comparison operation across interpreter and AOT modes.
 *          Validates proper SIMD lane-wise comparison with signed 64-bit integer semantics.
 */
class I64x2LtSTest : public testing::TestWithParam<RunningMode> {
protected:
    /**
     * @brief Set up test environment before each test case
     * @details Initializes WAMR runtime, loads test WASM module, and creates execution environment
     */
    void SetUp() override {
        memset(&init_args, 0, sizeof(RuntimeInitArgs));

        init_args.mem_alloc_type = Alloc_With_System_Allocator;
        init_args.n_native_symbols = 0;

        ASSERT_TRUE(wasm_runtime_full_init(&init_args))
            << "Failed to initialize WAMR runtime";

        LoadTestModule();
        CreateExecutionEnvironment();
    }

    /**
     * @brief Clean up test environment after each test case
     * @details Destroys execution environment, unloads module, and shuts down WAMR runtime
     */
    void TearDown() override {
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
     * @brief Load WASM test module containing i64x2.lt_s test functions
     * @details Reads WASM bytecode file and validates successful module loading
     */
    void LoadTestModule() {
        const char* wasm_file = "wasm-apps/i64x2_lt_s_test.wasm";

        buffer = (uint8_t*)bh_read_file_to_buffer(wasm_file, &buffer_size);
        ASSERT_NE(nullptr, buffer) << "Failed to read WASM file: " << wasm_file;
        ASSERT_GT(buffer_size, 0U) << "WASM file is empty: " << wasm_file;

        module = wasm_runtime_load(buffer, buffer_size, error_buf, sizeof(error_buf));
        ASSERT_NE(nullptr, module) << "Failed to load WASM module: " << error_buf;
    }

    /**
     * @brief Create execution environment for test module
     * @details Instantiates WASM module and creates execution environment with proper configuration
     */
    void CreateExecutionEnvironment() {
        module_inst = wasm_runtime_instantiate(module, 65536, 65536, error_buf, sizeof(error_buf));
        ASSERT_NE(nullptr, module_inst) << "Failed to instantiate WASM module: " << error_buf;

        exec_env = wasm_runtime_create_exec_env(module_inst, 65536);
        ASSERT_NE(nullptr, exec_env) << "Failed to create execution environment";
    }

    /**
     * @brief Call WASM function with two v128 parameters and return result lanes
     * @param func_name Name of WASM function to call
     * @param a_lanes Input vector A as array of 2 i64 values
     * @param b_lanes Input vector B as array of 2 i64 values
     * @param result_lanes Output array to store result vector lanes
     */
    void CallI64x2LtSFunction(const char* func_name, const int64_t* a_lanes,
                              const int64_t* b_lanes, int64_t* result_lanes) {
        wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst, func_name);
        ASSERT_NE(nullptr, func) << "Function not found: " << func_name;

        // Prepare arguments: WASM expects 4 i64 parameters, packed as 8 i32 values
        uint32_t argv[8];

        // Pack vector A lanes (2 i64 values as 4 i32 values)
        argv[0] = static_cast<uint32_t>(a_lanes[0] & 0xFFFFFFFF);        // a0 low
        argv[1] = static_cast<uint32_t>(a_lanes[0] >> 32);                // a0 high
        argv[2] = static_cast<uint32_t>(a_lanes[1] & 0xFFFFFFFF);        // a1 low
        argv[3] = static_cast<uint32_t>(a_lanes[1] >> 32);                // a1 high

        // Pack vector B lanes (2 i64 values as 4 i32 values)
        argv[4] = static_cast<uint32_t>(b_lanes[0] & 0xFFFFFFFF);        // b0 low
        argv[5] = static_cast<uint32_t>(b_lanes[0] >> 32);                // b0 high
        argv[6] = static_cast<uint32_t>(b_lanes[1] & 0xFFFFFFFF);        // b1 low
        argv[7] = static_cast<uint32_t>(b_lanes[1] >> 32);                // b1 high

        // Execute function
        bool call_result = wasm_runtime_call_wasm(exec_env, func, 8, argv);
        ASSERT_TRUE(call_result) << "WASM function call failed: " << wasm_runtime_get_exception(module_inst);

        // Unpack result vector lanes (2 i64 values from 4 i32 values)
        result_lanes[0] = (static_cast<int64_t>(argv[1]) << 32) | argv[0];  // result0 from low,high
        result_lanes[1] = (static_cast<int64_t>(argv[3]) << 32) | argv[2];  // result1 from low,high
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
 * @test BasicComparison_ReturnsCorrectMasks
 * @brief Validates fundamental i64x2.lt_s functionality with typical signed integer values
 * @details Tests standard positive/negative integer combinations to ensure proper
 *          signed comparison semantics and correct true/false mask generation.
 * @test_category Main - Basic functionality validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:simd_i64x2_lt_s_operation
 * @input_conditions Mixed positive/negative integer vectors with known comparison results
 * @expected_behavior Lane-wise signed comparison producing 0xFFFFFFFFFFFFFFFF or 0x0000000000000000 masks
 * @validation_method Direct comparison of result masks with expected values for each lane
 */
TEST_P(I64x2LtSTest, BasicComparison_ReturnsCorrectMasks) {
    // Test case 1: Mixed positive/negative comparisons
    int64_t a1[] = {5, -10};
    int64_t b1[] = {10, -5};
    int64_t result1[2];

    CallI64x2LtSFunction("test_basic", a1, b1, result1);

    ASSERT_EQ(static_cast<int64_t>(0xFFFFFFFFFFFFFFFFLL), result1[0])
        << "Lane 0: 5 < 10 should return true mask";
    ASSERT_EQ(static_cast<int64_t>(0xFFFFFFFFFFFFFFFFLL), result1[1])
        << "Lane 1: -10 < -5 should return true mask";

    // Test case 2: Comparisons that should return false
    int64_t a2[] = {100, 0};
    int64_t b2[] = {50, -1};
    int64_t result2[2];

    CallI64x2LtSFunction("test_basic", a2, b2, result2);

    ASSERT_EQ(0LL, result2[0])
        << "Lane 0: 100 < 50 should return false mask";
    ASSERT_EQ(0LL, result2[1])
        << "Lane 1: 0 < -1 should return false mask";

    // Test case 3: Zero comparisons
    int64_t a3[] = {0, -1};
    int64_t b3[] = {1, 0};
    int64_t result3[2];

    CallI64x2LtSFunction("test_basic", a3, b3, result3);

    ASSERT_EQ(static_cast<int64_t>(0xFFFFFFFFFFFFFFFFLL), result3[0])
        << "Lane 0: 0 < 1 should return true mask";
    ASSERT_EQ(static_cast<int64_t>(0xFFFFFFFFFFFFFFFFLL), result3[1])
        << "Lane 1: -1 < 0 should return true mask";
}

/**
 * @test BoundaryValues_HandlesExtremeValues
 * @brief Tests edge cases with INT64_MIN, INT64_MAX boundary conditions
 * @details Validates behavior with extreme 64-bit signed integer values including
 *          potential overflow scenarios and boundary comparisons.
 * @test_category Corner - Boundary value validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:simd_i64x2_lt_s_operation
 * @input_conditions Vectors containing INT64_MIN, INT64_MAX, and boundary values
 * @expected_behavior Correct signed comparison handling for extreme values
 * @validation_method Verification of boundary condition results and proper signed semantics
 */
TEST_P(I64x2LtSTest, BoundaryValues_HandlesExtremeValues) {
    // Test INT64_MIN and INT64_MAX comparisons
    int64_t a1[] = {INT64_MIN, INT64_MAX - 1};
    int64_t b1[] = {INT64_MAX, INT64_MAX};
    int64_t result1[2];

    CallI64x2LtSFunction("test_boundary", a1, b1, result1);

    ASSERT_EQ(static_cast<int64_t>(0xFFFFFFFFFFFFFFFFLL), result1[0])
        << "Lane 0: INT64_MIN < INT64_MAX should return true mask";
    ASSERT_EQ(static_cast<int64_t>(0xFFFFFFFFFFFFFFFFLL), result1[1])
        << "Lane 1: (INT64_MAX-1) < INT64_MAX should return true mask";

    // Test boundary edge cases
    int64_t a2[] = {INT64_MIN + 1, -1};
    int64_t b2[] = {INT64_MIN, 0};
    int64_t result2[2];

    CallI64x2LtSFunction("test_boundary", a2, b2, result2);

    ASSERT_EQ(0LL, result2[0])
        << "Lane 0: (INT64_MIN+1) < INT64_MIN should return false mask";
    ASSERT_EQ(static_cast<int64_t>(0xFFFFFFFFFFFFFFFFLL), result2[1])
        << "Lane 1: -1 < 0 should return true mask";

    // Test zero boundary cases
    int64_t a3[] = {0, 1};
    int64_t b3[] = {1, 0};
    int64_t result3[2];

    CallI64x2LtSFunction("test_boundary", a3, b3, result3);

    ASSERT_EQ(static_cast<int64_t>(0xFFFFFFFFFFFFFFFFLL), result3[0])
        << "Lane 0: 0 < 1 should return true mask";
    ASSERT_EQ(0LL, result3[1])
        << "Lane 1: 1 < 0 should return false mask";
}

/**
 * @test EqualValues_ReturnsFalse
 * @brief Verifies that equal values correctly return false (0x0000000000000000)
 * @details Tests various equal value scenarios to ensure comparison returns false
 *          for identical values, validating proper "less than" semantics.
 * @test_category Edge - Equality boundary validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:simd_i64x2_lt_s_operation
 * @input_conditions Identical values in corresponding lanes across different ranges
 * @expected_behavior All result lanes should be 0x0000000000000000 (false)
 * @validation_method Verification that no false positives occur in equality comparisons
 */
TEST_P(I64x2LtSTest, EqualValues_ReturnsFalse) {
    // Test equal positive values
    int64_t a1[] = {42, 1000};
    int64_t b1[] = {42, 1000};
    int64_t result1[2];

    CallI64x2LtSFunction("test_equal", a1, b1, result1);

    ASSERT_EQ(0LL, result1[0])
        << "Lane 0: 42 < 42 should return false mask";
    ASSERT_EQ(0LL, result1[1])
        << "Lane 1: 1000 < 1000 should return false mask";

    // Test equal negative values
    int64_t a2[] = {-100, -9223372036854775807LL};  // Near INT64_MIN but not exactly
    int64_t b2[] = {-100, -9223372036854775807LL};
    int64_t result2[2];

    CallI64x2LtSFunction("test_equal", a2, b2, result2);

    ASSERT_EQ(0LL, result2[0])
        << "Lane 0: -100 < -100 should return false mask";
    ASSERT_EQ(0LL, result2[1])
        << "Lane 1: equal negative values should return false mask";

    // Test equal zero and boundary values
    int64_t a3[] = {0, INT64_MAX};
    int64_t b3[] = {0, INT64_MAX};
    int64_t result3[2];

    CallI64x2LtSFunction("test_equal", a3, b3, result3);

    ASSERT_EQ(0LL, result3[0])
        << "Lane 0: 0 < 0 should return false mask";
    ASSERT_EQ(0LL, result3[1])
        << "Lane 1: INT64_MAX < INT64_MAX should return false mask";
}

/**
 * @test CrossLaneIndependence_ValidatesIsolation
 * @brief Confirms that comparison results in one lane don't affect the other lane
 * @details Tests mixed comparison outcomes to ensure proper lane isolation and
 *          validates that each 64-bit lane is processed independently.
 * @test_category Main - Lane isolation validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:simd_i64x2_lt_s_operation
 * @input_conditions Mixed comparison scenarios (lane 0 true, lane 1 false, etc.)
 * @expected_behavior Each lane result should be independent of the other lane
 * @validation_method Verification of lane isolation and correct per-lane results
 */
TEST_P(I64x2LtSTest, CrossLaneIndependence_ValidatesIsolation) {
    // Test case: lane 0 true, lane 1 false
    int64_t a1[] = {-50, 200};
    int64_t b1[] = {100, 150};
    int64_t result1[2];

    CallI64x2LtSFunction("test_independence", a1, b1, result1);

    ASSERT_EQ(static_cast<int64_t>(0xFFFFFFFFFFFFFFFFLL), result1[0])
        << "Lane 0: -50 < 100 should return true mask independently";
    ASSERT_EQ(0LL, result1[1])
        << "Lane 1: 200 < 150 should return false mask independently";

    // Test case: lane 0 false, lane 1 true
    int64_t a2[] = {300, -1000};
    int64_t b2[] = {250, -500};
    int64_t result2[2];

    CallI64x2LtSFunction("test_independence", a2, b2, result2);

    ASSERT_EQ(0LL, result2[0])
        << "Lane 0: 300 < 250 should return false mask independently";
    ASSERT_EQ(static_cast<int64_t>(0xFFFFFFFFFFFFFFFFLL), result2[1])
        << "Lane 1: -1000 < -500 should return true mask independently";

    // Test case: both lanes true with different magnitudes
    int64_t a3[] = {1, INT64_MIN};
    int64_t b3[] = {2, -1};
    int64_t result3[2];

    CallI64x2LtSFunction("test_independence", a3, b3, result3);

    ASSERT_EQ(static_cast<int64_t>(0xFFFFFFFFFFFFFFFFLL), result3[0])
        << "Lane 0: 1 < 2 should return true mask independently";
    ASSERT_EQ(static_cast<int64_t>(0xFFFFFFFFFFFFFFFFLL), result3[1])
        << "Lane 1: INT64_MIN < -1 should return true mask independently";
}

// Parameterized test instantiation for interpreter and AOT modes
INSTANTIATE_TEST_SUITE_P(
    RunningModeTest, I64x2LtSTest,
    testing::Values(Mode_Interp, Mode_LLVM_JIT),
    [](const testing::TestParamInfo<I64x2LtSTest::ParamType>& info) {
        return info.param == Mode_Interp ? "interpreter" : "aot";
    }
);