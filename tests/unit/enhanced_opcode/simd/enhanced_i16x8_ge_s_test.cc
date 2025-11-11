/**
 * Enhanced unit tests for i16x8.ge_s WASM SIMD opcode
 * Tests signed greater-than-or-equal comparison on 8 x 16-bit integer lanes
 */

#include <gtest/gtest.h>
#include <cstdint>
#include <climits>
#include "wasm_runtime_common.h"
#include "wasm_exec_env.h"
#include "bh_read_file.h"
#include "wasm_memory.h"

// Using WAMR's built-in RunningMode from wasm_export.h

/**
 * @class I16x8GeSTest
 * @brief Comprehensive test suite for i16x8.ge_s SIMD opcode
 * @details Tests signed greater-than-or-equal comparison operation across interpreter and AOT modes.
 *          Validates proper SIMD lane-wise comparison with signed 16-bit integer semantics.
 */
class I16x8GeSTest : public testing::TestWithParam<RunningMode> {
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
     * @brief Load WASM test module containing i16x8.ge_s test functions
     * @details Reads WASM bytecode file and validates successful module loading
     */
    void LoadTestModule() {
        const char* wasm_file = "wasm-apps/i16x8_ge_s_test.wasm";

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
     * @param a_lanes Input vector A as array of 8 i16 values
     * @param b_lanes Input vector B as array of 8 i16 values
     * @param result_lanes Output array to store result vector lanes
     */
    void CallI16x8GeSFunction(const char* func_name, const int16_t* a_lanes,
                              const int16_t* b_lanes, int16_t* result_lanes) {
        wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst, func_name);
        ASSERT_NE(nullptr, func) << "Function not found: " << func_name;

        // Prepare arguments: two v128 vectors as 16 i32 values total (8 i16 per vector, but passed as i32)
        uint32_t argv[16];

        // Pack vector A lanes (convert i16 to i32 for WASM interface)
        for (int i = 0; i < 8; ++i) {
            argv[i] = static_cast<uint32_t>(static_cast<int32_t>(a_lanes[i]));
        }

        // Pack vector B lanes
        for (int i = 0; i < 8; ++i) {
            argv[i + 8] = static_cast<uint32_t>(static_cast<int32_t>(b_lanes[i]));
        }

        // Execute function
        bool call_result = wasm_runtime_call_wasm(exec_env, func, 16, argv);
        ASSERT_TRUE(call_result) << "WASM function call failed: " << wasm_runtime_get_exception(module_inst);

        // Unpack result vector lanes (convert back from i32 to i16)
        for (int i = 0; i < 8; ++i) {
            result_lanes[i] = static_cast<int16_t>(static_cast<int32_t>(argv[i]));
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
 * @test BasicComparison_ReturnsCorrectMasks
 * @brief Validates fundamental i16x8.ge_s functionality with typical signed integer values
 * @details Tests standard positive/negative integer combinations to ensure proper
 *          signed comparison semantics and correct true/false mask generation.
 * @test_category Main - Basic functionality validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:simd_i16x8_ge_s_operation
 * @input_conditions Mixed positive/negative integer vectors with known comparison results
 * @expected_behavior Lane-wise signed comparison producing 0xFFFF or 0x0000 masks
 * @validation_method Direct comparison of result masks with expected values for each lane
 */
TEST_P(I16x8GeSTest, BasicComparison_ReturnsCorrectMasks) {
    // Test case: Mixed positive/negative comparisons
    int16_t a[] = {1, -2, 10, 0, -100, 200, -32768, 32767};
    int16_t b[] = {0, -2, 5, 1, -50, 250, -32768, 32766};
    int16_t result[8];
    int16_t expected[] = {-1, -1, -1, 0, 0, 0, -1, -1}; // 0xFFFF = -1, 0x0000 = 0

    CallI16x8GeSFunction("test_i16x8_ge_s_basic", a, b, result);

    // Validate each lane individually
    ASSERT_EQ(expected[0], result[0]) << "Lane 0: 1 >= 0 should be true";
    ASSERT_EQ(expected[1], result[1]) << "Lane 1: -2 >= -2 should be true (equal)";
    ASSERT_EQ(expected[2], result[2]) << "Lane 2: 10 >= 5 should be true";
    ASSERT_EQ(expected[3], result[3]) << "Lane 3: 0 >= 1 should be false";
    ASSERT_EQ(expected[4], result[4]) << "Lane 4: -100 >= -50 should be false";
    ASSERT_EQ(expected[5], result[5]) << "Lane 5: 200 >= 250 should be false";
    ASSERT_EQ(expected[6], result[6]) << "Lane 6: INT16_MIN >= INT16_MIN should be true (equal)";
    ASSERT_EQ(expected[7], result[7]) << "Lane 7: INT16_MAX >= (INT16_MAX-1) should be true";
}

/**
 * @test EqualValues_ReturnsAllTrue
 * @brief Validates i16x8.ge_s returns true for all equal values (>= includes equality)
 * @details Tests that identical values in both operands produce all true results,
 *          confirming the "or equal" part of greater-than-or-equal comparison.
 * @test_category Main - Equal value validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:simd_i16x8_ge_s_operation
 * @input_conditions Identical vectors with diverse signed values including extremes
 * @expected_behavior All result lanes return 0xFFFF (true) for equal comparisons
 * @validation_method Verify all lanes return true mask for identical input vectors
 */
TEST_P(I16x8GeSTest, EqualValues_ReturnsAllTrue) {
    // Test equal values across all lanes
    int16_t equal_vec[] = {100, -100, 0, 1, -1, 32767, -32768, 500};
    int16_t result[8];
    int16_t expected_all_true[] = {-1, -1, -1, -1, -1, -1, -1, -1};

    CallI16x8GeSFunction("test_i16x8_ge_s_basic", equal_vec, equal_vec, result);

    // All lanes should return true (0xFFFF) for equal values
    for (int i = 0; i < 8; ++i) {
        ASSERT_EQ(expected_all_true[i], result[i])
            << "Lane " << i << ": Equal values should return true for >= comparison";
    }
}

/**
 * @test BoundaryValues_HandlesExtremes
 * @brief Validates i16x8.ge_s handles INT16_MIN and INT16_MAX boundary conditions
 * @details Tests extreme signed values to ensure proper signed comparison semantics
 *          at the boundaries of 16-bit integer range.
 * @test_category Main - Boundary condition validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:simd_i16x8_ge_s_operation
 * @input_conditions Combinations of INT16_MIN, INT16_MAX, and intermediate values
 * @expected_behavior Correct signed comparison results at extreme values
 * @validation_method Verify boundary comparisons follow signed integer semantics
 */
TEST_P(I16x8GeSTest, BoundaryValues_HandlesExtremes) {
    // Test boundary conditions with INT16_MIN and INT16_MAX
    int16_t a[] = {32767, -32768, 32767, -32768, 0, -1, 1, 0};
    int16_t b[] = {32766, -32767, 32767, -32768, -1, 0, 0, 1};
    int16_t result[8];
    int16_t expected[] = {-1, 0, -1, -1, -1, 0, -1, 0}; // Expected: T,F,T,T,T,F,T,F

    CallI16x8GeSFunction("test_i16x8_ge_s_basic", a, b, result);

    // Validate boundary comparisons
    ASSERT_EQ(expected[0], result[0]) << "Lane 0: INT16_MAX >= (INT16_MAX-1) should be true";
    ASSERT_EQ(expected[1], result[1]) << "Lane 1: INT16_MIN >= (INT16_MIN+1) should be false";
    ASSERT_EQ(expected[2], result[2]) << "Lane 2: INT16_MAX >= INT16_MAX should be true (equal)";
    ASSERT_EQ(expected[3], result[3]) << "Lane 3: INT16_MIN >= INT16_MIN should be true (equal)";
    ASSERT_EQ(expected[4], result[4]) << "Lane 4: 0 >= -1 should be true";
    ASSERT_EQ(expected[5], result[5]) << "Lane 5: -1 >= 0 should be false";
    ASSERT_EQ(expected[6], result[6]) << "Lane 6: 1 >= 0 should be true";
    ASSERT_EQ(expected[7], result[7]) << "Lane 7: 0 >= 1 should be false";
}

/**
 * @test MixedSignComparison_ReturnsCorrectResults
 * @brief Validates i16x8.ge_s handles positive vs negative comparisons correctly
 * @details Tests cross-sign comparisons to ensure signed semantics where
 *          positive values are greater than negative values.
 * @test_category Main - Sign handling validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:simd_i16x8_ge_s_operation
 * @input_conditions Combinations of positive and negative values across lanes
 * @expected_behavior Positive >= negative returns true, negative >= positive returns false
 * @validation_method Verify correct signed comparison semantics for cross-sign operations
 */
TEST_P(I16x8GeSTest, MixedSignComparison_ReturnsCorrectResults) {
    // Test mixed sign comparisons
    int16_t a[] = {1000, -1000, 5, -5, 32000, -32000, 10, -10};
    int16_t b[] = {-1000, 1000, -5, 5, -32000, 32000, -10, 10};
    int16_t result[8];
    int16_t expected[] = {-1, 0, -1, 0, -1, 0, -1, 0}; // All positive >= negative = true

    CallI16x8GeSFunction("test_i16x8_ge_s_basic", a, b, result);

    // Validate mixed sign comparisons
    ASSERT_EQ(expected[0], result[0]) << "Lane 0: 1000 >= -1000 should be true";
    ASSERT_EQ(expected[1], result[1]) << "Lane 1: -1000 >= 1000 should be false";
    ASSERT_EQ(expected[2], result[2]) << "Lane 2: 5 >= -5 should be true";
    ASSERT_EQ(expected[3], result[3]) << "Lane 3: -5 >= 5 should be false";
    ASSERT_EQ(expected[4], result[4]) << "Lane 4: 32000 >= -32000 should be true";
    ASSERT_EQ(expected[5], result[5]) << "Lane 5: -32000 >= 32000 should be false";
    ASSERT_EQ(expected[6], result[6]) << "Lane 6: 10 >= -10 should be true";
    ASSERT_EQ(expected[7], result[7]) << "Lane 7: -10 >= 10 should be false";
}

// Parameterized test instantiation for cross-execution mode validation
INSTANTIATE_TEST_SUITE_P(
    RunningModeTest,
    I16x8GeSTest,
    testing::Values(
        Mode_Interp,
        Mode_LLVM_JIT
    )
);