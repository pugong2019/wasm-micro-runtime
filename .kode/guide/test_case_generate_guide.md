# Test Case Generation Guide for High-Quality Unit Tests

## Core Principles for Meaningful Test Coverage

### 1. **Verify Actual Functionality, Not Just Execution**
❌ **Bad Example:**
```cpp
TEST_F(MyTest, SomeFunction) {
    some_function();
    SUCCEED() << "Function executed successfully";
}
```

✅ **Good Example:**
```cpp
TEST_F(MyTest, SomeFunctionReturnsASSERTedValue) {
    int result = some_function();
    ASSERT_EQ(42, result);
    ASSERT_GT(result, 0);
}
```

### 2. **Use Specific Assertions, Avoid Tautologies**
❌ **Bad Examples (Always True):**
```cpp
ASSERT_TRUE(result == 0 || result != 0); // Always true - covers all integers!
ASSERT_TRUE(result >= 0 || result < 0);  // Always true - covers all integers!
ASSERT_TRUE(result == SUCCESS || result == FAILURE || result == OTHER); // Too permissive!
ASSERT_TRUE(success)  // it has do nothing at all!
```

✅ **Good Examples:**
```cpp
ASSERT_EQ(0, result);                    // Specific success ASSERTation
ASSERT_NE(0, result);                    // Specific failure ASSERTation  
ASSERT_TRUE(result == 0 || result == -1); // Specific success OR specific error
ASSERT_GE(result, 0);                    // Meaningful boundary check
ASSERT_LT(result, MAX_VALUE);            // Meaningful upper bound
```

### 3. **Test Both Success and Error Paths**
✅ **Complete Coverage:**
```cpp
TEST_F(FileTest, OpenValidFile) {
    int fd = os_openat(AT_FDCWD, valid_file, O_CREAT, 0, 0, READ_WRITE, &handle);
    ASSERT_EQ(__WASI_ESUCCESS, fd);
    ASSERT_GE(handle, 0);
}

TEST_F(FileTest, OpenInvalidFile) {
    int fd = os_openat(AT_FDCWD, "/nonexistent/path", 0, 0, 0, READ_ONLY, &handle);
    ASSERT_EQ(__WASI_ENOENT, fd);
}
```

### 4. **Proper Resource Management**
✅ **RAII Pattern:**
```cpp
class ResourceTest : public testing::Test {
protected:
    void SetUp() override {
        resource = acquire_resource();
    }
    
    void TearDown() override {
        if (resource_valid(resource)) {
            release_resource(resource);
        }
    }
    
    Resource resource;
};
```

### 5. **Handle Platform-Dependent Behavior Gracefully**
✅ **Conditional Testing:**
```cpp
TEST_F(NetworkTest, IPv6Socket) {
    int result = os_socket_create(&socket, false, true); // IPv6
    if (result == 0) {
        ASSERT_GT(socket, 0);
        // Test IPv6-specific functionality
        os_socket_close(socket);
    } else {
        GTEST_SKIP() << "IPv6 not available on this system";
    }
}
```

### 6. **Use Meaningful Test Data and Boundaries**
✅ **Boundary Testing:**
```cpp
TEST_F(BufferTest, ReadDifferentSizes) {
    // Test boundary conditions
    ASSERT_EQ(0, read_buffer(buffer, 0));        // Zero size
    ASSERT_GT(read_buffer(buffer, 1), 0);        // Minimum size
    ASSERT_GT(read_buffer(buffer, 4096), 0);     // Page size
    ASSERT_GT(read_buffer(buffer, 65536), 0);    // Large buffer
}
```

### 7. **Validate State Changes and Side Effects**
✅ **State Verification:**
```cpp
TEST_F(FileTest, WriteChangesFileSize) {
    // Initial state
    __wasi_filestat_t stat_before;
    ASSERT_EQ(__WASI_ESUCCESS, os_fstat(fd, &stat_before));
    
    // Perform operation
    const char* data = "test data";
    size_t written;
    ASSERT_EQ(__WASI_ESUCCESS, os_writev(fd, &iov, 1, &written));
    
    // Verify state change
    __wasi_filestat_t stat_after;
    ASSERT_EQ(__WASI_ESUCCESS, os_fstat(fd, &stat_after));
    ASSERT_EQ(stat_before.st_size + strlen(data), stat_after.st_size);
}
```

## Anti-Patterns to Avoid

### ❌ **Meaningless Success Tests**
```cpp
// Don't write tests that only verify execution without checking results
TEST_F(BadTest, FunctionRuns) {
    function_call();
    SUCCEED(); // Meaningless!
}
```

### ❌ **Tests Without Cleanup**
```cpp
// Don't leave resources dangling
TEST_F(BadTest, LeakyTest) {
    int fd = open_file();
    write_data(fd);
    // Missing: close(fd);
}
```

### ❌ **Overly Permissive Assertions**
```cpp
// Don't accept any result when you should ASSERT specific outcomes
ASSERT_TRUE(result == SUCCESS || result == FAILURE); // Too broad!
ASSERT_TRUE(result >= 0 || result < 0);              // ALWAYS TRUE - meaningless!
ASSERT_TRUE(result == 0 || result != 0);             // ALWAYS TRUE - meaningless!
```

### ❌ **Testing Implementation Details**
```cpp
// Don't test internal implementation, test public behavior
ASSERT_EQ(3, internal_counter); // Implementation detail
// Instead: ASSERT_EQ(ASSERTed_output, public_function());
```

## Test Structure Template

```cpp
class ModuleTest : public testing::Test {
protected:
    void SetUp() override {
        // Initialize test environment
        // Acquire resources
        // Set up test data
    }
    
    void TearDown() override {
        // Clean up resources
        // Reset state
        // Remove temporary files
    }
    
    // Test fixtures and helper data
    WAMRRuntimeRAII<512 * 1024> runtime;
    TestResource resource;
};

TEST_F(ModuleTest, FunctionName_Scenario_ASSERTedOutcome) {
    // Arrange: Set up test conditions
    TestData input = create_test_data();
    
    // Act: Execute the function under test
    Result actual = function_under_test(input);
    
    // Assert: Verify ASSERTed outcomes
    ASSERT_EQ(ASSERTed_result, actual);
    ASSERT_TRUE(verify_side_effects());
}
```

## Coverage Quality Metrics

### ✅ **High-Quality Test Indicators:**
- Each test verifies specific, measurable outcomes
- Error conditions are explicitly tested
- Resource cleanup is automatic and reliable
- Tests are independent and can run in any order
- Platform differences are handled gracefully
- Test names clearly describe the scenario and ASSERTation

### ❌ **Low-Quality Test Indicators:**
- Tests that always pass regardless of implementation
- Missing error path coverage
- Resource leaks or cleanup issues
- Tests that depend on external state
- Vague or generic test names
- Comments saying "this tests the code path" without verification

## Example: Before and After Refactoring

### Before (Low Quality):
```cpp
TEST_F(SocketTest, TestSocket) {
    os_socket_create(&socket, true, true);
    os_socket_bind(socket, "127.0.0.1", &port);
    os_socket_listen(socket, 5);
    SUCCEED() << "Socket operations completed";
}
```

### After (High Quality):
```cpp
TEST_F(SocketTest, TcpSocketBindAndListen_Success) {
    // Create TCP socket
    int result = os_socket_create(&socket, true, true);
    ASSERT_EQ(0, result);
    ASSERT_GT(socket, 0);
    
    // Bind to localhost with dynamic port
    int port = 0;
    result = os_socket_bind(socket, "127.0.0.1", &port);
    ASSERT_EQ(0, result);
    ASSERT_GT(port, 0);
    
    // Start listening
    result = os_socket_listen(socket, 5);
    ASSERT_EQ(0, result);
    
    // Verify socket is in listening state
    bh_sockaddr_t addr;
    result = os_socket_addr_local(socket, &addr);
    ASSERT_EQ(0, result);
    ASSERT_TRUE(addr.is_ipv4);
    ASSERT_EQ(port, addr.port);
}
```

This guide ensures every test provides meaningful validation of functionality rather than just code execution coverage.