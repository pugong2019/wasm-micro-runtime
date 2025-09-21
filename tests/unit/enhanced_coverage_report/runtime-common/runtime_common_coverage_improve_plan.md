# Code Coverage Improve Plan for Runtime Common Module

## Current Coverage Status
- **Module**: core/iwasm/common/
- **Line Coverage**: 2870/7230 (39.7%)
- **Function Coverage**: 413/748 (55.2%)
- **Branch Coverage**: 1402/3840 (36.5%)
- **Coverage Report**: `tests/unit/wamr-lcov/wamr-lcov/index.html`
- **Target**: +30% coverage improvement (to reach ~68.9% total line coverage)

## Enhanced Coverage Analysis from LCOV Reports

### Critical Uncovered Functions with Line Details

Based on detailed LCOV coverage analysis of key runtime common files:

#### File: wasm_runtime_common.c (Current: 35.7% line coverage, 53.5% function coverage)
**High-Impact Uncovered Functions - 0 Hits (Completely Uncovered)**:

##### Function: `wasm_runtime_call_indirect()` [0 hits, HIGH IMPACT]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6236
- **Estimated Lines**: 25+ lines
- **Priority**: HIGH (indirect function calls)
- **Function Category**: Core execution

##### Function: `wasm_runtime_dump_call_stack()` [0 hits, HIGH IMPACT]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6812
- **Estimated Lines**: 35+ lines
- **Priority**: HIGH (debugging support)
- **Function Category**: Debug utilities

##### Function: `wasm_runtime_dump_call_stack_to_buf()` [0 hits, HIGH IMPACT]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6849
- **Estimated Lines**: 40+ lines
- **Priority**: HIGH (debugging support)
- **Function Category**: Debug utilities

##### Function: `wasm_runtime_get_call_stack_buf_size()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6829
- **Estimated Lines**: 15+ lines
- **Priority**: HIGH (stack management)
- **Function Category**: Debug utilities

##### Function: `wasm_runtime_spawn_exec_env()` [0 hits, HIGH IMPACT]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6314
- **Estimated Lines**: 30+ lines
- **Priority**: HIGH (threading support)
- **Function Category**: Threading

##### Function: `wasm_runtime_spawn_thread()` [0 hits, HIGH IMPACT]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6342
- **Estimated Lines**: 35+ lines
- **Priority**: HIGH (threading support)
- **Function Category**: Threading

##### Function: `wasm_runtime_join_thread()` [0 hits, HIGH IMPACT]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6373
- **Estimated Lines**: 25+ lines
- **Priority**: HIGH (threading support)
- **Function Category**: Threading

##### Function: `wasm_runtime_thread_routine()` [0 hits, HIGH IMPACT]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6326
- **Estimated Lines**: 40+ lines
- **Priority**: HIGH (threading support)
- **Function Category**: Threading

##### Function: `wasm_runtime_destroy_spawned_exec_env()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6320
- **Estimated Lines**: 20+ lines
- **Priority**: HIGH (threading cleanup)
- **Function Category**: Threading

##### Function: `wasm_runtime_terminate()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 3223
- **Estimated Lines**: 15+ lines
- **Priority**: HIGH (runtime termination)
- **Function Category**: Runtime lifecycle

#### File: wasm_memory.c (Current: 71.8% line coverage, 92.1% function coverage)
**Remaining Uncovered Functions**:

##### Function: `wasm_enlarge_memory()` [0 hits]
- **File**: `core/iwasm/common/wasm_memory.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Estimated Lines**: 45+ lines
- **Priority**: HIGH (memory growth)
- **Function Category**: Memory management

##### Function: `wasm_memory_init_with_pool()` [0 hits]
- **File**: `core/iwasm/common/wasm_memory.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Estimated Lines**: 35+ lines
- **Priority**: HIGH (memory pool initialization)
- **Function Category**: Memory management

#### File: wasm_application.c (Current: 0.0% line coverage, 0.0% function coverage)
**All 7 functions completely uncovered - CRITICAL PRIORITY**:

##### Function: `wasm_application_execute_main()` [0 hits, CRITICAL]
- **File**: `core/iwasm/common/wasm_application.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Estimated Lines**: 50+ lines
- **Priority**: CRITICAL (main execution)
- **Function Category**: Application lifecycle

##### Function: `wasm_application_execute_func()` [0 hits, CRITICAL]
- **File**: `core/iwasm/common/wasm_application.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Estimated Lines**: 40+ lines
- **Priority**: CRITICAL (function execution)
- **Function Category**: Application lifecycle

#### File: wasm_blocking_op.c (Current: 0.0% line coverage, 0.0% function coverage)
**All 3 functions completely uncovered - HIGH PRIORITY**:

##### Function: `wasm_cluster_create_blocking_op()` [0 hits]
- **File**: `core/iwasm/common/wasm_blocking_op.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Estimated Lines**: 20+ lines
- **Priority**: HIGH (blocking operations)
- **Function Category**: Concurrency

#### File: wasm_shared_memory.c (Current: 7.1% line coverage, 20.0% function coverage)
**12 out of 15 functions completely uncovered - HIGH PRIORITY**:

##### Function: `shared_memory_set_memory_inst()` [0 hits]
- **File**: `core/iwasm/common/wasm_shared_memory.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Estimated Lines**: 15+ lines
- **Priority**: HIGH (shared memory setup)
- **Function Category**: Shared memory

##### Function: `wasm_runtime_atomic_wait()` [0 hits]
- **File**: `core/iwasm/common/wasm_shared_memory.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Estimated Lines**: 35+ lines
- **Priority**: HIGH (atomic operations)
- **Function Category**: Shared memory

##### Function: `wasm_runtime_atomic_notify()` [0 hits]
- **File**: `core/iwasm/common/wasm_shared_memory.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Estimated Lines**: 25+ lines
- **Priority**: HIGH (atomic operations)
- **Function Category**: Shared memory

## Enhanced Test Generation Sub-Plans

### Step 1: Core Runtime Functions (10 functions maximum) ✅ COMPLETED
**Status**: ✅ COMPLETED (Date: 2025-09-21)
- **Functions Tested**: 8/8 (100%)
- **Test Cases**: 20/20 passed (100% success rate)
- **Coverage Impact**: ~150 lines covered

### Step 2: WASI Integration Functions (10 functions maximum) ✅ COMPLETED
**Status**: ✅ COMPLETED (Date: 2025-09-21)
- **Functions Tested**: 10+ functions
- **Test Cases**: 9/9 passed (100% success rate)
- **Coverage Impact**: ~200 lines covered

### Step 3: Shared Memory Operations (10 functions maximum) ✅ COMPLETED
**Status**: ✅ COMPLETED (Date: 2025-09-21)
- **Functions Tested**: 10+ shared memory functions
- **Test Cases**: 5/5 passed (100% success rate)
- **Coverage Impact**: ~150 lines covered

### Step 4: Threading and Concurrency Functions (10 functions maximum)
**Target Functions with Line Coverage Goals**:

##### Function 1: `wasm_runtime_spawn_exec_env()` [0 hits, HIGH IMPACT]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6314
- **Estimated Lines**: 30+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_runtime_spawn_exec_env_success()` → **Target: Successful thread spawning**
  - [ ] `test_wasm_runtime_spawn_exec_env_failure()` → **Target: Thread spawn failure handling**

##### Function 2: `wasm_runtime_spawn_thread()` [0 hits, HIGH IMPACT]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6342
- **Estimated Lines**: 35+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_runtime_spawn_thread_valid()` → **Target: Valid thread creation**
  - [ ] `test_wasm_runtime_spawn_thread_invalid()` → **Target: Invalid thread parameters**

##### Function 3: `wasm_runtime_join_thread()` [0 hits, HIGH IMPACT]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6373
- **Estimated Lines**: 25+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_runtime_join_thread_success()` → **Target: Successful thread join**
  - [ ] `test_wasm_runtime_join_thread_timeout()` → **Target: Thread join timeout**

##### Function 4: `wasm_runtime_thread_routine()` [0 hits, HIGH IMPACT]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6326
- **Estimated Lines**: 40+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_runtime_thread_routine_execution()` → **Target: Thread routine execution**

##### Function 5: `wasm_runtime_destroy_spawned_exec_env()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6320
- **Estimated Lines**: 20+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_runtime_destroy_spawned_exec_env()` → **Target: Spawned env cleanup**

##### Function 6: `wasm_cluster_create_blocking_op()` [0 hits]
- **File**: `core/iwasm/common/wasm_blocking_op.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Estimated Lines**: 20+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_cluster_create_blocking_op()` → **Target: Blocking operation creation**

##### Function 7: `wasm_runtime_atomic_wait()` [0 hits]
- **File**: `core/iwasm/common/wasm_shared_memory.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Estimated Lines**: 35+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_runtime_atomic_wait_i32()` → **Target: i32 atomic wait**
  - [ ] `test_wasm_runtime_atomic_wait_i64()` → **Target: i64 atomic wait**

##### Function 8: `wasm_runtime_atomic_notify()` [0 hits]
- **File**: `core/iwasm/common/wasm_shared_memory.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Estimated Lines**: 25+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_runtime_atomic_notify()` → **Target: Atomic notification**

**Step Metrics**:
- **Total Functions in Step**: 8 (≤10 maximum)
- **Expected Coverage**: 230+ lines (estimated 3% coverage improvement)
- **Status**: PENDING
- **Implementation Strategy**: Multi-threading test environment with proper synchronization

### Step 5: Debug and Introspection Functions (10 functions maximum)
**Target Functions with Line Coverage Goals**:

##### Function 1: `wasm_runtime_dump_call_stack()` [0 hits, HIGH IMPACT]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6812
- **Estimated Lines**: 35+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_runtime_dump_call_stack_valid()` → **Target: Valid stack dump**
  - [ ] `test_wasm_runtime_dump_call_stack_empty()` → **Target: Empty stack handling**

##### Function 2: `wasm_runtime_dump_call_stack_to_buf()` [0 hits, HIGH IMPACT]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6849
- **Estimated Lines**: 40+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_runtime_dump_call_stack_to_buf()` → **Target: Stack dump to buffer**

##### Function 3: `wasm_runtime_get_call_stack_buf_size()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6829
- **Estimated Lines**: 15+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_runtime_get_call_stack_buf_size()` → **Target: Stack buffer size calculation**

##### Function 4: `wasm_runtime_call_indirect()` [0 hits, HIGH IMPACT]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 6236
- **Estimated Lines**: 25+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_runtime_call_indirect_valid()` → **Target: Valid indirect call**
  - [ ] `test_wasm_runtime_call_indirect_invalid()` → **Target: Invalid function index**

##### Function 5: `wasm_runtime_terminate()` [0 hits]
- **File**: `core/iwasm/common/wasm_runtime_common.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Function Line**: 3223
- **Estimated Lines**: 15+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_runtime_terminate()` → **Target: Runtime termination**

##### Functions 6-8: Memory introspection functions from wasm_memory.c
- **wasm_enlarge_memory()**: Memory growth operations
- **wasm_memory_init_with_pool()**: Memory pool initialization
- **Additional memory management functions**

**Step Metrics**:
- **Total Functions in Step**: 8 (≤10 maximum)
- **Expected Coverage**: 200+ lines (estimated 3% coverage improvement)
- **Status**: PENDING

### Step 6: Application Lifecycle Functions (7 functions maximum)
**Target Functions with Line Coverage Goals**:

##### Functions 1-7: Complete wasm_application.c coverage [CRITICAL]
- **File**: `core/iwasm/common/wasm_application.c`
- **All 7 functions completely uncovered**
- **Estimated Total Lines**: 268 lines (100% uncovered)
- **Priority**: CRITICAL (core application functionality)

##### Function 1: `wasm_application_execute_main()` [0 hits, CRITICAL]
- **Estimated Lines**: 50+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_application_execute_main_success()` → **Target: Successful main execution**
  - [ ] `test_wasm_application_execute_main_no_main()` → **Target: No main function handling**

##### Function 2: `wasm_application_execute_func()` [0 hits, CRITICAL]
- **Estimated Lines**: 40+ lines
- **Test Cases for this function**:
  - [ ] `test_wasm_application_execute_func_valid()` → **Target: Valid function execution**
  - [ ] `test_wasm_application_execute_func_invalid()` → **Target: Invalid function handling**

##### Functions 3-7: Additional application functions
- **Application initialization, cleanup, and management functions**
- **Complete application lifecycle coverage**

**Step Metrics**:
- **Total Functions in Step**: 7 (≤10 maximum)
- **Expected Coverage**: 268+ lines (estimated 4% coverage improvement)
- **Status**: PENDING
- **Implementation Strategy**: Application execution environment with proper module loading

## Overall Progress
- **Total Steps**: 6 (expanded from 4)
- **Completed Steps**: 3
- **Current Step**: 4 (Threading and Concurrency Functions)
- **Module Coverage Before**: 39.7%
- **Module Coverage Target**: ~68.9% (+30% improvement)
- **Estimated Total Lines to Cover**: ~1000+ lines across all steps

## Enhanced Step Status
- [x] **Step 1: Core Runtime Functions - ✅ COMPLETED (2025-09-21)**
  - **Coverage Impact**: ~150 lines covered (~2% improvement)
- [x] **Step 2: WASI Integration Functions - ✅ COMPLETED (2025-09-21)**  
  - **Coverage Impact**: ~200 lines covered (~3% improvement)
- [x] **Step 3: Shared Memory Operations - ✅ COMPLETED (2025-09-21)**
  - **Coverage Impact**: ~150 lines covered (~2% improvement)
- [ ] **Step 4: Threading and Concurrency Functions - ⏳ NEXT**
  - **Expected Coverage**: 230+ lines (~3% improvement)
  - **Target Functions**: 8 threading/concurrency functions
- [ ] **Step 5: Debug and Introspection Functions - PENDING**
  - **Expected Coverage**: 200+ lines (~3% improvement)
  - **Target Functions**: 8 debug/introspection functions
- [ ] **Step 6: Application Lifecycle Functions - PENDING**
  - **Expected Coverage**: 268+ lines (~4% improvement)
  - **Target Functions**: 7 application functions (CRITICAL)

## Implementation Guidance

### Threading Test Strategy
- **Multi-threading Environment**: Use pthread-based test scenarios
- **Synchronization Testing**: Proper mutex/condition variable usage
- **Resource Management**: Careful cleanup of spawned threads
- **Platform Compatibility**: Linux-specific threading features

### Debug Function Testing
- **Call Stack Generation**: Create nested function calls for stack traces
- **Buffer Management**: Test various buffer sizes and edge cases
- **Error Conditions**: Test stack overflow and memory limit scenarios

### Application Function Testing
- **Module Loading**: Create test WASM modules with main functions
- **Execution Environment**: Proper execution context setup
- **Error Handling**: Invalid module and function scenarios
- **Resource Cleanup**: Memory and execution environment cleanup

### Static Function Testing
- **Indirect Testing**: Use public APIs that internally call static functions
- **Code Path Coverage**: Design tests to reach specific uncovered lines
- **Edge Case Scenarios**: Boundary conditions and error paths
- **Integration Testing**: Cross-module functionality validation

## Plan Metadata JSON
```json
{
  "plan_id": "runtime_common_enhanced_20250921_170000",
  "module_name": "runtime-common",
  "target_coverage": "+30%",
  "total_steps": 6,
  "current_step": 4,
  "plan_file": "tests/unit/enhanced_coverage_report/runtime-common/runtime_common_coverage_improve_plan.md",
  "metadata": {
    "total_functions": 748,
    "uncovered_functions": 335,
    "current_line_coverage": "39.7%",
    "target_line_coverage": "68.9%",
    "complexity_level": "high",
    "dependencies": ["test_helper.h", "wasm_runtime.h", "wasm_memory.h", "pthread.h"],
    "platform_constraints": ["linux", "threading_support", "memory_limits"],
    "estimated_duration": "8-12 hours",
    "completed_steps": 3,
    "remaining_coverage_target": "+23%",
    "estimated_remaining_lines": "1000+"
  }
}
```