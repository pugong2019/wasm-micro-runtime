/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

/**
 * Enhanced Unit Tests for i32.add WASM Opcode
 *
 * Tests comprehensive functionality of 32-bit integer addition operation
 * across interpreter and AOT execution modes with focus on:
 * - Basic arithmetic operations
 * - Boundary condition handling
 * - Overflow/underflow wrap-around behavior
 * - Edge case validation (zero operands, identity operations)
 * - Cross-mode execution consistency
 *
 * Target Coverage:
 * - core/iwasm/interpreter/wasm_interp_classic.c: i32.add implementation
 * - core/iwasm/aot/aot_runtime.c: AOT i32.add execution path
 */

#include <gtest/gtest.h>
#include <climits>
#include <cstdint>
#include "wasm_export.h"
#include "bh_read_file.h"

class I32AddTestSuite : public testing::TestWithParam<RunningMode> {
protected:
    void SetUp() override {
        // Initialize WAMR runtime
        RuntimeInitArgs init_args;
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_System_Allocator;

        ASSERT_TRUE(wasm_runtime_full_init(&init_args))
            << "Failed to initialize WAMR runtime";

        // Load WASM module
        std::string wasm_file = "./wasm-apps/i32_add_test.wasm";
        module_buffer = reinterpret_cast<uint8_t*>(
            bh_read_file_to_buffer(wasm_file.c_str(), &buffer_size));
        ASSERT_NE(nullptr, module_buffer)
            << "Failed to load WASM file: " << wasm_file;

        // Load and validate module
        char error_buf[128];
        module = wasm_runtime_load(module_buffer, buffer_size,
                                   error_buf, sizeof(error_buf));
        ASSERT_NE(nullptr, module)
            << "Failed to load WASM module: " << error_buf;

        // Instantiate module
        exec_env = wasm_runtime_instantiate(module, 8192, 8192,
                                           error_buf, sizeof(error_buf));
        ASSERT_NE(nullptr, exec_env)
            << "Failed to instantiate module: " << error_buf;

        // Set execution mode
        wasm_runtime_set_running_mode(exec_env, GetParam());
    }

    void TearDown() override {
        if (exec_env) {
            wasm_runtime_deinstantiate(exec_env);
            exec_env = nullptr;
        }
        if (module) {
            wasm_runtime_unload(module);
            module = nullptr;
        }
        if (module_buffer) {
            BH_FREE(module_buffer);
            module_buffer = nullptr;
        }
        wasm_runtime_destroy();
    }

    // Execute i32.add test function
    int32_t ExecuteI32Add(int32_t operand1, int32_t operand2) {
        wasm_function_inst_t func = wasm_runtime_lookup_function(
            exec_env, "test_i32_add", nullptr);
        EXPECT_NE(nullptr, func) << "Function 'test_i32_add' not found";

        uint32_t argv[2] = {static_cast<uint32_t>(operand1),
                           static_cast<uint32_t>(operand2)};
        bool ret = wasm_runtime_call_wasm(exec_env, func, 2, argv);
        EXPECT_TRUE(ret) << "Function execution failed";

        return static_cast<int32_t>(argv[0]);
    }

private:
    uint8_t *module_buffer = nullptr;
    uint32_t buffer_size = 0;
    wasm_module_t module = nullptr;
    wasm_module_inst_t exec_env = nullptr;
};

// Test both execution modes
INSTANTIATE_TEST_SUITE_P(
    CrossMode, I32AddTestSuite,
    testing::Values(Running_Mode_Interp, Running_Mode_AoT),
    [](const testing::TestParamInfo<RunningMode>& info) {
        return info.param == Running_Mode_Interp ? "Interpreter" : "AOT";
    }
);

// === MAIN ROUTINE TESTS ===

TEST_P(I32AddTestSuite, Basic_Positive_Addition_ReturnsCorrectSum) {
    int32_t result = ExecuteI32Add(5, 3);
    ASSERT_EQ(8, result);
}

TEST_P(I32AddTestSuite, Basic_Negative_Addition_ReturnsCorrectSum) {
    int32_t result = ExecuteI32Add(-10, -15);
    ASSERT_EQ(-25, result);
}

TEST_P(I32AddTestSuite, Mixed_Sign_Addition_ReturnsCorrectSum) {
    int32_t result = ExecuteI32Add(20, -8);
    ASSERT_EQ(12, result);
}

TEST_P(I32AddTestSuite, Large_Value_Addition_ReturnsCorrectSum) {
    int32_t result = ExecuteI32Add(1000000, 2000000);
    ASSERT_EQ(3000000, result);
}

TEST_P(I32AddTestSuite, Commutative_Property_ReturnsIdenticalResults) {
    int32_t result1 = ExecuteI32Add(123, 456);
    int32_t result2 = ExecuteI32Add(456, 123);
    ASSERT_EQ(579, result1);
    ASSERT_EQ(result1, result2);
}

// === CORNER CASE TESTS ===

TEST_P(I32AddTestSuite, MaxBoundary_Overflow_WrapsToMinimum) {
    int32_t result = ExecuteI32Add(INT32_MAX, 1);
    ASSERT_EQ(INT32_MIN, result);
}

TEST_P(I32AddTestSuite, MaxDouble_Overflow_WrapsCorrectly) {
    int32_t result = ExecuteI32Add(INT32_MAX, INT32_MAX);
    ASSERT_EQ(-2, result);
}

TEST_P(I32AddTestSuite, MinBoundary_Underflow_WrapsToMaximum) {
    int32_t result = ExecuteI32Add(INT32_MIN, -1);
    ASSERT_EQ(INT32_MAX, result);
}

TEST_P(I32AddTestSuite, MinDouble_Underflow_WrapsCorrectly) {
    int32_t result = ExecuteI32Add(INT32_MIN, INT32_MIN);
    ASSERT_EQ(0, result);
}

TEST_P(I32AddTestSuite, BoundaryCross_Addition_ReturnsCorrectResult) {
    int32_t result = ExecuteI32Add(INT32_MAX, INT32_MIN);
    ASSERT_EQ(-1, result);
}

// === EDGE CASE TESTS ===

TEST_P(I32AddTestSuite, ZeroIdentity_Left_ReturnsRightOperand) {
    int32_t result = ExecuteI32Add(0, 42);
    ASSERT_EQ(42, result);
}

TEST_P(I32AddTestSuite, ZeroIdentity_Right_ReturnsLeftOperand) {
    int32_t result = ExecuteI32Add(42, 0);
    ASSERT_EQ(42, result);
}

TEST_P(I32AddTestSuite, ZeroBoth_Operands_ReturnsZero) {
    int32_t result = ExecuteI32Add(0, 0);
    ASSERT_EQ(0, result);
}

TEST_P(I32AddTestSuite, AdditiveInverse_Property_ReturnsZero) {
    int32_t result = ExecuteI32Add(1, -1);
    ASSERT_EQ(0, result);
}

TEST_P(I32AddTestSuite, PowerOfTwo_Overflow_WrapsCorrectly) {
    // 2^30 + 2^30 = 2^31 (overflow to INT32_MIN)
    int32_t result = ExecuteI32Add(1073741824, 1073741824);
    ASSERT_EQ(INT32_MIN, result);
}