# Code Coverage Improve Plan for Runtime Common Module

## Current Coverage Status
- **Module**: core/iwasm/common/
- **Line Coverage**: 2816/7230 (38.9%)
- **Function Coverage**: 406/748 (54.3%)
- **Branch Coverage**: 1386/3840 (36.1%)
- **Coverage Report**: `tests/unit/wamr-lcov/wamr-lcov/index.html`
- **Target**: +10% coverage improvement (48.9% target line coverage)

## Uncovered Code Analysis

### Critical Uncovered Functions with Line Details

Based on LCOV coverage analysis of key runtime common files:

#### File: wasm_runtime_common.c (Current: 34.8% line coverage, 52.5% function coverage)
**Priority Functions - 0 Hits (Completely Uncovered)**:

##### Function: `argv_to_params()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (parameter conversion utility)
- **Function Category**: WASI argument processing

##### Function: `exchange_uint32()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (endianness conversion)
- **Function Category**: Data conversion utility

##### Function: `exchange_uint64()` [0 hits]  
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (endianness conversion)
- **Function Category**: Data conversion utility

##### Function: `get_wasi_args_from_module()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (WASI integration)
- **Function Category**: WASI argument handling

##### Function: `results_to_argv()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (result conversion)
- **Function Category**: WASI result processing

##### Function: `set_error_buf_v()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (error handling)
- **Function Category**: Error management

##### Function: `val_type_to_val_kind()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (type conversion)
- **Function Category**: Type system utilities

#### File: wasm_memory.c (Current: 70.0% line coverage, 85.7% function coverage)
**Priority Functions - 0 Hits (Completely Uncovered)**:

##### Function: `wasm_memory_enlarge()` [0 hits]
- **File**: `core/iwasm/common/wasm_memory.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (memory management)
- **Function Category**: Memory operations

##### Function: `wasm_memory_get_base_address()` [0 hits]
- **File**: `core/iwasm/common/wasm_memory.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (memory access)
- **Function Category**: Memory introspection

##### Function: `wasm_memory_get_max_page_count()` [0 hits]
- **File**: `core/iwasm/common/wasm_memory.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (memory limits)
- **Function Category**: Memory introspection

##### Function: `wasm_memory_get_shared()` [0 hits]
- **File**: `core/iwasm/common/wasm_memory.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Priority**: HIGH (shared memory)
- **Function Category**: Shared memory operations

#### File: wasm_application.c (Current: 0.0% line coverage, 0.0% function coverage)
**All 7 functions completely uncovered - HIGH PRIORITY**

#### File: wasm_blocking_op.c (Current: 0.0% line coverage, 0.0% function coverage)
**All 3 functions completely uncovered - HIGH PRIORITY**

#### File: wasm_shared_memory.c (Current: 7.1% line coverage, 20.0% function coverage)
**12 out of 15 functions completely uncovered - HIGH PRIORITY**

## Test Generation Sub-Plans

### Step 1: Core Runtime Functions (10 functions maximum)
**Target Functions with Line Coverage Goals**:

##### Function 1: `set_error_buf_v()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_set_error_buf_v_with_format()` → **Target: Error formatting with va_args**
  - [ ] `test_set_error_buf_v_null_buffer()` → **Target: Null buffer handling**

##### Function 2: `exchange_uint32()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_exchange_uint32_endianness()` → **Target: Byte order conversion**

##### Function 3: `exchange_uint64()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_exchange_uint64_endianness()` → **Target: 64-bit byte order conversion**

##### Function 4: `val_type_to_val_kind()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_val_type_to_val_kind_i32()` → **Target: i32 type conversion**
  - [ ] `test_val_type_to_val_kind_i64()` → **Target: i64 type conversion**
  - [ ] `test_val_type_to_val_kind_f32()` → **Target: f32 type conversion**
  - [ ] `test_val_type_to_val_kind_f64()` → **Target: f64 type conversion**

##### Function 5: `wasm_memory_get_base_address()` [0 hits]
- **File**: `core/iwasm/common/wasm_memory.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasm_memory_get_base_address_valid()` → **Target: Valid memory base retrieval**
  - [ ] `test_wasm_memory_get_base_address_null()` → **Target: Null memory handling**

##### Function 6: `wasm_memory_get_max_page_count()` [0 hits]
- **File**: `core/iwasm/common/wasm_memory.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasm_memory_get_max_page_count_valid()` → **Target: Max page retrieval**
  - [ ] `test_wasm_memory_get_max_page_count_unlimited()` → **Target: Unlimited memory case**

##### Function 7: `wasm_memory_get_shared()` [0 hits]
- **File**: `core/iwasm/common/wasm_memory.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasm_memory_get_shared_enabled()` → **Target: Shared memory detection**
  - [ ] `test_wasm_memory_get_shared_disabled()` → **Target: Non-shared memory**

##### Function 8: `wasm_memory_enlarge()` [0 hits]
- **File**: `core/iwasm/common/wasm_memory.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasm_memory_enlarge_success()` → **Target: Successful memory enlargement**
  - [ ] `test_wasm_memory_enlarge_failure()` → **Target: Memory enlargement failure**

**Step Metrics**:
- **Total Functions in Step**: 8 (≤10 maximum)
- **Expected Coverage**: 150+ lines (estimated 2% coverage improvement)
- **Status**: PENDING
- **Completion Criteria**: 
  - [ ] All test cases compile and run successfully
  - [ ] All assertions provide meaningful validation (no tautologies)
  - [ ] Test quality meets WAMR standards
  - [ ] LCOV report shows ≥2% coverage improvement
  - [ ] Each test case covers its specific target functions

### Step 2: WASI Integration Functions (10 functions maximum)
**Target Functions with Line Coverage Goals**:

##### Function 1: `argv_to_params()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_argv_to_params_valid_args()` → **Target: Valid argument conversion**
  - [ ] `test_argv_to_params_empty_args()` → **Target: Empty argument handling**

##### Function 2: `results_to_argv()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_results_to_argv_conversion()` → **Target: Result to argv conversion**
  - [ ] `test_results_to_argv_null_results()` → **Target: Null result handling**

##### Function 3: `get_wasi_args_from_module()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_get_wasi_args_from_module_valid()` → **Target: Valid WASI args extraction**
  - [ ] `test_get_wasi_args_from_module_no_wasi()` → **Target: Non-WASI module handling**

##### Functions 4-7: wasm_application.c functions [0 hits each]
- **File**: `core/iwasm/common/wasm_application.c`
- **All 7 functions completely uncovered**
- **Test Cases**: Application lifecycle, memory management, execution environment

**Step Metrics**:
- **Total Functions in Step**: 10 (≤10 maximum)
- **Expected Coverage**: 200+ lines (estimated 3% coverage improvement)
- **Status**: PENDING

### Step 3: Shared Memory Operations (10 functions maximum)
**Target Functions with Line Coverage Goals**:

##### Functions 1-10: wasm_shared_memory.c functions
- **File**: `core/iwasm/common/wasm_shared_memory.c`
- **12 out of 15 functions completely uncovered**
- **Test Cases**: Shared memory creation, synchronization, cleanup

**Step Metrics**:
- **Total Functions in Step**: 10 (≤10 maximum)
- **Expected Coverage**: 150+ lines (estimated 2% coverage improvement)
- **Status**: PENDING

### Step 4: Blocking Operations and Advanced Features (10 functions maximum)
**Target Functions with Line Coverage Goals**:

##### Functions 1-3: wasm_blocking_op.c functions [0 hits each]
- **File**: `core/iwasm/common/wasm_blocking_op.c`
- **All 3 functions completely uncovered**
- **Test Cases**: Blocking operation management, thread synchronization

##### Additional runtime-common functions with partial coverage
- **Target**: Functions with >10 uncovered lines for completion

**Step Metrics**:
- **Total Functions in Step**: 10 (≤10 maximum)
- **Expected Coverage**: 100+ lines (estimated 3% coverage improvement)
- **Status**: PENDING

## Overall Progress
- **Total Steps**: 4
- **Completed Steps**: 0
- **Current Step**: 1
- **Module Coverage Before**: 38.9%
- **Module Coverage After**: 48.9% (target)
- **Target Coverage**: +10% improvement

## Step Status
- [ ] Step 1: Core Runtime Functions - PENDING
- [ ] Step 2: WASI Integration Functions - PENDING  
- [ ] Step 3: Shared Memory Operations - PENDING
- [ ] Step 4: Blocking Operations and Advanced Features - PENDING

## Plan Metadata JSON
```json
{
  "plan_id": "runtime_common_20250921_145500",
  "module_name": "runtime-common",
  "target_coverage": "+10%",
  "total_steps": 4,
  "current_step": 1,
  "plan_file": "tests/unit/enhanced_coverage_report/runtime-common/runtime_common_coverage_improve_plan.md",
  "metadata": {
    "total_functions": 748,
    "uncovered_functions": 342,
    "current_line_coverage": "38.9%",
    "target_line_coverage": "48.9%",
    "complexity_level": "high",
    "dependencies": ["test_helper.h", "wasm_runtime.h", "wasm_memory.h"],
    "platform_constraints": ["linux", "memory_limits"],
    "estimated_duration": "4-6 hours"
  }
}
```