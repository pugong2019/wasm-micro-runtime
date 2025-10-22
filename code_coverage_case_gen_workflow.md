# WAMR Unit Test Coverage Enhancement Workflow

## Comprehensive Workflow for Improving Code Line Coverage in WAMR Modules

### Prerequisites
- WAMR source code repository
- CMake build system with coverage support
- lcov/gcov for coverage analysis
- GTest framework (automatically fetched)

### Overview
This workflow systematically improves code coverage by targeting uncovered lines, including functions, branches, and edge cases. It includes iterative optimization to ensure meaningful coverage improvements.

---

## Step-by-Step Workflow

### 1. **Get Uncovered Code Lines (Input Analysis)**

#### 1.1 Generate Initial Coverage Report
```bash
cd /path/to/wasm-micro-runtime/tests/unit

# Build with coverage enabled
cmake -S . -B build -DCOLLECT_CODE_COVERAGE=1
cmake --build build

# Run existing tests to establish baseline
ctest --test-dir build

# Generate comprehensive coverage report
lcov --capture --directory build --output-file baseline_coverage.info
lcov --remove baseline_coverage.info "*/test/*" "*/tests/*" --output-file filtered_coverage.info
genhtml filtered_coverage.info --output-directory baseline_report
```

#### 1.2 Identify Uncovered Lines
```bash
# Extract uncovered lines for target module
lcov --list filtered_coverage.info | grep "target_module.c"

# Get detailed line coverage for specific file
lcov --extract filtered_coverage.info "*/target_file.c" --output-file target_baseline.info

# Analyze uncovered lines (DA:line,0 indicates uncovered)
grep "DA:.*,0$" target_baseline.info > uncovered_lines.txt

# Extract uncovered functions (FNDA:0 indicates uncovered function)
grep "FNDA:0" target_baseline.info > uncovered_functions.txt
```

#### 1.3 Prioritize Coverage Targets
```bash
# Create prioritized list based on:
# - Critical error handling paths
# - Complex conditional branches  
# - Edge case scenarios
# - Functions with high cyclomatic complexity

# Example output format:
# File: core/iwasm/aot/aot_loader.c
# Uncovered Lines: 1234, 1245-1250, 1267, 1289-1295
# Uncovered Functions: validate_sections, handle_relocation_error
# Priority: HIGH (error handling), MEDIUM (edge cases), LOW (logging)
```

### 2. **Code Analysis**

#### 2.1 Analyze Uncovered Code Paths
```bash
# Examine uncovered lines in context
for line in $(cat uncovered_lines.txt | cut -d: -f2 | cut -d, -f1); do
    echo "=== Line $line ==="
    sed -n "$((line-2)),$((line+2))p" target_file.c
done
```

#### 2.2 Identify Code Categories
```cpp
// Categorize uncovered code:
// 1. ERROR_PATHS: Error handling and validation
// 2. EDGE_CASES: Boundary conditions and rare scenarios  
// 3. BRANCHES: Conditional logic paths
// 4. FUNCTIONS: Complete uncovered functions
// 5. CLEANUP: Resource cleanup and teardown paths

// Example analysis:
// Line 1234: if (sections == NULL) return NULL;     // ERROR_PATH
// Line 1245: if (section_count > MAX_SECTIONS)      // EDGE_CASE  
// Line 1267: cleanup_failed_module(module);         // CLEANUP
// Line 1289: validate_section_integrity(section);   // FUNCTION
```

#### 2.3 Understand Dependencies
```bash
# Map function dependencies and call chains
grep -n "function_name\|variable_name" target_file.c
grep -rn "struct_name\|typedef.*name" core/iwasm/include/

# Identify required test setup:
# - Data structures needed
# - Initialization requirements  
# - Mock dependencies
# - Resource cleanup needs
```

### 3. **Identify Test Module Structure**

#### 3.1 Locate/Create Test Module
```bash
# Check existing test structure
ls tests/unit/ | grep -E "(aot|interpreter|runtime|memory)" #or other modules

# For new modules, create structure:
mkdir -p tests/unit/[module-name]
cat > tests/unit/[module-name]/CMakeLists.txt << 'EOF'
# Standard WAMR test module CMakeLists.txt template
include (${IWASM_DIR}/compilation/iwasm_compl.cmake)
include (${SHARED_DIR}/utils/shared_utils.cmake)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file (GLOB_RECURSE source_all ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

set (UNIT_SOURCE ${source_all})

add_executable (${module}_test ${UNIT_SOURCE})

target_link_libraries (${module}_test ${LLVM_AVAILABLE_LIBS} ${UV_A_LIBS} vmlib -lm -ldl -lpthread ${lib_ubsan})
gtest_discover_tests(${module}_test)
EOF
```
# For existing modules, copy structure:
copy the esisting test code's header test fixture into a new code file named
enhanced_gen_[module]_test.cc (if not exist before)
and generate the case into the new file.

#### 3.2 Examine Existing Test Patterns
```bash
# Study successful test patterns in current or similar modules
head -50 tests/unit/aot/aot_test.cc
head -50 tests/unit/runtime-common/runtime_common_test.cc

# Identify common test utilities and helpers
grep -r "WAMRRuntimeRAII\|DummyExecEnv\|TestHelper" tests/unit/
```

### 4. **Create Unit Test Cases**

#### 4.1 Test Case Design Strategy
```cpp
// Design tests to target specific uncovered lines:

class ModuleTest : public testing::Test {
protected:
    void SetUp() override {
        RuntimeInitArgs init_args;
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_System_Allocator;
        
        ASSERT_TRUE(wasm_runtime_full_init(&init_args));
        setup_test_data();
    }
    
    void TearDown() override {
        cleanup_test_data();
        wasm_runtime_destroy();
    }
    
private:
    void setup_test_data() {
        // Initialize test data structures
        // Create mock dependencies
        // Prepare edge case scenarios
    }
    
    void cleanup_test_data() {
        // Clean up allocated resources
        // Reset global state
    }
    
    // Test data members
    char global_heap_buf[512 * 1024];
    TestDataStructure test_data;
};
```

#### 4.2 Target-Specific Test Cases
```cpp
// ERROR_PATH coverage: Test NULL/invalid inputs
TEST_F(ModuleTest, Function_NullInput_ReturnsError) {
    // Target: if (input == NULL) return ERROR;
    result_t result = target_function(NULL, valid_param);
    ASSERT_EQ(WASM_RUNTIME_ERROR_NULL_POINTER, result);
}

// EDGE_CASE coverage: Test boundary conditions  
TEST_F(ModuleTest, Function_MaxBoundary_HandlesCorrectly) {
    // Target: if (count > MAX_COUNT) return ERROR;
    uint32_t max_count = UINT32_MAX;
    result_t result = target_function(valid_input, max_count);
    ASSERT_EQ(WASM_RUNTIME_ERROR_OUT_OF_BOUNDS, result);
}

// BRANCH coverage: Test conditional paths
TEST_F(ModuleTest, Function_ConditionTrue_ExecutesTruePath) {
    // Target: if (condition) { true_path_code; }
    setup_condition_true();
    result_t result = target_function(test_input);
    ASSERT_TRUE(verify_true_path_executed());
}

TEST_F(ModuleTest, Function_ConditionFalse_ExecutesFalsePath) {
    // Target: if (condition) { } else { false_path_code; }
    setup_condition_false();
    result_t result = target_function(test_input);
    ASSERT_TRUE(verify_false_path_executed());
}

// CLEANUP coverage: Test resource cleanup paths
TEST_F(ModuleTest, Function_FailureScenario_CleansUpResources) {
    // Target: cleanup_resources(); return ERROR;
    force_internal_failure();
    result_t result = target_function(test_input);
    ASSERT_EQ(WASM_RUNTIME_ERROR_INTERNAL, result);
    ASSERT_TRUE(verify_resources_cleaned());
}
```

### 5. **Build Configuration (Module-Specific)**

#### 5.1 Incremental Build
```bash
cd tests/unit

# Build only the target module to save time
cmake --build build --target [module]_test

# Verify build success
echo $? # Should be 0 for success
```

#### 5.2 Build Verification
```bash
# Check executable exists and links properly
ls -la build/[module]/[module]_test
ldd build/[module]/[module]_test # Check dependencies

# Quick smoke test
./build/[module]/[module]_test --gtest_list_tests
```

### 6. **Fix Build Errors**

#### 6.1 Common Build Issues
```bash
# Missing includes
grep -n "#include" tests/unit/[module]/[module]_test.cc
# Add missing: #include "wasm_runtime.h", #include "aot_loader.h"

# Undefined symbols  
nm build/[module]/[module]_test | grep " U "
# Check CMakeLists.txt for missing libraries

# Compilation errors
cmake --build build --target [module]_test 2>&1 | tee build_errors.log
# Fix syntax, type mismatches, missing declarations
```

#### 6.2 Build Error Resolution
```bash
# Iterative fix approach:
while ! cmake --build build --target [module]_test; do
    echo "Build failed, analyzing errors..."
    # Fix one error at a time
    # Re-run build
    # Continue until success
done
```

### 7. **Execute Tests (New Cases Only)**

#### 7.1 Targeted Test Execution
```bash
# Run only newly added test cases
./build/[module]/[module]_test --gtest_filter="*TargetFunction*" --gtest_brief=1

# Verify test discovery
./build/[module]/[module]_test --gtest_list_tests | grep -i target
```

#### 7.2 Test Execution Verification
```bash
# Expected output format:
# [==========] Running X tests from 1 test suite.
# [----------] X tests from ModuleTest  
# [ RUN      ] ModuleTest.Function_Scenario_ExpectedOutcome
# [       OK ] ModuleTest.Function_Scenario_ExpectedOutcome
# [==========] X tests from 1 test suite ran.
# [  PASSED  ] X tests.
```

### 8. **Fix Runtime Issues**

#### 8.1 Handle Crashes and Segfaults
```bash
# Run with debugging
gdb --args ./build/[module]/[module]_test --gtest_filter="*failing_test*"
# (gdb) run
# (gdb) bt  # Get backtrace on crash

# Common crash causes:
# - Uninitialized pointers: Check SetUp() initialization
# - Memory corruption: Verify buffer sizes and bounds
# - Double free: Check TearDown() cleanup logic
# - Stack overflow: Reduce recursive calls or increase stack
```

#### 8.2 Fix Failed Test Cases
```bash
# Analyze test failures
./build/[module]/[module]_test --gtest_filter="*failing_test*" 2>&1 | tee test_failures.log

# Common failure patterns:
# ASSERT_EQ failures: Check expected vs actual values
# ASSERT_TRUE failures: Verify condition logic
# Timeout failures: Reduce test complexity or increase timeout
```

#### 8.3 Iterative Issue Resolution
```cpp
// Debug approach for each failing test:
TEST_F(ModuleTest, Function_Scenario_ExpectedOutcome) {
    // Add debug output
    printf("Debug: Input value = %d\n", test_input);
    
    // Verify preconditions
    ASSERT_NE(nullptr, test_input);
    ASSERT_GT(buffer_size, 0);
    
    // Execute with error checking
    result_t result = target_function(test_input);
    
    // Debug actual result
    printf("Debug: Actual result = %d, Expected = %d\n", result, expected);
    
    ASSERT_EQ(expected_result, result);
}
```

### 9. **Generate Coverage Report**

#### 9.1 Fresh Coverage Collection
```bash
# Clear previous coverage data
find build/[module] -name "*.gcda" -delete

# Run tests to generate fresh coverage
./build/[module]/[module]_test --gtest_filter="*TargetFunction*"

# Capture coverage for target module only
lcov --capture --directory build/[module] --output-file new_coverage.info
```

#### 9.2 Coverage Analysis
```bash
# Extract target file coverage
lcov --extract new_coverage.info "*/target_file.c" --output-file target_new_coverage.info

# Generate detailed HTML report
genhtml target_new_coverage.info --output-directory new_coverage_report --show-details

# Compare with baseline
lcov --diff baseline_coverage.info target_new_coverage.info --output-file coverage_diff.info
```

### 10. **Verify Coverage Improvement**

#### 10.1 Quantitative Verification
```bash
# Check line coverage improvement
lcov --summary target_new_coverage.info > new_summary.txt
lcov --summary target_baseline.info > baseline_summary.txt

# Calculate improvement
echo "Baseline coverage:"
cat baseline_summary.txt
echo "New coverage:"  
cat new_summary.txt

# Verify specific lines are now covered
grep "DA:.*,[1-9]" target_new_coverage.info | wc -l  # Covered lines
grep "DA:.*,0$" target_new_coverage.info | wc -l     # Still uncovered
```

#### 10.2 Qualitative Verification
```bash
# Verify target lines are covered
for line in $(cat uncovered_lines.txt | cut -d: -f2 | cut -d, -f1); do
    coverage=$(grep "DA:$line," target_new_coverage.info | cut -d, -f2)
    if [ "$coverage" -gt 0 ]; then
        echo "✅ Line $line now covered ($coverage executions)"
    else
        echo "❌ Line $line still uncovered"
    fi
done
```

### 11. **Iterative Optimization (Max 3 Iterations)**

#### 11.1 Root Cause Analysis for Insufficient Coverage
```bash
# If coverage didn't improve, analyze why:

# Check if tests actually execute target code paths
echo "=== Test Execution Analysis ==="
gdb --batch --ex run --ex bt --args ./build/[module]/[module]_test --gtest_filter="*target*"

# Verify test data reaches target conditions
grep -n "if\|switch\|for\|while" target_file.c | head -10
# Ensure test cases trigger these conditions
```

#### 11.2 Test Optimization Strategies

**Iteration 1: Enhance Test Data**
```cpp
// Add more comprehensive test scenarios
TEST_F(ModuleTest, Function_ComplexScenario_CoversMorePaths) {
    // Use more realistic test data
    // Trigger multiple code paths in single test
    // Add boundary value testing
}
```

**Iteration 2: Add Missing Edge Cases**  
```cpp
// Target specific uncovered branches
TEST_F(ModuleTest, Function_RareCondition_ExecutesSpecialPath) {
    // Force rare conditions that normal tests miss
    // Use mock objects to simulate failure scenarios
    // Test error recovery paths
}
```

**Iteration 3: Integration-Style Testing**
```cpp
// Use broader integration approach if unit tests insufficient
TEST_F(ModuleTest, Function_IntegrationScenario_CoversComplexFlow) {
    // Test with real WASM modules
    // Use complete execution contexts
    // Test full workflows that exercise target code
}
```

#### 11.3 Iteration Tracking
```bash
# Track progress across iterations
echo "Iteration 1 - Baseline: $(grep 'lines......:' baseline_summary.txt)"
echo "Iteration 1 - Result:   $(grep 'lines......:' iteration1_summary.txt)"
echo "Iteration 2 - Result:   $(grep 'lines......:' iteration2_summary.txt)"  
echo "Iteration 3 - Result:   $(grep 'lines......:' iteration3_summary.txt)"

# Stop after 3 iterations or when target coverage achieved
if [ $iteration -eq 3 ] || [ $coverage_improvement -gt $target_threshold ]; then
    echo "Optimization complete: $coverage_improvement% improvement achieved"
    exit 0
fi
```

---

## Success Criteria

### Quantitative Metrics
- ✅ **Line Coverage**: Target lines show `DA:line,>0` in coverage report
- ✅ **Function Coverage**: Functions show `FNDA:>0,function_name`  
- ✅ **Branch Coverage**: Conditional branches covered in both directions
- ✅ **Improvement**: Measurable increase in coverage percentage

### Qualitative Metrics
- ✅ **All Tests Pass**: No failing test cases
- ✅ **No Crashes**: Tests execute without segfaults or core dumps
- ✅ **Real Functionality**: Tests validate actual WAMR behavior
- ✅ **Maintainable**: Tests follow WAMR coding standards

---

## Key Standards & Best Practices

### Testing Standards
- **Use ASSERT_* not EXPECT_***: For definitive pass/fail validation
- **Never use GTEST_SKIP()**: Handle unsupported features with early return
- **Follow naming**: `TEST_F(ModuleTest, Function_Scenario_ExpectedOutcome)`
- **Resource management**: Proper SetUp/TearDown with RAII patterns
- **Platform awareness**: Handle platform differences gracefully

### Coverage Standards
- **Target meaningful coverage**: Focus on error paths and edge cases
- **Avoid coverage gaming**: Don't create tests just to execute code
- **Iterative improvement**: Use 3-iteration optimization cycle
- **Document rationale**: Explain why certain lines remain uncovered

---

## Reusable Commands & Scripts

### Quick Coverage Check Script
```bash
#!/bin/bash
# check_line_coverage.sh - Check coverage for specific lines
function check_line_coverage() {
    local target_file=$1
    local module=$2
    local lines=$3  # Comma-separated list: "123,456,789"
    
    cd /path/to/wasm-micro-runtime/tests/unit
    
    # Run tests
    ./build/${module}/${module}_test
    
    # Generate coverage
    lcov --capture --directory build/${module} --output-file temp_coverage.info
    lcov --extract temp_coverage.info "*/${target_file}" --output-file filtered_coverage.info
    
    # Check each line
    IFS=',' read -ra LINE_ARRAY <<< "$lines"
    for line in "${LINE_ARRAY[@]}"; do
        coverage=$(grep "DA:$line," filtered_coverage.info | cut -d, -f2)
        if [ "$coverage" -gt 0 ] 2>/dev/null; then
            echo "✅ Line $line: covered ($coverage executions)"
        else
            echo "❌ Line $line: not covered"
        fi
    done
}

# Usage: check_line_coverage "aot_loader.c" "aot" "1234,1245,1267"
```


## Module-Specific Considerations

### AOT Module (`core/iwasm/aot/`)
- **Focus Areas**: Section validation, relocation handling, module loading
- **Common Uncovered**: Error cleanup paths, edge case validations
- **Test Strategy**: Use malformed AOT sections, trigger allocation failures

### Interpreter Module (`core/iwasm/interpreter/`)  
- **Focus Areas**: Instruction execution, stack operations, memory access
- **Common Uncovered**: Exception handling, stack overflow recovery
- **Test Strategy**: Use edge case WASM bytecode, trigger runtime errors

### Runtime Common (`core/iwasm/common/`)
- **Focus Areas**: Module lifecycle, execution environment management  
- **Common Uncovered**: Resource cleanup, multi-threading edge cases
- **Test Strategy**: Test concurrent access, resource exhaustion scenarios

### Memory Management (`core/iwasm/common/wasm_memory.c`)
- **Focus Areas**: Linear memory operations, heap management, bounds checking
- **Common Uncovered**: Memory growth edge cases, allocation failure paths
- **Test Strategy**: Test memory limits, fragmentation scenarios, OOM conditions

---

This comprehensive workflow ensures systematic improvement of code coverage by targeting specific uncovered lines rather than just functions, with built-in iteration and optimization to achieve meaningful coverage improvements.