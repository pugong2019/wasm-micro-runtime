/*
 * Copyright (C) 2023 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#ifndef SIMD_TEST_MOCKS_H
#define SIMD_TEST_MOCKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "aot_export.h"
#include "wasm_export.h"

// Platform simulation mechanism - more robust implementation
extern bool is_target_x86_mock;

// Platform types for simulation
typedef enum {
    PLATFORM_X86,
    PLATFORM_ARM,
    PLATFORM_RISCV,
    PLATFORM_GENERIC
} SimulatedPlatform;

// Set the simulated platform
extern SimulatedPlatform current_simulated_platform;

// Mock implementation of is_target_x86 function
bool
is_target_x86_mock_impl(void);

// Set the platform to simulate
void
simulate_platform(SimulatedPlatform platform);

// Get the current simulated platform
SimulatedPlatform
get_current_simulated_platform(void);

// Helper functions for common platform simulations
void
simulate_x86_platform(void);
void
simulate_non_x86_platform(void);

// Check if we're simulating a specific platform
bool
is_simulating_platform(SimulatedPlatform platform);

// Override hook for the actual is_target_x86 function
#define is_target_x86(comp_ctx) is_target_x86_mock_impl()

// Error injection framework
enum MockFailureType {
    MOCK_FAILURE_NONE,
    MOCK_FAILURE_LLVM_API,
    MOCK_FAILURE_MEMORY_ALLOCATION,
    MOCK_FAILURE_LLVM_API_SPECIFIC
};

enum LLVMFailureCode {
    LLVM_FAILURE_NULL,
    LLVM_FAILURE_INVALID_VALUE,
    LLVM_FAILURE_OUT_OF_MEMORY,
    LLVM_FAILURE_UNSUPPORTED_OPERATION
};

struct MockFailureConfig {
    enum MockFailureType type;
    const char* api_name;
    enum LLVMFailureCode failure_code;
    int call_count;
    int failure_on_call;
};

extern struct MockFailureConfig mock_failure_config;

// Mock function registration mechanism
void setup_llvm_api_mock_failure(const char* api_name);
void setup_specific_llvm_api_mock_failure(const char* api_name, enum LLVMFailureCode code);
void setup_memory_allocation_failure_mock();
void setup_failure_on_call(int call_number);
void reset_mock_failures();
void increment_call_count();

// Helper function to check if LLVM API failure should be injected
bool should_inject_llvm_failure(const char* api_name);

// Get failure code for the current API
enum LLVMFailureCode get_llvm_failure_code();

// Override memory allocation functions for testing
void* mock_malloc(size_t size);
void mock_free(void* ptr);

// Function existence checking helper
void expect_function_exists(wasm_module_inst_t module_inst, const char* func_name);

#ifdef __cplusplus
}
#endif

#endif // SIMD_TEST_MOCKS_H