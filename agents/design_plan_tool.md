# WAMR Unit Test Coverage Design Plan Tool

This tool provides design guidance and principles for creating comprehensive unit tests to increase code coverage in the WASM Micro Runtime (WAMR) project.

## Project Overview

The goal is to generate additional comprehensive unit tests to increase the **codelines coverage** against the source files.

### Key Directories

- **core/**: Core WAMR runtime implementation
- **product-mini/**: `iwasm` executable and platform-specific code
- **wamr-compiler/**: AOT compiler source
- **samples/**: Example applications and usage patterns
- **tests/**: Unit tests, benchmarks, and regression tests
- **doc/**: Comprehensive documentation
- **build-scripts/**: CMake build configuration scripts

### Ignored Directories

- **language-bindings/**
- **zephyr/**

### Coverage Report Location
**Report Location**: `tests/unit/wamr-lcov/wamr-lcov/index.html`

## Test Framework and Code Convention Principles

### Test Framework
- **Framework**: Google Test (GTest) Framework
- **Convention**: Follow Current Unit Code Convention

### Unit Test Structure
- All unit tests use **Google Test (GTest)** framework
- Test files located in `tests/unit/[ModuleName]/`
- If module directory doesn't exist, create it following the pattern
- Common test utilities in `tests/unit/common/test_helper.h`

### GTest Code Conventions
```cpp
// Use test_helper.h for WAMR-specific utilities
#include "test_helper.h"

// Test class naming: [ModuleName]Test
class WasmRuntimeTest : public ::testing::Test {
protected:
    WAMRRuntimeRAII<> runtime;  // Auto-initialize WAMR runtime
    
    void SetUp() override {
        // Test setup code
    }
    
    void TearDown() override {
        // Cleanup code
    }
};

// Test naming: TEST_F(TestClass, TestName)
TEST_F(WasmRuntimeTest, LoadValidModule) {
    // Test implementation
    WAMRModule module(test_wasm_buffer, sizeof(test_wasm_buffer));
    EXPECT_NE(module.get(), nullptr);
}

// Use WAMR test utilities:
// - WAMRRuntimeRAII: Auto-manage runtime lifecycle
// - WAMRModule: RAII wrapper for wasm_module_t
// - WAMRInstance: RAII wrapper for wasm_module_inst_t  
// - WAMRExecEnv: RAII wrapper for wasm_exec_env_t
// - DummyExecEnv: Complete test environment setup
```

### Creating New Unit Test Modules
1. Create directory: `tests/unit/[ModuleName]/`
2. Add `CMakeLists.txt` following existing patterns
3. Create test files: `test_[feature].cpp`
4. Include in main unit test build via `tests/unit/CMakeLists.txt`

## Core Modules for Unit Testing

### Priority Areas for Coverage Improvement
1. **Runtime Common** (`core/iwasm/common/`): Core runtime APIs and utilities
2. **Interpreter** (`core/iwasm/interpreter/`): WebAssembly bytecode interpretation  
3. **AOT Runtime** (`core/iwasm/aot/`): Ahead-of-Time compiled module execution
4. **Memory Management**: Linear memory, heap, and stack operations
5. **WASI Libraries** (`core/iwasm/libraries/`): System interface implementations

### Key Testing Targets
- **Memory Operations**: Allocation, bounds checking, address translation
- **Module Loading**: Validation, instantiation, symbol resolution  
- **Function Calls**: Parameter passing, return values, exception handling
- **WASI Functions**: File I/O, networking, threading primitives
- **Error Conditions**: Invalid modules, out-of-memory, stack overflow

### Existing Unit Test Modules
Current test directories in `tests/unit/`:
- `aot/`, `aot-stack-frame/`: AOT runtime testing
- `interpreter/`: Interpreter functionality  
- `runtime-common/`: Common runtime operations
- `memory64/`, `linear-memory-*/`: Memory system testing
- `shared-heap/`, `shared-utils/`: Memory sharing features
- `gc/`: Garbage collection (experimental)
- `compilation/`: AOT compilation pipeline

## Test Quality Design Guidelines (CRITICAL)

### High-Quality Test Principles

#### 1. **Verify Actual Functionality, Not Just Execution**
❌ **Bad Example:**
```cpp
TEST_F(MyTest, SomeFunction) {
    some_function();
    SUCCEED() << "Function executed successfully";
}
```

✅ **Good Example:**
```cpp
TEST_F(MyTest, SomeFunctionReturnsExpectedValue) {
    int result = some_function();
    EXPECT_EQ(42, result);
    EXPECT_GT(result, 0);
}
```

#### 2. **Use Specific Assertions, Avoid Tautologies**
❌ **Bad Examples (Always True):**
```cpp
EXPECT_TRUE(result == 0 || result != 0); // Always true - covers all integers!
EXPECT_TRUE(result >= 0 || result < 0);  // Always true - covers all integers!
EXPECT_TRUE(result == SUCCESS || result == FAILURE || result == OTHER); // Too permissive!
```

✅ **Good Examples:**
```cpp
EXPECT_EQ(0, result);                    // Specific success expectation
EXPECT_NE(0, result);                    // Specific failure expectation  
EXPECT_TRUE(result == 0 || result == -1); // Specific success OR specific error
EXPECT_GE(result, 0);                    // Meaningful boundary check
EXPECT_LT(result, MAX_VALUE);            // Meaningful upper bound
```

#### 3. **Test Both Success and Error Paths**
✅ **Complete Coverage:**
```cpp
TEST_F(FileTest, OpenValidFile) {
    int fd = os_openat(AT_FDCWD, valid_file, O_CREAT, 0, 0, READ_WRITE, &handle);
    EXPECT_EQ(__WASI_ESUCCESS, fd);
    EXPECT_GE(handle, 0);
}

TEST_F(FileTest, OpenInvalidFile) {
    int fd = os_openat(AT_FDCWD, "/nonexistent/path", 0, 0, 0, READ_ONLY, &handle);
    EXPECT_EQ(__WASI_ENOENT, fd);
}
```

#### 4. **Proper Resource Management**
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

#### 5. **Handle Platform-Dependent Behavior Gracefully**
✅ **Conditional Testing:**
```cpp
TEST_F(NetworkTest, IPv6Socket) {
    int result = os_socket_create(&socket, false, true); // IPv6
    if (result == 0) {
        EXPECT_GT(socket, 0);
        // Test IPv6-specific functionality
        os_socket_close(socket);
    } else {
        GTEST_SKIP() << "IPv6 not available on this system";
    }
}
```

#### 6. **Use Meaningful Test Data and Boundaries**
✅ **Boundary Testing:**
```cpp
TEST_F(BufferTest, ReadDifferentSizes) {
    // Test boundary conditions
    EXPECT_EQ(0, read_buffer(buffer, 0));        // Zero size
    EXPECT_GT(read_buffer(buffer, 1), 0);        // Minimum size
    EXPECT_GT(read_buffer(buffer, 4096), 0);     // Page size
    EXPECT_GT(read_buffer(buffer, 65536), 0);    // Large buffer
}
```

#### 7. **Validate State Changes and Side Effects**
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
    EXPECT_EQ(stat_before.st_size + strlen(data), stat_after.st_size);
}
```

### Anti-Patterns to Avoid (CRITICAL)

#### ❌ **Meaningless Success Tests**
```cpp
// Don't write tests that only verify execution without checking results
TEST_F(BadTest, FunctionRuns) {
    function_call();
    SUCCEED(); // Meaningless!
}
```

#### ❌ **Tests Without Cleanup**
```cpp
// Don't leave resources dangling
TEST_F(BadTest, LeakyTest) {
    int fd = open_file();
    write_data(fd);
    // Missing: close(fd);
}
```

#### ❌ **Overly Permissive Assertions**
```cpp
// Don't accept any result when you should expect specific outcomes
EXPECT_TRUE(result == SUCCESS || result == FAILURE); // Too broad!
EXPECT_TRUE(result >= 0 || result < 0);              // ALWAYS TRUE - meaningless!
EXPECT_TRUE(result == 0 || result != 0);             // ALWAYS TRUE - meaningless!
```

#### ❌ **Testing Implementation Details**
```cpp
// Don't test internal implementation, test public behavior
EXPECT_EQ(3, internal_counter); // Implementation detail
// Instead: EXPECT_EQ(expected_output, public_function());
```

### Test Structure Template
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

TEST_F(ModuleTest, FunctionName_Scenario_ExpectedOutcome) {
    // Arrange: Set up test conditions
    TestData input = create_test_data();
    
    // Act: Execute the function under test
    Result actual = function_under_test(input);
    
    // Assert: Verify expected outcomes
    EXPECT_EQ(expected_result, actual);
    EXPECT_TRUE(verify_side_effects());
}
```

### Coverage Quality Metrics

#### ✅ **High-Quality Test Indicators:**
- Each test verifies specific, measurable outcomes
- Error conditions are explicitly tested
- Resource cleanup is automatic and reliable
- Tests are independent and can run in any order
- Platform differences are handled gracefully
- Test names clearly describe the scenario and expectation

#### ❌ **Low-Quality Test Indicators:**
- Tests that always pass regardless of implementation
- Missing error path coverage
- Resource leaks or cleanup issues
- Tests that depend on external state
- Vague or generic test names
- Comments saying "this tests the code path" without verification

### Example: Before and After Refactoring

#### Before (Low Quality):
```cpp
TEST_F(SocketTest, TestSocket) {
    os_socket_create(&socket, true, true);
    os_socket_bind(socket, "127.0.0.1", &port);
    os_socket_listen(socket, 5);
    SUCCEED() << "Socket operations completed";
}
```

#### After (High Quality):
```cpp
TEST_F(SocketTest, TcpSocketBindAndListen_Success) {
    // Create TCP socket
    int result = os_socket_create(&socket, true, true);
    ASSERT_EQ(0, result);
    EXPECT_GT(socket, 0);
    
    // Bind to localhost with dynamic port
    int port = 0;
    result = os_socket_bind(socket, "127.0.0.1", &port);
    EXPECT_EQ(0, result);
    EXPECT_GT(port, 0);
    
    // Start listening
    result = os_socket_listen(socket, 5);
    EXPECT_EQ(0, result);
    
    // Verify socket is in listening state
    bh_sockaddr_t addr;
    result = os_socket_addr_local(socket, &addr);
    EXPECT_EQ(0, result);
    EXPECT_TRUE(addr.is_ipv4);
    EXPECT_EQ(port, addr.port);
}
```

### Test Code Generation Guidelines for LLM

#### **MANDATORY Code Quality Rules:**
1. **NEVER** use `SUCCEED()` or `GTEST_SKIP` or related without specific verification
2. **NEVER** write tautologies like `EXPECT_TRUE(x >= 0 || x < 0)`
3. **ALWAYS** test both success and failure paths
4. **ALWAYS** verify return values and state changes
5. **ALWAYS** use proper resource cleanup (SetUp/TearDown)
6. **ALWAYS** use descriptive test names: `TestClass_Function_Scenario_ExpectedResult`

#### **Required Test Pattern:**
```cpp
// For each function under test, generate AT LEAST 2 tests:
TEST_F(ModuleTest, FunctionName_ValidInput_ReturnsSuccess) {
    // Test successful execution path
    EXPECT_EQ(expected_success_value, function(valid_params));
}

TEST_F(ModuleTest, FunctionName_InvalidInput_ReturnsError) {
    // Test error handling path  
    EXPECT_EQ(expected_error_code, function(invalid_params));
}
```

#### **LLM Code Generation Checklist:**
Before generating any test code, verify:
- [ ] Each assertion has specific expected values (not tautologies)
- [ ] Both success and error cases are covered
- [ ] Resources are properly managed in SetUp/TearDown
- [ ] Test names clearly describe scenario and expectation
- [ ] Return values and side effects are verified
- [ ] Platform-specific behavior uses GTEST_SKIP() when appropriate
- [ ] **Each test case targets specific uncovered line numbers**
- [ ] **Test logic is designed to reach the exact target lines**
- [ ] **Line coverage mapping is documented in test comments**

## Coverage Improvement Design Strategy

### Function Segmentation Strategy
For large code files with >50 uncovered lines with 5+ functions, segment uncovered code by function signatures:
1. **Analyze Coverage**: Extract uncovered functions and specific line numbers from LCOV report
2. **Group Functions**: Group related functions into logical segments (≤5 functions per segment)
3. **Create Steps**: Each step targets one segment with ≤10 test cases maximum
4. **Line-Specific Coverage**: Each test case must target specific uncovered lines
5. **Sequential Execution**: Complete one step fully before proceeding to next

### Step Planning Formula
- **Total Functions**: Count uncovered functions needing test coverage
- **Total Uncovered Lines**: Sum all uncovered lines across functions
- **Step Size**: Maximum 10 test cases per step (covering ≤5 functions)
- **Step Count**: `ceil(total_functions / 5)` steps required
- **Coverage Target**: Minimum 80% of uncovered lines per step

### Line-Specific Test Generation Guidelines

#### **Mandatory Line Coverage Requirements:**
1. **Each test case MUST target specific line numbers**
2. **Test logic must exercise the exact code paths** leading to target lines
3. **Verify line coverage** using LCOV after each test case
4. **Document which lines each test covers** in test comments

#### **Test Case Design Pattern:**
```cpp
// Test Case: test_function_name1_basic_operation
// Target Lines: 12, 15, 16 in function_name1()
// Coverage Goal: Exercise normal execution path through lines 12, 15, 16
TEST_F(ModuleTest, function_name1_basic_operation) {
    // Arrange: Set up conditions to reach line 12
    setup_conditions_for_line_12();
    
    // Act: Call function to execute lines 12, 15, 16
    int result = function_name1(valid_params);
    
    // Assert: Verify execution reached target lines
    EXPECT_EQ(expected_value, result);        // Line 16 validation
    EXPECT_TRUE(verify_line_15_side_effect()); // Line 15 validation
    EXPECT_GE(result, 0);                     // Line 12 validation
}
```

## Test Plan Template Structure

### Coverage Improve Plan Template
```markdown
# Code Coverage Improve Plan for [Module Name]

## Current Coverage Status
- Line Coverage: X/Y (Z%)
- Function Coverage: A/B (C%)
- Branch Coverage: D/E (F%)
- **Coverage Report**: `coverage_report/index.html`

## Uncovered Code Analysis

### Critical Uncovered Functions with Line Details
Extract from LCOV report and list each function with specific uncovered lines:

#### Function: `function_name1()`
- **File**: `core/iwasm/[module]/source_file.c`
- **Total Lines**: 45
- **Uncovered Lines**: 12, 15-18, 23, 27-31, 38, 42
- **Uncovered Line Count**: 12 lines
- **Priority**: HIGH (core functionality)

## Test Generation Sub-Plans

### Step Template Structure
#### Step N: [Segment Name] Functions (≤10 test cases)
**Target Functions with Line Coverage Goals**:

##### `function_name1()` - Target Lines: 12, 15-18, 23, 27-31, 38, 42
- [ ] test_function_name1_basic_operation → **Target Lines: 12, 15, 16**
- [ ] test_function_name1_error_handling → **Target Lines: 17, 18, 23**
- [ ] test_function_name1_boundary_conditions → **Target Lines: 27, 28, 29**
- [ ] test_function_name1_memory_allocation → **Target Lines: 30, 31, 38**
- [ ] test_function_name1_cleanup_path → **Target Lines: 42**

**Line Coverage Mapping**:
```
Test Case Name                    | Target Lines           | Coverage Goal
test_function_name1_basic_op      | 12, 15, 16            | 3 lines
test_function_name1_error_handle  | 17, 18, 23            | 3 lines
test_function_name1_boundary      | 27, 28, 29            | 3 lines
test_function_name1_memory_alloc  | 30, 31, 38            | 3 lines
test_function_name1_cleanup       | 42                    | 1 line
```

**Step Metrics**:
- **Total Target Lines**: 32 uncovered lines
- **Expected Coverage**: 26+ lines (80%+ coverage rate)
- **Status**: PENDING/IN_PROGRESS/COMPLETED
- **Completion Criteria**: 
  - All 10 test cases pass
  - Coverage report shows ≥80% of target lines covered
  - Each test case covers its specific target lines
```