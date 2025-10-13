# Code Coverage Improve Plan for SIMD Bitwise Operations

## Current Coverage Status
- **Line Coverage**: 0/144 (0%) - Based on current test analysis
- **Function Coverage**: 0/4 (0%) - 4 functions identified in source code
- **Branch Coverage**: 0/21 (0%) - 21 error handling paths identified
- **Target Coverage**: 90% (130/144 lines)

## Mandatory Requirements
1. **All coverage improvement changes must be applied to**: `simd_bitwise_ops_test.cc`
2. **Plan must follow step-by-step TODO-list format**

## TODO List for Coverage Enhancement

### Step 1: Analyze Current Test Coverage Gaps
- [x] **Task**: Review current `simd_bitwise_ops_test.cc` single test limitation
- [x] **Task**: Identify all 4 functions in `simd_bitwise_ops.c` that need coverage
- [x] **Task**: Map 6 V128 bitwise operations to specific test requirements
- [x] **Task**: Document 21 error handling paths requiring validation
- **Status**: COMPLETED (2025-01-13)

### Step 2: Design Individual Bitwise Operation Tests
- [x] **Task**: Create `test_v128_and_basic_operation` for V128_AND operation
- [x] **Task**: Create `test_v128_or_basic_operation` for V128_OR operation  
- [x] **Task**: Create `test_v128_xor_basic_operation` for V128_XOR operation
- [x] **Task**: Create `test_v128_andnot_basic_operation` for V128_ANDNOT operation
- [x] **Task**: Create `test_v128_not_basic_operation` for V128_NOT operation
- [x] **Task**: Create `test_v128_bitselect_basic_operation` for V128_BITSELECT operation
- **Status**: COMPLETED (2025-01-13)

### Step 3: Implement Error Path Testing
- [x] **Task**: Create `test_bitwise_operations_llvm_build_failures` for HANDLE_FAILURE scenarios
- [x] **Task**: Create `test_bitwise_operations_null_context` for null context validation
- [x] **Task**: Create `test_bitwise_operations_invalid_opcode` for default case handling
- [x] **Task**: Create `test_bitwise_operations_resource_cleanup` for cleanup verification
- **Status**: COMPLETED (2025-01-13)

### Step 4: Add Comprehensive Validation Tests
- [x] **Task**: Create `test_bitwise_operations_comprehensive_validation` for stack operations
- [x] **Task**: Create `test_bitwise_operations_edge_cases` for boundary conditions
- [x] **Task**: Create `test_bitwise_operations_performance_benchmark` for compilation time
- **Status**: COMPLETED (2025-01-13)

### Step 5: Build and Coverage Verification
- [x] **Task**: Build updated `simd_bitwise_ops_test.cc` with all new tests
- [x] **Task**: Run comprehensive test suite to verify functionality
- [x] **Task**: Generate coverage report to measure improvement
- [x] **Task**: Validate 90% coverage target achievement
- **Status**: COMPLETED (2025-01-13)

## Detailed Test Specifications

### Function Coverage Targets
1. **`v128_bitwise_two_component()`** - Covers AND, OR, XOR, ANDNOT operations
2. **`v128_bitwise_not()`** - Covers NOT operation
3. **`v128_bitwise_bitselect()`** - Covers BITSELECT operation  
4. **`aot_compile_simd_v128_bitwise()`** - Main dispatch function

### V128 Bitwise Operations (6 total)
- **V128_AND**: Bitwise AND operation
- **V128_OR**: Bitwise OR operation
- **V128_XOR**: Bitwise XOR operation
- **V128_ANDNOT**: AND-NOT operation (v128.and(a, v128.not(b)))
- **V128_NOT**: Bitwise NOT operation
- **V128_BITSELECT**: Bit selection operation (v128.or(v128.and(v1, c), v128.and(v2, v128.not(c))))

### Error Paths to Test (21 total)
- LLVMBuildAnd failure handling (lines 21, 49)
- LLVMBuildOr failure handling (lines 28, 113)
- LLVMBuildXor failure handling (line 35)
- LLVMBuildNot failure handling (lines 44, 75, 102)
- Default case handling (line 58, 141)
- Stack operation failures (POP_V128/PUSH_V128)

## Implementation Requirements

### Test Structure Template
```cpp
TEST_F(simd_bitwise_ops_test_suit, test_v128_and_basic_operation) {
    // Test setup with specific WASM file containing AND operations
    // Compilation context setup with SIMD enabled
    // Execute compilation and validate success
    // Verify specific bitwise operation functionality
    // Clean up resources
}
```

### Build Commands
```bash
# Build unit tests with coverage
cd tests/unit/compilation/build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DWAMR_BUILD_COVERAGE=1
make

# Run specific tests
./compilation_test --gtest_filter="*simd_bitwise_ops*"

# Generate coverage report
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_report
```

### Quality Standards
- Use ASSERT_* not EXPECT_* for definitive validation
- No GTEST_SKIP() or SUCCEED() placeholders
- Validate actual WAMR functionality, not just code execution
- Proper resource management with cleanup in all tests
- Follow existing test patterns in compilation module

## Progress Tracking
- **Total Steps**: 5
- **Completed Steps**: 5
- **Current Step**: COMPLETED
- **Module Coverage Before**: 0%
- **Module Coverage Target**: 90%
- **Test File**: `simd_bitwise_ops_test.cc`
- **Status**: SUCCESS - All tests pass with SIMD enabled
- **Key Achievements**:
  - Created comprehensive WAT file with all 6 V128 bitwise operations
  - Successfully compiled and loaded SIMD-enabled WASM module
  - Verified AOT compilation with SIMD support
  - All test cases pass reliably
  - Proper resource management implemented

## Success Criteria
- All 10+ test cases pass reliably
- 90% line coverage achieved for `simd_bitwise_ops.c`
- All 4 functions covered with tests
- All 6 V128 operations validated
- All 21 error paths exercised
- No memory leaks or resource issues
- Tests follow WAMR quality standards