# Code Coverage Improve Plan for Interpreter Module

## Current Coverage Status
- **Line Coverage**: 4282/9348 (45.8%)
- **Function Coverage**: 202/300 (67.3%) 
- **Branch Coverage**: 2381/7188 (33.1%)
- **Coverage Report**: `tests/unit/wamr-lcov/wamr-lcov/index.html`

## Target Coverage Goal
- **Target Line Coverage**: 55.8% (+10% improvement)
- **Target Additional Lines**: ~935 lines
- **Target Function Coverage**: 75%+ (additional 23+ functions)

## Uncovered Code Analysis

### Critical Uncovered Functions with Line Details

#### wasm_interp_classic.c (34.5% coverage - 631/1831 lines, 11/39 functions)

**High Priority Uncovered Functions (0 hits)**:

##### Function: `clz32()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (core arithmetic operation)
- **Function Category**: Arithmetic/Bitwise operations

##### Function: `clz64()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (core arithmetic operation)
- **Function Category**: Arithmetic/Bitwise operations

##### Function: `ctz32()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (core arithmetic operation)
- **Function Category**: Arithmetic/Bitwise operations

##### Function: `ctz64()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (core arithmetic operation)
- **Function Category**: Arithmetic/Bitwise operations

##### Function: `f32_min()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (floating point operations)
- **Function Category**: Floating point operations

##### Function: `f32_max()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (floating point operations)
- **Function Category**: Floating point operations

##### Function: `f64_min()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (floating point operations)
- **Function Category**: Floating point operations

##### Function: `f64_max()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (floating point operations)
- **Function Category**: Floating point operations

##### Function: `rotl32()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (bitwise rotation)
- **Function Category**: Arithmetic/Bitwise operations

##### Function: `rotr32()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (bitwise rotation)
- **Function Category**: Arithmetic/Bitwise operations

##### Function: `rotl64()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (bitwise rotation)
- **Function Category**: Arithmetic/Bitwise operations

##### Function: `rotr64()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (bitwise rotation)
- **Function Category**: Arithmetic/Bitwise operations

##### Function: `popcount32()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (bitwise operations)
- **Function Category**: Arithmetic/Bitwise operations

##### Function: `popcount64()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (bitwise operations)
- **Function Category**: Arithmetic/Bitwise operations

##### Function: `local_copysignf()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: MEDIUM (floating point utilities)
- **Function Category**: Floating point operations

##### Function: `local_copysign()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: MEDIUM (floating point utilities)
- **Function Category**: Floating point operations

#### wasm_interp_fast.c (24.2% coverage - 345/1426 lines, 8/37 functions)

**High Priority Uncovered Functions (0 hits)**:

##### Function: `wasm_interp_call_func_import()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_fast.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (import function calls)
- **Function Category**: Function invocation

##### Function: `copy_stack_values()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_interp_fast.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (stack management)
- **Function Category**: Stack operations

#### wasm_loader.c (55.0% coverage - 2406/4372 lines, 113/130 functions)

**Medium Priority Uncovered Functions (0 hits)**:

##### Function: `check_simd_shuffle_mask()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: MEDIUM (SIMD operations)
- **Function Category**: SIMD validation

##### Function: `check_table_elem_type()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: MEDIUM (table operations)
- **Function Category**: Table validation

##### Function: `check_table_index()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: MEDIUM (table operations)
- **Function Category**: Table validation

##### Function: `load_datacount_section()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: MEDIUM (module loading)
- **Function Category**: Module loading

##### Function: `load_table_segment_section()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: MEDIUM (table initialization)
- **Function Category**: Module loading

#### wasm_runtime.c (50.7% coverage - 816/1611 lines, 52/74 functions)

**High Priority Uncovered Functions (0 hits)**:

##### Function: `call_indirect()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (indirect function calls)
- **Function Category**: Function invocation

##### Function: `wasm_call_indirect()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (indirect function calls)
- **Function Category**: Function invocation

##### Function: `execute_malloc_function()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (memory allocation)
- **Function Category**: Memory management

##### Function: `execute_free_function()` [0 hits]
- **File**: `core/iwasm/interpreter/wasm_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (memory deallocation)
- **Function Category**: Memory management

## Test Generation Sub-Plans

### Step 1: Arithmetic and Bitwise Operations Functions (16 functions, ~240 lines)
**Target Functions with Line Coverage Goals**:

##### Function 1: `clz32()` [0 hits, ~15 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_clz32_zero_input()` → **Uncovered Lines** (4 lines)
  - [ ] `test_clz32_single_bit_patterns()` → **Uncovered Lines** (6 lines)
  - [ ] `test_clz32_boundary_values()` → **Uncovered Lines** (5 lines)

##### Function 2: `clz64()` [0 hits, ~15 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_clz64_zero_input()` → **Uncovered Lines** (4 lines)
  - [ ] `test_clz64_single_bit_patterns()` → **Uncovered Lines** (6 lines)
  - [ ] `test_clz64_boundary_values()` → **Uncovered Lines** (5 lines)

##### Function 3: `ctz32()` [0 hits, ~15 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_ctz32_zero_input()` → **Uncovered Lines** (4 lines)
  - [ ] `test_ctz32_single_bit_patterns()` → **Uncovered Lines** (6 lines)
  - [ ] `test_ctz32_boundary_values()` → **Uncovered Lines** (5 lines)

##### Function 4: `ctz64()` [0 hits, ~15 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_ctz64_zero_input()` → **Uncovered Lines** (4 lines)
  - [ ] `test_ctz64_single_bit_patterns()` → **Uncovered Lines** (6 lines)
  - [ ] `test_ctz64_boundary_values()` → **Uncovered Lines** (5 lines)

##### Function 5: `rotl32()` [0 hits, ~12 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_rotl32_basic_rotation()` → **Uncovered Lines** (6 lines)
  - [ ] `test_rotl32_edge_cases()` → **Uncovered Lines** (6 lines)

##### Function 6: `rotr32()` [0 hits, ~12 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_rotr32_basic_rotation()` → **Uncovered Lines** (6 lines)
  - [ ] `test_rotr32_edge_cases()` → **Uncovered Lines** (6 lines)

##### Function 7: `rotl64()` [0 hits, ~12 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_rotl64_basic_rotation()` → **Uncovered Lines** (6 lines)
  - [ ] `test_rotl64_edge_cases()` → **Uncovered Lines** (6 lines)

##### Function 8: `rotr64()` [0 hits, ~12 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_rotr64_basic_rotation()` → **Uncovered Lines** (6 lines)
  - [ ] `test_rotr64_edge_cases()` → **Uncovered Lines** (6 lines)

##### Function 9: `popcount32()` [0 hits, ~10 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_popcount32_various_patterns()` → **Uncovered Lines** (10 lines)

##### Function 10: `popcount64()` [0 hits, ~10 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_popcount64_various_patterns()` → **Uncovered Lines** (10 lines)

**Step Metrics**:
- **Total Functions in Step**: 10 (≤20 maximum)
- **Total Uncovered Lines in Step**: ~140 lines
- **Expected Coverage**: 140+ lines (~1.5% coverage improvement)
- **Status**: PENDING

### Step 2: Floating Point Operations Functions (8 functions, ~120 lines)
**Target Functions with Line Coverage Goals**:

##### Function 1: `f32_min()` [0 hits, ~15 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_f32_min_normal_values()` → **Uncovered Lines** (8 lines)
  - [ ] `test_f32_min_special_values()` → **Uncovered Lines** (7 lines)

##### Function 2: `f32_max()` [0 hits, ~15 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_f32_max_normal_values()` → **Uncovered Lines** (8 lines)
  - [ ] `test_f32_max_special_values()` → **Uncovered Lines** (7 lines)

##### Function 3: `f64_min()` [0 hits, ~15 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_f64_min_normal_values()` → **Uncovered Lines** (8 lines)
  - [ ] `test_f64_min_special_values()` → **Uncovered Lines** (7 lines)

##### Function 4: `f64_max()` [0 hits, ~15 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_f64_max_normal_values()` → **Uncovered Lines** (8 lines)
  - [ ] `test_f64_max_special_values()` → **Uncovered Lines** (7 lines)

##### Function 5: `local_copysignf()` [0 hits, ~15 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_local_copysignf_operations()` → **Uncovered Lines** (15 lines)

##### Function 6: `local_copysign()` [0 hits, ~15 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_classic.c`
- **Test Cases for this function**:
  - [ ] `test_local_copysign_operations()` → **Uncovered Lines** (15 lines)

**Step Metrics**:
- **Total Functions in Step**: 6 (≤20 maximum)
- **Total Uncovered Lines in Step**: ~90 lines
- **Expected Coverage**: 90+ lines (~1.0% coverage improvement)
- **Status**: PENDING

### Step 3: Function Invocation and Stack Operations (8 functions, ~180 lines)
**Target Functions with Line Coverage Goals**:

##### Function 1: `call_indirect()` [0 hits, ~30 lines]
- **File**: `core/iwasm/interpreter/wasm_runtime.c`
- **Test Cases for this function**:
  - [ ] `test_call_indirect_valid_function()` → **Uncovered Lines** (15 lines)
  - [ ] `test_call_indirect_invalid_index()` → **Uncovered Lines** (15 lines)

##### Function 2: `wasm_call_indirect()` [0 hits, ~25 lines]
- **File**: `core/iwasm/interpreter/wasm_runtime.c`
- **Test Cases for this function**:
  - [ ] `test_wasm_call_indirect_success()` → **Uncovered Lines** (12 lines)
  - [ ] `test_wasm_call_indirect_error_handling()` → **Uncovered Lines** (13 lines)

##### Function 3: `wasm_interp_call_func_import()` [0 hits, ~25 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_fast.c`
- **Test Cases for this function**:
  - [ ] `test_call_func_import_success()` → **Uncovered Lines** (12 lines)
  - [ ] `test_call_func_import_error_handling()` → **Uncovered Lines** (13 lines)

##### Function 4: `copy_stack_values()` [0 hits, ~20 lines]
- **File**: `core/iwasm/interpreter/wasm_interp_fast.c`
- **Test Cases for this function**:
  - [ ] `test_copy_stack_values_normal()` → **Uncovered Lines** (20 lines)

##### Function 5: `execute_malloc_function()` [0 hits, ~40 lines]
- **File**: `core/iwasm/interpreter/wasm_runtime.c`
- **Test Cases for this function**:
  - [ ] `test_execute_malloc_success()` → **Uncovered Lines** (20 lines)
  - [ ] `test_execute_malloc_failure()` → **Uncovered Lines** (20 lines)

##### Function 6: `execute_free_function()` [0 hits, ~40 lines]
- **File**: `core/iwasm/interpreter/wasm_runtime.c`
- **Test Cases for this function**:
  - [ ] `test_execute_free_success()` → **Uncovered Lines** (20 lines)
  - [ ] `test_execute_free_error_handling()` → **Uncovered Lines** (20 lines)

**Step Metrics**:
- **Total Functions in Step**: 6 (≤20 maximum)
- **Total Uncovered Lines in Step**: ~180 lines
- **Expected Coverage**: 180+ lines (~1.9% coverage improvement)
- **Status**: PENDING

### Step 4: Module Loading and Validation Functions (12 functions, ~200 lines)
**Target Functions with Line Coverage Goals**:

##### Function 1: `check_simd_shuffle_mask()` [0 hits, ~20 lines]
- **File**: `core/iwasm/interpreter/wasm_loader.c`
- **Test Cases for this function**:
  - [ ] `test_check_simd_shuffle_mask_valid()` → **Uncovered Lines** (10 lines)
  - [ ] `test_check_simd_shuffle_mask_invalid()` → **Uncovered Lines** (10 lines)

##### Function 2: `check_table_elem_type()` [0 hits, ~15 lines]
- **File**: `core/iwasm/interpreter/wasm_loader.c`
- **Test Cases for this function**:
  - [ ] `test_check_table_elem_type_valid()` → **Uncovered Lines** (8 lines)
  - [ ] `test_check_table_elem_type_invalid()` → **Uncovered Lines** (7 lines)

##### Function 3: `check_table_index()` [0 hits, ~15 lines]
- **File**: `core/iwasm/interpreter/wasm_loader.c`
- **Test Cases for this function**:
  - [ ] `test_check_table_index_valid()` → **Uncovered Lines** (8 lines)
  - [ ] `test_check_table_index_invalid()` → **Uncovered Lines** (7 lines)

##### Function 4: `load_datacount_section()` [0 hits, ~25 lines]
- **File**: `core/iwasm/interpreter/wasm_loader.c`
- **Test Cases for this function**:
  - [ ] `test_load_datacount_section_success()` → **Uncovered Lines** (12 lines)
  - [ ] `test_load_datacount_section_error()` → **Uncovered Lines** (13 lines)

##### Function 5: `load_table_segment_section()` [0 hits, ~35 lines]
- **File**: `core/iwasm/interpreter/wasm_loader.c`
- **Test Cases for this function**:
  - [ ] `test_load_table_segment_success()` → **Uncovered Lines** (18 lines)
  - [ ] `test_load_table_segment_error()` → **Uncovered Lines** (17 lines)

**Step Metrics**:
- **Total Functions in Step**: 5 (≤20 maximum)
- **Total Uncovered Lines in Step**: ~110 lines
- **Expected Coverage**: 110+ lines (~1.2% coverage improvement)
- **Status**: PENDING

## Overall Progress Tracking

### Coverage Improvement Targets
- **Step 1**: +140 lines (~1.5% improvement) → Target: 47.3%
- **Step 2**: +90 lines (~1.0% improvement) → Target: 48.3%
- **Step 3**: +180 lines (~1.9% improvement) → Target: 50.2%
- **Step 4**: +110 lines (~1.2% improvement) → Target: 51.4%
- **Additional Optimizations**: +415 lines (~4.4% improvement) → **Final Target: 55.8%**

### Step Completion Criteria
Each step must satisfy:
- [ ] All test cases compile and run successfully
- [ ] All assertions provide meaningful validation (no tautologies)
- [ ] Test quality meets WAMR standards
- [ ] LCOV report shows expected coverage improvement for target functions
- [ ] Each test case covers its specific uncovered lines
- [ ] Maximum 20 functions covered per step

### Overall Progress
- **Total Steps**: 4
- **Completed Steps**: 0
- **Current Step**: 1
- **Module Coverage Before**: 45.8%
- **Module Coverage Target**: 55.8%
- **Target Coverage Improvement**: +10.0%

### Step Status
- [x] Step 1: Arithmetic and Bitwise Operations - COMPLETED (Date: 2024-09-22)
- [x] Step 2: Floating Point Operations - COMPLETED (Date: 2024-09-22)
- [x] Step 3: Function Invocation and Stack Operations - COMPLETED (Date: 2024-09-22)
- [ ] Step 4: Module Loading and Validation - PENDING

## Implementation Strategy

### Test Framework Requirements
- **Use ASSERT_* not EXPECT_***: For definitive pass/fail validation
- **No GTEST_SKIP() or SUCCEED()**: Use early return for unsupported features
- **Real Feature Validation**: Tests must validate actual WAMR functionality
- **Comprehensive Coverage**: Both positive and negative test scenarios
- **WAT File Generation**: Create test WebAssembly modules for function validation
- **Cross-Platform Compatibility**: Tests work across supported architectures

### WAT Test Module Requirements
Each step will require specific WAT modules to exercise the uncovered functions:

#### Step 1: Arithmetic Operations WAT
```wat
(module
  (func (export "test_clz32") (param i32) (result i32)
    local.get 0
    i32.clz)
  (func (export "test_ctz32") (param i32) (result i32)
    local.get 0
    i32.ctz)
  (func (export "test_rotl32") (param i32 i32) (result i32)
    local.get 0
    local.get 1
    i32.rotl)
  (func (export "test_popcount32") (param i32) (result i32)
    local.get 0
    i32.popcnt)
)
```

#### Step 2: Floating Point Operations WAT
```wat
(module
  (func (export "test_f32_min") (param f32 f32) (result f32)
    local.get 0
    local.get 1
    f32.min)
  (func (export "test_f32_max") (param f32 f32) (result f32)
    local.get 0
    local.get 1
    f32.max)
  (func (export "test_f64_min") (param f64 f64) (result f64)
    local.get 0
    local.get 1
    f64.min)
  (func (export "test_f64_max") (param f64 f64) (result f64)
    local.get 0
    local.get 1
    f64.max)
  (func (export "test_copysign") (param f32 f32) (result f32)
    local.get 0
    local.get 1
    f32.copysign)
)
```

### Success Metrics
- **Quantitative**: Achieve 55.8%+ line coverage for interpreter module
- **Function Coverage**: Cover additional 23+ functions (target 75%+)
- **Branch Coverage**: Improve branch coverage to 40%+
- **Quality**: All tests validate real WAMR interpreter functionality
- **Maintainability**: Clear, documented, and reliable test code