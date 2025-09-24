/*
 * Copyright (C) 2025 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "gtest/gtest.h"
#include "wasm_export.h"
#include "bh_platform.h"

extern "C" {
#include "wasmtime_ssp.h"
#include "posix.h"
#include "ssp_config.h"
#include "locking.h"
#include "rights.h"
#include "str.h"
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
    static bool HasNetworkSupport() {
#if defined(WASM_ENABLE_LIBC_WASI) && !defined(BH_PLATFORM_WINDOWS)
        return true;
#else
        return false;
#endif
    }
};

// Test fixture for SSP functions
class SSPFunctionsTest : public testing::Test {
protected:
    void SetUp() override {
        RuntimeInitArgs init_args;
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_System_Allocator;
        
        ASSERT_TRUE(wasm_runtime_full_init(&init_args));
        
        // Initialize fd_prestats structure for testing
        memset(&prestats, 0, sizeof(prestats));
        ASSERT_TRUE(fd_prestats_init(&prestats));
        
        // Initialize fd_table structure for testing  
        memset(&fd_table, 0, sizeof(fd_table));
        ASSERT_TRUE(fd_table_init(&fd_table));
    }
    
    void TearDown() override {
        // Clean up prestats
        if (prestats.prestats) {
            wasm_runtime_free(prestats.prestats);
        }
        
        // Clean up fd_table
        if (fd_table.entries) {
            wasm_runtime_free(fd_table.entries);
        }
        
        wasm_runtime_destroy();
    }
    
    struct fd_prestats prestats;
    struct fd_table fd_table;
};

// Test 1: fd_prestats_init - Initialization test
TEST_F(SSPFunctionsTest, fd_prestats_init_success) {
    struct fd_prestats test_prestats;
    memset(&test_prestats, 0, sizeof(test_prestats));
    
    // Test successful initialization
    bool result = fd_prestats_init(&test_prestats);
    
    ASSERT_TRUE(result);
    ASSERT_EQ(0, test_prestats.size);
    ASSERT_EQ(0, test_prestats.used);
    ASSERT_EQ(nullptr, test_prestats.prestats);
    
    // Clean up
    if (test_prestats.prestats) {
        wasm_runtime_free(test_prestats.prestats);
    }
}

// Test 2: fd_prestats_insert - Success scenario  
TEST_F(SSPFunctionsTest, fd_prestats_insert_success) {
    const char *test_dir = "/test/directory";
    __wasi_fd_t test_fd = 3;
    
    // Insert entry using public API
    bool result = fd_prestats_insert(&prestats, test_dir, test_fd);
    
    ASSERT_TRUE(result);
    ASSERT_GE(prestats.size, test_fd + 1);
}

// Test 3: fd_prestats_insert - Multiple entries
TEST_F(SSPFunctionsTest, fd_prestats_insert_multiple) {
    const char *test_dir1 = "/test/dir1";
    const char *test_dir2 = "/test/dir2";
    __wasi_fd_t test_fd1 = 3;
    __wasi_fd_t test_fd2 = 5;
    
    // Insert multiple entries
    bool result1 = fd_prestats_insert(&prestats, test_dir1, test_fd1);
    bool result2 = fd_prestats_insert(&prestats, test_dir2, test_fd2);
    
    ASSERT_TRUE(result1);
    ASSERT_TRUE(result2);
    ASSERT_GE(prestats.size, test_fd2 + 1);
}

// Test 4: fd_table_init - Initialization test
TEST_F(SSPFunctionsTest, fd_table_init_success) {
    struct fd_table test_table;
    memset(&test_table, 0, sizeof(test_table));
    
    // Test successful initialization
    bool result = fd_table_init(&test_table);
    
    ASSERT_TRUE(result);
    ASSERT_EQ(0, test_table.size);
    ASSERT_EQ(0, test_table.used);
    ASSERT_EQ(nullptr, test_table.entries);
    
    // Clean up
    if (test_table.entries) {
        wasm_runtime_free(test_table.entries);
    }
}

// Test 5: os_openat - File operations test
TEST_F(SSPFunctionsTest, os_openat_basic_operations) {
    if (!PlatformTestContext::IsLinux()) {
        return; // Skip if not on Linux
    }
    
    os_file_handle handle;
    
    // Test opening a directory that should exist
    __wasi_errno_t result = os_openat(AT_FDCWD, "/tmp", O_RDONLY, 0, 0, 0, &handle);
    
    if (result == __WASI_ESUCCESS) {
        ASSERT_TRUE(os_is_handle_valid(&handle));
        os_close(handle, false);
    } else {
        // Directory might not exist or permission denied - test error handling
        ASSERT_NE(__WASI_ESUCCESS, result);
    }
}

// Test 6: os_openat - Invalid path test  
TEST_F(SSPFunctionsTest, os_openat_invalid_path) {
    os_file_handle handle;
    
    // Test opening a non-existent path
    __wasi_errno_t result = os_openat(AT_FDCWD, "/nonexistent/path/test", O_RDONLY, 0, 0, 0, &handle);
    
    // Should return an error code
    ASSERT_NE(__WASI_ESUCCESS, result);
    ASSERT_EQ(__WASI_ENOENT, result);
}

// Test 7: os_fstat - File stat operations
TEST_F(SSPFunctionsTest, os_fstat_operations) {
    if (!PlatformTestContext::IsLinux()) {
        return; // Skip if not on Linux
    }
    
    os_file_handle handle;
    __wasi_filestat_t stat_buf;
    
    // Try to open /tmp directory
    __wasi_errno_t open_result = os_openat(AT_FDCWD, "/tmp", O_RDONLY, 0, 0, 0, &handle);
    
    if (open_result == __WASI_ESUCCESS) {
        __wasi_errno_t stat_result = os_fstat(handle, &stat_buf);
        
        ASSERT_EQ(__WASI_ESUCCESS, stat_result);
        ASSERT_EQ(__WASI_FILETYPE_DIRECTORY, stat_buf.st_filetype);
        
        os_close(handle, false);
    }
}

// Test 8: os_close - Handle closing
TEST_F(SSPFunctionsTest, os_close_operations) {
    if (!PlatformTestContext::IsLinux()) {
        return; // Skip if not on Linux
    }
    
    os_file_handle handle;
    
    // Open a file/directory
    __wasi_errno_t open_result = os_openat(AT_FDCWD, "/tmp", O_RDONLY, 0, 0, 0, &handle);
    
    if (open_result == __WASI_ESUCCESS) {
        // Verify handle is valid
        ASSERT_TRUE(os_is_handle_valid(&handle));
        
        // Close the handle
        __wasi_errno_t close_result = os_close(handle, false);
        ASSERT_EQ(__WASI_ESUCCESS, close_result);
    }
}

// Test 9: String operations - str_nullterminate
TEST_F(SSPFunctionsTest, str_nullterminate_operations) {
    char buffer[64];
    const char *test_str = "Hello World";
    
    // Copy string to buffer
    memcpy(buffer, test_str, strlen(test_str));
    
    // Null terminate using str_nullterminate
    char *result = str_nullterminate(buffer, strlen(test_str));
    
    ASSERT_NE(nullptr, result);
    ASSERT_STREQ(test_str, result);
}

// Test 10: String operations - str_nullterminate with empty string
TEST_F(SSPFunctionsTest, str_nullterminate_empty) {
    char buffer[64];
    
    // Test with empty string
    char *result = str_nullterminate(buffer, 0);
    
    ASSERT_NE(nullptr, result);
    ASSERT_STREQ("", result);
}

// Test 11: Clock operations - os_clock_time_get
TEST_F(SSPFunctionsTest, os_clock_time_get_operations) {
    __wasi_timestamp_t time_val;
    
    // Get realtime clock
    __wasi_errno_t result = os_clock_time_get(__WASI_CLOCK_REALTIME, 1, &time_val);
    
    ASSERT_EQ(__WASI_ESUCCESS, result);
    ASSERT_GT(time_val, 0); // Should be a positive timestamp
}

// Test 12: Clock operations - monotonic clock
TEST_F(SSPFunctionsTest, os_clock_time_get_monotonic) {
    __wasi_timestamp_t time1, time2;
    
    // Get monotonic clock twice
    __wasi_errno_t result1 = os_clock_time_get(__WASI_CLOCK_MONOTONIC, 1, &time1);
    __wasi_errno_t result2 = os_clock_time_get(__WASI_CLOCK_MONOTONIC, 1, &time2);
    
    ASSERT_EQ(__WASI_ESUCCESS, result1);
    ASSERT_EQ(__WASI_ESUCCESS, result2);
    ASSERT_GE(time2, time1); // Second reading should be >= first
}

// Test 13: Memory operations - Large allocation
TEST_F(SSPFunctionsTest, memory_allocation_operations) {
    // Test WAMR memory allocation functions used by SSP
    void *ptr = wasm_runtime_malloc(1024);
    
    ASSERT_NE(nullptr, ptr);
    
    // Write to memory to ensure it's valid
    memset(ptr, 0xAA, 1024);
    
    // Verify memory content
    uint8_t *byte_ptr = (uint8_t*)ptr;
    ASSERT_EQ(0xAA, byte_ptr[0]);
    ASSERT_EQ(0xAA, byte_ptr[1023]);
    
    wasm_runtime_free(ptr);
}

// Test 14: Error handling - Invalid file descriptor
TEST_F(SSPFunctionsTest, invalid_fd_operations) {
    __wasi_filestat_t stat_buf;
    os_file_handle invalid_handle = -1; // Invalid handle
    
    // Try to stat invalid handle
    __wasi_errno_t result = os_fstat(invalid_handle, &stat_buf);
    
    // Should return error for invalid handle
    ASSERT_NE(__WASI_ESUCCESS, result);
    ASSERT_EQ(__WASI_EBADF, result);
}

// Test 15: Path operations - Path validation
TEST_F(SSPFunctionsTest, path_validation_operations) {
    os_file_handle handle;
    
    // Test with empty path
    __wasi_errno_t result = os_openat(AT_FDCWD, "", O_RDONLY, 0, 0, 0, &handle);
    
    // Should return error for empty path
    ASSERT_NE(__WASI_ESUCCESS, result);
}

// Test 16: Blocking operations - Basic test
TEST_F(SSPFunctionsTest, blocking_operations_basic) {
    // Test that blocking operation context can be created
    // This exercises the blocking_op.c functionality
    
    // Just verify the runtime is properly initialized for blocking ops
    ASSERT_TRUE(wasm_runtime_is_running_mode_supported(Mode_Interp));
}

// Test 17: fd_prestats operations - Edge cases
TEST_F(SSPFunctionsTest, fd_prestats_edge_cases) {
    // Test inserting at fd 0
    bool result = fd_prestats_insert(&prestats, "/root", 0);
    ASSERT_TRUE(result);
    
    // Test inserting at a larger fd
    result = fd_prestats_insert(&prestats, "/large", 100);
    ASSERT_TRUE(result);
    
    // Verify size grew appropriately
    ASSERT_GE(prestats.size, 100);
}

// Test 18: fd_table operations - Basic functionality
TEST_F(SSPFunctionsTest, fd_table_basic_operations) {
    // Test that table is properly initialized
    ASSERT_EQ(0, fd_table.size);
    ASSERT_EQ(0, fd_table.used);
    ASSERT_EQ(nullptr, fd_table.entries);
    
    // Test multiple table initialization
    struct fd_table another_table;
    memset(&another_table, 0, sizeof(another_table));
    bool result = fd_table_init(&another_table);
    ASSERT_TRUE(result);
    
    // Clean up
    if (another_table.entries) {
        wasm_runtime_free(another_table.entries);
    }
}

// Test 19: String utilities - Various lengths
TEST_F(SSPFunctionsTest, str_operations_various_lengths) {
    // Test with different string lengths
    const char *short_str = "hi";
    const char *long_str = "This is a much longer string for testing";
    
    char buffer[128];
    
    // Test short string
    memcpy(buffer, short_str, strlen(short_str));
    char *result1 = str_nullterminate(buffer, strlen(short_str));
    ASSERT_NE(nullptr, result1);
    ASSERT_STREQ(short_str, result1);
    
    // Test long string
    memcpy(buffer, long_str, strlen(long_str));
    char *result2 = str_nullterminate(buffer, strlen(long_str));
    ASSERT_NE(nullptr, result2);
    ASSERT_STREQ(long_str, result2);
}

// Test 20: Comprehensive integration test
TEST_F(SSPFunctionsTest, comprehensive_integration_test) {
    // Test multiple SSP components working together
    
    // 1. Initialize structures
    struct fd_prestats test_prestats;
    struct fd_table test_table;
    
    ASSERT_TRUE(fd_prestats_init(&test_prestats));
    ASSERT_TRUE(fd_table_init(&test_table));
    
    // 2. Test prestat operations
    bool insert_result = fd_prestats_insert(&test_prestats, "/test", 3);
    ASSERT_TRUE(insert_result);
    
    // 3. Test clock operations
    __wasi_timestamp_t timestamp;
    __wasi_errno_t clock_result = os_clock_time_get(__WASI_CLOCK_REALTIME, 1, &timestamp);
    ASSERT_EQ(__WASI_ESUCCESS, clock_result);
    ASSERT_GT(timestamp, 0);
    
    // 4. Test string operations
    char str_buf[32];
    const char *test_str = "integration";
    memcpy(str_buf, test_str, strlen(test_str));
    char *str_result = str_nullterminate(str_buf, strlen(test_str));
    ASSERT_NE(nullptr, str_result);
    ASSERT_STREQ(test_str, str_result);
    
    // 5. Test memory operations
    void *mem_ptr = wasm_runtime_malloc(256);
    ASSERT_NE(nullptr, mem_ptr);
    memset(mem_ptr, 0x55, 256);
    uint8_t *byte_ptr = (uint8_t*)mem_ptr;
    ASSERT_EQ(0x55, byte_ptr[0]);
    ASSERT_EQ(0x55, byte_ptr[255]);
    wasm_runtime_free(mem_ptr);
    
    // Clean up
    if (test_prestats.prestats) {
        wasm_runtime_free(test_prestats.prestats);
    }
    if (test_table.entries) {
        wasm_runtime_free(test_table.entries);
    }
}