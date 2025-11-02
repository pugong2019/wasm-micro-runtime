# Generate Comprehensive WASM Opcode Test Suite

Generate complete, production-ready WASM opcode test suites using a systematic 6-phase approach with mandatory TODO list management.

## Usage
`/generate-opcode-test <OPCODE_NAME>`

**Examples:**
```bash
/generate-opcode-test i32.add
/generate-opcode-test br_if
/generate-opcode-test memory.grow
/generate-opcode-test v128.add
```
---

## 🔐 MANDATORY: TODO List Management Protocol

**CRITICAL REQUIREMENT**: You MUST create and maintain a structured TODO list before initiating any work.

**Operational Flow**:
1. Create TODO List → 2. Execute Current Task → 3. Update TODO List → 4. Repeat until completion

### Standardized TODO List Template
```markdown
## WASM Opcode Test Generation TODO List

### Phase 1: Ultra-Deep Opcode Analysis
- [ ] 1.1 Semantic analysis of opcode functionality and purpose
- [ ] 1.2 Type system analysis (input/output types, conversions, polymorphism)
- [ ] 1.3 Stack effect documentation (pop/push behavior, height changes)
- [ ] 1.4 Edge case identification (boundary values, special numeric values, overflow)
- [ ] 1.5 Error condition mapping (traps, type mismatches, stack underflow, out-of-bounds)
- [ ] 1.6 Opcode category classification (numeric, memory, control-flow, variable, reference, extension)

### Phase 2: Strategic Test Planning
- [ ] 2.1 Main routine test case design (basic functionality with typical values)
- [ ] 2.2 Corner case test design (boundary conditions, overflow/underflow scenarios)
- [ ] 2.3 Edge case test design (zero operands, identity operations, extreme values)
- [ ] 2.4 Error exception test design (invalid operands, stack underflow, type mismatches)
- [ ] 2.5 Cross-execution mode validation strategy planning
- [ ] 2.6 Detailed test case descriptions and expected outcomes specification

### Phase 3: Complete Code Generation
- [ ] 3.1 Directory structure creation in tests/unit/enhanced_opcode/{CATEGORY}/
- [ ] 3.2 enhanced_{opcode}_test.cc generation with GTest framework
- [ ] 3.3 {opcode}_common.h header file generation (if needed)
- [ ] 3.4 Comprehensive WASM test files generation ({opcode}_test.wat and .wasm)
- [ ] 3.5 CMakeLists.txt generation with proper dependencies and coverage support
- [ ] 3.6 ALL test cases implementation with meaningful ASSERT_* statements (NO GTEST_SKIP/SUCCEED/FAIL)

### Phase 4: Build & Test Execution
- [ ] 4.1 Navigate to tests/unit/ and configure CMake build
- [ ] 4.2 Build test suite with parallel compilation
- [ ] 4.3 Execute test suite and capture detailed output
- [ ] 4.4 Validate all tests pass (0 failures) and no runtime crashes
- [ ] 4.5 Generate coverage report and verify measurable improvement
- [ ] 4.6 Document any build/test failures for Phase 5 resolution

### Phase 5: Issue Detection & Resolution (Conditional - Only if Phase 4 fails)
- [ ] 5.1 Compilation errors, runtime crashes, or assertion failures analysis
- [ ] 5.2 Issue categorization (compilation, runtime, test logic, coverage)
- [ ] 5.3 Targeted fixes application (missing includes, null checks, correct expected values)
- [ ] 5.4 Re-run build and test to confirm resolution
- [ ] 5.5 Iterate until all issues resolved and tests pass

### Phase 6: Code Review & Standardized Commit
- [ ] 6.1 Comprehensive code quality review
- [ ] 6.2 Test coverage validation across all scenarios
- [ ] 6.3 Performance and resource usage assessment
- [ ] 6.4 Files staging and standardized commit message creation
- [ ] 6.5 Commit execution and repository state validation
```

### TODO Update Protocol
After EVERY task completion:
1. Mark completed tasks with ✅ checkbox
2. Update current progress status with explicit percentage
3. Display the updated TODO list in its entirety
4. Explicitly declare the next task to be executed

---
## 🚨 Enforcement Policies

### ABSOLUTE REQUIREMENTS

**Project Management:**  
1. **TODO List Creation**: Create structured TODO list before initiating any work  
2. **Phase Sequential Execution**: Complete phases in strict order (1→2→3→4→(5 if needed)→6)  
3. **Progress Tracking**: Update TODO list after every task completion  

**Code Quality Standards:**  

4. **Assertion Standards**: Use ASSERT_* assertions exclusively (never EXPECT_*)  
5. **No Test Skipping**: NEVER use GTEST_SKIP(), SUCCEED(), or FAIL()  
6. **Functional Validation**: Validate actual WAMR runtime behavior, not just code execution  
7. **Meaningful Assertions**: Include substantial assertions in every test case  
8. **Naming Conventions**: Follow `TEST_F({opcode}_test_suite, Function_Scenario_ExpectedOutcome)` pattern  
9. **Resource Management**: Implement proper SetUp/TearDown with RAII patterns  

**Build and Testing:**  

10. **Build Location**: Build exclusively in `tests/unit/` directory  
11. **Directory Structure**: Create files in `tests/unit/enhanced_opcode/{CATEGORY}/` following exact patterns  
12. **Test Completeness**: Implement ALL test cases from Phase 2 strategy  
13. **Build Success**: Achieve zero compilation errors and warnings  
14. **Test Success**: Achieve 100% test pass rate (zero failures)  
15. **Commit Compliance**: Use EXACT commit message template  

### ABSOLUTE PROHIBITIONS

**Workflow Violations:**
1. Starting work without creating TODO list  
2. Bypassing any required phase or task within phases  
3. Skipping iterative issue resolution cycles  

**Invalid Code Constructs:**  

4. Using GTEST_SKIP(), placeholder assertions, or non-substantive validations  
5. Using trivial assertions: ASSERT(true), ASSERT_TRUE(true), ASSERT_FALSE(false)  
6. Using if conditions without ASSERT validation for operation results  
7. Building tests outside tests/unit/ directory  

**Quality Compromises:**  

8. Generating partial test suites or missing test categories  
9. Accepting compilation warnings, test failures, or runtime crashes  
10. Missing function comments with source location and coverage goals  
11. Adding content beyond specified commit message template  

### Critical Success Gates

- **Gate 1 (Phase 1)**: Comprehensive opcode analysis with category classification
- **Gate 2 (Phase 2)**: Complete test strategy with all 4 test categories
- **Gate 3 (Phase 3)**: Production-ready code with proper directory structure
- **Gate 4 (Phase 4)**: Build successful with 100% test pass rate and measurable coverage
- **Gate 5 (Phase 5)**: All issues resolved (conditional - only if Phase 4 fails)
- **Gate 6 (Phase 6)**: Quality review passed and standardized commit created

**FAILURE ESCALATION**: If any gate fails after 3 attempts, mark task as FAILED and document blocking issues.

---

## Sequential Execution Workflow

## 🚨 ABSOLUTE COMPLIANCE REQUIREMENTS 🚨

**MANDATORY EXECUTION ORDER - NO EXCEPTIONS PERMITTED:**

### STRICT ADHERENCE RULES (NON-NEGOTIABLE):
1. **SEQUENTIAL EXECUTION ONLY**: Execute phases in EXACT order: 1→2→3→4→(5 if needed)→6
2. **NO STEP SKIPPING**: Complete EVERY sub-step within each phase before proceeding
3. **NO SHORTCUTS**: Follow ALL validation criteria and checkpoints
4. **NO IMPROVISATION**: Do not deviate from prescribed methods or add unlisted steps
5. **MANDATORY TODO TRACKING**: Update TODO list after EVERY single step completion

### ZERO TOLERANCE VIOLATIONS:
- ❌ Starting any phase without completing the previous phase entirely
- ❌ Skipping any numbered sub-step within a phase
- ❌ Proceeding without proper validation at each checkpoint
- ❌ Bypassing TODO list updates after step completion
- ❌ Adding custom steps or modifications to the prescribed workflow

### ENFORCEMENT PROTOCOL:
- **IMMEDIATE FAILURE**: Any deviation from prescribed steps results in IMMEDIATE task failure
- **NO RECOVERY**: Violations cannot be corrected - task must be restarted from Phase 1
- **STRICT VALIDATION**: Each step must meet ALL specified criteria before proceeding

---

**CRITICAL FIRST STEP**:
Create the standardized TODO list using the TodoWrite tool before starting any work.

**EXECUTE THIS WORKFLOW FOR THE OPCODE `${1}` WITH ABSOLUTE ADHERENCE TO EVERY STEP:**

### PHASE 1: Ultra-Deep Opcode Analysis

**🔒 MANDATORY PHASE 1 COMPLIANCE:**
- **EXECUTE ALL 6 STEPS IN ORDER**: Steps 1.1 → 1.2 → 1.3 → 1.4 → 1.5 → 1.6 (NO EXCEPTIONS)
- **COMPLETE EACH STEP FULLY**: Do not proceed to next step until current step is 100% complete
- **UPDATE TODO AFTER EACH STEP**: Mark step as completed in TODO list immediately after finishing

Perform comprehensive analysis of the target opcode through sequential steps:

#### Step 1.1: Semantic Analysis
- **Research opcode specification** from WebAssembly documentation
- **Identify primary function**: What does this opcode do?
- **Document operation purpose**: Why does this opcode exist?
- **Map to runtime behavior**: How does WAMR implement this operation?

#### Step 1.2: Type System Analysis
- **Input types identification**: What types does the opcode consume from stack?
- **Output types determination**: What types does the opcode produce on stack?
- **Type conversion rules**: Any implicit conversions or validations?
- **Polymorphism handling**: Does the opcode work with multiple types?

#### Step 1.3: Stack Effect Analysis
- **Stack pop behavior**: How many values removed and from which positions?
- **Stack push behavior**: How many values added and to which positions?
- **Net stack height change**: Calculate overall stack impact
- **Stack state validation**: Document pre/post conditions

#### Step 1.4: Edge Case Discovery
- **Boundary value identification**: MIN/MAX values for numeric types
- **Special numeric values**: NaN, Infinity, -0.0 for floating-point
- **Overflow/underflow conditions**: When does arithmetic wrap or trap?
- **Zero operand scenarios**: Behavior with zero inputs

#### Step 1.5: Error Condition Mapping
- **Type mismatch scenarios**: Wrong types on stack
- **Stack underflow cases**: Insufficient stack values
- **Out-of-bounds access**: Memory/table/element access violations
- **Trap conditions**: When does the opcode cause execution traps?

#### Step 1.6: Category Classification
Classify into one of these categories based on analysis:
- **Numeric**: `i32.add`, `f64.mul`, `i64.eqz` - Arithmetic, comparison, bitwise operations
- **Memory**: `i32.load`, `memory.grow` - Load, store, memory control operations
- **Control Flow**: `br_if`, `call`, `loop` - Branching, loops, function calls
- **Variable**: `local.get`, `global.set` - Local and global variable access
- **Reference**: `ref.null`, `ref.func` - Reference type operations
- **Extension**: `v128.add`, `i32.atomic.load` - SIMD, atomic operations

**Reference**: Complete WebAssembly instruction set at https://webassembly.github.io/spec/core/appendix/index-instructions.html

**Phase 1 Completion**:
Update TODO list marking tasks 1.1-1.6 as completed, display progress percentage, declare Phase 2 as next.

### PHASE 2: Strategic Test Planning

**🔒 MANDATORY PHASE 2 COMPLIANCE:**
- **EXECUTE ALL 6 STEPS IN ORDER**: Steps 2.1 → 2.2 → 2.3 → 2.4 → 2.5 → 2.6 (NO EXCEPTIONS)
- **COMPLETE EACH STEP FULLY**: Do not proceed to next step until current step is 100% complete
- **UPDATE TODO AFTER EACH STEP**: Mark step as completed in TODO list immediately after finishing
- **PREREQUISITE CHECK**: Ensure Phase 1 is 100% complete before starting Phase 2

Generate comprehensive test strategy through systematic planning steps:

#### Step 2.1: Main Routine Test Case Design
Design tests for basic opcode functionality:
- **Typical input scenarios**: Common, expected use cases
- **Standard value ranges**: Normal operational parameters
- **Positive test cases**: Verify correct behavior under normal conditions
- **Cross-execution mode validation**: Ensure consistency between interpreter and AOT modes
- **Expected success outcomes**: Document anticipated results for valid inputs

#### Step 2.2: Corner Case Test Design
Design tests for boundary conditions:
- **Numeric boundaries**: MIN_VALUE, MAX_VALUE for integer types
- **Signed/unsigned boundaries**: Test edge cases around zero and limits
- **Overflow/underflow scenarios**: Values that cause arithmetic wrapping
- **Type boundary conditions**: Largest/smallest values for each supported type
- **Memory boundary cases**: Edge of valid memory ranges (for memory opcodes)

#### Step 2.3: Edge Case Test Design
Design tests for extreme and special scenarios:
- **Zero operand scenarios**: Behavior with zero inputs
- **Identity operations**: Operations that should return input unchanged
- **Extreme values**: MIN/MAX, NaN, Infinity, -0.0 for floating-point
- **Mathematical properties**: Commutative, associative, distributive validation
- **Special numeric behaviors**: Denormal numbers, rounding modes

#### Step 2.4: Error Exception Test Design
Design tests for invalid scenarios and error conditions:
- **Invalid operand types**: Wrong types on stack (type mismatch)
- **Stack underflow scenarios**: Insufficient values on execution stack
- **Out-of-bounds access**: Invalid memory/table/element indices
- **Trap condition triggers**: Scenarios that should cause execution traps
- **Runtime error validation**: Proper error handling and reporting

#### Step 2.5: Cross-Execution Mode Validation Strategy Planning
Plan validation across WAMR execution modes:
- **Interpreter mode testing**: Direct bytecode interpretation validation
- **AOT mode testing**: Ahead-of-time compiled module validation
- **Consistency verification**: Ensure identical results across modes
- **Performance comparison**: Document any behavioral differences
- **Mode-specific optimizations**: Test optimized code paths

#### Step 2.6: Detailed Test Case Specifications
For each test category, document comprehensive specifications:
- **Test case descriptions**: Clear, specific test scenario explanations
- **Input conditions**: Exact parameter values and setup requirements
- **Expected outcomes**: Precise anticipated results and behaviors
- **Assertion statements**: Meaningful ASSERT_* validations with descriptive messages
- **WASM module requirements**: Module structure, exports, and dependencies

**Consolidated Test Case Design Strategy**

Group related scenarios to reduce method proliferation:

| Category | Implementation Strategy | Consolidation Pattern |
|----------|------------------------|----------------------|
| **Main** | Combine related scenarios | Group by operation type |
| **Corner** | Group boundary conditions | Combine overflow/underflow cases |
| **Edge** | Consolidate mathematical properties | Group identity/inverse operations |
| **Exception** | Group error scenarios | Combine similar trap conditions |

**Implementation Example:**
```cpp
// ✅ RECOMMENDED: Consolidated approach
TEST_P(I32AddTest, BasicAddition_ReturnsCorrectSum) {
    ASSERT_EQ(call_i32_add(5, 3), 8);        // Positive numbers
    ASSERT_EQ(call_i32_add(-10, -15), -25);  // Negative numbers
    ASSERT_EQ(call_i32_add(20, -8), 12);     // Mixed signs
}

// ❌ AVOID: Separate methods for similar scenarios
TEST_P(I32AddTest, BasicAddition_SmallPositives_ReturnsCorrectSum) { ... }
TEST_P(I32AddTest, BasicAddition_SmallNegatives_ReturnsCorrectSum) { ... }
```

**Test Naming Guidelines:**
- Use broader, category-based names (e.g., `BasicAddition_ReturnsCorrectSum`)
- Focus on validation concept rather than specific input variations
- Each method validates one primary concept through multiple assertions
- Follow pattern: `TEST_P(OpcodeTest, Concept_ExpectedOutcome)`

**Phase 2 Completion**:
Update TODO list marking tasks 2.1-2.6 as completed, display progress percentage, declare Phase 3 as next.

### PHASE 3: Complete Code Generation

**🔒 MANDATORY PHASE 3 COMPLIANCE:**
- **EXECUTE ALL 6 STEPS IN ORDER**: Steps 3.1 → 3.2 → 3.3 → 3.4 → 3.5 → 3.6 (NO EXCEPTIONS)
- **COMPLETE EACH STEP FULLY**: Do not proceed to next step until current step is 100% complete
- **UPDATE TODO AFTER EACH STEP**: Mark step as completed in TODO list immediately after finishing
- **PREREQUISITE CHECK**: Ensure Phase 2 is 100% complete before starting Phase 3
- **NO CODE SHORTCUTS**: Generate ALL required files as specified in each step

Generate production-ready test suite through systematic implementation steps:

#### Step 3.1: Directory Structure Creation
Create the complete directory structure in `tests/unit/enhanced_opcode/{CATEGORY}/`:
```bash
# Navigate to target location
cd tests/unit/enhanced_opcode/

# Create category directory if it doesn't exist
mkdir -p {CATEGORY}

# Create subdirectories
mkdir -p {CATEGORY}/wasm-apps
```

**Required directory structure:**
```
tests/unit/enhanced_opcode/{CATEGORY}/
├── CMakeLists.txt
├── enhanced_{opcode}_test.cc
├── {opcode}_common.h (if needed)
└── wasm-apps/
    ├── {opcode}_test.wat
    └── {opcode}_test.wasm
```

#### Step 3.2: GTest Framework File Generation
Generate the main C++ test file `enhanced_{opcode}_test.cc`:

**File Structure and Headers:**
- **Include proper headers**: WAMR runtime, GTest, test utilities
- **Header comments**: Add comprehensive file header with description, purpose, and coverage goals
- **Import line comments**: Document the purpose of each critical #include directive

**Class and Method Documentation:**
- **Test fixture class**: Inherit from `testing::TestWithParam<RunningMode>` with comprehensive class documentation
- **SetUp/TearDown methods**: WAMR initialization and cleanup with RAII, including method documentation
- **Test parameter setup**: Configure interpreter and AOT modes with parameter documentation
- **Helper functions**: Common test utilities and WASM module loaders with detailed function documentation

#### Step 3.3: Common Header File Generation (if needed)
Generate `{opcode}_common.h` if shared definitions are needed:
- **Common constants**: Test values, buffer sizes, error codes
- **Shared structures**: Test data containers, helper structs
- **Utility macros**: Common assertions, test setup patterns
- **Function declarations**: Shared helper function prototypes

#### Step 3.4: WASM Test Module Generation
Generate comprehensive WASM test files:

**WAT file (`{opcode}_test.wat`):**
- **Module structure**: Complete WASM module with all required sections
- **Test functions**: Functions exercising the target opcode
- **Export declarations**: Make test functions accessible from C++
- **Memory/table setup**: Required memory or table sections (if applicable)
- **Edge case scenarios**: Include all test cases from Phase 2 planning

**WASM binary (`{opcode}_test.wasm`):**
- **Compile WAT to WASM**: Use `wat2wasm` or equivalent tool
- **Validate binary**: Ensure proper WASM format and structure
- **Test loading**: Verify WAMR can load the generated module

#### Step 3.5: CMakeLists.txt Build Configuration
Generate comprehensive build configuration:
- **WAMR build flags**: Enable required features (WAMR_BUILD_INTERP, WAMR_BUILD_AOT, etc.)
- **Feature-specific flags**: Add macros to enable the opcode being tested
- **Include unit_common.cmake**: Shared build configuration
- **Platform definitions**: Set OS-specific flags
- **Library linking**: Link against gtest_main, LLVM (if needed)
- **WASM file copying**: POST_BUILD commands to copy test files
- **Coverage support**: Enable code coverage collection flags

#### Step 3.6: Test Case Implementation
Implement ALL test cases from Phase 2 with mandatory quality standards:

**🚨 MANDATORY: Test Function Documentation Requirements**
Every test case function MUST include comprehensive documentation:

**Required Test Function Documentation Format:**
```cpp
/**
 * @test {TestCategory}_{TestScenario}_{ExpectedOutcome}
 * @brief {Brief description of what this test validates}
 * @details {Detailed explanation of test purpose and validation logic}
 * @test_category {Main/Corner/Edge/Error} - Test category from Phase 2 planning
 * @coverage_target {Specific WAMR source functions being tested}
 * @input_conditions {Description of test input setup and conditions}
 * @expected_behavior {Exact expected behavior and validation criteria}
 * @validation_method {How the test verifies correct behavior}
 */
TEST_P({Opcode}Test, {TestCategory}_{TestScenario}_{ExpectedOutcome}) {
    // Test implementation with documented steps
}
```

**Documentation Example:**
```cpp
/**
 * @test BasicAddition_ReturnsCorrectSum
 * @brief Validates i32.add produces correct arithmetic results for typical inputs
 * @details Tests fundamental addition operation with positive, negative, and mixed-sign integers.
 *          Verifies that i32.add correctly computes a + b for various input combinations.
 * @test_category Main - Basic functionality validation
 * @coverage_target core/iwasm/interpreter/wasm_interp_classic.c:i32_add_operation
 * @input_conditions Standard integer pairs: (5,3), (-10,-15), (20,-8)
 * @expected_behavior Returns mathematical sum: 8, -25, 12 respectively
 * @validation_method Direct comparison of WASM function result with expected values
 */
TEST_P(I32AddTest, BasicAddition_ReturnsCorrectSum) {
    // Load WASM module with i32.add test function
    wasm_module_t module = load_test_module("i32_add_test.wasm");
    ASSERT_NE(nullptr, module) << "Failed to load i32.add test module";

    // Execute test cases with documented validation
    ASSERT_EQ(8, call_i32_add(5, 3))      << "Addition of positive integers failed";
    ASSERT_EQ(-25, call_i32_add(-10, -15)) << "Addition of negative integers failed";
    ASSERT_EQ(12, call_i32_add(20, -8))    << "Addition of mixed-sign integers failed";
}
```

**Implementation Requirements:**
- **Test function documentation**: Every TEST_P function must have comprehensive header documentation
- **Assertion documentation**: Include descriptive messages for all ASSERT_* statements

**CRITICAL: Mandatory ASSERT Usage Rule**
Eliminate ALL conditional blocks in GTest cases - Use ASSERT_* exclusively:

```cpp
// ❌ FORBIDDEN: Conditional blocks in test cases
underflow_module = wasm_runtime_load(underflow_buf, underflow_buf_size,
                                   error_buf, sizeof(error_buf));
if (underflow_module) {
    // Test logic here - NEVER DO THIS
}
```

```cpp
// ✅ REQUIRED: Use ASSERT statements with descriptive messages
underflow_module = wasm_runtime_load(underflow_buf, underflow_buf_size,
                                   error_buf, sizeof(error_buf));

// For successful load scenarios:
ASSERT_NE(nullptr, underflow_module)
    << "Failed to load test module: " << error_buf;

// For expected failure scenarios:
ASSERT_EQ(nullptr, underflow_module)
    << "Expected module load to fail for invalid bytecode, but got valid module";
```

**Implementation Requirements:**
- **Main routine tests**: Implement all basic functionality test cases
- **Corner case tests**: Implement all boundary condition test cases
- **Edge case tests**: Implement all extreme scenario test cases
- **Error exception tests**: Implement all invalid scenario test cases
- **🚨 MANDATORY DOCUMENTATION**: Every TEST_P function must have comprehensive header documentation with @test, @brief, @details, @test_category, @coverage_target, @input_conditions, @expected_behavior, @validation_method tags
- **Code comments**: Add inline comments for critical operations, module loading, and function calls
- **Import documentation**: Document the purpose of each #include directive with inline comments
- **ASSERT-only validation**: Use ASSERT_* statements exclusively (never EXPECT_*)
- **Descriptive messages**: Include context in all assertion failure messages
- **No conditional logic**: Eliminate all if/else blocks in test methods
- **Prohibited constructs**: NO GTEST_SKIP(), SUCCEED(), or FAIL() calls

**CMakeLists.txt Integration Requirements:**
- **Parent CMakeLists update**: Add subdirectory to `tests/unit/enhanced_opcode/CMakeLists.txt`
- **WAMR build flags**: Enable WAMR_BUILD_INTERP, WAMR_BUILD_AOT, etc.
- **Feature-specific flags**: Add macros to enable the opcode being tested
- **Shared configuration**: Include `unit_common.cmake` for common setup
- **Platform definitions**: Set OS-specific flags if needed (e.g., `-DRUN_ON_LINUX`)
- **Library dependencies**: Link against gtest_main, LLVM (if needed)
- **WASM file deployment**: POST_BUILD commands to copy test files to build directory

**Phase 3 Completion**:
Update TODO list marking tasks 3.1-3.6 as completed, display progress percentage, declare Phase 4 as next.

### PHASE 4: Build & Test Execution

**🔒 MANDATORY PHASE 4 COMPLIANCE:**
- **EXECUTE ALL 6 STEPS IN ORDER**: Steps 4.1 → 4.2 → 4.3 → 4.4 → 4.5 → 4.6 (NO EXCEPTIONS)
- **COMPLETE EACH STEP FULLY**: Do not proceed to next step until current step is 100% complete
- **UPDATE TODO AFTER EACH STEP**: Mark step as completed in TODO list immediately after finishing
- **PREREQUISITE CHECK**: Ensure Phase 3 is 100% complete before starting Phase 4
- **MANDATORY SUCCESS**: All build and test steps MUST succeed before proceeding to Phase 6

Execute the build and test process through systematic steps:

#### Step 4.1: Build Environment Setup
Navigate to the unit test directory and configure the build:
```bash
# Navigate to unit test root
cd tests/unit/

# Verify directory structure exists
ls -la enhanced_opcode/{CATEGORY}/
```
**Setup validation:**
- Confirm all generated files exist in correct locations
- Verify CMakeLists.txt files are properly configured
- Check WASM test files are present in wasm-apps/ directory

#### Step 4.2: CMake Configuration
Configure the build system with coverage support:
```bash
# Configure CMake with coverage enabled
cmake -S enhanced_opcode/{CATEGORY} -B build/enhanced_opcode/{CATEGORY} \
      -DCOLLECT_CODE_COVERAGE=1
```
**Configuration validation:**
- Verify CMake configuration completes without errors
- Confirm all required WAMR features are enabled
- Check coverage collection is properly configured

#### Step 4.3: Parallel Compilation
Build the test suite using parallel compilation:
```bash
# Build with parallel jobs for faster compilation
cmake --build build/enhanced_opcode/{CATEGORY} --parallel $(nproc)
```
**Compilation validation:**
- Ensure zero compilation errors
- Verify zero compilation warnings
- Confirm all test executables are generated

#### Step 4.4: Test Execution
Execute the complete test suite with detailed output:
```bash
# Run tests with verbose output and failure details
ctest --test-dir build/enhanced_opcode/{CATEGORY} \
      --output-on-failure \
      --verbose \
      --parallel $(nproc)
```
**Execution validation:**
- Verify all tests pass (0 failures)
- Confirm no runtime crashes occur
- Check no memory leaks are detected
- Validate test output shows expected assertions

#### Step 4.5: Coverage Report Generation
Generate and analyze code coverage:
```bash
# Generate coverage report (if configured)
cd build/enhanced_opcode/{CATEGORY}
make coverage  # or appropriate coverage target
```
**Coverage validation:**
- Confirm measurable coverage improvement
- Verify coverage report generation succeeds
- Document coverage percentage increase
- Identify any uncovered code paths

#### Step 4.6: Results Documentation
Document build and test execution results:
- **Compilation status**: Record successful compilation
- **Test results**: Document pass/fail counts and specific test outcomes
- **Performance metrics**: Note execution times and resource usage
- **Coverage metrics**: Record coverage percentage and improvements
- **Issue identification**: Log any failures for Phase 5 resolution (if needed)

**Success Criteria Verification:**
- ✅ All source files compile without warnings
- ✅ All unit tests pass (100% success rate)
- ✅ No runtime crashes or memory leaks detected
- ✅ Coverage improvement is measurable and documented
- ✅ Build completes in reasonable time

**Phase 4 Completion**: Update TODO list marking tasks 4.1-4.6 as completed. If all criteria met, proceed to Phase 6. If any failures occurred, declare Phase 5 as next.

### PHASE 5: Issue Detection & Resolution
**(Conditional - Apply only if Phase 4 fails)**

**🔒 MANDATORY PHASE 5 COMPLIANCE:**
- **EXECUTE ALL 5 STEPS IN ORDER**: Steps 5.1 → 5.2 → 5.3 → 5.4 → 5.5 (NO EXCEPTIONS)
- **COMPLETE EACH STEP FULLY**: Do not proceed to next step until current step is 100% complete
- **UPDATE TODO AFTER EACH STEP**: Mark step as completed in TODO list immediately after finishing
- **ITERATIVE REQUIREMENT**: Repeat steps 5.3-5.5 until ALL issues are resolved
- **ESCALATION LIMIT**: Maximum 3 resolution attempts before declaring FAILURE
- **🚨 CRITICAL: PRESERVE TEST INTENTION**: When fixing issues, NEVER change the test's original purpose or validation logic - only fix technical problems while maintaining the exact same test objectives and coverage goals

Execute systematic issue resolution through iterative steps:

#### Step 5.1: Comprehensive Issue Detection
Analyze all failures from Phase 4:
- **Compilation error analysis**: Parse compiler output for syntax, include, and linking errors
- **Runtime crash investigation**: Examine crash dumps, stack traces, and error messages
- **Test assertion failures**: Review failed test output and assertion messages
- **Coverage analysis issues**: Identify problems with coverage collection or reporting
- **Build system problems**: Check CMake configuration and dependency issues

#### Step 5.2: Root Cause Categorization
Classify each identified issue into specific categories:
- **Compilation issues**: Missing headers, syntax errors, linking problems
- **Runtime issues**: Null pointer dereferences, memory leaks, stack overflows
- **Test logic issues**: Incorrect expected values, wrong test setup, assertion problems
- **Coverage issues**: Missing coverage flags, tool configuration problems
- **Build configuration issues**: Incorrect CMake settings, missing dependencies

#### Step 5.3: Targeted Resolution Application
Apply specific fixes based on issue categorization:

**🚨 CRITICAL RULE: PRESERVE TEST INTENTION AT ALL TIMES**
- **NEVER modify test objectives**: The original test purpose MUST remain unchanged
- **NEVER weaken test coverage**: Do not reduce the scope or rigor of test validation
- **NEVER change expected behavior**: Maintain the exact same validation logic and assertions
- **ONLY fix technical problems**: Address syntax, compilation, runtime issues without changing test goals

**Permitted Fix Categories:**
- **For compilation issues**: Add missing includes, fix syntax, resolve dependencies
- **For runtime issues**: Add null checks, fix memory management, validate pointers
- **For test logic issues**: Correct expected values ONLY if they were factually wrong, fix test setup, improve assertion messages (but not assertion logic)
- **For coverage issues**: Fix coverage configuration, ensure proper tool installation
- **For build issues**: Update CMakeLists.txt, add missing flags, resolve dependencies

**FORBIDDEN Fix Actions:**
- ❌ Changing test scenarios to make them "easier" to pass
- ❌ Reducing the number of test cases or assertions
- ❌ Modifying expected values to match incorrect implementation behavior
- ❌ Removing "difficult" test cases that expose real issues
- ❌ Weakening validation criteria to avoid failures
- ❌ Adding GTEST_SKIP() to bypass failing tests

#### Step 5.4: Iterative Verification
Re-run build and test to confirm issue resolution:
```bash
# Clean previous build artifacts
rm -rf build/enhanced_opcode/{CATEGORY}

# Reconfigure and rebuild
cmake -S enhanced_opcode/{CATEGORY} -B build/enhanced_opcode/{CATEGORY} -DCOLLECT_CODE_COVERAGE=1
cmake --build build/enhanced_opcode/{CATEGORY} --parallel $(nproc)
ctest --test-dir build/enhanced_opcode/{CATEGORY} --output-on-failure --verbose
```
**Verification criteria:**
- Confirm specific fixes resolve identified issues
- Ensure no new issues are introduced
- Validate that all previously passing tests still pass
- **🚨 VERIFY TEST INTENTION PRESERVED**: Confirm that all test objectives, coverage goals, and validation logic remain exactly as originally designed

#### Step 5.5: Resolution Iteration
Continue resolution cycles until complete success:
- **Issue tracking**: Maintain list of resolved vs. remaining issues
- **Progress monitoring**: Document resolution progress after each iteration
- **Escalation criteria**: If 3 resolution attempts fail, escalate to FAILURE status
- **Success validation**: Achieve same success criteria as Phase 4
- **Documentation**: Record all applied fixes for future reference

**Resolution Success Criteria:**
- ✅ All compilation errors resolved
- ✅ All runtime crashes eliminated
- ✅ All test assertions pass
- ✅ Coverage collection works properly
- ✅ Build system operates correctly

**Phase 5 Completion**: Update TODO list marking tasks 5.1-5.5 as completed, then declare Phase 6 as next.

### PHASE 6: Code Review & Standardized Commit

**🔒 MANDATORY PHASE 6 COMPLIANCE:**
- **EXECUTE ALL 5 STEPS IN ORDER**: Steps 6.1 → 6.2 → 6.3 → 6.4 → 6.5 (NO EXCEPTIONS)
- **COMPLETE EACH STEP FULLY**: Do not proceed to next step until current step is 100% complete
- **UPDATE TODO AFTER EACH STEP**: Mark step as completed in TODO list immediately after finishing
- **PREREQUISITE CHECK**: Ensure Phase 4 (or Phase 5 if applicable) is 100% complete before starting Phase 6
- **EXACT COMMIT FORMAT**: Use ONLY the specified commit template - NO modifications allowed

Execute comprehensive final review and commit process:

#### Step 6.1: Comprehensive Code Quality Review
Perform thorough code quality assessment:
- **WAMR coding standards compliance**: Verify adherence to project conventions
- **Test description clarity**: Ensure meaningful, descriptive test names and comments
- **Resource management validation**: Confirm proper SetUp/TearDown implementation
- **Assertion quality check**: Validate meaningful ASSERT_* statements with descriptive messages
- **Code organization**: Review file structure, includes, and function organization
- **Documentation completeness**: Verify function comments with source locations

#### Step 6.2: Test Coverage Validation
Analyze comprehensive test coverage across all scenarios:
- **Main routine coverage**: Confirm all basic functionality scenarios are tested
- **Corner case coverage**: Verify all boundary conditions are covered
- **Edge case coverage**: Ensure all extreme scenarios are tested
- **Error exception coverage**: Validate all error conditions are properly tested
- **Cross-mode coverage**: Confirm testing across interpreter and AOT modes
- **Coverage metrics**: Document coverage percentage improvement

#### Step 6.3: Performance and Resource Assessment
Evaluate execution efficiency and resource utilization:
- **Execution time validation**: Ensure reasonable test execution duration
- **Memory usage assessment**: Confirm efficient memory utilization
- **Resource leak detection**: Verify no memory or file handle leaks
- **Build time evaluation**: Document compilation time requirements
- **Test scalability**: Assess performance with larger test datasets

#### Step 6.4: Files Staging and Commit Message Creation
Prepare files for commit with standardized process:
```bash
# Stage all generated test files
git add tests/unit/enhanced_opcode/{CATEGORY}/enhanced_{OPCODE_NAME}_test.cc
git add tests/unit/enhanced_opcode/{CATEGORY}/wasm-apps/{OPCODE_NAME}_test.wat
git add tests/unit/enhanced_opcode/{CATEGORY}/wasm-apps/{OPCODE_NAME}_test.wasm
git add tests/unit/enhanced_opcode/{CATEGORY}/CMakeLists.txt

# Stage any header files if generated
git add tests/unit/enhanced_opcode/{CATEGORY}/{OPCODE_NAME}_common.h  # if exists

# Stage parent CMakeLists.txt if modified
git add tests/unit/enhanced_opcode/CMakeLists.txt  # if modified
```

#### Step 6.5: Commit Execution and Repository State Validation
Execute commit with exact template format:
```bash
git commit -s -m "Enhanced unit tests for {OPCODE_NAME} opcode - Comprehensive test coverage

## Summary
- Opcode: {OPCODE_NAME} (Category: {CATEGORY})
- Test Cases: {TEST_COUNT} comprehensive tests generated
- Files Modified: {FILES_MODIFIED}"
```

**Post-commit validation:**
- **Commit verification**: Confirm commit was created successfully
- **Repository state check**: Verify working directory is clean
- **Commit message validation**: Ensure exact template format was used
- **File inclusion verification**: Confirm all required files are included in commit

**Quality Assurance Checklist:**
- ✅ Code follows WAMR standards and conventions
- ✅ All test scenarios are comprehensively covered
- ✅ Performance is acceptable and efficient
- ✅ All files are properly staged and committed
- ✅ Commit message follows exact template format
- ✅ Repository state is clean and consistent

**Phase 6 Completion**:
Update TODO list marking tasks 6.1-6.5 as completed.

---

## Success Criteria Checklist

Validate ALL items before completion:

**Phase Completion Requirements:**
- [ ] **Phase 1**: Opcode analysis with 6 components completed
- [ ] **Phase 2**: Test strategy with 4 categories documented
- [ ] **Phase 3**: Production code generated in correct directory structure
- [ ] **Phase 4**: Build successful with 100% test pass rate
- [ ] **Phase 5**: Issues resolved (if applicable)
- [ ] **Phase 6**: Quality review and commit completed

**Quality Gate Validation:**
- [ ] **TODO List**: Maintained throughout with progress updates
- [ ] **Assertion Standards**: All tests use ASSERT_* exclusively
- [ ] **Test Quality**: Zero GTEST_SKIP() calls or placeholder assertions
- [ ] **Build Success**: Build process completes without errors or warnings
- [ ] **Test Success**: All generated test cases pass (100% success rate)
- [ ] **🚨 COMPREHENSIVE DOCUMENTATION**:
  - [ ] File header with @file, @brief, @details, @coverage_target, @test_modes documentation
  - [ ] All #include directives have inline comments explaining their purpose
  - [ ] Every TEST_P function has complete @test documentation block
  - [ ] All critical code sections have inline comments
  - [ ] Function comments include source locations and coverage targets
- [ ] **Resource Management**: Proper SetUp/TearDown implementation
- [ ] **Repository Integration**: Git commit using EXACT template format

**ENFORCEMENT**: Any unchecked item constitutes IMMEDIATE FAILURE of the generation process.

**EXECUTION ORDER**: Execute all phases sequentially. Only proceed to next phase after current phase completes successfully. Phase 5 is conditional - only execute if Phase 4 reports failures.

---

## 🚨 FINAL STRICT ENFORCEMENT DECLARATION 🚨

### ABSOLUTE COMPLIANCE MANDATE:
The LLM executing this workflow is **STRICTLY REQUIRED** to follow every step in the exact sequence specified. Any deviation, shortcut, or improvisation is **STRICTLY FORBIDDEN** and will result in **IMMEDIATE TASK FAILURE**.

### NON-NEGOTIABLE REQUIREMENTS:
1. **SEQUENTIAL STEP EXECUTION**: Complete steps 1.1→1.2→1.3→1.4→1.5→1.6, then 2.1→2.2→2.3→2.4→2.5→2.6, etc.
2. **MANDATORY TODO UPDATES**: Update TODO list after EVERY single step completion - NO EXCEPTIONS
3. **COMPLETE STEP VALIDATION**: Each step must meet ALL specified criteria before proceeding
4. **NO CREATIVE ADDITIONS**: Do not add steps, modify requirements, or deviate from prescribed methods
5. **EXACT TEMPLATE USAGE**: Use only the specified templates, commands, and formats provided
6. **🚨 PRESERVE TEST INTENTION**: Throughout ALL phases, especially during issue resolution, NEVER compromise the original test objectives, coverage goals, or validation logic - only fix technical problems while maintaining the exact same test purpose

### VIOLATION CONSEQUENCES:
- **IMMEDIATE TERMINATION**: Any step skipping results in immediate workflow termination
- **NO PARTIAL CREDIT**: Incomplete steps do not count toward phase completion
- **RESTART REQUIREMENT**: Violations require complete restart from Phase 1
- **ZERO TOLERANCE**: No exceptions or special circumstances permitted

### SUCCESS VALIDATION:
Task is complete ONLY when:
- ✅ ALL 6 phases executed in exact sequence(Conditional for Phase 5 if Phase 4 Success)
- ✅ ALL sub-steps within each phase completed fully
- ✅ TODO list properly maintained throughout entire process
- ✅ ALL quality gates and success criteria met
- ✅ Commit created using EXACT template format

**This workflow demands absolute precision and complete adherence to every specified requirement.**