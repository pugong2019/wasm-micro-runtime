# Code Coverage Improve Plan for AOT Module

## Current Coverage Status
- **Line Coverage**: 1913/3592 (53.3%)
- **Function Coverage**: 177/242 (73.1%)
- **Branch Coverage**: 971/2914 (33.3%)
- **Coverage Report**: `tests/unit/wamr-lcov/wamr-lcov/index.html`
- **Target Coverage**: 73.3% (20% improvement from 53.3%)

## AOT Module Files Analysis

### File Coverage Summary
| File | Line Coverage | Function Coverage | Uncovered Functions |
|------|---------------|-------------------|-------------------|
| aot_runtime.c | 689/1610 (42.8%) | 52/83 (62.7%) | 31 functions (0 hits) |
| aot_loader.c | 928/1580 (58.7%) | 64/80 (80.0%) | 16 functions (0 hits) |
| aot_intrinsic.c | 296/402 (73.6%) | 61/79 (77.2%) | 18 functions (0 hits) |

## Uncovered Code Analysis

### Critical Uncovered Functions with Line Details

#### AOT Runtime Functions (aot_runtime.c) - 31 Functions with 0 hits
**LCOV Extraction Verified**: All functions confirmed with 0 hits from LCOV function table

##### Function: `aot_alloc_tiny_frame()`
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: HIGH (memory management function)
- **Function Category**: Memory allocation

##### Function: `aot_call_indirect()`
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: HIGH (core runtime function)
- **Function Category**: Function execution

##### Function: `aot_const_str_set_insert()`
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: MEDIUM (string management)
- **Function Category**: String operations

##### Function: `aot_data_drop()`
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: HIGH (memory management)
- **Function Category**: Memory operations

##### Function: `aot_enlarge_memory_with_idx()`
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: HIGH (memory management)
- **Function Category**: Memory operations

##### Function: `aot_frame_update_profile_info()`
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: MEDIUM (profiling)
- **Function Category**: Profiling

##### Function: `aot_free_tiny_frame()`
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: HIGH (memory management)
- **Function Category**: Memory allocation

##### Function: `aot_get_aux_stack()`
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: HIGH (stack management)
- **Function Category**: Stack operations

##### Function: `aot_get_exception()`
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: HIGH (error handling)
- **Function Category**: Exception handling

##### Function: `aot_get_function_instance()`
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: HIGH (function management)
- **Function Category**: Function operations

#### AOT Loader Functions (aot_loader.c) - 16 Functions with 0 hits

##### Function: `aot_load_from_sections()`
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: HIGH (core loading function)
- **Function Category**: Module loading

##### Function: `destroy_import_globals()`
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: MEDIUM (cleanup function)
- **Function Category**: Import management

##### Function: `destroy_import_memories()`
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: MEDIUM (cleanup function)
- **Function Category**: Import management

##### Function: `do_data_relocation()`
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: HIGH (relocation function)
- **Function Category**: Code relocation

##### Function: `exchange_uint16()`, `exchange_uint32()`, `exchange_uint64()`
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: MEDIUM (endianness handling)
- **Function Category**: Data conversion

#### AOT Intrinsic Functions (aot_intrinsic.c) - 18 Functions with 0 hits

##### Function: `add_f32xi32_intrinsics()`, `add_f32xi64_intrinsics()`, `add_f64xi32_intrinsics()`, `add_f64xi64_intrinsics()`
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: MEDIUM (intrinsic setup)
- **Function Category**: Intrinsic management

##### Function: `aot_intrinsic_i32_div_s()`, `aot_intrinsic_i32_div_u()`, `aot_intrinsic_i32_rem_s()`, `aot_intrinsic_i32_rem_u()`
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: HIGH (arithmetic operations)
- **Function Category**: Integer arithmetic

##### Function: `aot_intrinsic_i64_div_s()`, `aot_intrinsic_i64_div_u()`, `aot_intrinsic_i64_rem_s()`, `aot_intrinsic_i64_rem_u()`
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Priority**: HIGH (arithmetic operations)
- **Function Category**: Integer arithmetic

## Test Generation Sub-Plans

### Step 1: AOT Runtime Core Functions (10 functions maximum)
**Target Functions with Line Coverage Goals**:

##### Function 1: `aot_call_indirect()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_call_indirect_valid_function()` → **Target: Indirect function call validation**
  - [ ] `test_aot_call_indirect_invalid_type()` → **Target: Type mismatch error handling**

##### Function 2: `aot_get_exception()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_get_exception_with_error()` → **Target: Exception retrieval**

##### Function 3: `aot_get_function_instance()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_get_function_instance_valid_index()` → **Target: Function instance retrieval**
  - [ ] `test_aot_get_function_instance_invalid_index()` → **Target: Invalid index handling**

##### Function 4: `aot_enlarge_memory_with_idx()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_enlarge_memory_with_idx_success()` → **Target: Memory enlargement**
  - [ ] `test_aot_enlarge_memory_with_idx_failure()` → **Target: Memory limit error**

##### Function 5: `aot_data_drop()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_data_drop_valid_segment()` → **Target: Data segment dropping**

##### Function 6: `aot_get_aux_stack()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_get_aux_stack_valid()` → **Target: Auxiliary stack retrieval**

##### Function 7: `aot_set_aux_stack()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_set_aux_stack_valid()` → **Target: Auxiliary stack setting**

##### Function 8: `aot_alloc_tiny_frame()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_alloc_tiny_frame_success()` → **Target: Tiny frame allocation**

##### Function 9: `aot_free_tiny_frame()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_free_tiny_frame_valid()` → **Target: Tiny frame deallocation**

##### Function 10: `aot_memory_init()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_memory_init_valid_segment()` → **Target: Memory initialization**

**Step Metrics**:
- **Total Functions in Step**: 10 (maximum)
- **Expected Coverage**: 300+ lines (estimated 15-20% improvement)
- **Status**: PENDING
- **Completion Criteria**: 
  - [ ] All test cases compile and run successfully
  - [ ] All assertions provide meaningful validation (no tautologies)
  - [ ] Test quality meets WAMR standards
  - [ ] LCOV report shows ≥20% coverage improvement
  - [ ] Each test case covers its specific target functions

### Step 2: AOT Loader Functions (10 functions maximum)
**Target Functions with Line Coverage Goals**:

##### Function 1: `aot_load_from_sections()` [0 hits]
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_load_from_sections_valid()` → **Target: Section-based loading**
  - [ ] `test_aot_load_from_sections_invalid()` → **Target: Invalid section handling**

##### Function 2: `do_data_relocation()` [0 hits]
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_do_data_relocation_success()` → **Target: Data relocation**

##### Function 3: `exchange_uint16()` [0 hits]
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_exchange_uint16_endianness()` → **Target: 16-bit endianness conversion**

##### Function 4: `exchange_uint32()` [0 hits]
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_exchange_uint32_endianness()` → **Target: 32-bit endianness conversion**

##### Function 5: `exchange_uint64()` [0 hits]
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_exchange_uint64_endianness()` → **Target: 64-bit endianness conversion**

##### Function 6: `load_import_globals()` [0 hits]
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_load_import_globals_valid()` → **Target: Import global loading**

##### Function 7: `destroy_import_globals()` [0 hits]
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_destroy_import_globals_cleanup()` → **Target: Import global cleanup**

##### Function 8: `destroy_import_memories()` [0 hits]
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_destroy_import_memories_cleanup()` → **Target: Import memory cleanup**

##### Function 9: `load_name_section()` [0 hits]
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_load_name_section_valid()` → **Target: Name section loading**

##### Function 10: `load_native_symbol_section()` [0 hits]
- **File**: `core/iwasm/aot/aot_loader.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_load_native_symbol_section_valid()` → **Target: Native symbol loading**

**Step Metrics**:
- **Total Functions in Step**: 10 (maximum)
- **Expected Coverage**: 250+ lines (estimated 10-15% improvement)
- **Status**: PENDING

### Step 3: AOT Intrinsic Functions (10 functions maximum)
**Target Functions with Line Coverage Goals**:

##### Function 1: `aot_intrinsic_i32_div_s()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_intrinsic_i32_div_s_normal()` → **Target: Signed 32-bit division**
  - [ ] `test_aot_intrinsic_i32_div_s_by_zero()` → **Target: Division by zero handling**

##### Function 2: `aot_intrinsic_i32_div_u()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_intrinsic_i32_div_u_normal()` → **Target: Unsigned 32-bit division**

##### Function 3: `aot_intrinsic_i64_div_s()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_intrinsic_i64_div_s_normal()` → **Target: Signed 64-bit division**

##### Function 4: `aot_intrinsic_i64_div_u()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_intrinsic_i64_div_u_normal()` → **Target: Unsigned 64-bit division**

##### Function 5: `aot_intrinsic_i32_rem_s()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_intrinsic_i32_rem_s_normal()` → **Target: Signed 32-bit remainder**

##### Function 6: `aot_intrinsic_i32_rem_u()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_intrinsic_i32_rem_u_normal()` → **Target: Unsigned 32-bit remainder**

##### Function 7: `aot_intrinsic_i64_rem_s()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_intrinsic_i64_rem_s_normal()` → **Target: Signed 64-bit remainder**

##### Function 8: `aot_intrinsic_i64_rem_u()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_intrinsic_i64_rem_u_normal()` → **Target: Unsigned 64-bit remainder**

##### Function 9: `aot_intrinsic_i64_shl()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_intrinsic_i64_shl_normal()` → **Target: 64-bit left shift**

##### Function 10: `aot_intrinsic_i64_shr_s()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_intrinsic_i64_shr_s_normal()` → **Target: Signed 64-bit right shift**

**Step Metrics**:
- **Total Functions in Step**: 10 (maximum)
- **Expected Coverage**: 180+ lines (estimated 8-12% improvement)
- **Status**: PENDING

### Step 4: AOT Utility and Support Functions (10 functions maximum)
**Target Functions with Line Coverage Goals**:

##### Function 1: `aot_lookup_memory()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_lookup_memory_valid()` → **Target: Memory lookup**

##### Function 2: `aot_get_memory_with_idx()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_get_memory_with_idx_valid()` → **Target: Memory retrieval by index**

##### Function 3: `aot_lookup_function_with_idx()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_lookup_function_with_idx_valid()` → **Target: Function lookup by index**

##### Function 4: `aot_memmove()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_memmove_normal()` → **Target: Memory move operation**

##### Function 5: `aot_sqrt()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_sqrt_normal()` → **Target: Square root calculation**

##### Function 6: `aot_sqrtf()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_sqrtf_normal()` → **Target: Float square root calculation**

##### Function 7: `aot_resolve_function()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_resolve_function_valid()` → **Target: Function resolution**

##### Function 8: `aot_resolve_function_ex()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_resolve_function_ex_valid()` → **Target: Extended function resolution**

##### Function 9: `aot_resolve_symbols()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_resolve_symbols_valid()` → **Target: Symbol resolution**

##### Function 10: `aot_get_module_name()` [0 hits]
- **File**: `core/iwasm/aot/aot_runtime.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_get_module_name_valid()` → **Target: Module name retrieval**

**Step Metrics**:
- **Total Functions in Step**: 10 (maximum)
- **Expected Coverage**: 200+ lines (estimated 8-10% improvement)
- **Status**: PENDING

### Step 5: AOT Intrinsic Support Functions (5 functions)
**Target Functions with Line Coverage Goals**:

##### Function 1: `add_f32xi32_intrinsics()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_add_f32xi32_intrinsics_registration()` → **Target: F32xI32 intrinsic registration**

##### Function 2: `add_f32xi64_intrinsics()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_add_f32xi64_intrinsics_registration()` → **Target: F32xI64 intrinsic registration**

##### Function 3: `add_f64xi32_intrinsics()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_add_f64xi32_intrinsics_registration()` → **Target: F64xI32 intrinsic registration**

##### Function 4: `add_f64xi64_intrinsics()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_add_f64xi64_intrinsics_registration()` → **Target: F64xI64 intrinsic registration**

##### Function 5: `aot_intrinsic_i64_shr_u()` [0 hits]
- **File**: `core/iwasm/aot/aot_intrinsic.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_aot_intrinsic_i64_shr_u_normal()` → **Target: Unsigned 64-bit right shift**

**Step Metrics**:
- **Total Functions in Step**: 5
- **Expected Coverage**: 120+ lines (estimated 5-8% improvement)
- **Status**: PENDING

## Overall Progress
- **Total Steps**: 5
- **Completed Steps**: 0
- **Current Step**: 1
- **Module Coverage Before**: 53.3%
- **Module Coverage After**: 73.3% (Target)
- **Target Coverage**: 73.3% (20% improvement)

## Step Status
- [ ] Step 1: AOT Runtime Core Functions - PENDING
- [ ] Step 2: AOT Loader Functions - PENDING  
- [ ] Step 3: AOT Intrinsic Functions - PENDING
- [ ] Step 4: AOT Utility and Support Functions - PENDING
- [ ] Step 5: AOT Intrinsic Support Functions - PENDING

## Implementation Strategy

### Phase 1: Foundation Setup
1. **Enhanced Directory Structure**: ✅ Created `tests/unit/enhanced_coverage_report/aot/`
2. **CMakeLists.txt Configuration**: ✅ AOT-specific build configuration with LLVM support
3. **Test Infrastructure**: Prepare GTest framework with AOT module dependencies

### Phase 2: Sequential Step Execution
1. **Step-by-Step Implementation**: Complete each step before proceeding to next
2. **Coverage Validation**: Verify coverage improvement after each step
3. **Quality Assurance**: Ensure all tests meet WAMR standards

### Phase 3: Integration and Validation
1. **Cross-Step Integration**: Validate functionality across all steps
2. **Performance Impact**: Ensure no regression in existing functionality
3. **Documentation**: Update test documentation and coverage reports

## Success Metrics

### Quantitative Targets
- **Line Coverage**: From 53.3% to 73.3% (20% improvement)
- **Function Coverage**: From 73.1% to 85%+ (target 65 uncovered functions)
- **Branch Coverage**: From 33.3% to 50%+ (significant improvement expected)

### Qualitative Standards
- **Functionality Validation**: Each test validates specific AOT behavior
- **Error Path Coverage**: Comprehensive exception and error handling
- **Platform Compatibility**: Tests work across supported architectures
- **Maintainability**: Clear, documented, and reliable test code