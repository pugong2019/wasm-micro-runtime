# LLM-Driven WASM Opcode Testing Framework

This document translates the script-based opcode test workflow into LLM prompt content for automated WASM opcode test generation in the WAMR project.

## Framework Overview

The LLM-driven framework consists of 6 sequential phases, each with specific prompts to guide the LLM through comprehensive opcode test generation:

1. **Phase 1**: Ultra-Deep Opcode Analysis
2. **Phase 2**: Strategic Test Planning
3. **Phase 3**: Complete Code Generation
4. **Phase 4**: Build & Test Execution
5. **Phase 5**: Issue Detection & Resolution                                                                                            
6. **Phase 6**: Code Review & Standardized Commit

---

## Input requirements:
*  {OPCODE_NAME}` with the target opcode (e.g., "i32.eqz", "br_if", "v128.add")

## Workflow

### Phase 1: Ultra-Deep Opcode Analysis
Ultra-Deep Opcode Analysis
Given the opcode "{OPCODE_NAME}", perform comprehensive analysis:

## 1.1 Semantic Analysis
Analyze the opcode semantics by:
- Identifying the opcode's primary function and purpose
- Determining input/output stack behavior
- Understanding the mathematical/logical operation performed
- Documenting any special behaviors or side effects

## 1.2 Type System Analysis
Examine type behavior by:
- Identifying input operand types (i32, i64, f32, f64, v128, etc.)
- Determining result type(s)
- Analyzing type conversion requirements
- Checking for type polymorphism

## 1.3 Stack Effect Analysis
Map stack transformations by:
- Documenting stack height changes (pop count, push count)
- Identifying stack operand positions
- Verifying stack type consistency requirements
- Noting any conditional stack effects

## 1.4 Edge Case Discovery
Identify potential edge cases including:
- Boundary values (MIN/MAX for numeric types)
- Special numeric values (NaN, Infinity for floats)
- Zero operations (division by zero, etc.)
- Overflow/underflow conditions
- Memory boundary conditions (for memory opcodes)

## 1.5 Error Condition Mapping
Map all possible error conditions:
- Type mismatches
- Stack underflow scenarios
- Out-of-bounds access (memory/table opcodes)
- Division by zero traps
- Invalid conversion traps
- Unreachable code scenarios

## 1.6 Category Classification
Classify the opcode into one of these categories:
- numeric/constants, numeric/comparison, numeric/arithmetic, numeric/bitwise
- memory/load, memory/store, memory/control
- control-flow/structured, control-flow/branching, control-flow/calls
- variable/local, variable/global
- parametric, reference, table
- extension/simd, extension/atomic
- runtime-specific

Provide detailed analysis with specific examples and test scenarios for each area.


---
### Phase 2: Ultra-Comprehensive Test Strategy Generation
Based on the opcode analysis from Phase 1, generate a comprehensive test strategy for "{OPCODE_NAME}" in category "{CATEGORY}":
Generate comprehensive test specifications that ensure thorough validation of the opcode's behavior across all scenarios.

## 2.1 Main Routine Test Cases
Design core functionality tests:
- Basic operation validation with typical input values
- Type consistency verification across all execution modes (interpreter, fast-jit, llvm-jit)
- Standard use case scenarios that exercise the opcode's primary function
- Positive test cases that should succeed under normal conditions

## 2.2 Corner Case Test Cases
Design boundary condition tests:
- Integer overflow/underflow scenarios for arithmetic opcodes
- Division by zero handling for division opcodes
- Signed/unsigned boundary interpretations
- Memory boundary access for memory opcodes
- Maximum nesting depths for control flow opcodes
- Branch table edge indices for branching opcodes

## 2.3 Edge Case Test Cases
Design extreme scenario tests:
- Zero operand behaviors (0 + x, 0 * x, x / 0)
- Mathematical identity cases (x + 0, x * 1, x - 0)
- Extreme numeric values (MAX_VALUE, MIN_VALUE, INFINITY, NaN)
- Empty memory or zero-sized memory operations
- Maximum memory address access scenarios

## 2.4 Error Exception Test Cases
Design failure scenario tests:
- Invalid operand types on stack
- Stack underflow conditions (insufficient operands)
- Out-of-bounds memory access
- Invalid memory instance operations
- Invalid branch targets for control flow
- Type mismatches in control structures

For each category, provide:
1. Detailed test case descriptions
2. Specific input conditions and setup
3. Expected error types and trap conditions
4. Meaningful assertion statements
5. WASM module requirements for testing

For each test case, specify:
- Test description and purpose
- Input values and setup requirements
- Expected outcomes and behaviors
- Specific assertions to validate correctness

## Phase 3: Complete Test Suite Code Generation

Based on the test strategy from Phase 2, generate a complete, working test suite for "{OPCODE_NAME}" in category "{CATEGORY}":

## 3.1 Directory Structure Creation
Create the following directory structure:
- tests/unit/enhanced_opcode/{CATEGORY}/
- tests/unit/enhanced_opcode/{CATEGORY}/wasm-apps/
- Include all necessary files for compilation and testing

Final Direture shoul be like(in enhanced_opcode/{CATEGORY}):
├── CMakeLists.txt
├── enhanced_{opcode}_test.cc
├── {helpper}.h (If need)
└── wasm-apps
    ├── *.wasm
    ├── *.wat

## 3.2 Common Header File Generation
Generate {OPCODE_NAME}_common.h with:
- Required WAMR includes (#include "wasm_runtime.h", etc.)
- Test utility macros and helper functions
- Common test data structures
- WAMRRuntimeRAII template for resource management
- Running mode definitions and support arrays

## 3.3 Test Implementation Generation
Generate enhanced_{OPCODE_NAME}_test.cc with:
- GTest test class inheriting from testing::TestWithParam<RunningMode>
- Proper SetUp() and TearDown() methods with WAMR initialization
- All test cases from the strategy (main routine, corner, edge, error cases)
- Meaningful ASSERT_* statements (No SUCCEED(), FAIL(), GTEST_SKIP()...)
- Proper resource management and cleanup
- Parameterized test instantiation for all running modes

### 3.3.1 Required Test Structure
```cpp
class {OPCODE_NAME}_test_suite : public testing::TestWithParam<RunningMode>
{
protected:
    // Setup/teardown with proper WAMR initialization
    virtual void SetUp() override;
    virtual void TearDown() override;

    // Helper methods for WASM loading and execution
    bool load_wasm_file(const char *wasm_file);
    bool init_exec_env();
    void destroy_exec_env();

    // Test resources
    RuntimeInitArgs init_args;
    wasm_module_t module = NULL;
    wasm_module_inst_t module_inst = NULL;
    wasm_exec_env_t exec_env = NULL;
    char error_buf[128];
    char global_heap_buf[512 * 1024];
    bool cleanup = true;
};
```

### 3.3.2 Assertion Guidelines
- Use ASSERT_EQ, ASSERT_TRUE, ASSERT_FALSE for definitive validation
- NO GTEST_SKIP() or SUCCEED() calls
- Include meaningful error messages in assertions
- Validate actual WAMR functionality, not just code execution

## 3.4 WASM Test File Generation
Generate {OPCODE_NAME}_test.wat containing:
- WebAssembly Text format module for testing the opcode
- Export functions for each test scenario
- Proper memory setup if required
- Table setup for table opcodes
- Global variables if needed
- All test data embedded in the WASM module

## 3.5 CMakeLists.txt Configuration
Generate CMakeLists.txt with:
- Proper WAMR dependencies
- Test executable configuration
- WASM file compilation (wat2wasm)
- Coverage collection support
- Platform-specific configurations

## 3.6 Code Quality Requirements
Ensure all generated code:
- Follows WAMR coding standards
- Includes proper error handling
- Uses RAII for resource management
- Has no memory leaks
- Compiles without warnings
- Passes all static analysis

Generate complete, production-ready code that can be immediately compiled and run without modifications.


## Phase 4: Build & Test Execution

Execute the following build and test sequence for the generated "{OPCODE_NAME}" test suite:


## 4.1 Build Environment Setup
Navigate to the WAMR unit test directory and configure the build:
1. Change directory to `tests/unit/`
2. Configure CMake build with coverage collection enabled
3. Use Debug build type for maximum debugging information
4. Enable parallel build compilation

Execute these bash commands:
```bash
cd tests/unit/
cmake -S enhanced_opcode/{CATEGORY} -B build/enhanced_opcode/{CATEGORY} \
      -DCOLLECT_CODE_COVERAGE=1 \
      -DCMAKE_BUILD_TYPE=Debug
```

## 4.2 Compilation Process
Build the test suite with parallel compilation:
```bash
cmake --build build/enhanced_opcode/{CATEGORY} --parallel $(nproc)
```

Monitor for:
- Compilation errors and warnings
- Missing dependencies
- Linking issues
- WASM file compilation (wat2wasm) success

## 4.3 Test Execution
Run the complete test suite with detailed output:
```bash
ctest --test-dir build/enhanced_opcode/{CATEGORY} \
      --output-on-failure \
      --verbose
```

Capture and analyze:
- Test pass/fail results
- Assertion failures and error messages
- Runtime exceptions or crashes
- Performance metrics
- Memory usage patterns

## 4.4 Build Validation Checklist
Verify the following success criteria:
- [ ] CMake configuration completes without errors
- [ ] All source files compile without warnings
- [ ] WASM test files compile successfully (wat2wasm)
- [ ] All unit tests pass (0 failures)
- [ ] No runtime crashes or memory leaks detected
- [ ] Coverage report generates successfully
- [ ] Coverage improvement is measurable

## 4.6 Failure Handling
If build or tests fail:
1. Capture detailed error messages and logs
2. Identify the failure category (compilation, linking, runtime, assertion)
3. Document specific file and line number information
4. Prepare for Phase 5 issue resolution




## Phase 5: Intelligent Issue Detection & Resolution

Apply this phase only if Phase 4 reports build failures or test issues.

## 5.1 Issue Detection Categories

### 5.1.1 Compilation Issues Analysis
If compilation failed, analyze:
- Missing include files or headers
- Undefined symbols or linking errors
- Syntax errors in generated code
- CMake configuration problems
- WASM compilation failures (wat2wasm)
- Compiler warnings that should be addressed

### 5.1.2 Runtime Issues Analysis
If tests crash or fail at runtime:
- Segmentation faults or memory access violations
- Null pointer dereferences
- Stack overflow conditions
- Memory leaks or resource cleanup issues
- WASM module loading failures
- Execution environment setup problems

### 5.1.3 Test Logic Issues Analysis
If tests run but assertions fail:
- Incorrect expected values in assertions
- Wrong test input data or setup
- Misunderstanding of opcode behavior
- Incomplete test coverage scenarios
- Race conditions in multi-threaded tests
- Platform-specific behavior differences

### 5.1.4 Coverage Issues Analysis
If coverage is insufficient:
- Unreachable code paths in tests
- Missing test scenarios for edge cases
- Incomplete error condition testing
- Insufficient input value variations

## 5.2 Automated Issue Resolution Strategies

### 5.2.1 Compilation Error Fixes
For compilation issues:
1. Add missing includes and dependencies
2. Fix syntax errors and type mismatches
3. Resolve undefined symbol references
4. Update CMakeLists.txt configurations
5. Fix WASM module compilation errors

### 5.2.2 Runtime Error Fixes
For runtime issues:
1. Add null pointer checks and validation
2. Fix memory management and cleanup
3. Correct WASM module loading and setup
4. Add proper error handling and recovery
5. Fix resource initialization order

### 5.2.3 Test Logic Fixes
For assertion failures:
1. Verify opcode behavior understanding
2. Correct expected values and outcomes
3. Fix test input data and scenarios
4. Add proper test setup and teardown
5. Handle platform-specific differences

### 5.2.4 Coverage Enhancement Fixes
For insufficient coverage:
1. Add missing test scenarios
2. Expand input value ranges
3. Include additional error conditions
4. Add cross-platform test variations

## 5.3 Resolution Process
1. **Identify Root Cause**: Analyze error messages, stack traces, and logs
2. **Categorize Issue Type**: Determine if it's compilation, runtime, logic, or coverage
3. **Apply Targeted Fix**: Use appropriate resolution strategy for the issue type
4. **Verify Fix**: Re-run build and test to confirm resolution
5. **Document Changes**: Record what was fixed and why

## 5.4 Quality Validation After Fixes
Ensure all fixes maintain:
- Code quality standards
- WAMR coding conventions
- Test framework compliance
- No introduction of new issues
- Comprehensive test coverage
- Performance considerations

## 5.5 Iterative Resolution
If initial fixes don't resolve all issues:
1. Re-analyze remaining problems
2. Apply additional targeted fixes
3. Re-test until all issues are resolved
4. Ensure stability across multiple test runs


## Phase 6: Code Review & Standardized Commit
Execute this phase after successful build and test completion from Phase 4 (or after successful issue resolution from Phase 5).
## 6.1 Automated Code Review Checklist

### 6.1.1 Code Quality Assessment
Review all generated files for:
- [ ] WAMR coding standards compliance
- [ ] Proper include guard usage and header organization
- [ ] Consistent naming conventions (snake_case for C, PascalCase for classes)
- [ ] No magic numbers or hardcoded values
- [ ] Proper const correctness and type safety
- [ ] No compiler warnings or static analysis issues

### 6.1.2 Test Coverage Analysis
Verify comprehensive coverage:
- [ ] All main routine test cases implemented
- [ ] All corner case scenarios covered
- [ ] All edge case conditions tested
- [ ] All error exception paths validated
- [ ] Cross-execution mode testing (interpreter, fast-jit, llvm-jit)
- [ ] Platform compatibility considerations

### 6.1.3 Performance Impact Assessment
Evaluate performance implications:
- [ ] Test execution time is reasonable
- [ ] Memory usage patterns are efficient
- [ ] No unnecessary resource allocation
- [ ] Proper cleanup and resource management
- [ ] Minimal impact on overall test suite runtime

### 6.1.4 Documentation Completeness
Ensure proper documentation:
- [ ] Test case descriptions are clear and meaningful
- [ ] Complex test logic is commented
- [ ] WASM module functionality is documented
- [ ] Error conditions are properly explained
- [ ] Setup and teardown procedures are documented

## 6.2 Commit Preparation

### 6.2.1 File Staging
Stage the following files for commit:
```bash
git add tests/unit/enhanced_opcode/{CATEGORY}/{OPCODE_NAME}*
git add tests/unit/enhanced_opcode/{CATEGORY}/wasm-apps/{OPCODE_NAME}*
git add tests/unit/enhanced_opcode/{CATEGORY}/CMakeLists.txt
```

### 6.2.2 Commit Metadata Collection
Calculate commit statistics:
- Test case count: {COUNT_OF_GENERATED_TESTS}
- Coverage improvement: {PERCENTAGE_IMPROVEMENT}%
- Files modified: {NUMBER_OF_FILES}
- Lines of code added: {LOC_COUNT}

## 6.3 Standardized Commit Message Generation

Create commit message following this template:

```
Enhanced unit tests for {OPCODE_NAME} opcode - Comprehensive test coverage

## Summary
- Opcode: {OPCODE_NAME} (Category: {CATEGORY})
- Test Cases: {TEST_COUNT} comprehensive tests generated
- Files Modified: {FILES_MODIFIED}

## Execution Mode Validation
- Interpreter mode: ✅ Validated
- Fast JIT mode: ✅ Validated
- LLVM JIT mode: ✅ Validated
- Multi-tier JIT mode: ✅ Validated
```

## 6.4 Commit Execution
Execute the commit with the standardized message:
```bash
git commit -s -m "$(cat <<'EOF'
[Generated commit message from template above]
EOF
)"
```

## 6.5 Post-Commit Validation
After committing, verify:
- [ ] All files are properly committed
- [ ] Commit message follows standards
- [ ] Repository state is clean
- [ ] Coverage reports are updated
- [ ] Test suite integration is successful

Provide a comprehensive review report and execute the standardized commit process.


## Usage Guidelines

### Sequential Execution
Execute phases in strict order: 1 → 2 → 3 → 4 → (5 if needed) → 6

### Variable Substitution
Replace these placeholders in prompts:
- `{OPCODE_NAME}`: Target opcode (e.g., "i32.eqz")
- `{CATEGORY}`: Opcode category from Phase 1 analysis
- `{TEST_COUNT}`: Number of generated test cases
- `{COVERAGE_IMPROVEMENT}`: Percentage coverage improvement
- `{FILES_MODIFIED}`: Count of modified files

### Quality Assurance
Each phase must be completed successfully before proceeding to the next phase. Phase 5 is conditional and only executed if Phase 4 reports failures.

### Integration with WAMR Project
This framework is specifically designed for the WAMR project structure and follows WAMR coding standards, test frameworks, and build processes.