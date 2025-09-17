# WAMR Unit Test Coverage Execution Plan Tool

This tool provides step-by-step execution guidance for implementing comprehensive unit tests to increase code coverage in the WASM Micro Runtime (WAMR) project.

## Build and Run Unit Tests

```bash
# Build unit tests with coverage
cd tests/unit/{module}
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DWAMR_BUILD_COVERAGE=1
make
./[EXECUTABLE_MODULE_NAME]
```

## Coverage Improvement Workflow (CRITICAL)

### Phase 1: Module Analysis
1. When user specifies a target module and target code coverage (e.g., `interpreter`, `aot`, `runtime-common`):
    - Analyze module coverage from current LCOV report
    - Navigate to coverage report and examine specific module
    ```bash
    firefox coverage_report/wamr-lcov/index.html
    # Or examine specific module path: coverage_report/wamr-lcov/wasm-micro-runtime/core/iwasm/[module]/
    ```
2. Extract uncovered functions and lines for the module
    ```bash
    grep -A 5 -B 5 "uncovered" coverage_report/wamr-lcov/wasm-micro-runtime/core/iwasm/[module]/index.html
    ```

### Phase 2: Test Plan Generation

#### Enhanced Test Directory Structure (MANDATORY)
**CRITICAL REQUIREMENT**: To prevent pollution of existing unit tests, ALL new enhanced test code MUST be created in an isolated directory structure:

```
tests/unit/enhanced_unit_test/[ModuleName]/
├── CMakeLists.txt                           # Copied and modified from original
├── test_[feature]_core_enhanced.cc          # Step 1: Core operations
├── test_[feature]_advanced_enhanced.cc      # Step 2: Advanced operations  
├── test_[feature]_integration_enhanced.cc   # Step 3: Integration testing
├── [ModuleName]_feature_test_plan.md        # Feature test plan document
├── wasm-apps/                               # Mirror original structure if exists
│   ├── [enhanced_test_files].wat           # New enhanced WAT test files
│   └── [enhanced_test_files].wasm          # Compiled enhanced test modules
└── [other_subdirs]/                         # Mirror any other subdirectories from original
```

**Directory Creation Protocol (MUST FOLLOW)**:
1. **Base Directory**: Create `tests/unit/enhanced_unit_test/[ModuleName]/`
2. **Structure Mirroring**: Copy directory structure from `tests/unit/[ModuleName]/`
3. **CMake Integration**: Copy CMakeLists.txt from original and modify for enhanced tests
4. **File Naming Convention**: Use `*_enhanced.cc` suffix for ALL new test files
5. **Isolation Principle**: ZERO modifications to existing `tests/unit/[ModuleName]/` files
6. **Build Integration**: Enhanced tests build independently from original tests

**Example Implementation for memory64 module**:
```bash
# Original structure (DO NOT MODIFY)
tests/unit/memory64/
├── CMakeLists.txt
├── test_memory64.cc
└── wasm-apps/
    ├── address_translation.wat
    └── address_translation.wasm

# Enhanced structure (CREATE NEW)
tests/unit/enhanced_unit_test/memory64/
├── CMakeLists.txt                           # Copied and modified from original
├── test_memory64_core_enhanced.cc           # Step 1: Core operations (≤20 cases)
├── test_memory64_advanced_enhanced.cc       # Step 2: Advanced operations (≤20 cases)
├── test_memory64_integration_enhanced.cc    # Step 3: Integration testing (≤20 cases)
├── memory64_feature_test_plan.md            # Feature enhancement plan
└── wasm-apps/                               # Enhanced test modules only
    ├── memory64_boundary_enhanced.wat       # New boundary test WAT
    ├── memory64_stress_enhanced.wat         # New stress test WAT
    ├── memory64_edge_cases_enhanced.wat     # New edge case test WAT
    └── [compiled_enhanced_wasm_files]       # Compiled enhanced modules
```

**Build Command for Enhanced Tests**:
```bash
# Build enhanced tests independently
cd tests/unit/enhanced_unit_test/[ModuleName]
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DWAMR_BUILD_COVERAGE=1
make
./[ENHANCED_EXECUTABLE_NAME]
```

Create a systematic sub-plan in `tests/unit/enhanced_unit_test/[ModuleName]/[ModuleName]_coverage_improve_plan.md`:

```markdown
# Code Coverage Improve Plan for [Module Name]

## Current Coverage Status
- Line Coverage: X/Y (Z%)
- Function Coverage: A/B (C%)
- Branch Coverage: D/E (F%)
- **Coverage Report**: `coverage_report/index.html`

## Uncovered Code Analysis

### Critical Uncovered Functions with Line Details
Extract from LCOV report and list each function with specific uncovered lines:

#### Function: `function_name1()`
- **File**: `core/iwasm/[module]/source_file.c`
- **Total Lines**: 45
- **Uncovered Lines**: 12, 15-18, 23, 27-31, 38, 42
- **Uncovered Line Count**: 12 lines
- **Priority**: HIGH (core functionality)

#### Function: `function_name2()`
- **File**: `core/iwasm/[module]/source_file.c`
- **Total Lines**: 28
- **Uncovered Lines**: 5-8, 14, 19-22, 26
- **Uncovered Line Count**: 9 lines
- **Priority**: MEDIUM (error handling)

#### Function: `function_name3()`
- **File**: `core/iwasm/[module]/source_file.c`
- **Total Lines**: 33
- **Uncovered Lines**: 3, 7-11, 18, 25-28, 31
- **Uncovered Line Count**: 11 lines
- **Priority**: HIGH (memory operations)

## Test Generation Sub-Plans

### Step Template Structure
#### Step N: [Segment Name] Functions (≤10 test cases)
**Target Functions with Line Coverage Goals**:

##### `function_name1()` - Target Lines: 12, 15-18, 23, 27-31, 38, 42
- [ ] test_function_name1_basic_operation → **Target Lines: 12, 15, 16**
- [ ] test_function_name1_error_handling → **Target Lines: 17, 18, 23**
- [ ] test_function_name1_boundary_conditions → **Target Lines: 27, 28, 29**
- [ ] test_function_name1_memory_allocation → **Target Lines: 30, 31, 38**
- [ ] test_function_name1_cleanup_path → **Target Lines: 42**

##### `function_name2()` - Target Lines: 5-8, 14, 19-22, 26
- [ ] test_function_name2_success_path → **Target Lines: 5, 6, 7**
- [ ] test_function_name2_validation_error → **Target Lines: 8, 14**
- [ ] test_function_name2_resource_error → **Target Lines: 19, 20, 21**
- [ ] test_function_name2_cleanup_error → **Target Lines: 22, 26**

##### `function_name3()` - Target Lines: 3, 7-11, 18, 25-28, 31
- [ ] test_function_name3_initialization → **Target Lines: 3, 7, 8**
- [ ] test_function_name3_processing_loop → **Target Lines: 9, 10, 11, 18**

**Line Coverage Mapping**:
```
Test Case Name                    | Target Lines           | Coverage Goal
test_function_name1_basic_op      | 12, 15, 16            | 3 lines
test_function_name1_error_handle  | 17, 18, 23            | 3 lines
test_function_name1_boundary      | 27, 28, 29            | 3 lines
test_function_name1_memory_alloc  | 30, 31, 38            | 3 lines
test_function_name1_cleanup       | 42                    | 1 line
test_function_name2_success       | 5, 6, 7               | 3 lines
test_function_name2_validation    | 8, 14                 | 2 lines
test_function_name2_resource      | 19, 20, 21            | 3 lines
test_function_name2_cleanup_err   | 22, 26                | 2 lines
test_function_name3_init          | 3, 7, 8               | 3 lines
```

**Step Metrics**:
- **Total Target Lines**: 32 uncovered lines
- **Expected Coverage**: 26+ lines (80%+ coverage rate)
- **Status**: PENDING/IN_PROGRESS/COMPLETED
- **Completion Criteria**: 
  - All 10 test cases pass
  - Coverage report shows ≥80% of target lines covered
  - Each test case covers its specific target lines

### Multi-Step Execution Protocol
1. **Step Preparation**: Identify target function segment and specific uncovered lines
2. **Line Analysis**: Study source code to understand how to reach each target line
3. **Test Generation**: Create ≤10 test cases, each targeting specific lines
4. **Build & Test**: Compile and run current step tests in isolation
5. **Line Coverage Verification**: Generate step-specific coverage report and verify target lines
6. **Coverage Validation**: Ensure ≥80% of target lines are covered
7. **Status Update**: Mark step as COMPLETED only when line coverage criteria met
8. **Next Step**: Proceed to next function segment only after current step completion
```

### Phase 3: Step-by-Step Execution

#### Execute One Step at a Time

1. **Generate test cases for current step (≤20 cases)**
   **MANDATORY**: Create tests in `tests/unit/enhanced_unit_test/[ModuleName]/test_[step_name]_enhanced.cc` and related CMakeLists.txt (If needed)
   
   **File Location Requirements**:
   - All new test files MUST be in `enhanced_unit_test/[ModuleName]/` directory
   - Use `*_enhanced.cc` naming convention for all test files
   - Copy and modify CMakeLists.txt from original module if needed
   - DO NOT modify any files in original `tests/unit/[ModuleName]/` directory

2. **Build with coverage**
```bash
cd tests/unit/enhanced_unit_test/[ModuleName]
rm -rf build/  # Clean previous build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DWAMR_BUILD_COVERAGE=1
make
```

3. **Run tests immediately after building successfully**
```bash
./[ENHANCED_EXECUTABLE_NAME] --gtest_filter="*[ModuleName]*[StepName]*"
```

4. **Generate coverage report for verification**
```bash
lcov --capture --directory . --output-file step_coverage.info
genhtml step_coverage.info --output-directory step_coverage_report
```

5. **Line-Specific Coverage Verification (CRITICAL)**
```bash
# Extract line coverage for specific functions
lcov --extract step_coverage.info "*source_file.c" --output-file function_coverage.info
genhtml function_coverage.info --output-directory function_coverage_report
```

**Verify Target Line Coverage**:
- Open `function_coverage_report/source_file.c.gcov.html`
- Check each target line number is marked as covered (green)
- Document which lines were successfully covered by each test
- Identify any target lines still uncovered (red)

6. **Coverage Validation Checklist**
- [ ] ≥80% of target lines covered in this step
- [ ] Each test case covers its designated target lines
- [ ] No regression in previously covered lines
- [ ] Coverage report shows improvement from baseline

7. **Update Line Coverage Status**
Update `[ModuleName]_coverage_improve_plan.md` with actual results:
```markdown
#### Step N Results:
**Target Lines**: 32 uncovered lines
**Achieved Coverage**: 28 lines covered (87.5%)
**Remaining Uncovered**: Lines 18, 31, 25, 28

**Test Case Results**:
- test_function_name1_basic_operation ✅ → Covered lines: 12, 15, 16
- test_function_name1_error_handling ✅ → Covered lines: 17, 23 (line 18 still uncovered)
- test_function_name1_boundary_conditions ✅ → Covered lines: 27, 29 (line 28 still uncovered)
```

8. **Step Completion Criteria**
Mark step as COMPLETED only when:
- [ ] All test cases pass
- [ ] ≥80% target line coverage achieved
- [ ] Specific line coverage documented
- [ ] Any uncovered lines identified for next iteration

### Phase 4: Issue Resolution Protocol (CRITICAL)
When generated test cases fail to build or run, follow this systematic fix protocol:

#### 4.1 Fix CMakeLists.txt Build Errors
- **FIRST**: Refer to other unit test module's CMakeLists.txt for patterns
- Check existing modules: aot/, shared-utils/, interpreter/, runtime-common/, etc.
- Copy working CMake configuration and adapt for current module
- Verify WAMR_BUILD_* flags match working examples
- Ensure all required source files and dependencies are included

#### 4.2 Fix Compilation Errors
- Check include paths and header file locations
- Verify test_helper.h usage and WAMR utility imports
- Fix C++ syntax errors and type mismatches
- Resolve missing function declarations or undefined symbols
- Add missing platform-specific conditional compilation

#### 4.3 Fix Test Logic Failures (MOST CRITICAL)
When tests fail during execution, systematically debug:

##### **Step 1: Analyze Test Failure Output**
```bash
./[EXECUTABLE_NAME] --gtest_filter="*[FailedTest]*" --gtest_output=xml:test_results.xml
```
- Read GTest failure messages carefully
- Identify whether it's assertion failure or runtime error
- Check expected vs actual values in failed assertions

##### **Step 2: Fix Common Test Issues**
```cpp
// ❌ Common Issue: Wrong expected values
EXPECT_EQ(0, result);  // If result is actually -1 for valid error case
// ✅ Fix: Use correct expected value
EXPECT_EQ(-1, result); // Or EXPECT_LT(result, 0) for error cases

// ❌ Common Issue: Uninitialized resources  
TEST_F(MyTest, SomeTest) {
    int result = some_function(uninitialized_ptr);  // Crash!
}
// ✅ Fix: Proper initialization
TEST_F(MyTest, SomeTest) {
    setup_valid_resource();
    int result = some_function(valid_ptr);
    EXPECT_GE(result, 0);
}

// ❌ Common Issue: Platform-specific failures
EXPECT_EQ(specific_value, platform_dependent_function());
// ✅ Fix: Handle platform differences
int result = platform_dependent_function();
if (result == -1) {
    GTEST_SKIP() << "Feature not available on this platform";
} else {
    EXPECT_GE(result, 0);
}
```

##### **Step 3: Validate Test Logic Against Source Code**
- **Read the actual function implementation** being tested
- **Analyze code paths to understand how to reach target lines**
- Verify expected return values match the source code behavior
- Check error conditions and edge cases in the source
- Ensure test parameters are valid for the function
- **Map test inputs to specific code paths and line numbers**
- **Verify conditional branches lead to target uncovered lines**

##### **Step 4: Fix Resource Management Issues**
```cpp
// ❌ Resource leak causing test pollution
TEST_F(ResourceTest, TestFunction) {
    int fd = open_resource();
    test_operation(fd);
    // Missing cleanup - affects next tests!
}

// ✅ Proper cleanup in TearDown
class ResourceTest : public ::testing::Test {
protected:
    void TearDown() override {
        if (fd > 0) {
            close_resource(fd);
            fd = -1;
        }
    }
    int fd = -1;
};
```

##### **Step 5: Iterative Fix Process**
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

#### 4.4 Test Quality Validation After Fixes
After fixing test failures, verify quality:
- [ ] Test still provides meaningful coverage (not just passes)
- [ ] Assertions verify specific expected outcomes
- [ ] Both success and error paths are tested
- [ ] No tautologies or always-true conditions
- [ ] Proper resource cleanup maintained
- [ ] Test names still describe scenario accurately

#### 4.5 Re-run Build and Test Cycle
```bash
# Clean rebuild to ensure no cached issues
rm -rf build/
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DWAMR_BUILD_COVERAGE=1
make

# Run specific fixed tests first
./[EXECUTABLE_NAME] --gtest_filter="*[FixedTestPattern]*"

# Then run full module test suite
./[EXECUTABLE_NAME] --gtest_filter="*[ModuleName]*"
```

#### 4.6 Mandatory Success Criteria
**Continue to next step ONLY after:**
- [ ] All generated tests compile without errors
- [ ] All tests pass when executed
- [ ] Tests provide genuine functionality verification
- [ ] No resource leaks or test pollution
- [ ] Coverage report shows improvement in target functions

**NEVER proceed with failing tests** - fix issues completely before moving forward.

### Phase 5: Progress Tracking
For each module, maintain status in `tests/unit/enhanced_unit_test/[ModuleName]/[ModuleName]_feature_test_plan.md`:

```markdown
## Overall Progress
- Total Steps: X
- Completed Steps: Y
- Current Step: Z
- Module Coverage Before: A%
- Module Coverage After: B%
- Target Coverage: C%

## Step Status
- [x] Step 1: Core Functions - COMPLETED (Date: YYYY-MM-DD)
- [x] Step 2: Error Handling - COMPLETED (Date: YYYY-MM-DD) 
- [ ] Step 3: Edge Cases - IN_PROGRESS
- [ ] Step 4: Integration Tests - PENDING
```

### Workflow Commands Summary

**Start coverage improvement for a module:**
```bash
cd tests/unit/enhanced_unit_test/[ModuleName]/
```

1. **Create [ModuleName]_feature_test_plan.md** if not exists or not the date is not latest
2. **Implement Step 1 test cases** code generate in enhanced directory
3. **Build, fix, test** in enhanced directory
4. **Verify coverage** with line-specific analysis:
```bash
make && ./[ENHANCED_EXECUTABLE_NAME] --gtest_filter="*[ModuleName]*" && \
lcov --capture --directory . --output-file temp.info && \
genhtml temp.info --output-directory temp_report && \
# Extract specific file coverage for line analysis
lcov --extract temp.info "*[target_source_file].c" --output-file file_coverage.info && \
genhtml file_coverage.info --output-directory line_report
```

5. **Validate Line Coverage Results**
```bash
# Check specific line coverage in the generated report
firefox line_report/[target_source_file].c.gcov.html
# Document covered vs uncovered target lines
```

6. **Update [ModuleName]_feature_test_plan.md** with line-specific results
7. **Proceed to Step 2** only after line coverage validation

## Final Coverage Collection
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

## Execution Checklist

### Before Starting Each Step:
- [ ] Target functions and uncovered lines identified
- [ ] Step plan created with ≤10 test cases
- [ ] Line coverage mapping documented
- [ ] Source code analyzed for reaching target lines

### During Step Execution:
- [ ] Tests generated with line-specific targeting
- [ ] Build successful without errors
- [ ] All tests pass execution
- [ ] Coverage report generated and analyzed
- [ ] Target line coverage verified (≥80%)

### After Step Completion:
- [ ] Results documented in improvement plan
- [ ] Coverage metrics updated
- [ ] Uncovered lines identified for next iteration
- [ ] Step marked as COMPLETED
- [ ] Ready to proceed to next step

### Final Validation:
- [ ] Overall module coverage improved
- [ ] All test cases provide meaningful verification
- [ ] No test pollution or resource leaks
- [ ] Documentation updated with results
- [ ] Coverage report reflects improvements