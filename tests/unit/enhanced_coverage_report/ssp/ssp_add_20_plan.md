# Code Coverage Improve Plan for SSP (+20% Coverage)

## Plan Metadata
- **Plan ID**: `ssp_add_20_20250924_120000`
- **Module**: Sandboxed System Primitives (SSP)
- **Target Coverage**: +20% improvement
- **Plan File**: `ssp_add_20_plan.md`
- **Progress File**: `ssp_add_20_progress.json`
- **Generated**: 2025-09-24 12:00:00

## Current Coverage Status
- Line Coverage: 271/1636 (16.6%)
- Function Coverage: 42/169 (24.9%)
- Branch Coverage: 94/908 (10.4%)
- **Coverage Report**: `tests/unit/wamr-lcov/BUILD_WPE/wasm-micro-runtime/core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/index.html`
- **Target Coverage**: 16.6% + 20% = 36.6%

## Uncovered Code Analysis

### Critical Uncovered Functions with Line Details
**MANDATORY**: Extracted from LCOV report with verification. Listed ONLY functions meeting criteria:

#### LCOV Extraction Checklist:
- [x] Function has 0 hits (completely uncovered) OR >10 uncovered lines
- [x] Uncovered line count verified from LCOV red highlighting
- [x] Function is reachable in current build configuration
- [x] Function is not platform-specific (unless targeting specific platform)

#### Function: `wasmtime_ssp_fd_close()`
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Total Function Lines**: 35
- **Uncovered Lines Count**: 35 lines (verified from LCOV)
- **Priority**: HIGH (0 hits, critical file descriptor operation)
- **Function Category**: Core functionality

**Verification Notes**:
- ✅ Confirmed 0 hits in LCOV function table
- ✅ Function exists in current source tree
- ✅ Function is built in current configuration

#### Function: `wasmtime_ssp_fd_pread()`
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Total Function Lines**: 42
- **Uncovered Lines Count**: 42 lines (verified from LCOV)
- **Priority**: HIGH (0 hits, critical I/O operation)
- **Function Category**: Core functionality

#### Function: `wasmtime_ssp_fd_pwrite()`
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Total Function Lines**: 38
- **Uncovered Lines Count**: 38 lines (verified from LCOV)
- **Priority**: HIGH (0 hits, critical I/O operation)
- **Function Category**: Core functionality

#### Function: `wasmtime_ssp_fd_read()`
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Total Function Lines**: 45
- **Uncovered Lines Count**: 45 lines (verified from LCOV)
- **Priority**: HIGH (0 hits, critical I/O operation)
- **Function Category**: Core functionality

#### Function: `wasmtime_ssp_fd_write()`
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Total Function Lines**: 40
- **Uncovered Lines Count**: 40 lines (verified from LCOV)
- **Priority**: HIGH (0 hits, critical I/O operation)
- **Function Category**: Core functionality

#### Function: `wasmtime_ssp_fd_seek()`
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Total Function Lines**: 32
- **Uncovered Lines Count**: 32 lines (verified from LCOV)
- **Priority**: HIGH (0 hits, critical file positioning)
- **Function Category**: Core functionality

## Test Generation Sub-Plans

### Step 1: Core File Descriptor Operations (6 functions)
**Implementation File**: `ssp_add_20_step_1.cc`

**Target Functions with Line Coverage Goals**:

##### Function 1: `wasmtime_ssp_fd_close()` [0 hits, 35 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasmtime_ssp_fd_close_valid_fd()` → **Uncovered Lines** (18 lines)
  - [ ] `test_wasmtime_ssp_fd_close_invalid_fd()` → **Uncovered Lines** (17 lines)

##### Function 2: `wasmtime_ssp_fd_pread()` [0 hits, 42 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasmtime_ssp_fd_pread_normal_operation()` → **Uncovered Lines** (25 lines)
  - [ ] `test_wasmtime_ssp_fd_pread_invalid_params()` → **Uncovered Lines** (17 lines)

##### Function 3: `wasmtime_ssp_fd_pwrite()` [0 hits, 38 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasmtime_ssp_fd_pwrite_normal_operation()` → **Uncovered Lines** (22 lines)
  - [ ] `test_wasmtime_ssp_fd_pwrite_invalid_params()` → **Uncovered Lines** (16 lines)

##### Function 4: `wasmtime_ssp_fd_read()` [0 hits, 45 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasmtime_ssp_fd_read_normal_operation()` → **Uncovered Lines** (28 lines)
  - [ ] `test_wasmtime_ssp_fd_read_error_conditions()` → **Uncovered Lines** (17 lines)

##### Function 5: `wasmtime_ssp_fd_write()` [0 hits, 40 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasmtime_ssp_fd_write_normal_operation()` → **Uncovered Lines** (24 lines)
  - [ ] `test_wasmtime_ssp_fd_write_error_conditions()` → **Uncovered Lines** (16 lines)

##### Function 6: `wasmtime_ssp_fd_seek()` [0 hits, 32 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasmtime_ssp_fd_seek_position_operations()` → **Uncovered Lines** (18 lines)
  - [ ] `test_wasmtime_ssp_fd_seek_error_conditions()` → **Uncovered Lines** (14 lines)

**Line Coverage Mapping**:
Function Name              | LCOV Hits | Uncovered Lines | Test Case Name               
wasmtime_ssp_fd_close      | 0         | 35             | test_wasmtime_ssp_fd_close_valid_fd      
wasmtime_ssp_fd_close      | 0         | 35             | test_wasmtime_ssp_fd_close_invalid_fd      
wasmtime_ssp_fd_pread      | 0         | 42             | test_wasmtime_ssp_fd_pread_normal_operation
wasmtime_ssp_fd_pread      | 0         | 42             | test_wasmtime_ssp_fd_pread_invalid_params
wasmtime_ssp_fd_pwrite     | 0         | 38             | test_wasmtime_ssp_fd_pwrite_normal_operation        
wasmtime_ssp_fd_pwrite     | 0         | 38             | test_wasmtime_ssp_fd_pwrite_invalid_params        
wasmtime_ssp_fd_read       | 0         | 45             | test_wasmtime_ssp_fd_read_normal_operation        
wasmtime_ssp_fd_read       | 0         | 45             | test_wasmtime_ssp_fd_read_error_conditions        
wasmtime_ssp_fd_write      | 0         | 40             | test_wasmtime_ssp_fd_write_normal_operation        
wasmtime_ssp_fd_write      | 0         | 40             | test_wasmtime_ssp_fd_write_error_conditions        
wasmtime_ssp_fd_seek       | 0         | 32             | test_wasmtime_ssp_fd_seek_position_operations        
wasmtime_ssp_fd_seek       | 0         | 32             | test_wasmtime_ssp_fd_seek_error_conditions                         

**Step Metrics**:
- **Total Functions in Step**: 6 (≤20 maximum)
- **Total Uncovered Lines in Step**: 232 lines
- **Expected Coverage**: 232+ lines (14.2%+ coverage rate)
- **Status**: PENDING
- **Completion Criteria**: 
  - [ ] All test cases compile and run successfully
  - [ ] All assertions provide meaningful validation (no tautologies)
  - [ ] Test quality meets WAMR standards
  - [ ] LCOV report shows ≥14.2% coverage improvement
  - [ ] Each test case covers its specific Uncovered Lines
  - [ ] Maximum 6 functions covered in this step

### Step 2: File Descriptor Management & Metadata Operations (8 functions)
**Implementation File**: `ssp_add_20_step_2.cc`

**Target Functions with Line Coverage Goals**:

##### Function 1: `wasmtime_ssp_fd_fdstat_get()` [0 hits, 28 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasmtime_ssp_fd_fdstat_get_valid_fd()` → **Uncovered Lines** (16 lines)
  - [ ] `test_wasmtime_ssp_fd_fdstat_get_invalid_fd()` → **Uncovered Lines** (12 lines)

##### Function 2: `wasmtime_ssp_fd_fdstat_set_flags()` [0 hits, 22 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasmtime_ssp_fd_fdstat_set_flags_valid()` → **Uncovered Lines** (13 lines)
  - [ ] `test_wasmtime_ssp_fd_fdstat_set_flags_invalid()` → **Uncovered Lines** (9 lines)

##### Function 3: `wasmtime_ssp_fd_fdstat_set_rights()` [0 hits, 19 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasmtime_ssp_fd_fdstat_set_rights_valid()` → **Uncovered Lines** (11 lines)
  - [ ] `test_wasmtime_ssp_fd_fdstat_set_rights_invalid()` → **Uncovered Lines** (8 lines)

##### Function 4: `wasmtime_ssp_fd_filestat_get()` [0 hits, 26 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasmtime_ssp_fd_filestat_get_valid()` → **Uncovered Lines** (15 lines)
  - [ ] `test_wasmtime_ssp_fd_filestat_get_invalid()` → **Uncovered Lines** (11 lines)

##### Function 5: `wasmtime_ssp_fd_filestat_set_size()` [0 hits, 24 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasmtime_ssp_fd_filestat_set_size_valid()` → **Uncovered Lines** (14 lines)
  - [ ] `test_wasmtime_ssp_fd_filestat_set_size_invalid()` → **Uncovered Lines** (10 lines)

##### Function 6: `wasmtime_ssp_fd_filestat_set_times()` [0 hits, 31 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasmtime_ssp_fd_filestat_set_times_valid()` → **Uncovered Lines** (18 lines)
  - [ ] `test_wasmtime_ssp_fd_filestat_set_times_invalid()` → **Uncovered Lines** (13 lines)

##### Function 7: `wasmtime_ssp_fd_datasync()` [0 hits, 15 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasmtime_ssp_fd_datasync_valid()` → **Uncovered Lines** (15 lines)

##### Function 8: `wasmtime_ssp_fd_sync()` [0 hits, 18 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasmtime_ssp_fd_sync_valid()` → **Uncovered Lines** (18 lines)

**Step Metrics**:
- **Total Functions in Step**: 8 (≤20 maximum)
- **Total Uncovered Lines in Step**: 183 lines
- **Expected Coverage**: 183+ lines (11.2%+ coverage rate)
- **Status**: PENDING

### Step 3: Socket Operations & Network Functions (10 functions)
**Implementation File**: `ssp_add_20_step_3.cc`

**Target Functions with Line Coverage Goals**:

##### Function 1: `wasi_ssp_sock_open()` [0 hits, 25 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasi_ssp_sock_open_tcp()` → **Uncovered Lines** (13 lines)
  - [ ] `test_wasi_ssp_sock_open_udp()` → **Uncovered Lines** (12 lines)

##### Function 2: `wasi_ssp_sock_bind()` [0 hits, 22 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasi_ssp_sock_bind_valid()` → **Uncovered Lines** (22 lines)

##### Function 3: `wasi_ssp_sock_listen()` [0 hits, 18 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasi_ssp_sock_listen_valid()` → **Uncovered Lines** (18 lines)

##### Function 4: `wasi_ssp_sock_accept()` [0 hits, 28 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasi_ssp_sock_accept_valid()` → **Uncovered Lines** (28 lines)

##### Function 5: `wasi_ssp_sock_connect()` [0 hits, 24 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasi_ssp_sock_connect_valid()` → **Uncovered Lines** (24 lines)

##### Function 6: `wasi_ssp_sock_addr_local()` [0 hits, 16 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasi_ssp_sock_addr_local_valid()` → **Uncovered Lines** (16 lines)

##### Function 7: `wasi_ssp_sock_addr_remote()` [0 hits, 16 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasi_ssp_sock_addr_remote_valid()` → **Uncovered Lines** (16 lines)

##### Function 8: `wasi_ssp_sock_addr_resolve()` [0 hits, 35 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasi_ssp_sock_addr_resolve_valid()` → **Uncovered Lines** (20 lines)
  - [ ] `test_wasi_ssp_sock_addr_resolve_invalid()` → **Uncovered Lines** (15 lines)

##### Function 9: `wasi_ssp_sock_get_recv_buf_size()` [0 hits, 14 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasi_ssp_sock_get_recv_buf_size_valid()` → **Uncovered Lines** (14 lines)

##### Function 10: `wasi_ssp_sock_set_recv_buf_size()` [0 hits, 12 uncovered lines]
- **File**: `core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/posix.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Test Cases for this function**:
  - [ ] `test_wasi_ssp_sock_set_recv_buf_size_valid()` → **Uncovered Lines** (12 lines)

**Step Metrics**:
- **Total Functions in Step**: 10 (≤20 maximum)
- **Total Uncovered Lines in Step**: 210 lines
- **Expected Coverage**: 210+ lines (12.8%+ coverage rate)
- **Status**: PENDING

## Multi-Step Execution Protocol

**Phase 1: Accurate Coverage Analysis**
1. **LCOV Report Parsing**: Access and parse actual coverage report HTML
2. **Function Extraction**: Extract functions with 0 hits OR >10 uncovered lines
3. **Line Count Verification**: Manually verify uncovered line counts from LCOV
4. **Data Accuracy Check**: Cross-reference with source files and build configuration

**Phase 2: Strategic Step Planning**
1. **Function Categorization**: Group by functionality (I/O, file descriptors, networking, etc.)
2. **Complexity Assessment**: Evaluate function complexity and test requirements
3. **Step Segmentation**: Divide into balanced steps (6-10 functions per step)
4. **Dependency Mapping**: Ensure prerequisite functions are covered in earlier steps

**Phase 3: Sequential Execution**
1. **Step-by-Step Implementation**: Complete Step N before proceeding to Step N+1
2. **Coverage Validation**: Verify coverage improvement after each step
3. **Quality Assurance**: Ensure test quality meets WAMR standards
4. **Integration Testing**: Validate cross-step functionality and overall coverage

**Step Completion Validation**:
- [ ] LCOV report shows expected coverage improvement
- [ ] All target functions show increased hit counts
- [ ] No regression in existing test coverage
- [ ] Test quality meets all WAMR standards

#### Step Completion Criteria
Each step must satisfy:
- [ ] All test cases compile and run successfully
- [ ] All assertions provide meaningful validation (no tautologies)
- [ ] Test quality meets WAMR standards
- [ ] Test case coverage improvement for features is measurable
- [ ] No regression in existing functionality

## Overall Progress
- Total Steps: 3
- Completed Steps: 0
- Current Step: 1
- Module Coverage Before: 16.6%
- Module Coverage After: TBD
- Target Coverage: 36.6%

## Step Status
- [ ] Step 1: Core File Descriptor Operations - PENDING
- [ ] Step 2: File Descriptor Management & Metadata Operations - PENDING
- [ ] Step 3: Socket Operations & Network Functions - PENDING

## Implementation Notes

### Key Testing Strategies
1. **File Descriptor Validation**: Create and manage test file descriptors properly
2. **Error Path Coverage**: Test invalid parameters and edge cases extensively
3. **Platform Compatibility**: Handle platform differences in file I/O and networking
4. **Resource Management**: Ensure proper cleanup of file descriptors and sockets
5. **WASI Compliance**: Validate WASI specification compliance in all test scenarios

### Coverage Enhancement Focus Areas
1. **Critical I/O Operations**: fd_read, fd_write, fd_pread, fd_pwrite
2. **File Descriptor Management**: fd_close, fd_fdstat operations
3. **File Metadata Operations**: filestat_get, filestat_set operations
4. **Socket Operations**: Complete socket lifecycle testing
5. **Error Handling**: Comprehensive error condition coverage

### Expected Outcomes
- **Quantitative**: Achieve 36.6% line coverage (20% improvement)
- **Qualitative**: Comprehensive validation of SSP core functionality
- **Reliability**: Robust error handling and edge case coverage
- **Maintainability**: Clear, documented test cases for future development