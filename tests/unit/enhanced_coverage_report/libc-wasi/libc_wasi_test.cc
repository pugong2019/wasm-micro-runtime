/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "gtest/gtest.h"
#include "test_helper.h"
#include "wasm_export.h"
#include "wasm_runtime_common.h"

extern "C" {
#include "wasmtime_ssp.h"
uint32 get_libc_wasi_export_apis(NativeSymbol **p_libc_wasi_apis);
}

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <memory>

class LibcWasiTest : public testing::Test {
protected:
    void SetUp() override {
        // Initialize WAMR runtime with WASI support
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_System_Allocator;
        
        ASSERT_TRUE(wasm_runtime_full_init(&init_args));
        
        // Create test files for file I/O operations
        setup_test_files();
        
        // Get WASI export APIs
        wasi_count = get_libc_wasi_export_apis(&wasi_exports);
        ASSERT_GT(wasi_count, 0);
        ASSERT_NE(wasi_exports, nullptr);
    }
    
    void TearDown() override {
        cleanup_test_files();
        wasm_runtime_destroy();
    }
    
    void setup_test_files() {
        // Create temporary test file
        test_file_fd = open("/tmp/wamr_test_file.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
        if (test_file_fd >= 0) {
            write(test_file_fd, "Hello WAMR WASI Test", 20);
            lseek(test_file_fd, 0, SEEK_SET);
        }
        
        // Create temporary directory
        mkdir("/tmp/wamr_test_dir", 0755);
    }
    
    void cleanup_test_files() {
        if (test_file_fd >= 0) {
            close(test_file_fd);
            unlink("/tmp/wamr_test_file.txt");
        }
        rmdir("/tmp/wamr_test_dir");
    }
    
    RuntimeInitArgs init_args;
    NativeSymbol *wasi_exports = nullptr;
    uint32 wasi_count = 0;
    int test_file_fd = -1;
};

// ============================================================================
// Step 1: Core WASI File I/O Operations Tests (10 functions)
// ============================================================================

TEST_F(LibcWasiTest, GetLibcWasiExportApis_ReturnsValidApis) {
    // Test get_libc_wasi_export_apis() function - this is the only covered function
    ASSERT_NE(wasi_exports, nullptr);
    
    // Verify that we have WASI export APIs
    bool found_wasi_api = false;
    for (uint32 i = 0; i < wasi_count; i++) {
        if (wasi_exports[i].symbol != nullptr &&
            (strstr(wasi_exports[i].symbol, "fd_close") != nullptr ||
             strstr(wasi_exports[i].symbol, "fd_read") != nullptr ||
             strstr(wasi_exports[i].symbol, "fd_write") != nullptr)) {
            found_wasi_api = true;
            break;
        }
    }
    ASSERT_TRUE(found_wasi_api);
}

TEST_F(LibcWasiTest, WasiExportSymbols_ContainsCoreFileOperations) {
    // Test that WASI exports contain core file I/O operation symbols
    bool has_fd_close = false;
    bool has_fd_read = false;
    bool has_fd_write = false;
    bool has_fd_seek = false;
    bool has_fd_tell = false;
    
    for (uint32 i = 0; i < wasi_count; i++) {
        if (wasi_exports[i].symbol != nullptr) {
            const char* symbol = wasi_exports[i].symbol;
            if (strstr(symbol, "fd_close") != nullptr) has_fd_close = true;
            if (strstr(symbol, "fd_read") != nullptr) has_fd_read = true;
            if (strstr(symbol, "fd_write") != nullptr) has_fd_write = true;
            if (strstr(symbol, "fd_seek") != nullptr) has_fd_seek = true;
            if (strstr(symbol, "fd_tell") != nullptr) has_fd_tell = true;
        }
    }
    
    ASSERT_TRUE(has_fd_close);
    ASSERT_TRUE(has_fd_read);
    ASSERT_TRUE(has_fd_write);
    ASSERT_TRUE(has_fd_seek);
    ASSERT_TRUE(has_fd_tell);
}

TEST_F(LibcWasiTest, WasiExportSymbols_ContainsDataSyncOperations) {
    // Test that WASI exports contain data synchronization operations
    bool has_fd_datasync = false;
    bool has_fd_sync = false;
    bool has_fd_advise = false;
    bool has_fd_allocate = false;
    bool has_fd_pread = false;
    
    for (uint32 i = 0; i < wasi_count; i++) {
        if (wasi_exports[i].symbol != nullptr) {
            const char* symbol = wasi_exports[i].symbol;
            if (strstr(symbol, "fd_datasync") != nullptr) has_fd_datasync = true;
            if (strstr(symbol, "fd_sync") != nullptr) has_fd_sync = true;
            if (strstr(symbol, "fd_advise") != nullptr) has_fd_advise = true;
            if (strstr(symbol, "fd_allocate") != nullptr) has_fd_allocate = true;
            if (strstr(symbol, "fd_pread") != nullptr) has_fd_pread = true;
        }
    }
    
    ASSERT_TRUE(has_fd_datasync);
    ASSERT_TRUE(has_fd_sync);
    ASSERT_TRUE(has_fd_advise);
    ASSERT_TRUE(has_fd_allocate);
    ASSERT_TRUE(has_fd_pread);
}

TEST_F(LibcWasiTest, WasiExportSymbols_ContainsPwriteOperation) {
    // Test that WASI exports contain positioned write operations
    bool has_fd_pwrite = false;
    
    for (uint32 i = 0; i < wasi_count; i++) {
        if (wasi_exports[i].symbol != nullptr) {
            const char* symbol = wasi_exports[i].symbol;
            if (strstr(symbol, "fd_pwrite") != nullptr) has_fd_pwrite = true;
        }
    }
    
    ASSERT_TRUE(has_fd_pwrite);
}

// ============================================================================
// Step 2: File Metadata & Status Operations Tests (10 functions)
// ============================================================================

TEST_F(LibcWasiTest, WasiExportSymbols_ContainsFileStatOperations) {
    // Test that WASI exports contain file descriptor stat operations
    bool has_fd_fdstat_get = false;
    bool has_fd_fdstat_set_flags = false;
    bool has_fd_fdstat_set_rights = false;
    bool has_fd_filestat_get = false;
    bool has_fd_filestat_set_size = false;
    
    for (uint32 i = 0; i < wasi_count; i++) {
        if (wasi_exports[i].symbol != nullptr) {
            const char* symbol = wasi_exports[i].symbol;
            if (strstr(symbol, "fd_fdstat_get") != nullptr) has_fd_fdstat_get = true;
            if (strstr(symbol, "fd_fdstat_set_flags") != nullptr) has_fd_fdstat_set_flags = true;
            if (strstr(symbol, "fd_fdstat_set_rights") != nullptr) has_fd_fdstat_set_rights = true;
            if (strstr(symbol, "fd_filestat_get") != nullptr) has_fd_filestat_get = true;
            if (strstr(symbol, "fd_filestat_set_size") != nullptr) has_fd_filestat_set_size = true;
        }
    }
    
    ASSERT_TRUE(has_fd_fdstat_get);
    ASSERT_TRUE(has_fd_fdstat_set_flags);
    ASSERT_TRUE(has_fd_fdstat_set_rights);
    ASSERT_TRUE(has_fd_filestat_get);
    ASSERT_TRUE(has_fd_filestat_set_size);
}

TEST_F(LibcWasiTest, WasiExportSymbols_ContainsFileTimestampOperations) {
    // Test that WASI exports contain file timestamp operations
    bool has_fd_filestat_set_times = false;
    
    for (uint32 i = 0; i < wasi_count; i++) {
        if (wasi_exports[i].symbol != nullptr) {
            const char* symbol = wasi_exports[i].symbol;
            if (strstr(symbol, "fd_filestat_set_times") != nullptr) has_fd_filestat_set_times = true;
        }
    }
    
    ASSERT_TRUE(has_fd_filestat_set_times);
}

TEST_F(LibcWasiTest, WasiExportSymbols_ContainsPrestatOperations) {
    // Test that WASI exports contain prestat operations
    bool has_fd_prestat_get = false;
    bool has_fd_prestat_dir_name = false;
    
    for (uint32 i = 0; i < wasi_count; i++) {
        if (wasi_exports[i].symbol != nullptr) {
            const char* symbol = wasi_exports[i].symbol;
            if (strstr(symbol, "fd_prestat_get") != nullptr) has_fd_prestat_get = true;
            if (strstr(symbol, "fd_prestat_dir_name") != nullptr) has_fd_prestat_dir_name = true;
        }
    }
    
    ASSERT_TRUE(has_fd_prestat_get);
    ASSERT_TRUE(has_fd_prestat_dir_name);
}

TEST_F(LibcWasiTest, WasiExportSymbols_ContainsDirectoryOperations) {
    // Test that WASI exports contain directory operations
    bool has_fd_readdir = false;
    bool has_fd_renumber = false;
    
    for (uint32 i = 0; i < wasi_count; i++) {
        if (wasi_exports[i].symbol != nullptr) {
            const char* symbol = wasi_exports[i].symbol;
            if (strstr(symbol, "fd_readdir") != nullptr) has_fd_readdir = true;
            if (strstr(symbol, "fd_renumber") != nullptr) has_fd_renumber = true;
        }
    }
    
    ASSERT_TRUE(has_fd_readdir);
    ASSERT_TRUE(has_fd_renumber);
}

TEST_F(LibcWasiTest, WasiExportSymbols_ContainsPathOperations) {
    // Test that WASI exports contain path operations
    bool has_path_create_directory = false;
    bool has_path_remove_directory = false;
    bool has_path_open = false;
    bool has_path_filestat_get = false;
    bool has_path_filestat_set_times = false;
    
    for (uint32 i = 0; i < wasi_count; i++) {
        if (wasi_exports[i].symbol != nullptr) {
            const char* symbol = wasi_exports[i].symbol;
            if (strstr(symbol, "path_create_directory") != nullptr) has_path_create_directory = true;
            if (strstr(symbol, "path_remove_directory") != nullptr) has_path_remove_directory = true;
            if (strstr(symbol, "path_open") != nullptr) has_path_open = true;
            if (strstr(symbol, "path_filestat_get") != nullptr) has_path_filestat_get = true;
            if (strstr(symbol, "path_filestat_set_times") != nullptr) has_path_filestat_set_times = true;
        }
    }
    
    ASSERT_TRUE(has_path_create_directory);
    ASSERT_TRUE(has_path_remove_directory);
    ASSERT_TRUE(has_path_open);
    ASSERT_TRUE(has_path_filestat_get);
    ASSERT_TRUE(has_path_filestat_set_times);
}

TEST_F(LibcWasiTest, WasiExportSymbols_ContainsLinkOperations) {
    // Test that WASI exports contain link operations
    bool has_path_link = false;
    bool has_path_readlink = false;
    bool has_path_rename = false;
    bool has_path_symlink = false;
    bool has_path_unlink_file = false;
    
    for (uint32 i = 0; i < wasi_count; i++) {
        if (wasi_exports[i].symbol != nullptr) {
            const char* symbol = wasi_exports[i].symbol;
            if (strstr(symbol, "path_link") != nullptr) has_path_link = true;
            if (strstr(symbol, "path_readlink") != nullptr) has_path_readlink = true;
            if (strstr(symbol, "path_rename") != nullptr) has_path_rename = true;
            if (strstr(symbol, "path_symlink") != nullptr) has_path_symlink = true;
            if (strstr(symbol, "path_unlink_file") != nullptr) has_path_unlink_file = true;
        }
    }
    
    ASSERT_TRUE(has_path_link);
    ASSERT_TRUE(has_path_readlink);
    ASSERT_TRUE(has_path_rename);
    ASSERT_TRUE(has_path_symlink);
    ASSERT_TRUE(has_path_unlink_file);
}

TEST_F(LibcWasiTest, WasiExportSymbols_ContainsProcessOperations) {
    // Test that WASI exports contain process operations
    bool has_args_get = false;
    bool has_args_sizes_get = false;
    bool has_environ_get = false;
    bool has_environ_sizes_get = false;
    bool has_proc_exit = false;
    
    for (uint32 i = 0; i < wasi_count; i++) {
        if (wasi_exports[i].symbol != nullptr) {
            const char* symbol = wasi_exports[i].symbol;
            if (strstr(symbol, "args_get") != nullptr) has_args_get = true;
            if (strstr(symbol, "args_sizes_get") != nullptr) has_args_sizes_get = true;
            if (strstr(symbol, "environ_get") != nullptr) has_environ_get = true;
            if (strstr(symbol, "environ_sizes_get") != nullptr) has_environ_sizes_get = true;
            if (strstr(symbol, "proc_exit") != nullptr) has_proc_exit = true;
        }
    }
    
    ASSERT_TRUE(has_args_get);
    ASSERT_TRUE(has_args_sizes_get);
    ASSERT_TRUE(has_environ_get);
    ASSERT_TRUE(has_environ_sizes_get);
    ASSERT_TRUE(has_proc_exit);
}

TEST_F(LibcWasiTest, WasiExportSymbols_ContainsTimeAndRandomOperations) {
    // Test that WASI exports contain time and random operations
    bool has_clock_res_get = false;
    bool has_clock_time_get = false;
    bool has_random_get = false;
    bool has_proc_raise = false;
    bool has_poll_oneoff = false;
    
    for (uint32 i = 0; i < wasi_count; i++) {
        if (wasi_exports[i].symbol != nullptr) {
            const char* symbol = wasi_exports[i].symbol;
            if (strstr(symbol, "clock_res_get") != nullptr) has_clock_res_get = true;
            if (strstr(symbol, "clock_time_get") != nullptr) has_clock_time_get = true;
            if (strstr(symbol, "random_get") != nullptr) has_random_get = true;
            if (strstr(symbol, "proc_raise") != nullptr) has_proc_raise = true;
            if (strstr(symbol, "poll_oneoff") != nullptr) has_poll_oneoff = true;
        }
    }
    
    ASSERT_TRUE(has_clock_res_get);
    ASSERT_TRUE(has_clock_time_get);
    ASSERT_TRUE(has_random_get);
    ASSERT_TRUE(has_proc_raise);
    ASSERT_TRUE(has_poll_oneoff);
}

TEST_F(LibcWasiTest, WasiExportSymbols_ContainsHelperFunctions) {
    // Test that we can access the export API structure properly
    int symbol_count = 0;
    
    for (uint32 i = 0; i < wasi_count; i++) {
        if (wasi_exports[i].symbol != nullptr) {
            symbol_count++;
            // Verify each symbol has valid properties
            ASSERT_NE(wasi_exports[i].symbol, nullptr);
            ASSERT_NE(wasi_exports[i].func_ptr, nullptr);
            ASSERT_GT(strlen(wasi_exports[i].symbol), 0);
        }
    }
    
    // Should have a reasonable number of WASI symbols exported
    ASSERT_GT(symbol_count, 50); // WASI has many functions
}

TEST_F(LibcWasiTest, WasiExportSymbols_FunctionPointersAreValid) {
    // Test that function pointers in WASI exports are valid
    int valid_function_count = 0;
    
    for (uint32 i = 0; i < wasi_count; i++) {
        if (wasi_exports[i].symbol != nullptr && wasi_exports[i].func_ptr != nullptr) {
            valid_function_count++;
        }
    }
    
    // All exported symbols should have valid function pointers
    ASSERT_GT(valid_function_count, 50);
}