# WAMR Unit Test Development Guide

## Overview

This guide provides best practices for developing unit tests for the WebAssembly Micro Runtime (WAMR), focusing on maintaining synchronization between C++ test files and WASM modules.

## Test Architecture

### Test Structure
```cpp
class FeatureTest : public testing::Test {
protected:
    void SetUp() override {
        RuntimeInitArgs init_args;
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_System_Allocator;
        
        ASSERT_TRUE(wasm_runtime_full_init(&init_args));
    }

    void TearDown() override {
        wasm_runtime_destroy();
    }

    WAMRRuntimeRAII<512 * 1024> runtime;
};

TEST_F(FeatureTest, Function_Scenario_ExpectedOutcome) {
    // Arrange: Set up test conditions
    // Act: Execute the function under test
    // Assert: Verify expected outcomes with ASSERT (not EXPECT)
}
```

### Naming Convention
- **Pattern**: `TEST_F(FeatureTest, Function_Scenario_ExpectedOutcome)`
- **Examples**:
  - `LinearMemoryGrowth_ToMaximumSize_SucceedsCorrectly`
  - `ModuleLoading_WithInvalidFormat_FailsGracefully`

## WASM Module Development

### Creating Test WASM Modules

1. **Create WAT file** in `wasm-apps/` directory
2. **Export all test functions** with descriptive names
3. **Compile with appropriate features**:
   ```bash
   wat2wasm test_module.wat --enable-threads --enable-memory64 -o test_module.wasm
   ```

### Example WAT Structure
```wat
(module
  (memory 1)
  
  ;; Test functions for specific operations
  (func $test_feature_operation (export "test_feature_operation") (result i32)
    ;; Implementation
    (i32.const 42)
  )
  
  ;; Always include boundary condition tests
  (func $test_boundary_condition (export "test_boundary_condition") (result i32)
    ;; Test edge cases
    (i32.const 0)
  )
)
```

## Synchronization Best Practices

### Function Name Consistency
- **Test functions** in C++ must match **exported functions** in WASM
- Use the synchronization tool to detect mismatches:
  ```bash
  python3 test_wasm_sync_tool.py
  ```

### Build-Time Validation
- Run validation before committing changes:
  ```bash
  ./validate_test_wasm_sync.sh
  ```

### Avoiding Common Issues

1. **Never use GTEST_SKIP()** - Use early return instead
2. **Always use ASSERT_*** - Not EXPECT_* for definitive validation
3. **Check function existence** before calling:
   ```cpp
   wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst, "test_function");
   if (func != nullptr) {
       ASSERT_TRUE(wasm_runtime_call_wasm(exec_env, func, 0, argv));
   }
   ```

## Test Categories

### 1. Positive Tests
- Normal operation scenarios
- Expected success paths
- Valid input combinations

### 2. Negative Tests
- Error handling scenarios
- Invalid inputs
- Resource exhaustion

### 3. Boundary Tests
- Edge cases
- Minimum/maximum values
- Memory limits

### 4. Integration Tests
- Cross-module interactions
- Runtime lifecycle
- Resource management

## Development Workflow

### Phase 1: Planning
1. Identify test scenarios
2. Design WASM module functions
3. Create WAT file with exports

### Phase 2: Implementation
1. Write C++ test cases
2. Compile WASM modules
3. Run synchronization validation

### Phase 3: Validation
1. Execute test suite
2. Fix synchronization issues
3. Verify coverage improvements

## Tools and Scripts

### Synchronization Tool
- **Purpose**: Detect function name mismatches
- **Usage**: `python3 test_wasm_sync_tool.py`
- **Output**: Report of missing functions

### Validation Script
- **Purpose**: Build-time validation
- **Usage**: `./validate_test_wasm_sync.sh`
- **Output**: GTEST_SKIP usage and function lookup analysis

### Build and Test
- **Complete workflow**: `./build_and_execute.sh`
- **Individual steps**:
  ```bash
  cmake -S . -B build -DCOLLECT_CODE_COVERAGE=1
  cmake --build build
  ctest --test-dir build
  ```

## Coverage-Driven Development

### Target Coverage Goals
- **Module Coverage**: >65% line coverage
- **Function Coverage**: >80% public API functions
- **Branch Coverage**: >70% conditional branches

### Coverage Analysis
- **Report location**: `wamr-lcov/index.html`
- **Focus areas**: Error paths, edge cases, platform variants

## Quality Standards

### Mandatory Requirements
- ✅ Eliminate all GTEST_SKIP() calls
- ✅ Use ASSERT_* for definitive validation
- ✅ Validate real WAMR functionality
- ✅ Maintain comprehensive test scenarios
- ✅ Handle platform differences gracefully

### Prohibited Practices
- ❌ Never use GTEST_SKIP() or SUCCEED()/FAIL() placeholders
- ❌ Don't create tests that don't validate actual functionality
- ❌ Avoid modifying committed source files (except CMakeLists.txt)

## Troubleshooting

### Common Issues

1. **"Subprocess aborted" errors**:
   - Check WASM module compilation
   - Verify function name synchronization
   - Ensure proper resource cleanup

2. **Function lookup failures**:
   - Run synchronization tool
   - Check WAT file exports
   - Verify compilation flags

3. **Memory allocation failures**:
   - Increase heap/stack sizes
   - Check resource limits
   - Verify cleanup in TearDown

### Debugging Tips
- Use `--rerun-failed --output-on-failure` with ctest
- Check `build/Testing/Temporary/LastTest.log`
- Validate WASM modules with `wasm-validate`

## Conclusion

Following this guide ensures:
- Consistent test quality
- Proper synchronization between tests and WASM modules
- Comprehensive coverage of WAMR features
- Reliable test execution across platforms

Remember: The goal is not just code coverage, but validating real WAMR functionality and behavior.