/*
 * Copyright (C) 2025 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "gtest/gtest.h"
#include "bh_platform.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

extern "C" {
#include "blocking_op.h"
#include "ssp_config.h"
#include "wasm_export.h"
#include "wasm_runtime.h"
}

// Platform detection utility for tests - REQUIRED in every test file
class PlatformTestContext {
public:
    // Architecture detection
    static bool IsX86_64() {
#if defined(BUILD_TARGET_X86) || defined(BUILD_TARGET_X86_64)
        return true;
#else
        return false;
#endif
    }

    static bool IsARM64() {
#if defined(BUILD_TARGET_AARCH64) || defined(BUILD_TARGET_ARM64)
        return true;
#else
        return false;
#endif
    }

    static bool IsLinux() {
#if defined(__linux__)
        return true;
#else
        return false;
#endif
    }

    // Feature detection
    static bool HasFileSupport() {
#if defined(WASM_ENABLE_LIBC_WASI)
        return true;
#else
        return false;
#endif
    }
};

// Enhanced test fixture following existing patterns for blocking_op.c functions
class EnhancedBlockingOpTest : public testing::Test {
protected:
    void SetUp() override {
        // Initialize WASM runtime for testing
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_Pool;
        init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
        init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);

        // Initialize the runtime
        ASSERT_TRUE(wasm_runtime_init()) << "Failed to initialize WASM runtime";
        runtime_initialized = true;

        // Create a simple WASM module for testing
        create_test_module();
    }

    void TearDown() override {
        // Clean up exec env if created
        if (exec_env) {
            wasm_runtime_deinstantiate(module_inst);
            exec_env = nullptr;
            module_inst = nullptr;
        }

        // Clean up module if loaded
        if (module) {
            wasm_runtime_unload(module);
            module = nullptr;
        }

        // Clean up runtime
        if (runtime_initialized) {
            wasm_runtime_destroy();
            runtime_initialized = false;
        }
    }

    void create_test_module() {
        // Simple WASM module bytecode for testing
        static uint8_t simple_wasm[] = {
            0x00, 0x61, 0x73, 0x6d, // WASM magic
            0x01, 0x00, 0x00, 0x00, // version
            0x01, 0x04, 0x01, 0x60, // type section
            0x00, 0x00, 0x03, 0x02, // func section
            0x01, 0x00, 0x0a, 0x04, // code section
            0x01, 0x02, 0x00, 0x0b  // function body
        };

        char error_buf[128];
        module = wasm_runtime_load(simple_wasm, sizeof(simple_wasm), error_buf, sizeof(error_buf));
        ASSERT_NE(nullptr, module) << "Failed to load WASM module: " << error_buf;

        module_inst = wasm_runtime_instantiate(module, 0, 0, error_buf, sizeof(error_buf));
        ASSERT_NE(nullptr, module_inst) << "Failed to instantiate WASM module: " << error_buf;

        exec_env = wasm_runtime_create_exec_env(module_inst, 8192);
        ASSERT_NE(nullptr, exec_env) << "Failed to create execution environment";
    }

public:
    char global_heap_buf[512 * 1024];
    RuntimeInitArgs init_args;
    bool runtime_initialized = false;
    wasm_module_t module = nullptr;
    wasm_module_inst_t module_inst = nullptr;
    wasm_exec_env_t exec_env = nullptr;
};

/******
 * Test Case: blocking_op_close_ValidHandle_ReturnsSuccess
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:13-21
 * Target Lines: 16 (blocking op check), 19 (os_close call), 20 (end blocking op), 21 (return)
 * Functional Purpose: Validates that blocking_op_close() successfully handles valid file
 *                     operations by properly managing blocking operations and delegating
 *                     to os_close() with correct return value propagation.
 * Call Path: blocking_op_close() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise success path for valid file handle operations
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpClose_ValidHandle_ReturnsSuccess) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a temporary file for testing
    int test_fd = open("/tmp/test_blocking_op_close", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    ASSERT_NE(-1, test_fd) << "Failed to create test file: " << strerror(errno);

    // Convert to os_file_handle (platform-specific)
    os_file_handle handle = (os_file_handle)(uintptr_t)test_fd;

    // Test blocking_op_close with valid handle and is_stdio=false
    __wasi_errno_t result = blocking_op_close(exec_env, handle, false);

    // Verify the function returns success (0 or specific success code)
    ASSERT_EQ(0, result) << "blocking_op_close should succeed for valid file handle";

    // Cleanup - remove test file
    unlink("/tmp/test_blocking_op_close");
}

/******
 * Test Case: blocking_op_close_StdioHandle_ReturnsSuccess
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:13-21
 * Target Lines: 16 (blocking op check), 19 (os_close call), 20 (end blocking op), 21 (return)
 * Functional Purpose: Validates that blocking_op_close() properly handles stdio file
 *                     descriptors with the is_stdio flag set to true, ensuring special
 *                     handling for standard input/output/error streams.
 * Call Path: blocking_op_close() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise success path for stdio handle operations with is_stdio=true
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpClose_StdioHandle_ReturnsSuccess) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Use a duplicate of stdout for testing stdio handling
    int dup_stdout = dup(STDOUT_FILENO);
    ASSERT_NE(-1, dup_stdout) << "Failed to duplicate stdout: " << strerror(errno);

    // Convert to os_file_handle
    os_file_handle handle = (os_file_handle)(uintptr_t)dup_stdout;

    // Test blocking_op_close with stdio handle and is_stdio=true
    __wasi_errno_t result = blocking_op_close(exec_env, handle, true);

    // Verify the function handles stdio files appropriately
    // Note: May return success or specific stdio handling code depending on implementation
    ASSERT_TRUE(result == 0 || result == __WASI_ENOSYS) << "blocking_op_close should handle stdio files appropriately";
}

/******
 * Test Case: blocking_op_close_InvalidHandle_ReturnsError
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:13-21
 * Target Lines: 16 (blocking op check), 19 (os_close call), 20 (end blocking op), 21 (return error)
 * Functional Purpose: Validates that blocking_op_close() properly propagates error codes
 *                     from os_close() when given an invalid file handle, ensuring robust
 *                     error handling throughout the blocking operation lifecycle.
 * Call Path: blocking_op_close() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise error propagation path for invalid file handle operations
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpClose_InvalidHandle_ReturnsError) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Use an invalid file descriptor
    os_file_handle invalid_handle = (os_file_handle)(uintptr_t)-1;

    // Test blocking_op_close with invalid handle
    __wasi_errno_t result = blocking_op_close(exec_env, invalid_handle, false);

    // Verify the function returns an error code (not success)
    ASSERT_NE(0, result) << "blocking_op_close should return error for invalid file handle";

    // Common error codes for invalid file descriptor
    ASSERT_TRUE(result == __WASI_EBADF || result == __WASI_EINVAL || result == __WASI_ENOSYS)
        << "Expected EBADF, EINVAL, or ENOSYS for invalid handle, got: " << result;
}

/******
 * Test Case: blocking_op_close_NullExecEnv_HandlesGracefully
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:13-21
 * Target Lines: 16 (blocking op check), 19 (os_close call), 20 (end blocking op), 21 (return)
 * Functional Purpose: Validates that blocking_op_close() handles null exec_env gracefully
 *                     by still performing the file close operation when blocking operations
 *                     are not available or properly initialized.
 * Call Path: blocking_op_close() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise function behavior with null execution environment
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpClose_NullExecEnv_HandlesGracefully) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a null exec_env to test handling of null execution environment
    wasm_exec_env_t null_exec_env = nullptr;

    // Create a valid file handle for testing
    int test_fd = open("/tmp/test_blocking_op_null_env", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    ASSERT_NE(-1, test_fd) << "Failed to create test file: " << strerror(errno);
    os_file_handle handle = (os_file_handle)(uintptr_t)test_fd;

    // Test blocking_op_close with null exec_env
    __wasi_errno_t result = blocking_op_close(null_exec_env, handle, false);

    // Verify the function handles null exec_env appropriately
    // The function may return success (0) if it proceeds with os_close,
    // or an appropriate error code based on the implementation
    ASSERT_TRUE(result == 0 || result == __WASI_EINTR || result == __WASI_ENOSYS)
        << "blocking_op_close should handle null exec_env appropriately, got: " << result;

    // Cleanup - remove test file (file may already be closed by the function)
    unlink("/tmp/test_blocking_op_null_env");
}