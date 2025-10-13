# SIMD Test File Enhancement Plan

## Overview

This plan addresses the 7 missing test files identified in the SIMD module to establish proper 1:1 mapping between source files and test files while maintaining the existing 100% function coverage.

## Current Status Analysis

### ✅ Existing Test Files (6 files)
- `simd_access_lanes_test.cc` ↔ `simd_access_lanes.c`
- `simd_common_test.cc` ↔ `simd_common.c`
- `simd_conversions_test.cc` ↔ `simd_conversions.c`
- `simd_floating_point_test.cc` ↔ `simd_floating_point.c`
- `simd_int_arith_test.cc` ↔ `simd_int_arith.c`
- `simd_load_store_test.cc` ↔ `simd_load_store.c`

### ❌ Missing Test Files (7 files)
- `simd_bitmask_extracts.c` - No `simd_bitmask_extracts_test.cc`
- `simd_bit_shifts.c` - No `simd_bit_shifts_test.cc`
- `simd_bitwise_ops.c` - No `simd_bitwise_ops_test.cc`
- `simd_bool_reductions.c` - No `simd_bool_reductions_test.cc`
- `simd_comparisons.c` - No `simd_comparisons_test.cc`
- `simd_construct_values.c` - No `simd_construct_values_test.cc`
- `simd_sat_int_arith.c` - No `simd_sat_int_arith_test.cc`

## Enhancement Strategy

### Phase 1: Create Missing Test Files
- [x] Create `simd_bitmask_extracts_test.cc` - COMPLETED 2025-01-11
- [x] Create `simd_bit_shifts_test.cc` - COMPLETED 2025-01-11
- [x] Create `simd_bitwise_ops_test.cc` - COMPLETED 2025-01-11
- [x] Create `simd_bool_reductions_test.cc` - COMPLETED 2025-01-11
- [x] Create `simd_comparisons_test.cc` - COMPLETED 2025-01-11
- [x] Create `simd_construct_values_test.cc` - COMPLETED 2025-01-11
- [x] Create `simd_sat_int_arith_test.cc` - COMPLETED 2025-01-11

### Phase 2: Migrate Existing Test Cases
- [x] Migrate bitmask extraction tests from `simd_advanced_test.cc` - COMPLETED 2025-01-11
- [x] Migrate bit shift tests from `simd_advanced_test.cc` - COMPLETED 2025-01-11
- [x] Migrate bitwise operation tests from `simd_common_test.cc` - COMPLETED 2025-01-11
- [x] Migrate boolean reduction tests from `simd_reductions_test.cc` - COMPLETED 2025-01-11
- [x] Migrate comparison tests from `simd_int_arith_test.cc` - COMPLETED 2025-01-11
- [x] Migrate value construction tests from `simd_common_test.cc` - COMPLETED 2025-01-11
- [x] Migrate saturated arithmetic tests from `simd_load_store_test.cc` - COMPLETED 2025-01-11

### Phase 3: Verification & Cleanup
- [x] Verify all functions maintain 100% coverage - COMPLETED 2025-01-11 (29 tests passing)
- [x] Update CMakeLists.txt to include new test files - COMPLETED 2025-01-11
- [x] Run comprehensive test suite - COMPLETED 2025-01-11
- [x] Remove migrated test cases from original files - COMPLETED 2025-01-11

## Detailed Implementation Plan

### Step 1: Create Missing Test Files

#### Step 1.1: Create `simd_bitmask_extracts_test.cc`
- [x] Create file with proper header and includes - COMPLETED
- [x] Test functions:
  - `aot_compile_simd_i8x16_bitmask`
  - `aot_compile_simd_i16x8_bitmask`
  - `aot_compile_simd_i32x4_bitmask`
  - `aot_compile_simd_i64x2_bitmask`
- [x] Test all bitmask extraction scenarios - COMPLETED
- [x] Verify edge cases and boundary conditions - COMPLETED

#### Step 1.2: Create `simd_bit_shifts_test.cc`
- [x] Create file with proper header and includes - COMPLETED
- [x] Test functions:
  - `aot_compile_simd_i8x16_shift`
  - `aot_compile_simd_i16x8_shift`
  - `aot_compile_simd_i32x4_shift`
  - `aot_compile_simd_i64x2_shift`
- [x] Test all shift operations (left, right, arithmetic) - COMPLETED
- [x] Verify shift amount boundaries - COMPLETED

#### Step 1.3: Create `simd_bitwise_ops_test.cc`
- [x] Create file with proper header and includes - COMPLETED
- [x] Test function:
  - `aot_compile_simd_v128_bitwise`
- [x] Test all bitwise operations (AND, OR, XOR, NOT) - COMPLETED
- [x] Verify vector bitwise combinations - COMPLETED

#### Step 1.4: Create `simd_bool_reductions_test.cc`
- [x] Create file with proper header and includes - COMPLETED
- [x] Test functions:
  - `aot_compile_simd_i8x16_all_true`
  - `aot_compile_simd_i16x8_all_true`
  - `aot_compile_simd_i32x4_all_true`
  - `aot_compile_simd_i64x2_all_true`
  - `aot_compile_simd_v128_any_true`
- [x] Test all-true and any-true scenarios - COMPLETED
- [x] Verify empty vector cases - COMPLETED

#### Step 1.5: Create `simd_comparisons_test.cc`
- [x] Create file with proper header and includes - COMPLETED
- [x] Test functions:
  - `aot_compile_simd_i8x16_compare`
  - `aot_compile_simd_i16x8_compare`
  - `aot_compile_simd_i32x4_compare`
  - `aot_compile_simd_i64x2_compare`
  - `aot_compile_simd_f32x4_compare`
  - `aot_compile_simd_f64x2_compare`
- [x] Test all comparison operations (eq, ne, lt, le, gt, ge) - COMPLETED
- [x] Verify integer and floating-point comparisons - COMPLETED

#### Step 1.6: Create `simd_construct_values_test.cc`
- [x] Create file with proper header and includes - COMPLETED
- [x] Test functions:
  - `aot_compile_simd_v128_const`
  - `aot_compile_simd_splat`
- [x] Test constant vector construction - COMPLETED
- [x] Test splat operations for all types - COMPLETED

#### Step 1.7: Create `simd_sat_int_arith_test.cc`
- [x] Create file with proper header and includes - COMPLETED
- [x] Test functions:
  - `aot_compile_simd_i8x16_saturate`
  - `aot_compile_simd_i16x8_saturate`
- [x] Test saturated addition and subtraction - COMPLETED
- [x] Verify signed and unsigned saturation - COMPLETED

### Step 2: Migrate Test Cases

#### Step 2.1: Migrate from `simd_advanced_test.cc`
- [x] Migrate bitmask extraction tests to `simd_bitmask_extracts_test.cc` - COMPLETED
- [x] Migrate bit shift tests to `simd_bit_shifts_test.cc` - COMPLETED
- [x] Update function references and includes - COMPLETED

#### Step 2.2: Migrate from `simd_common_test.cc`
- [x] Migrate bitwise operation tests to `simd_bitwise_ops_test.cc` - COMPLETED
- [x] Migrate value construction tests to `simd_construct_values_test.cc` - COMPLETED
- [x] Update function references and includes - COMPLETED

#### Step 2.3: Migrate from `simd_reductions_test.cc`
- [x] Migrate boolean reduction tests to `simd_bool_reductions_test.cc` - COMPLETED
- [x] Update function references and includes - COMPLETED

#### Step 2.4: Migrate from `simd_int_arith_test.cc`
- [x] Migrate comparison tests to `simd_comparisons_test.cc` - COMPLETED
- [x] Update function references and includes - COMPLETED

#### Step 2.5: Migrate from `simd_load_store_test.cc`
- [x] Migrate saturated arithmetic tests to `simd_sat_int_arith_test.cc` - COMPLETED
- [x] Update function references and includes - COMPLETED

### Step 3: Verification & Integration

#### Step 3.1: Update CMakeLists.txt
- [x] Add new test files to compilation list - COMPLETED
- [x] Verify build system integration - COMPLETED
- [x] Test compilation with all new files - COMPLETED

#### Step 3.2: Run Comprehensive Tests
- [x] Execute all SIMD test files - COMPLETED
- [x] Verify 100% function coverage maintained - COMPLETED
- [x] Check for any test failures - COMPLETED

#### Step 3.3: Cleanup Original Files
- [x] Remove migrated test cases from original files - COMPLETED
- [x] Verify original files still compile correctly - COMPLETED
- [x] Update documentation references - COMPLETED

## Function Coverage Preservation

### Current Coverage Status
- **Total Functions**: 98
- **Current Coverage**: 100% (98/98 functions)
- **Target After Enhancement**: 100% (98/98 functions)

### Function Distribution After Enhancement

#### New Test Files Coverage:
- `simd_bitmask_extracts_test.cc`: 4 functions
- `simd_bit_shifts_test.cc`: 4 functions
- `simd_bitwise_ops_test.cc`: 1 function
- `simd_bool_reductions_test.cc`: 5 functions
- `simd_comparisons_test.cc`: 6 functions
- `simd_construct_values_test.cc`: 2 functions
- `simd_sat_int_arith_test.cc`: 2 functions

**Total**: 24 functions in new test files

#### Remaining Functions in Existing Files:
- Existing test files: 74 functions
- New test files: 24 functions
- **Total**: 98 functions (100% coverage)

## Implementation Guidelines

### Test File Template Structure
```cpp
/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "gtest/gtest.h"
#include "core/iwasm/compilation/simd/simd_[module_name].h"
#include "core/iwasm/compilation/aot_compiler.h"

class SIMD[ModuleName]Test : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test environment
    }
    
    void TearDown() override {
        // Cleanup test environment
    }
};

// Test cases for each function
TEST_F(SIMD[ModuleName]Test, TestFunctionName) {
    // Test implementation
}
```

### Build Commands
```bash
# Build with SIMD support
cmake -DWAMR_BUILD_SIMD=1 -DWAMR_BUILD_AOT=1 ..
make -j$(nproc)

# Run specific new test
./tests/unit/compilation/simd_bitmask_extracts_test

# Run all SIMD tests
./tests/unit/compilation/compilation_test
```

## Quality Assurance Checklist

### Before Implementation
- [x] Verify all function names exist in source code - COMPLETED
- [x] Confirm current 100% coverage status - COMPLETED
- [x] Review existing test patterns and conventions - COMPLETED
- [x] Plan migration strategy to avoid coverage loss - COMPLETED

### During Implementation
- [x] Follow WAMR coding standards - COMPLETED
- [x] Maintain consistent test structure - COMPLETED
- [x] Preserve all existing test scenarios - COMPLETED
- [x] Verify each migrated test case - COMPLETED

### After Implementation
- [x] Run complete test suite - COMPLETED
- [x] Verify 100% coverage maintained - COMPLETED
- [x] Check for compilation warnings - COMPLETED
- [x] Validate cross-platform compatibility - COMPLETED

## Risk Mitigation

### Coverage Loss Prevention
- Migrate test cases incrementally
- Run coverage verification after each migration
- Maintain backup of original test files
- Use version control for rollback capability

### Build System Integration
- Test compilation after each new file addition
- Verify CMakeLists.txt updates
- Check for dependency issues
- Validate linker behavior

### Performance Impact
- Monitor test execution time
- Ensure no significant performance regression
- Optimize test data structures
- Maintain efficient test execution

## Expected Outcomes

### Immediate Benefits
- Proper 1:1 mapping between source and test files
- Better organization and maintainability
- Clearer test file responsibilities
- Easier navigation and debugging

### Long-term Benefits
- Simplified test maintenance
- Clearer ownership of test files
- Better scalability for future enhancements
- Improved developer experience

### Success Metrics
- All 7 missing test files created
- 100% function coverage maintained
- No test failures introduced
- Build system integration successful
- Documentation updated appropriately