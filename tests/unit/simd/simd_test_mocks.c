/*
 * Copyright (C) 2024 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

// Include our mock header first to get all necessary definitions
#include "simd_test_mocks.h"

// 模拟GTest的一些必要宏定义
#define ASSERT_NE(val1, val2) \
    do { \
        if ((val1) == (val2)) { \
            abort(); \
        } \
    } while (0)

// 在C中使用NULL而不是nullptr
#define nullptr NULL

typedef struct AOTCompContext AOTCompContext;

// Include other necessary headers
#include "aot_export.h"
#include "wasm_export.h"

// Platform simulation mechanism
bool is_target_x86_mock = true; /* Default to x86 for compatibility */
SimulatedPlatform current_simulated_platform = PLATFORM_X86; /* Default to x86 */

bool
is_target_x86_mock_impl()
{
    return is_target_x86_mock;
}

// Set the platform to simulate
void
simulate_platform(SimulatedPlatform platform)
{
    current_simulated_platform = platform;
    // Update the x86 mock flag based on the selected platform
    is_target_x86_mock = (platform == PLATFORM_X86);
}

// Get the current simulated platform
SimulatedPlatform
get_current_simulated_platform(void)
{
    return current_simulated_platform;
}

// Helper functions for common platform simulations
void
simulate_x86_platform(void)
{
    simulate_platform(PLATFORM_X86);
}

void
simulate_non_x86_platform(void)
{
    simulate_platform(PLATFORM_GENERIC);
}

// Check if we're simulating a specific platform
bool
is_simulating_platform(SimulatedPlatform platform)
{
    return (current_simulated_platform == platform);
}

// Error injection framework
struct MockFailureConfig mock_failure_config = {
    .type = MOCK_FAILURE_NONE,
    .api_name = NULL,
    .failure_code = LLVM_FAILURE_NULL,
    .call_count = 0,
    .failure_on_call = 0
};

void
setup_llvm_api_mock_failure(const char* api_name)
{
    mock_failure_config.type = MOCK_FAILURE_LLVM_API;
    mock_failure_config.api_name = api_name;
    mock_failure_config.failure_code = LLVM_FAILURE_NULL;
    mock_failure_config.call_count = 0;
    mock_failure_config.failure_on_call = 0;
}

void
setup_specific_llvm_api_mock_failure(const char* api_name, enum LLVMFailureCode code)
{
    mock_failure_config.type = MOCK_FAILURE_LLVM_API_SPECIFIC;
    mock_failure_config.api_name = api_name;
    mock_failure_config.failure_code = code;
    mock_failure_config.call_count = 0;
    mock_failure_config.failure_on_call = 0;
}

void
setup_memory_allocation_failure_mock()
{
    mock_failure_config.type = MOCK_FAILURE_MEMORY_ALLOCATION;
    mock_failure_config.api_name = NULL;
    mock_failure_config.failure_code = LLVM_FAILURE_NULL;
    mock_failure_config.call_count = 0;
    mock_failure_config.failure_on_call = 0;
}

void
setup_failure_on_call(int call_number)
{
    mock_failure_config.failure_on_call = call_number;
}

void
reset_mock_failures()
{
    mock_failure_config.type = MOCK_FAILURE_NONE;
    mock_failure_config.api_name = NULL;
    mock_failure_config.failure_code = LLVM_FAILURE_NULL;
    mock_failure_config.call_count = 0;
    mock_failure_config.failure_on_call = 0;
}

void
increment_call_count()
{
    mock_failure_config.call_count++;
}

// Get failure code for the current API
enum LLVMFailureCode
get_llvm_failure_code()
{
    return mock_failure_config.failure_code;
}

// Mock memory allocation functions
void*
mock_malloc(size_t size)
{
    if (mock_failure_config.type == MOCK_FAILURE_MEMORY_ALLOCATION) {
        return NULL;
    }
    return malloc(size);
}

void
mock_free(void* ptr)
{
    if (ptr != NULL) {
        free(ptr);
    }
}

// Helper function for function existence checking
void
expect_function_exists(wasm_module_inst_t module_inst, const char *func_name)
{
    wasm_function_inst_t func_inst = wasm_runtime_lookup_function(module_inst, func_name);
    if (func_inst == NULL) {
        // 在实际环境中，这应该打印错误消息，但为了简单起见，我们直接断言失败
        abort();
    }
}

// Helper function to check if LLVM API failure should be injected
bool
should_inject_llvm_failure(const char* api_name)
{
    increment_call_count();
    
    // Check if failure is requested on specific call count
    if (mock_failure_config.failure_on_call > 0 && 
        mock_failure_config.call_count != mock_failure_config.failure_on_call) {
        return false;
    }
    
    // Check if we're in LLVM API failure mode
    if (mock_failure_config.type != MOCK_FAILURE_LLVM_API && 
        mock_failure_config.type != MOCK_FAILURE_LLVM_API_SPECIFIC) {
        return false;
    }
    
    // If no specific API name is set, fail all LLVM APIs
    if (mock_failure_config.api_name == NULL) {
        return true;
    }
    
    // Otherwise, only fail the specific API
    return strcmp(mock_failure_config.api_name, api_name) == 0;
}