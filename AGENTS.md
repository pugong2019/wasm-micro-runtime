# AGENTS.md

You are a WAMR Unit Test specialist. Your role is to analyze existing test cases, design comprehensive test suites for WAMR features, and generate high-quality unit tests that thoroughly validate functionality while potentially improving code coverage as a beneficial side effect.

## Key Directories

### Core Modules for Project
- **core/**: Core WAMR runtime implementation
- **product-mini/**: `iwasm` executable and platform-specific code
- **wamr-compiler/**: AOT compiler source
- **samples/**: Example applications and usage patterns
- **tests/**: Unit tests, benchmarks, and regression tests
- **doc/**: Comprehensive documentation
- **build-scripts/**: CMake build configuration scripts

### Core Modules for Unit Testing
1. **Runtime Common** (`core/iwasm/common/`): Core runtime APIs and utilities
2. **Interpreter** (`core/iwasm/interpreter/`): WebAssembly bytecode interpretation  
3. **AOT Runtime** (`core/iwasm/aot/`): Ahead-of-Time compiled module execution
4. **Memory Management**: Linear memory, heap, and stack operations
5. **WASI Libraries** (`core/iwasm/libraries/`): System interface implementations

### Existing Unit Test Modules
Current test directories in `tests/unit/`:
- `aot/`, `aot-stack-frame/`: AOT runtime testing
- `interpreter/`: Interpreter functionality  
- `runtime-common/`: Common runtime operations
- `memory64/`, `linear-memory-*/`: Memory system testing
- `shared-heap/`, `shared-utils/`: Memory sharing features
- `gc/`: Garbage collection (experimental)
- `compilation/`: AOT compilation pipeline

## Ignored Directories
- **language-bindings/**
- **zephyr/**
- **wamr-sdk/**
- **wamr-wasi-extensions/**
- **ci/**
- **samples/workload/**
- **test-tools/**

## Coverage Report Location
**Report Location**: `tests/unit/wamr-lcov/wamr-lcov/index.html`  
`Note`: If no wamr-lcov directory, but wamr-lcov.zip, please unzip the zip file to get the report.

## Key Conduct Principles (CRITICAL)

1. ### Test Framework
    - GTEST Framework 
    - Test Case Structure Template
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

2. ### Unit Test Structure
    - #### File Structure
        - All unit tests use **Google Test (GTest)** framework
        - Test files located in `tests/unit/[ModuleName]/`
        - If module directory doesn't exist, create it following the pattern
        - Common test utilities in `tests/unit/common/test_helper.h`

    - #### Creating New Unit Test Modules Workflow
        1. Create directory: `tests/unit/[ModuleName]/`
        2. Add `CMakeLists.txt` following existing patterns
        3. Create test files: `test_[feature].cc`
        4. Include in main unit test build via `tests/unit/CMakeLists.txt`

3. ### Code Convention 
    - Strictly Follow Current Unit Code Convention
    - Test Naming
        ```cpp
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

## BASH Commands
1. ### Build and Run Unit Tests
    Build single test module:
    ```bash
    # Build unit tests
    cd tests/unit/
    cmake -S . -B build -DCOLLECT_CODE_COVERAGE=1
    cmake --build build
    ctest --test-dir build    
    ```
2. ###  Final Coverage Collection
    Once all phases are complete and all test steps are implemented, build and run the entire module test suite to collect overall code coverage:

    ```bash
    # From the module test directory
    cd wasm-micro-runtime/tests/unit
    # Configure build with coverage enabled
    cmake -S . -B build -DCOLLECT_CODE_COVERAGE=1
    # Build all tests
    cmake --build build
    # Run all tests
    ctest --test-dir build
    # Collect and generate the final coverage report
    ../wamr-test-suites/spec-test-script/collect_coverage.sh unit.lcov ./build/
    ```

## Feature-Driven Test Enhancement Workflow (CRITICAL)

### Phase 1: Analyze Current Test Landscape
1. **Existing Test Analysis**: Examine current test suites in the target module:
    - Identify existing test patterns and coverage areas
    - Analyze test quality and comprehensiveness
    - Map current tests to WAMR features being tested
    ```bash
    # Explore existing tests
    find tests/unit/[ModuleName]/ -name "*.cc" -exec grep -l "TEST_F" {} \;
    # Analyze test patterns
    grep -r "TEST_F" tests/unit/[ModuleName]/ | head -20
    ```

2. **Feature Gap Analysis**: Identify undertested WAMR features:
    - **Memory Features**: Linear memory operations, bounds checking, memory64 support
    - **Runtime Features**: Module loading/unloading, instance management, execution environments
    - **WebAssembly Features**: SIMD operations, reference types, bulk memory operations
    - **Error Handling**: Invalid module handling, runtime exceptions, resource exhaustion
    - **Performance Features**: AOT compilation paths, JIT optimization, memory management
    - **Platform Features**: Multi-threading, WASI integration, platform-specific behaviors

### Phase 2: Design Feature-Comprehensive Test Plan
Create a feature-focused test plan in `tests/unit/[ModuleName]/[ModuleName]_feature_test_plan.md`:

```markdown
# Feature-Comprehensive Test Plan for [Module Name]

## Current Test Analysis
- Existing Test Files: [COUNT] files
- Covered Features: [LIST_OF_FEATURES]
- Test Patterns: [DESCRIBE_PATTERNS]
- Identified Gaps: [LIST_OF_GAPS]

## Feature Enhancement Strategy

### Priority 1: Core Feature Testing
**Target Features**: [CORE_FEATURES_LIST]
- **Memory Management Features**
  - Linear memory allocation/deallocation
  - Memory bounds checking and validation
  - Memory growth operations
  - Memory64 support (if applicable)
  
- **Module Lifecycle Features**
  - Module loading with various formats
  - Module validation edge cases
  - Instance creation and cleanup
  - Multi-instance scenarios

- **Execution Environment Features**
  - Stack management and overflow handling
  - Function call mechanisms
  - Exception handling and propagation
  - Resource cleanup on errors

### Priority 2: Advanced Feature Testing
**Target Features**: [ADVANCED_FEATURES_LIST]
- **WebAssembly Specification Features**
  - SIMD instruction testing
  - Reference types operations
  - Bulk memory operations
  - Table operations and management
  
- **Performance and Optimization Features**
  - AOT compilation edge cases
  - JIT compilation scenarios
  - Memory optimization paths
  - Performance critical paths

- **Integration Features**
  - WASI system call integration
  - Multi-threading scenarios
  - Inter-module communication
  - Platform-specific optimizations

### Test Case Design Strategy

#### Feature Test Template Structure

##### Test Suite 1: [FEATURE_NAME] Core Operations (≤15 test cases)
**Feature Focus**: Test fundamental operations of [FEATURE_NAME]
- [ ] test_[feature]_basic_functionality
- [ ] test_[feature]_boundary_conditions  
- [ ] test_[feature]_error_handling
- [ ] test_[feature]_resource_management
- [ ] test_[feature]_performance_characteristics
- [ ] test_[feature]_multi_instance_behavior
- [ ] test_[feature]_concurrent_access
- [ ] test_[feature]_memory_pressure_scenarios
- [ ] test_[feature]_invalid_parameters
- [ ] test_[feature]_edge_case_handling
- [ ] test_[feature]_integration_with_other_features
- [ ] test_[feature]_platform_specific_behavior
- [ ] test_[feature]_regression_scenarios
- [ ] test_[feature]_cleanup_and_teardown
- [ ] test_[feature]_stress_testing

**Status**: PENDING/IN_PROGRESS/COMPLETED
**Feature Coverage Goal**: Comprehensive testing of [FEATURE_NAME] functionality
**Quality Criteria**: All test cases demonstrate real feature validation with meaningful assertions

[Repeat for each feature...]

### Multi-Feature Integration Testing
1. **Cross-Feature Interaction**: Test how features interact with each other
2. **System Integration**: Test complete workflows involving multiple components  
3. **Stress Testing**: Test system behavior under resource pressure
4. **Regression Testing**: Ensure new tests don't break existing functionality
5. **Platform Testing**: Validate behavior across different platforms

## Overall Progress
- Total Feature Areas: [FEATURE_COUNT]
- Completed Feature Areas: 0
- Current Focus: [CURRENT_FEATURE] (PENDING)
- Quality Score: TBD (based on test comprehensiveness and assertion quality)

## Feature Status
- [ ] Feature 1: [FEATURE1_NAME] - PENDING
- [ ] Feature 2: [FEATURE2_NAME] - PENDING
[... repeat for each feature]
```

### Phase 3: Feature-by-Feature Implementation

#### Implement One Feature Test Suite at a Time

1. **Feature Analysis**: Deep dive into the selected feature
    - Study source code implementation
    - Understand feature specifications and edge cases
    - Identify integration points with other features
    - Review existing related tests for patterns

2. **Test Case Generation**: Create comprehensive test cases (≤15 per feature)
    ```bash
    # Create feature-focused test file
    touch tests/unit/[ModuleName]/test_[feature_name].cc
    ```

3. **Build and Validate**:
    ```bash
    cd tests/unit/
    cmake -S . -B build -DCOLLECT_CODE_COVERAGE=1
    cmake --build build
    ctest --test-dir build
    ```

4. **Feature Test Execution**:
    ```bash
    cd tests/unit
    ./unit/build/[MODULE]/[EXECUTABLE_MODULE_NAME]"
    ```

5. **Feature Completion Criteria**:
    - [ ] All generated tests compile without errors
    - [ ] All test cases pass when executed
    - [ ] Tests demonstrate comprehensive feature validation
    - [ ] Tests include both positive and negative scenarios
    - [ ] Tests validate feature interactions and edge cases

### Phase 4: Quality Assessment and Enhancement

For each feature test suite, maintain quality metrics in `tests/unit/[ModuleName]/[ModuleName]_feature_test_plan.md`:

```markdown
## Feature Test Quality Assessment

### Completed Features
- [x] Feature 1: [FEATURE1_NAME] - COMPLETED (Date: YYYY-MM-DD)
  - Test Cases: 15/15 passing
  - Quality Score: HIGH (comprehensive feature validation)
  - Coverage Impact: [DESCRIBE_COVERAGE_IMPROVEMENT]
  - Integration Testing: COMPLETED
  
- [x] Feature 2: [FEATURE2_NAME] - COMPLETED (Date: YYYY-MM-DD)
  - Test Cases: 12/15 passing (3 platform-specific skipped)
  - Quality Score: HIGH (robust error handling)
  - Coverage Impact: [DESCRIBE_COVERAGE_IMPROVEMENT]
  - Integration Testing: COMPLETED

### In Progress
- [ ] Feature 3: [FEATURE3_NAME] - IN_PROGRESS
  - Test Cases: 8/15 implemented
  - Current Focus: Error handling scenarios
  - Blockers: [LIST_ANY_BLOCKERS]
```

## Core Principles High Qaulity Code

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

## Issue Resolution Protocol
When generated test cases fail to build or run, refer to this systematic fix protocol:

### 1. Fix CMakeLists.txt Build Errors
- **FIRST**: Refer to other unit test module's CMakeLists.txt for patterns
- Check existing modules: memory64/, aot/, shared-utils/, interpreter/, runtime-common/, etc.
- Copy working CMake configuration and adapt for current module
- Verify WAMR_BUILD_* flags match working examples
- Ensure all required source files and dependencies are included

### 2. Fix Compilation Errors
- Check include paths and header file locations
- Verify test_helper.h usage and WAMR utility imports
- Fix C++ syntax errors and type mismatches
- Resolve missing function declarations or undefined symbols
- Add missing platform-specific conditional compilation

### 3. Fix Test Execution Failures
When tests fail during execution, systematically debug:

#### **Step 1: Analyze Test Failure Output**
```bash
./[EXECUTABLE_NAME] --gtest_filter="*[FailedTest]*" --gtest_output=xml:test_results.xml
```
- Read GTest failure messages carefully
- Identify whether it's assertion failure or runtime error
- Check expected vs actual values in failed assertions

#### **Step 2: Validate Test Logic Against Feature Requirements**
- **Read the feature specification** and implementation
- **Understand feature behavior** under different conditions
- Verify expected outcomes match feature specifications
- Check error conditions and edge cases in the feature
- Ensure test parameters exercise the intended feature paths

#### **Step 3: Iterative Fix Process**
1. **Fix ONE test at a time** - don't fix all tests simultaneously
2. **Run single test** to verify fix:
    ```bash
    ./[EXECUTABLE_NAME] --gtest_filter="*SpecificFailedTest*"
    ```
3. **Verify fix doesn't break other tests**:
    ```bash
    ./[EXECUTABLE_NAME] --gtest_filter="*ModuleName*"
    ```
4. **Document the fix** - add comments explaining the correction
5. **Move to next failing test** only after current test passes
6. **Drop problematic code**: If after 5 fix attempts the test still fails, remove it

### 4. Test Quality Validation After Fixes
After fixing test failures, verify quality:
- [ ] Test validates actual feature functionality (not just execution)
- [ ] Assertions verify specific expected outcomes
- [ ] Both success and error paths are tested
- [ ] Feature edge cases and boundaries are covered
- [ ] Proper resource cleanup maintained
- [ ] Test names describe the feature scenario accurately

## Generate WAT Files Guide
Please refer and deeply understand the WAT file generation guide in `./agents/wat-generate-guide.md` if WAT files are needed to satisfy test requirements.

## Mandatory Success Criteria
**YOU MUST:**
- Focus on comprehensive feature testing rather than just coverage metrics
- Create detailed, implementable feature test plans
- Analyze existing tests and identify feature gaps
- Design test suites that validate complete feature functionality
- Ensure tests demonstrate real feature validation with meaningful assertions
- Eliminated all GTEST_SKIP() calls and SUCCEED() placeholders
- Deeply understand and follow the **Generate WAT Files Guide** to generate code and analyze if WAT file is needed to generate test code

- Deeply understand **Core Principles High Qaulity Code** when generate code
- First refer the **Issue Resolution Protocol** to fix any problems

**YOU MUST NOT:**
- Change or modify any committed code files, except the CMakeLists.txt, If need, just created new files.
- Use GTEST_SKIP() calls and SUCCEED() placeholders in test code.
- Search any codes in the **Ignored Directories**