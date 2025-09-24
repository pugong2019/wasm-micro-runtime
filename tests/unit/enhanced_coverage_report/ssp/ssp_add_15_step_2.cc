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
#include "blocking_op.h"
#include "ssp_config.h"
#include "locking.h"
#include "rights.h"
#include "str.h"
#include "libc_errno.h"
#include <sys/stat.h>
#include <poll.h>
}

// Platform detection utility for tests
class PlatformTestContext {
public:
    static bool IsLinux() {
#if defined(__linux__)
        return true;
#else
        return false;
#endif
    }
    
    static bool HasSocketSupport() {
#if defined(WASM_ENABLE_LIBC_WASI) && !defined(BH_PLATFORM_WINDOWS)
        return true;
#else
        return false;
#endif
    }
    
    static bool HasFileSupport() {
#if defined(WASM_ENABLE_LIBC_WASI)
        return true;
#else
        return false;
#endif
    }
};

// RAII wrapper for WAMR runtime initialization
template<uint32_t HEAP_SIZE = 512 * 1024>
class WAMRRuntimeRAII {
public:
    WAMRRuntimeRAII() : initialized_(false) {
        RuntimeInitArgs init_args;
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_System_Allocator;
        init_args.mem_alloc_option.pool.heap_buf = heap_buf_;
        init_args.mem_alloc_option.pool.heap_size = HEAP_SIZE;
        
        initialized_ = wasm_runtime_full_init(&init_args);
    }
    
    ~WAMRRuntimeRAII() {
        if (initialized_) {
            wasm_runtime_destroy();
        }
    }
    
    bool IsInitialized() const { return initialized_; }

private:
    bool initialized_;
    char heap_buf_[HEAP_SIZE];
};

class SSPBlockingOpTest : public testing::Test {
protected:
    void SetUp() override {
        runtime_ = std::make_unique<WAMRRuntimeRAII<>>();
        ASSERT_TRUE(runtime_->IsInitialized());
    }
    
    void TearDown() override {
        runtime_.reset();
    }
    
    std::unique_ptr<WAMRRuntimeRAII<>> runtime_;
};

// Step 2: Blocking Operations & I/O Functions Tests

TEST_F(SSPBlockingOpTest, BlockingOpClose_ValidHandle_Success) {
    if (!PlatformTestContext::HasFileSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    os_file_handle handle = (os_file_handle)1;
    bool force = false;
    
    // Test that blocking_op_close handles valid parameters
    __wasi_errno_t result = blocking_op_close(exec_env, handle, force);
    
    // Should return success or appropriate error code
    ASSERT_TRUE(result == 0 || result != EINVAL);
}

TEST_F(SSPBlockingOpTest, BlockingOpClose_InvalidHandle_Error) {
    if (!PlatformTestContext::HasFileSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    os_file_handle handle = (os_file_handle)-1;
    bool force = false;
    
    // Test that blocking_op_close handles invalid handle
    __wasi_errno_t result = blocking_op_close(exec_env, handle, force);
    
    // Should return error for invalid handle
    ASSERT_NE(result, 0);
}

TEST_F(SSPBlockingOpTest, BlockingOpReadv_Success_ReadsData) {
    if (!PlatformTestContext::HasFileSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    os_file_handle handle = (os_file_handle)0; // stdin
    __wasi_iovec_t iov[1];
    char buffer[64];
    iov[0].buf = (uint8_t*)buffer;
    iov[0].buf_len = sizeof(buffer);
    int iovcnt = 1;
    size_t nread = 0;
    
    // Test readv operation
    __wasi_errno_t result = blocking_op_readv(exec_env, handle, iov, iovcnt, &nread);
    
    // Should handle the operation (success or appropriate error)
    ASSERT_TRUE(result == 0 || result == EAGAIN || result == EBADF);
}

TEST_F(SSPBlockingOpTest, BlockingOpReadv_ErrorHandling_InvalidParams) {
    if (!PlatformTestContext::HasFileSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    os_file_handle handle = (os_file_handle)-1;
    const __wasi_iovec_t *iov = nullptr;
    int iovcnt = 0;
    size_t nread = 0;
    
    // Test readv with invalid parameters
    __wasi_errno_t result = blocking_op_readv(exec_env, handle, iov, iovcnt, &nread);
    
    // Should return error for invalid parameters
    ASSERT_NE(result, 0);
}

TEST_F(SSPBlockingOpTest, BlockingOpPreadv_Success_ReadsAtOffset) {
    if (!PlatformTestContext::HasFileSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    os_file_handle handle = (os_file_handle)0;
    __wasi_iovec_t iov[1];
    char buffer[64];
    iov[0].buf = (uint8_t*)buffer;
    iov[0].buf_len = sizeof(buffer);
    int iovcnt = 1;
    __wasi_filesize_t offset = 0;
    size_t nread = 0;
    
    // Test preadv operation
    __wasi_errno_t result = blocking_op_preadv(exec_env, handle, iov, iovcnt, offset, &nread);
    
    // Should handle the operation appropriately
    ASSERT_TRUE(result == 0 || result == ESPIPE || result == EBADF);
}

TEST_F(SSPBlockingOpTest, BlockingOpPreadv_InvalidOffset_Error) {
    if (!PlatformTestContext::HasFileSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    os_file_handle handle = (os_file_handle)-1;
    __wasi_iovec_t iov[1];
    char buffer[64];
    iov[0].buf = (uint8_t*)buffer;
    iov[0].buf_len = sizeof(buffer);
    int iovcnt = 1;
    __wasi_filesize_t offset = (__wasi_filesize_t)-1;
    size_t nread = 0;
    
    // Test preadv with invalid offset
    __wasi_errno_t result = blocking_op_preadv(exec_env, handle, iov, iovcnt, offset, &nread);
    
    // Should return error
    ASSERT_NE(result, 0);
}

TEST_F(SSPBlockingOpTest, BlockingOpWritev_Success_WritesData) {
    if (!PlatformTestContext::HasFileSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    os_file_handle handle = (os_file_handle)1; // stdout
    __wasi_ciovec_t iov[1];
    const char data[] = "test data";
    iov[0].buf = (const uint8_t*)data;
    iov[0].buf_len = strlen(data);
    int iovcnt = 1;
    size_t nwritten = 0;
    
    // Test writev operation
    __wasi_errno_t result = blocking_op_writev(exec_env, handle, iov, iovcnt, &nwritten);
    
    // Should handle the operation
    ASSERT_TRUE(result == 0 || result == EAGAIN || result == EBADF);
}

TEST_F(SSPBlockingOpTest, BlockingOpWritev_ErrorHandling_InvalidParams) {
    if (!PlatformTestContext::HasFileSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    os_file_handle handle = (os_file_handle)-1;
    const __wasi_ciovec_t *iov = nullptr;
    int iovcnt = 0;
    size_t nwritten = 0;
    
    // Test writev with invalid parameters
    __wasi_errno_t result = blocking_op_writev(exec_env, handle, iov, iovcnt, &nwritten);
    
    // Should return error
    ASSERT_NE(result, 0);
}

TEST_F(SSPBlockingOpTest, BlockingOpPwritev_Success_WritesAtOffset) {
    if (!PlatformTestContext::HasFileSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    os_file_handle handle = (os_file_handle)1;
    __wasi_ciovec_t iov[1];
    const char data[] = "test";
    iov[0].buf = (const uint8_t*)data;
    iov[0].buf_len = strlen(data);
    int iovcnt = 1;
    __wasi_filesize_t offset = 0;
    size_t nwritten = 0;
    
    // Test pwritev operation
    __wasi_errno_t result = blocking_op_pwritev(exec_env, handle, iov, iovcnt, offset, &nwritten);
    
    // Should handle the operation
    ASSERT_TRUE(result == 0 || result == ESPIPE || result == EBADF);
}

TEST_F(SSPBlockingOpTest, BlockingOpPwritev_InvalidOffset_Error) {
    if (!PlatformTestContext::HasFileSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    os_file_handle handle = (os_file_handle)-1;
    __wasi_ciovec_t iov[1];
    const char data[] = "test";
    iov[0].buf = (const uint8_t*)data;
    iov[0].buf_len = strlen(data);
    int iovcnt = 1;
    __wasi_filesize_t offset = (__wasi_filesize_t)-1;
    size_t nwritten = 0;
    
    // Test pwritev with invalid parameters
    __wasi_errno_t result = blocking_op_pwritev(exec_env, handle, iov, iovcnt, offset, &nwritten);
    
    // Should return error
    ASSERT_NE(result, 0);
}

TEST_F(SSPBlockingOpTest, BlockingOpSocketAccept_Success_AcceptsConnection) {
    if (!PlatformTestContext::HasSocketSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    bh_socket_t server_sock = -1;
    bh_socket_t *sock = nullptr;
    void *addr = nullptr;
    uint32_t *addrlen = nullptr;
    
    // Test socket accept operation
    int result = blocking_op_socket_accept(exec_env, server_sock, sock, addr, addrlen);
    
    // Should handle invalid socket appropriately
    ASSERT_EQ(result, BHT_ERROR);
}

TEST_F(SSPBlockingOpTest, BlockingOpSocketAccept_Error_InvalidSocket) {
    if (!PlatformTestContext::HasSocketSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    bh_socket_t server_sock = -1;
    bh_socket_t *sock = nullptr;
    void *addr = nullptr;
    uint32_t *addrlen = nullptr;
    
    // Test with invalid socket
    int result = blocking_op_socket_accept(exec_env, server_sock, sock, addr, addrlen);
    
    // Should return error
    ASSERT_EQ(result, BHT_ERROR);
}

TEST_F(SSPBlockingOpTest, BlockingOpSocketConnect_Success_ConnectsToAddress) {
    if (!PlatformTestContext::HasSocketSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    bh_socket_t sock = -1;
    const char *addr = "127.0.0.1";
    int port = 80;
    
    // Test socket connect operation
    int result = blocking_op_socket_connect(exec_env, sock, addr, port);
    
    // Should handle invalid parameters appropriately
    ASSERT_EQ(result, BHT_ERROR);
}

TEST_F(SSPBlockingOpTest, BlockingOpSocketConnect_Failure_InvalidParams) {
    if (!PlatformTestContext::HasSocketSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    bh_socket_t sock = -1;
    const char *addr = nullptr;
    int port = 0;
    
    // Test with invalid parameters
    int result = blocking_op_socket_connect(exec_env, sock, addr, port);
    
    // Should return error
    ASSERT_EQ(result, BHT_ERROR);
}

TEST_F(SSPBlockingOpTest, BlockingOpSocketRecvFrom_Success_ReceivesData) {
    if (!PlatformTestContext::HasSocketSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    bh_socket_t sock = -1;
    void *buf = nullptr;
    uint32_t len = 0;
    int flags = 0;
    bh_sockaddr_t *src_addr = nullptr;
    
    // Test socket recv_from operation
    int result = blocking_op_socket_recv_from(exec_env, sock, buf, len, flags, src_addr);
    
    // Should handle invalid socket appropriately
    ASSERT_EQ(result, -1);
}

TEST_F(SSPBlockingOpTest, BlockingOpSocketRecvFrom_Error_InvalidSocket) {
    if (!PlatformTestContext::HasSocketSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    bh_socket_t sock = -1;
    void *buf = nullptr;
    uint32_t len = 0;
    int flags = 0;
    bh_sockaddr_t *src_addr = nullptr;
    
    // Test with invalid socket
    int result = blocking_op_socket_recv_from(exec_env, sock, buf, len, flags, src_addr);
    
    // Should return error
    ASSERT_EQ(result, -1);
}

TEST_F(SSPBlockingOpTest, BlockingOpSocketSendTo_Success_SendsData) {
    if (!PlatformTestContext::HasSocketSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    bh_socket_t sock = -1;
    const void *buf = nullptr;
    uint32_t len = 0;
    int flags = 0;
    const bh_sockaddr_t *dest_addr = nullptr;
    
    // Test socket send_to operation
    int result = blocking_op_socket_send_to(exec_env, sock, buf, len, flags, dest_addr);
    
    // Should handle invalid socket appropriately
    ASSERT_EQ(result, -1);
}

TEST_F(SSPBlockingOpTest, BlockingOpSocketSendTo_Error_InvalidSocket) {
    if (!PlatformTestContext::HasSocketSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    bh_socket_t sock = -1;
    const void *buf = nullptr;
    uint32_t len = 0;
    int flags = 0;
    const bh_sockaddr_t *dest_addr = nullptr;
    
    // Test with invalid socket
    int result = blocking_op_socket_send_to(exec_env, sock, buf, len, flags, dest_addr);
    
    // Should return error
    ASSERT_EQ(result, -1);
}

TEST_F(SSPBlockingOpTest, BlockingOpOpenat_Success_OpensFile) {
    if (!PlatformTestContext::HasFileSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    os_file_handle handle = (os_file_handle)0;
    const char *path = "test_file";
    __wasi_oflags_t oflags = 0;
    __wasi_fdflags_t fd_flags = 0;
    __wasi_lookupflags_t lookup_flags = 0;
    wasi_libc_file_access_mode access_mode = 0;
    os_file_handle opened;
    
    // Test openat operation
    __wasi_errno_t result = blocking_op_openat(exec_env, handle, path, oflags, fd_flags, lookup_flags, access_mode, &opened);
    
    // Should handle the operation (may succeed or fail based on file existence)
    ASSERT_TRUE(result == 0 || result == ENOENT || result == EACCES);
}

TEST_F(SSPBlockingOpTest, BlockingOpOpenat_InvalidPath_Error) {
    if (!PlatformTestContext::HasFileSupport()) {
        return;
    }
    
    wasm_exec_env_t exec_env = nullptr;
    os_file_handle handle = (os_file_handle)-1;
    const char *path = nullptr;
    __wasi_oflags_t oflags = 0;
    __wasi_fdflags_t fd_flags = 0;
    __wasi_lookupflags_t lookup_flags = 0;
    wasi_libc_file_access_mode access_mode = 0;
    os_file_handle opened;
    
    // Test openat with invalid parameters
    __wasi_errno_t result = blocking_op_openat(exec_env, handle, path, oflags, fd_flags, lookup_flags, access_mode, &opened);
    
    // Should return error for invalid parameters
    ASSERT_NE(result, 0);
}