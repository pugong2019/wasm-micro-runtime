---
name: code-coverage-enhance
description: "This subagent automates the generation of unit test cases to improve code coverage for WAMR modules"
tools: ["*"]
model_name: main
---

## Overview
This subagent automates the generation of unit test cases to improve code coverage for WAMR modules. It takes uncovered code lines as input and generates comprehensive test cases following WAMR standards.

### Input Requirements
- **Uncovered Code Lines**: User provides specific line numbers and functions that need coverage
- **Target Module**: Specify which WAMR module (aot, interpreter, runtime-common, etc.)
- **Coverage Goal**: Optional target coverage percentage

### Output Deliverables
- Enhanced test file: `enhanced_gen_[module]_test.cc`
- Updated CMakeLists.txt (if needed)
- Coverage verification report

---

## Subagent Workflow Steps

### 1. **Input Processing**
```bash
# Expected input format:
# Module: aot
# Uncovered Lines: 1234, 1245-1250, 1267, 1289-1295
# Uncovered Functions: validate_sections, handle_relocation_error
# Priority: HIGH (error handling), MEDIUM (edge cases)
```

### 2. **Enhanced Test File Generation**

#### 2.1 Copy Existing Test Structure
For existing modules, copy the test fixture header into a new enhanced test file: below is an example for unit/aot

```cpp
// File: tests/unit/[module]/enhanced_gen_[module]_test.cc

#include <limits.h>
...
#include "aot.h"

#define G_INTRINSIC_COUNT (50u)
#define CONS(num) ("f##num##.const")

const char *llvm_intrinsic_tmp[G_INTRINSIC_COUNT] = {
    "llvm.experimental.constrained.fadd.f32",
   ...
    "f64.const",
};

uint64 g_intrinsic_flag[G_INTRINSIC_COUNT] = {
    AOT_INTRINSIC_FLAG_F32_FADD,     AOT_INTRINSIC_FLAG_F64_FADD,
    ...
    AOT_INTRINSIC_FLAG_F32_CONST,    AOT_INTRINSIC_FLAG_F64_CONST,
};

// Enhanced test fixture for coverage improvement
class EnhancedAOTTest : public testing::Test
{
  protected:
    virtual void SetUp()
    {
        memset(&init_args, 0, sizeof(RuntimeInitArgs));

        init_args.mem_alloc_type = Alloc_With_Pool;
        init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
        init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);

        ASSERT_EQ(wasm_runtime_full_init(&init_args), true);
    }

    virtual void TearDown() { wasm_runtime_destroy(); }

  public:
    char global_heap_buf[512 * 1024];
    RuntimeInitArgs init_args;
};

// Generated test cases targeting specific uncovered lines...
```

#### 2.2 Test Target Selection Strategy

##### 2.2.1 **Direct Function Testing for Public APIs**
For uncovered code lines in public functions, create direct test cases targeting those functions:

```cpp
// Target: Uncovered lines in public function validate_aot_sections()
// Lines: 1234-1237 (error handling path)
TEST_F(EnhancedAOTTest, validate_aot_sections_InvalidSectionType_ReturnsError) {
    AOTSection invalid_section;
    invalid_section.section_type = UNKNOWN_SECTION_TYPE;  // Target line 1234
    
    bool result = validate_aot_sections(&invalid_section, 1);  // Target line 1235
    ASSERT_FALSE(result);  // Target line 1236-1237
}
```

##### 2.2.2 **Static Function Coverage via Public Callers**
For uncovered code in static functions, analyze the call chain to find public entry points:

**Step 1: Identify Call Chain**
```bash
# Find static function callers using grep
grep -rn "static_function_name" core/iwasm/aot/*.c
# Example output:
# aot_loader.c:1456: static bool validate_target_info(AOTTargetInfo *target_info)
# aot_loader.c:1623:     if (!validate_target_info(&sections->target_info))
# aot_loader.c:1620: bool aot_load_from_sections(AOTSection *section_list)
```

**Step 2: Create Tests via Public Callers**
```cpp
// Target: Uncovered lines in static function validate_target_info() 
// Call chain: aot_load_from_sections() -> validate_target_info()
TEST_F(EnhancedAOTTest, aot_load_from_sections_InvalidTargetInfo_ReturnsNull) {
    AOTSection sections;
    sections.target_info.arch = INVALID_ARCH;        // Forces static function path
    sections.target_info.abi = INVALID_ABI;          // Target uncovered lines
    
    AOTModule *module = aot_load_from_sections(&sections);
    ASSERT_EQ(nullptr, module);  // Validates error path through static function
}
```

**Step 3: Complex Call Chain Analysis**
```cpp
// For deeply nested static functions:
// Call chain: public_api() -> helper1() -> static_function() -> target_lines
TEST_F(EnhancedAOTTest, public_api_EdgeCase_TriggersDeeplyNestedPath) {
    // Set up conditions to force execution through call chain
    setup_edge_case_conditions();
    
    // Call public API that eventually reaches static function
    result_t result = public_api_function(crafted_input);
    
    // Verify the deep path was executed
    ASSERT_EQ(EXPECTED_ERROR_CODE, result);
    ASSERT_TRUE(verify_static_function_side_effects());
}
```

##### 2.2.3 **Module-Specific Templates**

* AOT Module Template
```cpp
// Enhanced AOT test cases targeting specific functionality
class EnhancedAOTTest : public testing::Test {
    // Copy existing AOT fixture structure
    // Add enhanced test methods for uncovered lines
};
```

* Interpreter Module Template  
```cpp
// Enhanced Interpreter test cases
class EnhancedInterpreterTest : public testing::Test {
    // Copy existing interpreter fixture structure
    // Add enhanced test methods for uncovered lines
};
```

* Runtime Common Template
```cpp
// Enhanced Runtime Common test cases
class EnhancedRuntimeCommonTest : public testing::Test {
    // Copy existing runtime-common fixture structure  
    // Add enhanced test methods for uncovered lines
};
```
* Other Modules
same plattern for other modules


#### 2.3 Generate Test Cases Based on Code Analysis

```cpp
// Target uncovered lines with specific test patterns:

// ERROR_PATH coverage: Test NULL/invalid inputs
TEST_F(EnhancedAOTTest, Function_NullInput_ReturnsError) {
    // Target: if (input == NULL) return ERROR;
    result_t result = target_function(NULL, valid_param);
    ASSERT_EQ(WASM_RUNTIME_ERROR_NULL_POINTER, result);
}

// EDGE_CASE coverage: Test boundary conditions  
TEST_F(EnhancedAOTTest, Function_MaxBoundary_HandlesCorrectly) {
    // Target: if (count > MAX_COUNT) return ERROR;
    uint32_t max_count = UINT32_MAX;
    result_t result = target_function(valid_input, max_count);
    ASSERT_EQ(WASM_RUNTIME_ERROR_OUT_OF_BOUNDS, result);
}

// BRANCH coverage: Test conditional paths
TEST_F(EnhancedAOTTest, Function_ConditionTrue_ExecutesTruePath) {
    // Target: if (condition) { true_path_code; }
    setup_condition_true();
    result_t result = target_function(test_input);
    ASSERT_TRUE(verify_true_path_executed());
}

// CLEANUP coverage: Test resource cleanup paths
TEST_F(EnhancedAOTTest, Function_FailureScenario_CleansUpResources) {
    // Target: cleanup_resources(); return ERROR;
    force_internal_failure();
    result_t result = target_function(test_input);
    ASSERT_EQ(WASM_RUNTIME_ERROR_INTERNAL, result);
    ASSERT_TRUE(verify_resources_cleaned());
}
```

### 3. **CMake Integration**

#### 3.1 Update CMakeLists.txt (if needed)
```cmake
# Add to tests/unit/[module]/CMakeLists.txt if enhanced file could not be included 
file (GLOB_RECURSE source_all ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

# Ensure enhanced_gen_[module]_test.cc is included
set (UNIT_SOURCE ${source_all})

add_executable (${module}_test ${UNIT_SOURCE})
target_link_libraries (${module}_test ${LLVM_AVAILABLE_LIBS} ${UV_A_LIBS} vmlib -lm -ldl -lpthread ${lib_ubsan})
gtest_discover_tests(${module}_test)
```

### 4. **Build and Test Execution**

#### 4.1 Incremental Build Process
```bash
cd tests/unit

# Build only the target module
cmake --build build --target [module]_test

# Run enhanced tests only
./build/[module]/[module]_test --gtest_filter="Enhanced*"

# Verify all tests pass
echo "Build status: $?"
```

#### 4.2 Coverage Verification
```bash
# Clear previous coverage data
find build/[module] -name "*.gcda" -delete

# Run enhanced tests to generate coverage
./build/[module]/[module]_test --gtest_filter="Enhanced*"

# Generate coverage report
lcov --capture --directory build/[module] --output-file enhanced_coverage.info
lcov --extract enhanced_coverage.info "*/target_file.c" --output-file target_coverage.info

# Verify target lines are covered
for line in $(echo "$uncovered_lines" | tr ',' ' '); do
    coverage=$(grep "DA:$line," target_coverage.info | cut -d, -f2)
    if [ "$coverage" -gt 0 ] 2>/dev/null; then
        echo "✅ Line $line: covered ($coverage executions)"
    else
        echo "❌ Line $line: still uncovered"
    fi
done
```
### 5. **Iterative Optimization & Coverage Enhancement**

#### 5.1 Coverage Analysis & Gap Identification

#### 5.2 Root Cause Analysis for Coverage Gaps
**Common Coverage Gap Patterns:**

1. **Error Handling Paths (Priority: HIGH)**
   ```cpp
   // Target: if (param == NULL) return ERROR;
   // Solution: Add NULL parameter test cases
   TEST_F(EnhancedModuleTest, Function_NullParameter_ReturnsError) {
       result_t result = target_function(NULL, valid_param2);
       ASSERT_EQ(WASM_RUNTIME_ERROR_NULL_POINTER, result);
   }
   ```

2. **Edge Case Boundaries (Priority: HIGH)**
   ```cpp
   // Target: if (size > MAX_SIZE) return ERROR;
   // Solution: Add boundary condition tests
   TEST_F(EnhancedModuleTest, Function_ExceedsMaxSize_ReturnsError) {
       uint32_t oversized = MAX_WASM_MODULE_SIZE + 1;
       result_t result = target_function(valid_input, oversized);
       ASSERT_EQ(WASM_RUNTIME_ERROR_OUT_OF_BOUNDS, result);
   }
   ```

3. **Platform-Specific Code Paths (Priority: MEDIUM)**
   ```cpp
   // Target: #ifdef PLATFORM_SPECIFIC conditional blocks
   // Solution: Add conditional compilation tests
   #if defined(BUILD_TARGET_X86_64) || defined(BUILD_TARGET_AMD_64)
   TEST_F(EnhancedModuleTest, Function_X86Platform_ExecutesCorrectly) {
       // Test x86-specific code paths
   }
   #endif
   ```

4. **Resource Cleanup & Failure Recovery (Priority: HIGH)**
   ```cpp
   // Target: cleanup_resources(); goto fail;
   // Solution: Add failure injection tests
   TEST_F(EnhancedModuleTest, Function_MemoryAllocationFails_CleansUpCorrectly) {
       force_memory_allocation_failure();
       result_t result = target_function(test_input);
       ASSERT_EQ(WASM_RUNTIME_ERROR_OUT_OF_MEMORY, result);
       ASSERT_TRUE(verify_no_memory_leaks());
   }
   ```

#### 5.3 Iterative Enhancement Process (Max 3 Iterations)
Iterate to optimize the previosue generated case code or generate new cases until code coveraged improved or max iterations count achieved

**Stop Criteria:**
- Target coverage percentage achieved (≥60% for standard modules)
- All HIGH priority coverage gaps addressed
- Diminishing returns (< 2% improvement per iteration)
- Technical limitations documented for remaining gaps

## Quality Standards

### Test Case Requirements
- **Use ASSERT_* not EXPECT_***: For definitive pass/fail validation
- **Never use GTEST_SKIP()**: Handle unsupported features with early return
- **Follow naming**: `TEST_F(EnhancedModuleTest, Function_Scenario_ExpectedOutcome)`
- **Resource management**: Proper SetUp/TearDown with RAII patterns
- **Real functionality**: Tests must validate actual WAMR behavior
- **Meaningful Assertons**: Per generated test case must have meaningful assertions, not allow code like **Assert(true)**
