# Code Coverage Improve Plan for AOT Architecture (+40% Coverage)

## Plan Metadata
- **Plan ID**: `aot_arch_add_40_20250923_143000`
- **Module**: AOT Architecture (core/iwasm/aot/arch/)
- **Target Coverage**: +40% improvement
- **Plan File**: `aot_arch_add_40_plan.md`
- **Progress File**: `aot_arch_add_40_progress.json`
- **Generated**: 2025-09-23 14:30:00

## Current Coverage Status
- Line Coverage: 55/95 (57.9%)
- Function Coverage: 7/8 (87.5%)
- Branch Coverage: 12/72 (16.7%)
- **Coverage Report**: `tests/unit/wamr-lcov/BUILD_WPE/wasm-micro-runtime/core/iwasm/aot/arch/index.html`
- **Target Coverage**: 57.9% + 40% = 97.9%

## Uncovered Code Analysis

### Critical Uncovered Functions with Line Details

#### LCOV Extraction Checklist:
- [x] Function has 0 hits (completely uncovered) OR >10 uncovered lines
- [x] Uncovered line count verified from LCOV red highlighting
- [x] Function is reachable in current build configuration
- [x] Function is not platform-specific (unless targeting specific platform)

#### Function: `set_error_buf()`
- **File**: `core/iwasm/aot/arch/aot_reloc_x86_64.c`
- **LCOV Hits**: 0 (completely uncovered)
- **Total Function Lines**: 5
- **Uncovered Lines Count**: 5 lines (lines 41-45)
- **Priority**: HIGH (0 hits)
- **Function Category**: Error handling

**Verification Notes**:
- ✅ Confirmed 0 hits in LCOV function table
- ✅ Manually counted 5 red-highlighted lines in LCOV source view
- ✅ Function exists in current source tree
- ✅ Function is built in current configuration

#### Partially Covered Functions with Significant Uncovered Lines:

#### Function: `apply_relocation()`
- **File**: `core/iwasm/aot/arch/aot_reloc_x86_64.c`
- **LCOV Hits**: 3474 hits with 40 uncovered lines
- **Total Function Lines**: 155
- **Uncovered Lines Count**: 40 lines (verified from LCOV)
- **Priority**: HIGH (>15 uncovered lines)
- **Function Category**: Core relocation functionality

**Uncovered Line Ranges**:
- Lines 117-128: R_X86_64_64 relocation type (completely uncovered)
- Lines 173-181: R_X86_64_PC64 relocation type (completely uncovered)
- Lines 183-207: R_X86_64_32/R_X86_64_32S relocation types (completely uncovered)
- Lines 255-261: Default case error handling (completely uncovered)
- Multiple error path branches throughout the function

## Test Generation Sub-Plans

### Step 1: Error Handling and Edge Cases (≤20 functions maximum)
**Implementation File**: `aot_arch_add_40_step_1.cc`

**Target Functions with Line Coverage Goals**:

##### Function 1: `set_error_buf()` [0 hits, 5 uncovered lines]
- **File**: `core/iwasm/aot/arch/aot_reloc_x86_64.c`
- **LCOV Data**: 0 hits (completely uncovered)
- **Uncovered Line Numbers**: Lines 41-45 (from LCOV report)
- **Test Cases for this function**:
  - [ ] `test_set_error_buf_valid_buffer()` → **Uncovered Lines** (3 lines)
  - [ ] `test_set_error_buf_null_buffer()` → **Uncovered Lines** (2 lines)

##### Function 2: `apply_relocation()` - Error Paths [3474 hits, 40 uncovered lines]
- **File**: `core/iwasm/aot/arch/aot_reloc_x86_64.c`
- **LCOV Data**: 3474 hits with significant uncovered error paths
- **Uncovered Line Numbers**: Lines 255-261 (default case), 162-167 (PC32 error), 102-104 (offset validation error)
- **Test Cases for this function**:
  - [ ] `test_apply_relocation_invalid_type()` → **Uncovered Lines** (7 lines - default case)
  - [ ] `test_apply_relocation_invalid_offset()` → **Uncovered Lines** (3 lines - offset validation)
  - [ ] `test_apply_relocation_pc32_overflow()` → **Uncovered Lines** (6 lines - PC32 error path)

**Line Coverage Mapping**:
| Function Name | LCOV Hits | Uncovered Lines | Test Case Name |
|---------------|------------|-----------------|----------------|
| set_error_buf | 0 | 5 | test_set_error_buf_valid_buffer |
| set_error_buf | 0 | 5 | test_set_error_buf_null_buffer |
| apply_relocation | 3474 | 7 | test_apply_relocation_invalid_type |
| apply_relocation | 3474 | 3 | test_apply_relocation_invalid_offset |
| apply_relocation | 3474 | 6 | test_apply_relocation_pc32_overflow |

**Step Metrics**:
- **Total Functions in Step**: 2 (≤20 maximum)
- **Total Uncovered Lines in Step**: 21 lines
- **Expected Coverage**: 21+ lines (22%+ coverage improvement)
- **Status**: PENDING

### Step 2: Relocation Type Coverage - R_X86_64_64 (≤20 functions maximum)
**Implementation File**: `aot_arch_add_40_step_2.cc`

**Target Functions with Line Coverage Goals**:

##### Function 1: `apply_relocation()` - R_X86_64_64 Support [3474 hits, 12 uncovered lines]
- **File**: `core/iwasm/aot/arch/aot_reloc_x86_64.c`
- **LCOV Data**: 3474 hits with R_X86_64_64 path completely uncovered
- **Uncovered Line Numbers**: Lines 117-128 (R_X86_64_64 relocation type)
- **Test Cases for this function**:
  - [ ] `test_apply_relocation_r_x86_64_64_valid()` → **Uncovered Lines** (8 lines)
  - [ ] `test_apply_relocation_r_x86_64_64_invalid_offset()` → **Uncovered Lines** (4 lines)

**Line Coverage Mapping**:
| Function Name | LCOV Hits | Uncovered Lines | Test Case Name |
|---------------|------------|-----------------|----------------|
| apply_relocation | 3474 | 8 | test_apply_relocation_r_x86_64_64_valid |
| apply_relocation | 3474 | 4 | test_apply_relocation_r_x86_64_64_invalid_offset |

**Step Metrics**:
- **Total Functions in Step**: 1 (≤20 maximum)
- **Total Uncovered Lines in Step**: 12 lines
- **Expected Coverage**: 12+ lines (12.6%+ coverage improvement)
- **Status**: PENDING

### Step 3: Advanced Relocation Types Coverage (≤20 functions maximum)
**Implementation File**: `aot_arch_add_40_step_3.cc`

**Target Functions with Line Coverage Goals**:

##### Function 1: `apply_relocation()` - R_X86_64_PC64 Support [3474 hits, 9 uncovered lines]
- **File**: `core/iwasm/aot/arch/aot_reloc_x86_64.c`
- **LCOV Data**: 3474 hits with R_X86_64_PC64 path completely uncovered
- **Uncovered Line Numbers**: Lines 173-181 (R_X86_64_PC64 relocation type)
- **Test Cases for this function**:
  - [ ] `test_apply_relocation_r_x86_64_pc64_valid()` → **Uncovered Lines** (6 lines)
  - [ ] `test_apply_relocation_r_x86_64_pc64_invalid_offset()` → **Uncovered Lines** (3 lines)

##### Function 2: `apply_relocation()` - R_X86_64_32/32S Support [3474 hits, 25 uncovered lines]
- **File**: `core/iwasm/aot/arch/aot_reloc_x86_64.c`
- **LCOV Data**: 3474 hits with R_X86_64_32/32S paths completely uncovered
- **Uncovered Line Numbers**: Lines 183-207 (R_X86_64_32/R_X86_64_32S relocation types)
- **Test Cases for this function**:
  - [ ] `test_apply_relocation_r_x86_64_32_valid()` → **Uncovered Lines** (8 lines)
  - [ ] `test_apply_relocation_r_x86_64_32s_valid()` → **Uncovered Lines** (8 lines)
  - [ ] `test_apply_relocation_r_x86_64_32_overflow()` → **Uncovered Lines** (5 lines)
  - [ ] `test_apply_relocation_r_x86_64_32s_overflow()` → **Uncovered Lines** (4 lines)

**Line Coverage Mapping**:
| Function Name | LCOV Hits | Uncovered Lines | Test Case Name |
|---------------|------------|-----------------|----------------|
| apply_relocation | 3474 | 6 | test_apply_relocation_r_x86_64_pc64_valid |
| apply_relocation | 3474 | 3 | test_apply_relocation_r_x86_64_pc64_invalid_offset |
| apply_relocation | 3474 | 8 | test_apply_relocation_r_x86_64_32_valid |
| apply_relocation | 3474 | 8 | test_apply_relocation_r_x86_64_32s_valid |
| apply_relocation | 3474 | 5 | test_apply_relocation_r_x86_64_32_overflow |
| apply_relocation | 3474 | 4 | test_apply_relocation_r_x86_64_32s_overflow |

**Step Metrics**:
- **Total Functions in Step**: 1 (≤20 maximum)
- **Total Uncovered Lines in Step**: 34 lines
- **Expected Coverage**: 34+ lines (35.8%+ coverage improvement)
- **Status**: PENDING

## Multi-Architecture Extension Strategy

### Cross-Platform Testing Approach
While the current analysis focuses on x86_64 architecture due to LCOV data availability, the plan includes provisions for extending coverage to other architecture files:

#### Architecture Files for Future Coverage:
- `aot_reloc_aarch64.c` - ARM64 relocations
- `aot_reloc_arm.c` - ARM32 relocations  
- `aot_reloc_riscv.c` - RISC-V relocations
- `aot_reloc_mips.c` - MIPS relocations
- `aot_reloc_thumb.c` - ARM Thumb relocations
- `aot_reloc_x86_32.c` - x86-32 relocations
- `aot_reloc_xtensa.c` - Xtensa relocations
- `aot_reloc_arc.c` - ARC relocations
- `aot_reloc_dummy.c` - Fallback implementation

#### Cross-Platform Test Strategy:
1. **Platform Detection**: Tests will detect target architecture and skip non-applicable tests
2. **Conditional Compilation**: Use `#ifdef` guards for architecture-specific code paths
3. **Mock Framework**: Create mock AOT modules for testing relocation logic
4. **Error Path Validation**: Focus on error handling which is common across architectures

## Overall Progress
- Total Steps: 3
- Completed Steps: 0
- Current Step: 1
- Module Coverage Before: 57.9%
- Module Coverage After: 97.9% (target)
- Target Coverage: +40%

## Step Status
- [ ] Step 1: Error Handling and Edge Cases - PENDING
- [ ] Step 2: Relocation Type Coverage - R_X86_64_64 - PENDING
- [ ] Step 3: Advanced Relocation Types Coverage - PENDING

## Step Completion Criteria
Each step must satisfy:
- [ ] All test cases compile and run successfully
- [ ] All assertions provide meaningful validation (no tautologies)
- [ ] Test quality meets WAMR standards
- [ ] LCOV report shows expected coverage improvement
- [ ] Each test case covers its specific Uncovered Lines
- [ ] No regression in existing functionality
- [ ] Platform compatibility maintained

## Implementation Notes

### Test Environment Requirements:
- **Build Configuration**: Requires AOT compilation support enabled
- **Platform**: Linux x86_64 (primary), with conditional support for other architectures
- **Dependencies**: LLVM libraries, AOT module loading capabilities
- **Memory Requirements**: 512MB heap for relocation testing

### Mock Data Strategy:
- Create minimal AOT modules with specific relocation requirements
- Generate test scenarios that trigger each relocation type
- Use controlled symbol addresses to test overflow conditions
- Implement error injection for negative test cases

### Quality Assurance:
- All tests must use ASSERT_* (not EXPECT_*) for definitive validation
- Comprehensive positive and negative test scenarios
- Proper resource management with RAII patterns
- Platform-aware testing with graceful handling of unsupported features