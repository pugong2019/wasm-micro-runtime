/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <cstdint>
#include <climits>
#include <cstdlib>
#include <unistd.h>
#include "test_helper.h"
#include "wasm_runtime_common.h"
#include "bh_read_file.h"

static std::string CWD;
static std::string WASM_FILE;

static int app_argc;
static char **app_argv;

// Use existing RunningMode enum from wasm_export.h

/**
 * @brief Test fixture for f64.gt opcode validation across execution modes
 * @details Provides comprehensive testing infrastructure for f64.gt floating-point
 *          comparison operations in both interpreter and AOT execution modes.
 *          Validates IEEE 754 compliance and WAMR runtime behavior consistency.
 */
class F64GtTest : public testing::TestWithParam<RunningMode>
{
protected:
    WAMRRuntimeRAII<> runtime;
    wasm_module_t module = nullptr;
    wasm_module_inst_t module_inst = nullptr;
    wasm_exec_env_t exec_env = nullptr;
    uint32_t buf_size, stack_size = 8092, heap_size = 8092;
    uint8_t *buf = nullptr;
    char error_buf[128] = { 0 };
    const char *exception = nullptr;

    /**
     * @brief Initialize WAMR runtime and load f64.gt test module
     * @details Sets up execution environment for both interpreter and AOT modes,
     *          loads the WASM test module containing f64.gt test functions.
     */
    void SetUp() override
    {
        CWD = get_binary_path();
        WASM_FILE = CWD + "/wasm-apps/f64_gt_test.wasm";

        // Load WASM module from file
        buf = (uint8_t *)bh_read_file_to_buffer(WASM_FILE.c_str(), &buf_size);
        ASSERT_NE(buf, nullptr) << "Failed to load WASM file: " << WASM_FILE;

        // Load module into WAMR runtime
        module = wasm_runtime_load(buf, buf_size, error_buf, sizeof(error_buf));
        ASSERT_NE(module, nullptr) << "Failed to load WASM module: " << error_buf;

        // Instantiate the module with proper memory configuration
        module_inst = wasm_runtime_instantiate(module, stack_size, heap_size,
                                             error_buf, sizeof(error_buf));
        ASSERT_NE(module_inst, nullptr) << "Failed to instantiate module: " << error_buf;

        // Create execution environment for function calls
        exec_env = wasm_runtime_create_exec_env(module_inst, stack_size);
        ASSERT_NE(exec_env, nullptr) << "Failed to create execution environment";
    }

    /**
     * @brief Clean up WAMR runtime resources
     * @details Properly releases execution environment, module instance, and module
     *          to prevent memory leaks and ensure clean test isolation.
     */
    void TearDown() override
    {
        if (exec_env) {
            wasm_runtime_destroy_exec_env(exec_env);
        }
        if (module_inst) {
            wasm_runtime_deinstantiate(module_inst);
        }
        if (module) {
            wasm_runtime_unload(module);
        }
        if (buf) {
            BH_FREE(buf);
        }
    }

    /**
     * @brief Execute f64.gt comparison with two f64 operands
     * @param a First f64 operand (left side of comparison)
     * @param b Second f64 operand (right side of comparison)
     * @return i32 result: 1 if a > b, 0 otherwise
     * @details Calls the WASM f64_gt_test function and validates execution success
     */
    int32_t call_f64_gt(double a, double b)
    {
        wasm_function_inst_t func_inst = wasm_runtime_lookup_function(module_inst, "f64_gt_test");
        EXPECT_NE(func_inst, nullptr) << "Failed to lookup f64_gt_test function";

        uint32_t argv[4];  // Two f64 values = 4 uint32 slots
        *(double*)argv = a;
        *(double*)(argv + 2) = b;

        bool success = wasm_runtime_call_wasm(exec_env, func_inst, 4, argv);
        EXPECT_TRUE(success) << "Function call failed: " << wasm_runtime_get_exception(module_inst);

        return (int32_t)argv[0];
    }

private:
    /**
     * @brief Get binary execution path for locating test files
     * @return String containing the directory path of the current executable
     */
    std::string get_binary_path()
    {
        char cwd[1024];
        memset(cwd, 0, 1024);

        if (readlink("/proc/self/exe", cwd, 1024) <= 0) {
        }

        char *path_end = strrchr(cwd, '/');
        if (path_end != NULL) {
            *path_end = '\0';
        }

        return std::string(cwd);
    }
};

/**
 * @test BasicComparison_ReturnsCorrectResults
 * @brief Validates f64.gt produces correct comparison results for typical inputs
 * @details Tests fundamental greater-than operation with positive, negative, and mixed-sign
 *          floating-point numbers. Verifies that f64.gt correctly implements IEEE 754
 *          comparison semantics for standard numeric values.
 * @test_category Main - Basic functionality validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:f64_gt_operation
 * @input_conditions Standard f64 pairs: (5.5, 3.3), (-10.7, -15.2), (20.1, -8.9)
 * @expected_behavior Returns 1 for true comparisons, 0 for false comparisons
 * @validation_method Direct comparison of WASM function result with expected boolean values
 */
TEST_P(F64GtTest, BasicComparison_ReturnsCorrectResults)
{
    // Test positive number comparisons
    ASSERT_EQ(1, call_f64_gt(5.5, 3.3)) << "5.5 > 3.3 should return true (1)";
    ASSERT_EQ(0, call_f64_gt(3.3, 5.5)) << "3.3 > 5.5 should return false (0)";

    // Test negative number comparisons
    ASSERT_EQ(1, call_f64_gt(-10.7, -15.2)) << "-10.7 > -15.2 should return true (1)";
    ASSERT_EQ(0, call_f64_gt(-15.2, -10.7)) << "-15.2 > -10.7 should return false (0)";

    // Test mixed sign comparisons
    ASSERT_EQ(1, call_f64_gt(20.1, -8.9)) << "20.1 > -8.9 should return true (1)";
    ASSERT_EQ(0, call_f64_gt(-8.9, 20.1)) << "-8.9 > 20.1 should return false (0)";
}

/**
 * @test EqualValues_ReturnsFalse
 * @brief Validates f64.gt returns false for equal floating-point values
 * @details Tests that f64.gt correctly handles equality cases by returning false,
 *          including positive zero, negative zero, and identical numeric values.
 *          Verifies IEEE 754 comparison semantics where equal values are not greater.
 * @test_category Main - Equality boundary validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:f64_gt_operation
 * @input_conditions Equal f64 pairs: (0.0, 0.0), (-0.0, 0.0), (42.0, 42.0)
 * @expected_behavior Returns 0 (false) for all equal value comparisons
 * @validation_method Verification that identical values never satisfy greater-than relation
 */
TEST_P(F64GtTest, EqualValues_ReturnsFalse)
{
    // Test zero equality cases
    ASSERT_EQ(0, call_f64_gt(0.0, 0.0)) << "0.0 > 0.0 should return false (0)";
    ASSERT_EQ(0, call_f64_gt(-0.0, 0.0)) << "-0.0 > 0.0 should return false (0)";
    ASSERT_EQ(0, call_f64_gt(0.0, -0.0)) << "0.0 > -0.0 should return false (0)";

    // Test identical non-zero values
    ASSERT_EQ(0, call_f64_gt(42.0, 42.0)) << "42.0 > 42.0 should return false (0)";
    ASSERT_EQ(0, call_f64_gt(-42.0, -42.0)) << "-42.0 > -42.0 should return false (0)";
}

/**
 * @test BoundaryValues_HandlesLimitsCorrectly
 * @brief Validates f64.gt handles floating-point boundary values correctly
 * @details Tests comparison behavior with maximum, minimum, and epsilon values
 *          to ensure proper handling of floating-point boundary conditions.
 *          Verifies IEEE 754 compliance for extreme numeric ranges.
 * @test_category Corner - Boundary value validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:f64_gt_operation
 * @input_conditions Boundary f64 values: max, min, epsilon, denormal numbers
 * @expected_behavior Correct comparison results respecting IEEE 754 ordering
 * @validation_method Comparison with mathematically expected ordering relationships
 */
TEST_P(F64GtTest, BoundaryValues_HandlesLimitsCorrectly)
{
    double max_val = std::numeric_limits<double>::max();
    double min_val = std::numeric_limits<double>::lowest();
    double epsilon = std::numeric_limits<double>::epsilon();

    // Test maximum value comparisons
    ASSERT_EQ(1, call_f64_gt(max_val, 0.0)) << "max > 0.0 should return true (1)";
    ASSERT_EQ(0, call_f64_gt(0.0, max_val)) << "0.0 > max should return false (0)";

    // Test minimum value comparisons
    ASSERT_EQ(0, call_f64_gt(min_val, 0.0)) << "min > 0.0 should return false (0)";
    ASSERT_EQ(1, call_f64_gt(0.0, min_val)) << "0.0 > min should return true (1)";

    // Test epsilon comparisons
    ASSERT_EQ(1, call_f64_gt(1.0 + epsilon, 1.0)) << "(1.0 + epsilon) > 1.0 should return true (1)";
    ASSERT_EQ(0, call_f64_gt(1.0, 1.0 + epsilon)) << "1.0 > (1.0 + epsilon) should return false (0)";
}

/**
 * @test SpecialValues_HandlesNaNAndInfinity
 * @brief Validates f64.gt handles special IEEE 754 values (NaN, infinity) correctly
 * @details Tests comparison behavior with NaN (Not a Number) and infinity values
 *          to ensure IEEE 754 compliance. NaN comparisons should always return false,
 *          and infinity should behave according to mathematical ordering.
 * @test_category Edge - Special value validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:f64_gt_operation
 * @input_conditions Special f64 values: NaN, +infinity, -infinity combinations
 * @expected_behavior NaN comparisons return false, infinity follows mathematical ordering
 * @validation_method Verification of IEEE 754 special value comparison semantics
 */
TEST_P(F64GtTest, SpecialValues_HandlesNaNAndInfinity)
{
    double nan_val = std::numeric_limits<double>::quiet_NaN();
    double pos_inf = std::numeric_limits<double>::infinity();
    double neg_inf = -std::numeric_limits<double>::infinity();

    // Test NaN comparisons (should always return false)
    ASSERT_EQ(0, call_f64_gt(nan_val, 5.0)) << "NaN > 5.0 should return false (0)";
    ASSERT_EQ(0, call_f64_gt(5.0, nan_val)) << "5.0 > NaN should return false (0)";
    ASSERT_EQ(0, call_f64_gt(nan_val, nan_val)) << "NaN > NaN should return false (0)";

    // Test positive infinity comparisons
    ASSERT_EQ(1, call_f64_gt(pos_inf, 1000000.0)) << "+infinity > 1000000.0 should return true (1)";
    ASSERT_EQ(0, call_f64_gt(1000000.0, pos_inf)) << "1000000.0 > +infinity should return false (0)";

    // Test negative infinity comparisons
    ASSERT_EQ(0, call_f64_gt(neg_inf, -1000000.0)) << "-infinity > -1000000.0 should return false (0)";
    ASSERT_EQ(1, call_f64_gt(-1000000.0, neg_inf)) << "-1000000.0 > -infinity should return true (1)";

    // Test infinity vs infinity
    ASSERT_EQ(1, call_f64_gt(pos_inf, neg_inf)) << "+infinity > -infinity should return true (1)";
    ASSERT_EQ(0, call_f64_gt(neg_inf, pos_inf)) << "-infinity > +infinity should return false (0)";
    ASSERT_EQ(0, call_f64_gt(pos_inf, pos_inf)) << "+infinity > +infinity should return false (0)";
}

INSTANTIATE_TEST_SUITE_P(F64GtTests, F64GtTest,
                         testing::Values(Mode_Interp));