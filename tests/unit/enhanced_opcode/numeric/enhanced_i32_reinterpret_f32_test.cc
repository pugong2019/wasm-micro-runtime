/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <gtest/gtest.h>  // Primary GTest framework for unit testing
#include <cfloat>         // IEEE 754 floating-point limits and constants
#include <cstdint>        // Standard integer types for precise type control
#include <cmath>          // Mathematical functions for special value handling
#include <vector>         // Container for batch test case management
#include "wasm_export.h"  // Core WAMR runtime API for module management
#include "bh_read_file.h" // WAMR utility for loading WASM binary files

/**
 * @file enhanced_i32_reinterpret_f32_test.cc
 * @brief Enhanced unit tests for i32.reinterpret_f32 opcode - Numeric Category
 * @details This test suite provides comprehensive coverage for the i32.reinterpret_f32 (bit reinterpretation)
 * WebAssembly instruction, focusing on:
 * - Basic bit reinterpretation functionality from f32 to i32 without arithmetic conversion
 * - Corner cases including IEEE 754 boundary values (FLT_MIN, FLT_MAX) and special values
 * - Edge cases with NaN patterns, infinity values, and subnormal numbers
 * - Special IEEE 754 bit patterns including sign bit, exponent, and mantissa field preservation
 * - Cross-execution mode validation between interpreter and AOT compilation
 *
 * @coverage_target core/iwasm/interpreter/wasm_interp_fast.c:i32.reinterpret_f32 operations
 * @coverage_target core/iwasm/compilation/aot_emit_conversion.c:LLVMBuildBitCast implementation
 * @coverage_target Bit reinterpretation behavior and IEEE 754 pattern preservation
 * @coverage_target Stack management for type reinterpretation operations
 * @test_modes Both interpreter (Mode_Interp) and AOT (Mode_LLVM_JIT) execution
 */

class I32ReinterpretF32TestSuite : public testing::TestWithParam<RunningMode> {
protected:
    void SetUp() override {
        // Initialize WAMR runtime with system allocator for test isolation
        RuntimeInitArgs init_args;
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_System_Allocator;

        ASSERT_TRUE(wasm_runtime_full_init(&init_args))
            << "Failed to initialize WAMR runtime for i32.reinterpret_f32 testing";

        // Load WASM test module containing i32.reinterpret_f32 test functions
        std::string wasm_file = "./wasm-apps/i32_reinterpret_f32_test.wasm";
        module_buffer = reinterpret_cast<uint8_t*>(
            bh_read_file_to_buffer(wasm_file.c_str(), &buffer_size));
        ASSERT_NE(nullptr, module_buffer)
            << "Failed to load WASM file: " << wasm_file;

        // Load and validate WASM module with error reporting
        char error_buf[128];
        module = wasm_runtime_load(module_buffer, buffer_size,
                                 error_buf, sizeof(error_buf));
        ASSERT_NE(nullptr, module)
            << "Failed to load WASM module: " << error_buf;

        // Create module instance for test execution
        module_inst = wasm_runtime_instantiate(module, 8192, 8192,
                                             error_buf, sizeof(error_buf));
        ASSERT_NE(nullptr, module_inst)
            << "Failed to instantiate WASM module: " << error_buf;

        // Set execution mode for parameterized testing
        wasm_runtime_set_running_mode(module_inst, GetParam());
    }

    void TearDown() override {
        // Clean up WASM resources in proper order
        if (module_inst) {
            wasm_runtime_deinstantiate(module_inst);
            module_inst = nullptr;
        }
        if (module) {
            wasm_runtime_unload(module);
            module = nullptr;
        }
        if (module_buffer) {
            wasm_runtime_free(module_buffer);
            module_buffer = nullptr;
        }

        // Shutdown WAMR runtime
        wasm_runtime_destroy();
    }

    /**
     * @brief Helper function to call i32.reinterpret_f32 WASM function
     * @param input f32 input value to be reinterpreted as i32
     * @return i32 bit pattern from WASM execution
     * @details Executes the WASM reinterpret_f32 function and returns the i32 bit pattern.
     * Function handles WAMR execution context and validates successful execution.
     */
    int32_t call_reinterpret_f32(float input) {
        wasm_function_inst_t func_inst = wasm_runtime_lookup_function(
            module_inst, "reinterpret_f32");
        EXPECT_NE(nullptr, func_inst) << "Failed to lookup reinterpret_f32 function";

        // Create execution environment for function calls
        wasm_exec_env_t exec_env = wasm_runtime_create_exec_env(module_inst, 65536);
        EXPECT_NE(nullptr, exec_env) << "Failed to create execution environment";

        // Prepare function arguments: f32 input as uint32 bit pattern
        union { float f; uint32_t u; } converter;
        converter.f = input;
        uint32_t argv[1] = { converter.u };

        // Execute function with error handling
        bool call_result = wasm_runtime_call_wasm(exec_env, func_inst, 1, argv);
        EXPECT_TRUE(call_result)
            << "reinterpret_f32 function call failed: " << wasm_runtime_get_exception(module_inst);

        // Clean up execution environment
        wasm_runtime_destroy_exec_env(exec_env);

        // Return result (i32 value is in argv[0] after call)
        return static_cast<int32_t>(argv[0]);
    }

    /**
     * @brief Helper function to call special values test WASM function
     * @param input f32 input value for special value testing
     * @return i32 bit pattern from special values test function
     * @details Executes the WASM reinterpret_f32_special function for testing IEEE 754 special values.
     */
    int32_t call_reinterpret_f32_special(float input) {
        wasm_function_inst_t func_inst = wasm_runtime_lookup_function(
            module_inst, "reinterpret_f32_special");
        EXPECT_NE(nullptr, func_inst) << "Failed to lookup reinterpret_f32_special function";

        // Create execution environment for function calls
        wasm_exec_env_t exec_env = wasm_runtime_create_exec_env(module_inst, 65536);
        EXPECT_NE(nullptr, exec_env) << "Failed to create execution environment";

        // Prepare function arguments: f32 input as uint32 bit pattern
        union { float f; uint32_t u; } converter;
        converter.f = input;
        uint32_t argv[1] = { converter.u };

        // Execute function with error handling
        bool call_result = wasm_runtime_call_wasm(exec_env, func_inst, 1, argv);
        EXPECT_TRUE(call_result)
            << "reinterpret_f32_special function call failed: " << wasm_runtime_get_exception(module_inst);

        // Clean up execution environment
        wasm_runtime_destroy_exec_env(exec_env);

        // Return result (i32 value is in argv[0] after call)
        return static_cast<int32_t>(argv[0]);
    }

    /**
     * @brief Helper function to convert f32 to expected i32 bit pattern
     * @param input f32 value to convert
     * @return Expected i32 bit pattern for comparison
     * @details Performs native bit reinterpretation for expected result validation.
     */
    int32_t f32_to_i32_bits(float input) {
        union { float f; int32_t i; } converter;
        converter.f = input;
        return converter.i;
    }

    // WAMR runtime resources
    wasm_module_t module = nullptr;
    wasm_module_inst_t module_inst = nullptr;
    uint8_t* module_buffer = nullptr;
    uint32_t buffer_size = 0;
};

/**
 * @test BasicReinterpretation_TypicalValues_ReturnsCorrectBitPattern
 * @brief Validates i32.reinterpret_f32 produces correct bit patterns for typical f32 inputs
 * @details Tests fundamental bit reinterpretation with positive, negative, and fractional values.
 *          Verifies that IEEE 754 bit patterns are preserved exactly without arithmetic conversion.
 * @test_category Main - Basic functionality validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_fast.c:i32_reinterpret_f32_operation
 * @input_conditions Standard f32 values: 1.0f, -1.0f, 2.0f, 0.5f
 * @expected_behavior Returns IEEE 754 bit patterns: 0x3F800000, 0xBF800000, 0x40000000, 0x3F000000
 * @validation_method Direct bit pattern comparison with IEEE 754 expected values
 */
TEST_P(I32ReinterpretF32TestSuite, BasicReinterpretation_TypicalValues_ReturnsCorrectBitPattern) {
    // Test positive unit value (1.0f)
    ASSERT_EQ(0x3F800000, call_reinterpret_f32(1.0f))
        << "1.0f should reinterpret to 0x3F800000 (IEEE 754 bit pattern)";

    // Test negative unit value (-1.0f)
    ASSERT_EQ(static_cast<int32_t>(0xBF800000), call_reinterpret_f32(-1.0f))
        << "-1.0f should reinterpret to 0xBF800000 (sign bit set)";

    // Test power of two (2.0f)
    ASSERT_EQ(0x40000000, call_reinterpret_f32(2.0f))
        << "2.0f should reinterpret to 0x40000000 (exponent increment)";

    // Test fractional value (0.5f)
    ASSERT_EQ(0x3F000000, call_reinterpret_f32(0.5f))
        << "0.5f should reinterpret to 0x3F000000 (exponent decrement)";
}

/**
 * @test SpecialValues_IEEE754Patterns_PreservesExactRepresentation
 * @brief Validates i32.reinterpret_f32 preserves IEEE 754 special value bit patterns
 * @details Tests special IEEE 754 values including zero variants, infinities, and NaN patterns.
 *          Verifies exact bit pattern preservation for all special floating-point cases.
 * @test_category Edge - Special value validation
 * @coverage_target core/iwasm/compilation/aot_emit_conversion.c:LLVMBuildBitCast_special_values
 * @input_conditions IEEE 754 special values: +0.0f, -0.0f, +∞, -∞, NaN
 * @expected_behavior Exact special value bit patterns: 0x00000000, 0x80000000, 0x7F800000, 0xFF800000, 0x7FC00000
 * @validation_method IEEE 754 standard compliance verification
 */
TEST_P(I32ReinterpretF32TestSuite, SpecialValues_IEEE754Patterns_PreservesExactRepresentation) {
    // Test positive zero (+0.0f)
    ASSERT_EQ(0x00000000, call_reinterpret_f32_special(0.0f))
        << "+0.0f should reinterpret to 0x00000000 (all bits clear)";

    // Test negative zero (-0.0f)
    ASSERT_EQ(static_cast<int32_t>(0x80000000), call_reinterpret_f32_special(-0.0f))
        << "-0.0f should reinterpret to 0x80000000 (only sign bit set)";

    // Test positive infinity
    ASSERT_EQ(0x7F800000, call_reinterpret_f32_special(INFINITY))
        << "+∞ should reinterpret to 0x7F800000 (exponent all 1s, mantissa all 0s)";

    // Test negative infinity
    ASSERT_EQ(static_cast<int32_t>(0xFF800000), call_reinterpret_f32_special(-INFINITY))
        << "-∞ should reinterpret to 0xFF800000 (sign + exponent all 1s, mantissa all 0s)";

    // Test canonical NaN (quiet NaN)
    ASSERT_EQ(0x7FC00000, call_reinterpret_f32_special(NAN))
        << "NaN should reinterpret to canonical pattern 0x7FC00000";
}

/**
 * @test BoundaryValues_FloatLimits_HandlesBitPatternLimits
 * @brief Validates i32.reinterpret_f32 handles IEEE 754 boundary conditions correctly
 * @details Tests boundary values including FLT_MIN, FLT_MAX, and subnormal transition boundaries.
 *          Verifies precise bit pattern preservation at floating-point numeric limits.
 * @test_category Corner - Boundary condition validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_fast.c:boundary_value_handling
 * @input_conditions IEEE 754 boundaries: FLT_MIN, FLT_MAX, subnormal boundaries
 * @expected_behavior Exact boundary bit patterns according to IEEE 754 specification
 * @validation_method Boundary value bit pattern verification
 */
TEST_P(I32ReinterpretF32TestSuite, BoundaryValues_FloatLimits_HandlesBitPatternLimits) {
    // Test FLT_MIN (smallest positive normal number)
    ASSERT_EQ(f32_to_i32_bits(FLT_MIN), call_reinterpret_f32_special(FLT_MIN))
        << "FLT_MIN should preserve exact IEEE 754 bit pattern";

    // Test FLT_MAX (largest finite number)
    ASSERT_EQ(f32_to_i32_bits(FLT_MAX), call_reinterpret_f32_special(FLT_MAX))
        << "FLT_MAX should preserve exact IEEE 754 bit pattern";

    // Test largest negative finite number
    ASSERT_EQ(f32_to_i32_bits(-FLT_MAX), call_reinterpret_f32_special(-FLT_MAX))
        << "-FLT_MAX should preserve exact IEEE 754 bit pattern with sign bit";

    // Test smallest positive subnormal number (0x00000001)
    union { float f; uint32_t u; } subnormal_converter;
    subnormal_converter.u = 0x00000001;
    ASSERT_EQ(static_cast<int32_t>(0x00000001), call_reinterpret_f32_special(subnormal_converter.f))
        << "Smallest subnormal should reinterpret to 0x00000001";
}

/**
 * @test SubnormalNumbers_PreservesExactRepresentation
 * @brief Validates i32.reinterpret_f32 preserves subnormal number bit patterns precisely
 * @details Tests various subnormal floating-point numbers and their bit field preservation.
 *          Verifies that subnormal exponent and mantissa fields are preserved exactly.
 * @test_category Edge - Subnormal value validation
 * @coverage_target core/iwasm/compilation/aot_emit_conversion.c:subnormal_bit_handling
 * @input_conditions Various subnormal patterns with different mantissa values
 * @expected_behavior Exact subnormal bit pattern preservation
 * @validation_method Subnormal bit field analysis and verification
 */
TEST_P(I32ReinterpretF32TestSuite, SubnormalNumbers_PreservesExactRepresentation) {
    // Test various subnormal bit patterns
    union { float f; uint32_t u; } converter;

    // Test smallest positive subnormal (0x00000001)
    converter.u = 0x00000001;
    ASSERT_EQ(static_cast<int32_t>(0x00000001), call_reinterpret_f32_special(converter.f))
        << "Smallest positive subnormal should preserve bit pattern 0x00000001";

    // Test largest positive subnormal (0x007FFFFF)
    converter.u = 0x007FFFFF;
    ASSERT_EQ(static_cast<int32_t>(0x007FFFFF), call_reinterpret_f32_special(converter.f))
        << "Largest positive subnormal should preserve bit pattern 0x007FFFFF";

    // Test negative subnormal pattern (0x80000001)
    converter.u = 0x80000001;
    ASSERT_EQ(static_cast<int32_t>(0x80000001), call_reinterpret_f32_special(converter.f))
        << "Negative subnormal should preserve bit pattern 0x80000001";

    // Test mid-range subnormal (0x00400000)
    converter.u = 0x00400000;
    ASSERT_EQ(static_cast<int32_t>(0x00400000), call_reinterpret_f32_special(converter.f))
        << "Mid-range subnormal should preserve bit pattern 0x00400000";
}

// Test suite instantiation for both interpreter and AOT modes
INSTANTIATE_TEST_SUITE_P(
    RunningModeTest,
    I32ReinterpretF32TestSuite,
    testing::Values(Mode_Interp, Mode_LLVM_JIT)
);