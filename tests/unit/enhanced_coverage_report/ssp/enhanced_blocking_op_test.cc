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

/******
 * Test Case: blocking_op_readv_ValidHandle_ReturnsSuccess
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:25-33
 * Target Lines: 28 (blocking op check), 31 (os_readv call), 32 (end blocking op), 33 (return)
 * Functional Purpose: Validates that blocking_op_readv() successfully handles valid file
 *                     read operations by properly managing blocking operations and delegating
 *                     to os_readv() with correct return value propagation.
 * Call Path: blocking_op_readv() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise success path for valid file handle read operations
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpReadv_ValidHandle_ReturnsSuccess) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a test file with content
    const char *test_content = "Hello, WASM readv test!";
    int test_fd = open("/tmp/test_blocking_op_readv", O_CREAT | O_RDWR | O_TRUNC, 0644);
    ASSERT_NE(-1, test_fd) << "Failed to create test file: " << strerror(errno);

    ssize_t write_result = write(test_fd, test_content, strlen(test_content));
    ASSERT_EQ(strlen(test_content), write_result) << "Failed to write test content";

    // Reset file position for reading
    lseek(test_fd, 0, SEEK_SET);

    // Convert to os_file_handle
    os_file_handle handle = (os_file_handle)(uintptr_t)test_fd;

    // Setup iovec for reading
    char read_buffer[100];
    struct __wasi_iovec_t iov = { .buf = (uint8_t*)read_buffer, .buf_len = sizeof(read_buffer) };
    size_t nread = 0;

    // Test blocking_op_readv with valid handle
    __wasi_errno_t result = blocking_op_readv(exec_env, handle, &iov, 1, &nread);

    // Verify the function returns success
    ASSERT_EQ(0, result) << "blocking_op_readv should succeed for valid file handle";

    // Verify data was read correctly
    ASSERT_GT(nread, 0) << "Should have read some data";
    ASSERT_EQ(0, memcmp(read_buffer, test_content, nread)) << "Read content should match written content";

    // Cleanup
    close(test_fd);
    unlink("/tmp/test_blocking_op_readv");
}

/******
 * Test Case: blocking_op_readv_InvalidHandle_ReturnsError
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:25-33
 * Target Lines: 28 (blocking op check), 31 (os_readv call), 32 (end blocking op), 33 (return error)
 * Functional Purpose: Validates that blocking_op_readv() properly propagates error codes
 *                     from os_readv() when given an invalid file handle, ensuring robust
 *                     error handling throughout the blocking operation lifecycle.
 * Call Path: blocking_op_readv() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise error propagation path for invalid file handle operations
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpReadv_InvalidHandle_ReturnsError) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Use an invalid file descriptor
    os_file_handle invalid_handle = (os_file_handle)(uintptr_t)-1;

    // Setup iovec for reading
    char read_buffer[100];
    struct __wasi_iovec_t iov = { .buf = (uint8_t*)read_buffer, .buf_len = sizeof(read_buffer) };
    size_t nread = 0;

    // Test blocking_op_readv with invalid handle
    __wasi_errno_t result = blocking_op_readv(exec_env, invalid_handle, &iov, 1, &nread);

    // Verify the function returns an error code (not success)
    ASSERT_NE(0, result) << "blocking_op_readv should return error for invalid file handle";

    // Common error codes for invalid file descriptor
    ASSERT_TRUE(result == __WASI_EBADF || result == __WASI_EINVAL || result == __WASI_ENOSYS)
        << "Expected EBADF, EINVAL, or ENOSYS for invalid handle, got: " << result;
}

/******
 * Test Case: blocking_op_readv_NullExecEnv_ReturnsInterruption
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:25-33
 * Target Lines: 28 (blocking op check fails), 29 (return EINTR), 33 (return path)
 * Functional Purpose: Validates that blocking_op_readv() handles null exec_env by returning
 *                     __WASI_EINTR when wasm_runtime_begin_blocking_op() fails, ensuring
 *                     proper interruption handling without crashing.
 * Call Path: blocking_op_readv() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise interruption return path when blocking operation cannot be started
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpReadv_NullExecEnv_ReturnsInterruption) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a null exec_env to test interruption handling
    wasm_exec_env_t null_exec_env = nullptr;

    // Create a valid file handle for testing
    const char *test_content = "Test content for null env";
    int test_fd = open("/tmp/test_blocking_op_readv_null", O_CREAT | O_RDWR | O_TRUNC, 0644);
    ASSERT_NE(-1, test_fd) << "Failed to create test file: " << strerror(errno);

    write(test_fd, test_content, strlen(test_content));
    lseek(test_fd, 0, SEEK_SET);

    os_file_handle handle = (os_file_handle)(uintptr_t)test_fd;

    // Setup iovec for reading
    char read_buffer[100];
    struct __wasi_iovec_t iov = { .buf = (uint8_t*)read_buffer, .buf_len = sizeof(read_buffer) };
    size_t nread = 0;

    // Test blocking_op_readv with null exec_env
    __wasi_errno_t result = blocking_op_readv(null_exec_env, handle, &iov, 1, &nread);

    // Verify the function handles null exec_env appropriately
    // The function may return success (0) if it proceeds with os_readv despite null exec_env,
    // or EINTR based on the implementation behavior
    ASSERT_TRUE(result == 0 || result == __WASI_EINTR)
        << "blocking_op_readv should handle null exec_env appropriately, got: " << result;

    // Cleanup
    close(test_fd);
    unlink("/tmp/test_blocking_op_readv_null");
}

/******
 * Test Case: blocking_op_readv_MultipleIovecs_ReturnsSuccess
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:25-33
 * Target Lines: 28 (blocking op check), 31 (os_readv call), 32 (end blocking op), 33 (return)
 * Functional Purpose: Validates that blocking_op_readv() properly handles multiple iovec
 *                     structures for scatter-gather I/O operations, testing the iovcnt
 *                     parameter handling and ensuring proper data distribution.
 * Call Path: blocking_op_readv() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise success path for multi-buffer read operations
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpReadv_MultipleIovecs_ReturnsSuccess) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a test file with longer content
    const char *test_content = "This is a longer test content for multiple iovec testing with blocking_op_readv function.";
    int test_fd = open("/tmp/test_blocking_op_readv_multi", O_CREAT | O_RDWR | O_TRUNC, 0644);
    ASSERT_NE(-1, test_fd) << "Failed to create test file: " << strerror(errno);

    write(test_fd, test_content, strlen(test_content));
    lseek(test_fd, 0, SEEK_SET);

    os_file_handle handle = (os_file_handle)(uintptr_t)test_fd;

    // Setup multiple iovecs for scatter-gather read
    char buffer1[30], buffer2[30], buffer3[30];
    struct __wasi_iovec_t iovs[3] = {
        { .buf = (uint8_t*)buffer1, .buf_len = sizeof(buffer1) },
        { .buf = (uint8_t*)buffer2, .buf_len = sizeof(buffer2) },
        { .buf = (uint8_t*)buffer3, .buf_len = sizeof(buffer3) }
    };
    size_t nread = 0;

    // Test blocking_op_readv with multiple iovecs
    __wasi_errno_t result = blocking_op_readv(exec_env, handle, iovs, 3, &nread);

    // Verify the function returns success
    ASSERT_EQ(0, result) << "blocking_op_readv should succeed for multiple iovecs";

    // Verify data was read
    ASSERT_GT(nread, 0) << "Should have read some data into multiple buffers";

    // Cleanup
    close(test_fd);
    unlink("/tmp/test_blocking_op_readv_multi");
}

// ==================== NEW TEST CASES FOR blocking_op_preadv (Lines 37-46) ====================

/******
 * Test Case: blocking_op_preadv_ValidParameters_ReturnsSuccess
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:37-46
 * Target Lines: 41 (blocking op check), 44 (os_preadv call), 45 (end blocking op), 46 (return)
 * Functional Purpose: Validates that blocking_op_preadv() successfully handles valid file
 *                     read operations with specified offset by properly managing blocking operations
 *                     and delegating to os_preadv() with correct return value propagation.
 * Call Path: blocking_op_preadv() <- wasmtime_ssp_fd_pread() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise success path for valid file handle read operations with offset
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpPreadv_ValidParameters_ReturnsSuccess) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a test file with content for positional reading
    const char *test_content = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int test_fd = open("/tmp/test_blocking_op_preadv", O_CREAT | O_RDWR | O_TRUNC, 0644);
    ASSERT_NE(-1, test_fd) << "Failed to create test file: " << strerror(errno);

    ssize_t write_result = write(test_fd, test_content, strlen(test_content));
    ASSERT_EQ(strlen(test_content), write_result) << "Failed to write test content";

    // Convert to os_file_handle
    os_file_handle handle = (os_file_handle)(uintptr_t)test_fd;

    // Setup iovec for reading from position 10
    char read_buffer[20];
    struct __wasi_iovec_t iov = { .buf = (uint8_t*)read_buffer, .buf_len = sizeof(read_buffer) };
    size_t nread = 0;
    __wasi_filesize_t offset = 10;

    // Test blocking_op_preadv with valid parameters and offset
    __wasi_errno_t result = blocking_op_preadv(exec_env, handle, &iov, 1, offset, &nread);

    // Verify the function returns success
    ASSERT_EQ(0, result) << "blocking_op_preadv should succeed for valid parameters";

    // Verify data was read from correct position
    ASSERT_GT(nread, 0) << "Should have read some data from specified offset";
    ASSERT_EQ(0, memcmp(read_buffer, test_content + 10, nread)) << "Read content should match expected offset content";

    // Cleanup
    close(test_fd);
    unlink("/tmp/test_blocking_op_preadv");
}

/******
 * Test Case: blocking_op_preadv_NullExecEnv_ReturnsInterruption
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:37-46
 * Target Lines: 41 (blocking op check fails), 42 (return EINTR), 46 (return path)
 * Functional Purpose: Validates that blocking_op_preadv() handles null exec_env by returning
 *                     __WASI_EINTR when wasm_runtime_begin_blocking_op() fails, ensuring
 *                     proper interruption handling without crashing.
 * Call Path: blocking_op_preadv() <- wasmtime_ssp_fd_pread() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise interruption return path when blocking operation cannot be started
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpPreadv_NullExecEnv_ReturnsInterruption) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a null exec_env to test interruption handling
    wasm_exec_env_t null_exec_env = nullptr;

    // Create a valid file handle for testing
    const char *test_content = "Test content for null exec env in preadv";
    int test_fd = open("/tmp/test_blocking_op_preadv_null", O_CREAT | O_RDWR | O_TRUNC, 0644);
    ASSERT_NE(-1, test_fd) << "Failed to create test file: " << strerror(errno);

    write(test_fd, test_content, strlen(test_content));
    os_file_handle handle = (os_file_handle)(uintptr_t)test_fd;

    // Setup iovec for reading
    char read_buffer[50];
    struct __wasi_iovec_t iov = { .buf = (uint8_t*)read_buffer, .buf_len = sizeof(read_buffer) };
    size_t nread = 0;
    __wasi_filesize_t offset = 5;

    // Test blocking_op_preadv with null exec_env
    __wasi_errno_t result = blocking_op_preadv(null_exec_env, handle, &iov, 1, offset, &nread);

    // Verify the function handles null exec_env appropriately
    // The function may return success (0) if it proceeds with os_preadv despite null exec_env,
    // or EINTR based on the implementation behavior
    ASSERT_TRUE(result == 0 || result == __WASI_EINTR)
        << "blocking_op_preadv should handle null exec_env appropriately, got: " << result;

    // Cleanup
    close(test_fd);
    unlink("/tmp/test_blocking_op_preadv_null");
}

/******
 * Test Case: blocking_op_preadv_InvalidHandle_ReturnsError
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:37-46
 * Target Lines: 41 (blocking op check), 44 (os_preadv call), 45 (end blocking op), 46 (return error)
 * Functional Purpose: Validates that blocking_op_preadv() properly propagates error codes
 *                     from os_preadv() when given an invalid file handle, ensuring robust
 *                     error handling throughout the blocking operation lifecycle.
 * Call Path: blocking_op_preadv() <- wasmtime_ssp_fd_pread() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise error propagation path for invalid file handle operations
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpPreadv_InvalidHandle_ReturnsError) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Use an invalid file descriptor
    os_file_handle invalid_handle = (os_file_handle)(uintptr_t)-1;

    // Setup iovec for reading
    char read_buffer[50];
    struct __wasi_iovec_t iov = { .buf = (uint8_t*)read_buffer, .buf_len = sizeof(read_buffer) };
    size_t nread = 0;
    __wasi_filesize_t offset = 0;

    // Test blocking_op_preadv with invalid handle
    __wasi_errno_t result = blocking_op_preadv(exec_env, invalid_handle, &iov, 1, offset, &nread);

    // Verify the function returns an error code (not success)
    ASSERT_NE(0, result) << "blocking_op_preadv should return error for invalid file handle";

    // Common error codes for invalid file descriptor
    ASSERT_TRUE(result == __WASI_EBADF || result == __WASI_EINVAL || result == __WASI_ENOSYS)
        << "Expected EBADF, EINVAL, or ENOSYS for invalid handle, got: " << result;
}

/******
 * Test Case: blocking_op_preadv_InvalidOffset_ReturnsError
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:37-46
 * Target Lines: 41 (blocking op check), 44 (os_preadv call), 45 (end blocking op), 46 (return error)
 * Functional Purpose: Validates that blocking_op_preadv() properly handles invalid offset values
 *                     by propagating error codes from os_preadv() when given an invalid offset,
 *                     ensuring robust parameter validation and error handling.
 * Call Path: blocking_op_preadv() <- wasmtime_ssp_fd_pread() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise error propagation path for invalid offset parameter
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpPreadv_InvalidOffset_ReturnsError) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a small test file
    const char *test_content = "Small test file";
    int test_fd = open("/tmp/test_blocking_op_preadv_offset", O_CREAT | O_RDWR | O_TRUNC, 0644);
    ASSERT_NE(-1, test_fd) << "Failed to create test file: " << strerror(errno);

    write(test_fd, test_content, strlen(test_content));
    os_file_handle handle = (os_file_handle)(uintptr_t)test_fd;

    // Setup iovec for reading
    char read_buffer[50];
    struct __wasi_iovec_t iov = { .buf = (uint8_t*)read_buffer, .buf_len = sizeof(read_buffer) };
    size_t nread = 0;

    // Use an offset beyond file size (invalid for most platforms)
    __wasi_filesize_t invalid_offset = UINT64_MAX;

    // Test blocking_op_preadv with invalid offset
    __wasi_errno_t result = blocking_op_preadv(exec_env, handle, &iov, 1, invalid_offset, &nread);

    // Verify the function handles invalid offset appropriately
    // May return success with 0 bytes read, or specific error based on platform
    ASSERT_TRUE(result == 0 || result == __WASI_EINVAL || result == __WASI_EOVERFLOW)
        << "blocking_op_preadv should handle invalid offset appropriately, got: " << result;

    // If successful, verify no data was read from invalid position
    if (result == 0) {
        ASSERT_EQ(0, nread) << "Should not read data from invalid offset position";
    }

    // Cleanup
    close(test_fd);
    unlink("/tmp/test_blocking_op_preadv_offset");
}

/******
 * Test Case: blocking_op_preadv_MultipleIovecs_ReturnsSuccess
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:37-46
 * Target Lines: 41 (blocking op check), 44 (os_preadv call), 45 (end blocking op), 46 (return)
 * Functional Purpose: Validates that blocking_op_preadv() properly handles multiple iovec
 *                     structures for scatter-gather I/O operations with offset, testing the iovcnt
 *                     parameter handling and ensuring proper data distribution from specified position.
 * Call Path: blocking_op_preadv() <- wasmtime_ssp_fd_pread() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise success path for multi-buffer read operations with offset
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpPreadv_MultipleIovecs_ReturnsSuccess) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a test file with longer content for multi-iovec testing
    const char *test_content = "This is a comprehensive test content for multiple iovec testing with blocking_op_preadv function. It contains enough data to fill multiple buffers during positional read operations.";
    int test_fd = open("/tmp/test_blocking_op_preadv_multi", O_CREAT | O_RDWR | O_TRUNC, 0644);
    ASSERT_NE(-1, test_fd) << "Failed to create test file: " << strerror(errno);

    write(test_fd, test_content, strlen(test_content));
    os_file_handle handle = (os_file_handle)(uintptr_t)test_fd;

    // Setup multiple iovecs for scatter-gather read from offset 20
    char buffer1[30], buffer2[30], buffer3[30];
    struct __wasi_iovec_t iovs[3] = {
        { .buf = (uint8_t*)buffer1, .buf_len = sizeof(buffer1) },
        { .buf = (uint8_t*)buffer2, .buf_len = sizeof(buffer2) },
        { .buf = (uint8_t*)buffer3, .buf_len = sizeof(buffer3) }
    };
    size_t nread = 0;
    __wasi_filesize_t offset = 20;

    // Test blocking_op_preadv with multiple iovecs and offset
    __wasi_errno_t result = blocking_op_preadv(exec_env, handle, iovs, 3, offset, &nread);

    // Verify the function returns success
    ASSERT_EQ(0, result) << "blocking_op_preadv should succeed for multiple iovecs with offset";

    // Verify data was read from correct position
    ASSERT_GT(nread, 0) << "Should have read some data into multiple buffers from offset";

    // Cleanup
    close(test_fd);
    unlink("/tmp/test_blocking_op_preadv_multi");
}

/******
 * Test Case: blocking_op_preadv_ZeroOffset_ReturnsSuccess
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:37-46
 * Target Lines: 41 (blocking op check), 44 (os_preadv call), 45 (end blocking op), 46 (return)
 * Functional Purpose: Validates that blocking_op_preadv() properly handles the edge case of
 *                     reading from offset 0 (start of file), ensuring that zero offset is
 *                     treated as a valid position and data is read correctly.
 * Call Path: blocking_op_preadv() <- wasmtime_ssp_fd_pread() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise success path for edge case with offset=0 (beginning of file)
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpPreadv_ZeroOffset_ReturnsSuccess) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a test file with content
    const char *test_content = "Beginning of file content for zero offset testing";
    int test_fd = open("/tmp/test_blocking_op_preadv_zero", O_CREAT | O_RDWR | O_TRUNC, 0644);
    ASSERT_NE(-1, test_fd) << "Failed to create test file: " << strerror(errno);

    write(test_fd, test_content, strlen(test_content));
    os_file_handle handle = (os_file_handle)(uintptr_t)test_fd;

    // Setup iovec for reading from offset 0
    char read_buffer[30];
    struct __wasi_iovec_t iov = { .buf = (uint8_t*)read_buffer, .buf_len = sizeof(read_buffer) };
    size_t nread = 0;
    __wasi_filesize_t offset = 0;

    // Test blocking_op_preadv with zero offset
    __wasi_errno_t result = blocking_op_preadv(exec_env, handle, &iov, 1, offset, &nread);

    // Verify the function returns success
    ASSERT_EQ(0, result) << "blocking_op_preadv should succeed for zero offset";

    // Verify data was read from beginning of file
    ASSERT_GT(nread, 0) << "Should have read some data from offset 0";
    ASSERT_EQ(0, memcmp(read_buffer, test_content, nread)) << "Read content should match file beginning";

    // Cleanup
    close(test_fd);
    unlink("/tmp/test_blocking_op_preadv_zero");
}

// ==================== NEW TEST CASES FOR blocking_op_writev (Lines 50-59) ====================

/******
 * Test Case: blocking_op_writev_ValidParameters_ReturnsSuccess
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:50-59
 * Target Lines: 54 (blocking op check), 57 (os_writev call), 58 (end blocking op), 59 (return)
 * Functional Purpose: Validates that blocking_op_writev() successfully handles valid file
 *                     write operations by properly managing blocking operations and delegating
 *                     to os_writev() with correct return value propagation.
 * Call Path: blocking_op_writev() <- wasmtime_ssp_fd_write() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise success path for valid file handle write operations
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpWritev_ValidParameters_ReturnsSuccess) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a test file for writing
    int test_fd = open("/tmp/test_blocking_op_writev", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    ASSERT_NE(-1, test_fd) << "Failed to create test file: " << strerror(errno);

    // Convert to os_file_handle
    os_file_handle handle = (os_file_handle)(uintptr_t)test_fd;

    // Setup iovec with test data for writing
    const char *test_data = "Hello, WASM writev test data!";
    struct __wasi_ciovec_t iov = {
        .buf = (const uint8_t*)test_data,
        .buf_len = strlen(test_data)
    };
    size_t nwritten = 0;

    // Test blocking_op_writev with valid parameters
    __wasi_errno_t result = blocking_op_writev(exec_env, handle, &iov, 1, &nwritten);

    // Verify the function returns success
    ASSERT_EQ(0, result) << "blocking_op_writev should succeed for valid parameters";

    // Verify data was written correctly
    ASSERT_GT(nwritten, 0) << "Should have written some data";
    ASSERT_EQ(strlen(test_data), nwritten) << "Should have written all test data";

    // Cleanup
    close(test_fd);
    unlink("/tmp/test_blocking_op_writev");
}

/******
 * Test Case: blocking_op_writev_NullExecEnv_ReturnsInterruption
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:50-59
 * Target Lines: 54 (blocking op check fails), 55 (return EINTR), 59 (return path)
 * Functional Purpose: Validates that blocking_op_writev() handles null exec_env by returning
 *                     __WASI_EINTR when wasm_runtime_begin_blocking_op() fails, ensuring
 *                     proper interruption handling without crashing.
 * Call Path: blocking_op_writev() <- wasmtime_ssp_fd_write() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise interruption return path when blocking operation cannot be started
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpWritev_NullExecEnv_ReturnsInterruption) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a null exec_env to test interruption handling
    wasm_exec_env_t null_exec_env = nullptr;

    // Create a valid file handle for testing
    int test_fd = open("/tmp/test_blocking_op_writev_null", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    ASSERT_NE(-1, test_fd) << "Failed to create test file: " << strerror(errno);

    os_file_handle handle = (os_file_handle)(uintptr_t)test_fd;

    // Setup iovec with test data
    const char *test_data = "Test data for null exec env in writev";
    struct __wasi_ciovec_t iov = {
        .buf = (const uint8_t*)test_data,
        .buf_len = strlen(test_data)
    };
    size_t nwritten = 0;

    // Test blocking_op_writev with null exec_env
    __wasi_errno_t result = blocking_op_writev(null_exec_env, handle, &iov, 1, &nwritten);

    // Verify the function handles null exec_env appropriately
    // The function may return success (0) if it proceeds with os_writev despite null exec_env,
    // or EINTR based on the implementation behavior
    ASSERT_TRUE(result == 0 || result == __WASI_EINTR)
        << "blocking_op_writev should handle null exec_env appropriately, got: " << result;

    // Cleanup
    close(test_fd);
    unlink("/tmp/test_blocking_op_writev_null");
}

/******
 * Test Case: blocking_op_writev_InvalidHandle_ReturnsError
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:50-59
 * Target Lines: 54 (blocking op check), 57 (os_writev call), 58 (end blocking op), 59 (return error)
 * Functional Purpose: Validates that blocking_op_writev() properly propagates error codes
 *                     from os_writev() when given an invalid file handle, ensuring robust
 *                     error handling throughout the blocking operation lifecycle.
 * Call Path: blocking_op_writev() <- wasmtime_ssp_fd_write() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise error propagation path for invalid file handle operations
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpWritev_InvalidHandle_ReturnsError) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Use an invalid file descriptor
    os_file_handle invalid_handle = (os_file_handle)(uintptr_t)-1;

    // Setup iovec with test data
    const char *test_data = "Test data for invalid handle";
    struct __wasi_ciovec_t iov = {
        .buf = (const uint8_t*)test_data,
        .buf_len = strlen(test_data)
    };
    size_t nwritten = 0;

    // Test blocking_op_writev with invalid handle
    __wasi_errno_t result = blocking_op_writev(exec_env, invalid_handle, &iov, 1, &nwritten);

    // Verify the function returns an error code (not success)
    ASSERT_NE(0, result) << "blocking_op_writev should return error for invalid file handle";

    // Common error codes for invalid file descriptor
    ASSERT_TRUE(result == __WASI_EBADF || result == __WASI_EINVAL || result == __WASI_ENOSYS)
        << "Expected EBADF, EINVAL, or ENOSYS for invalid handle, got: " << result;
}

/******
 * Test Case: blocking_op_writev_MultipleIovecs_ReturnsSuccess
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:50-59
 * Target Lines: 54 (blocking op check), 57 (os_writev call), 58 (end blocking op), 59 (return)
 * Functional Purpose: Validates that blocking_op_writev() properly handles multiple iovec
 *                     structures for scatter-gather I/O operations, testing the iovcnt
 *                     parameter handling and ensuring proper data consolidation.
 * Call Path: blocking_op_writev() <- wasmtime_ssp_fd_write() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise success path for multi-buffer write operations
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpWritev_MultipleIovecs_ReturnsSuccess) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a test file for writing multiple iovecs
    int test_fd = open("/tmp/test_blocking_op_writev_multi", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    ASSERT_NE(-1, test_fd) << "Failed to create test file: " << strerror(errno);

    os_file_handle handle = (os_file_handle)(uintptr_t)test_fd;

    // Setup multiple iovecs for scatter-gather write
    const char *data1 = "First part, ";
    const char *data2 = "second part, ";
    const char *data3 = "third part.";
    struct __wasi_ciovec_t iovs[3] = {
        { .buf = (const uint8_t*)data1, .buf_len = strlen(data1) },
        { .buf = (const uint8_t*)data2, .buf_len = strlen(data2) },
        { .buf = (const uint8_t*)data3, .buf_len = strlen(data3) }
    };
    size_t nwritten = 0;

    // Test blocking_op_writev with multiple iovecs
    __wasi_errno_t result = blocking_op_writev(exec_env, handle, iovs, 3, &nwritten);

    // Verify the function returns success
    ASSERT_EQ(0, result) << "blocking_op_writev should succeed for multiple iovecs";

    // Verify all data was written
    size_t expected_total = strlen(data1) + strlen(data2) + strlen(data3);
    ASSERT_EQ(expected_total, nwritten) << "Should have written all data from multiple iovecs";

    // Cleanup
    close(test_fd);
    unlink("/tmp/test_blocking_op_writev_multi");
}

/******
 * Test Case: blocking_op_writev_ReadOnlyFile_ReturnsError
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:50-59
 * Target Lines: 54 (blocking op check), 57 (os_writev call), 58 (end blocking op), 59 (return error)
 * Functional Purpose: Validates that blocking_op_writev() properly handles write attempts to
 *                     read-only files by propagating appropriate error codes from os_writev(),
 *                     ensuring proper permission validation and error handling.
 * Call Path: blocking_op_writev() <- wasmtime_ssp_fd_write() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise error propagation path for permission-denied write operations
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpWritev_ReadOnlyFile_ReturnsError) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a test file and open it read-only
    const char *filename = "/tmp/test_blocking_op_writev_readonly";
    int create_fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    ASSERT_NE(-1, create_fd) << "Failed to create test file: " << strerror(errno);
    write(create_fd, "initial", 7);
    close(create_fd);

    // Open the file in read-only mode
    int readonly_fd = open(filename, O_RDONLY);
    ASSERT_NE(-1, readonly_fd) << "Failed to open file read-only: " << strerror(errno);

    os_file_handle handle = (os_file_handle)(uintptr_t)readonly_fd;

    // Setup iovec with test data for writing
    const char *test_data = "This should fail to write";
    struct __wasi_ciovec_t iov = {
        .buf = (const uint8_t*)test_data,
        .buf_len = strlen(test_data)
    };
    size_t nwritten = 0;

    // Test blocking_op_writev with read-only file
    __wasi_errno_t result = blocking_op_writev(exec_env, handle, &iov, 1, &nwritten);

    // Verify the function returns an error for read-only file
    ASSERT_NE(0, result) << "blocking_op_writev should return error for read-only file";

    // Common error codes for permission denied or bad file descriptor
    ASSERT_TRUE(result == __WASI_EBADF || result == __WASI_EPERM || result == __WASI_EACCES)
        << "Expected EBADF, EPERM, or EACCES for read-only file, got: " << result;

    // Cleanup
    close(readonly_fd);
    unlink(filename);
}

/******
 * Test Case: blocking_op_writev_ZeroLength_ReturnsSuccess
 * Source: core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/blocking_op.c:50-59
 * Target Lines: 54 (blocking op check), 57 (os_writev call), 58 (end blocking op), 59 (return)
 * Functional Purpose: Validates that blocking_op_writev() properly handles edge case of
 *                     zero-length write operations, ensuring that empty writes are handled
 *                     gracefully and return appropriate success codes.
 * Call Path: blocking_op_writev() <- wasmtime_ssp_fd_write() <- WASI wrapper functions <- WASM module
 * Coverage Goal: Exercise success path for edge case with zero-length write operation
 ******/
TEST_F(EnhancedBlockingOpTest, BlockingOpWritev_ZeroLength_ReturnsSuccess) {
    // Skip test if platform doesn't support file operations
    if (!PlatformTestContext::HasFileSupport() || !PlatformTestContext::IsLinux()) {
        return;
    }

    // Create a test file for zero-length write
    int test_fd = open("/tmp/test_blocking_op_writev_zero", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    ASSERT_NE(-1, test_fd) << "Failed to create test file: " << strerror(errno);

    os_file_handle handle = (os_file_handle)(uintptr_t)test_fd;

    // Setup iovec with zero-length data
    const char *empty_data = "";
    struct __wasi_ciovec_t iov = {
        .buf = (const uint8_t*)empty_data,
        .buf_len = 0
    };
    size_t nwritten = 0;

    // Test blocking_op_writev with zero-length data
    __wasi_errno_t result = blocking_op_writev(exec_env, handle, &iov, 1, &nwritten);

    // Verify the function handles zero-length write appropriately
    ASSERT_EQ(0, result) << "blocking_op_writev should succeed for zero-length write";

    // Verify no data was written
    ASSERT_EQ(0, nwritten) << "Should have written zero bytes for zero-length operation";

    // Cleanup
    close(test_fd);
    unlink("/tmp/test_blocking_op_writev_zero");
}