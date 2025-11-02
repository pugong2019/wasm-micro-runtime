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

**CRITICAL FIRST STEP**:  
Create the standardized TODO list using the TodoWrite tool before starting any work.

Execute this workflow for the opcode `${1}` with mandatory TODO list management:

### PHASE 1: Ultra-Deep Opcode Analysis

Perform comprehensive analysis of the target opcode:

**Core Analysis Tasks:**
1. **Semantic Analysis**: Identify primary function, purpose, and operation
2. **Type System Analysis**: Determine input/output types, conversions, polymorphism
3. **Stack Effect Analysis**: Document stack transformations (pop/push counts, positions)
4. **Edge Case Discovery**: Identify boundary values, special numeric values, overflow conditions
5. **Error Condition Mapping**: Map type mismatches, stack underflow, out-of-bounds access, traps
6. **Category Classification**: Classify into appropriate category

**Opcode Categories:**
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

Generate comprehensive test strategy with four distinct test categories:

**Test Categories:**
1. **Main Routine Tests**: Basic functionality with typical input values
2. **Corner Case Tests**: Boundary conditions, overflow/underflow, signed/unsigned boundaries
3. **Edge Case Tests**: Zero operands, identity operations, extreme values (MIN/MAX, NaN, Infinity)
4. **Error Exception Tests**: Invalid operand types, stack underflow, out-of-bounds access

**Test Specification Requirements:**
For each category, document:
- Detailed test case descriptions
- Specific input conditions and setup requirements
- Expected outcomes and behaviors
- Meaningful assertion statements
- WASM module requirements

**Consolidated Test Case Design Strategy**

Group related scenarios in single test methods to reduce method proliferation:

| Category | Implementation Strategy | Pattern |
|----------|------------------------|---------|
| **Main** | Combine related scenarios | Group by operation type |
| **Corner** | Group boundary conditions | Combine overflow/underflow |
| **Edge** | Consolidate mathematical properties | Group identity/inverse |
| **Exception** | Consolidate | Group |


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

**Naming Guidelines:**
- Use broader, category-based names
- Focus on validation concept rather than specific input variations
- Each method validates one primary concept through multiple assertions

**Phase 2 Completion**:  
Update TODO list marking tasks 2.1-2.6 as completed, display progress percentage, declare Phase 3 as next.

### PHASE 3: Complete Code Generation

Generate production-ready test suite following this systematic approach:

#### Step 1: Directory Structure Setup
Create the required directory structure in `tests/unit/enhanced_opcode/{CATEGORY}/`:
The directory structure should be like below:
```
tests/unit/enhanced_opcode/{CATEGORY}/
├── CMakeLists.txt
├── enhanced_{opcode}_test.cc
├── {opcode}_common.h (if needed)
└── wasm-apps/
    ├── {opcode}_test.wat
    └── {opcode}_test.wasm
```

#### Step 2: CMakeLists Integration
Add the newly generated directory to the parent CMakeLists.txt:
- Update `tests/unit/enhanced_opcode/CMakeLists.txt` to include the new subdirectory

#### Step 3: Code Generation Requirements

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

**Code Generation Standards:**
- **Framework**: Use GTest with TestWithParam<RunningMode>
- **Assertions**: ASSERT_* statements exclusively (never EXPECT_*)
- **No Conditional Logic**: Eliminate all if/else blocks in test methods
- **Descriptive Messages**: Include context in assertion failure messages
- **Resource Management**: Proper SetUp/TearDown with WAMR initialization
- **Test Coverage**: Implement ALL test cases from Phase 2 strategy
- **Prohibited Constructs**: NO GTEST_SKIP(), SUCCEED(), or FAIL() calls

**CMakeLists.txt Build Structure:**
Follow `memory64/CMakeLists.txt` pattern:
- **WAMR Build Flags**: Enable WAMR_BUILD_INTERP, WAMR_BUILD_AOT, etc.
- **Feature Flags**: Add/modify build macros to enable the opcode feature being tested
- **Configuration**: Include `unit_common.cmake` for shared build setup
- **Definitions**: Set platform-specific definitions if the opcode is platform-specific (e.g., `-DRUN_ON_LINUX`)
- **Libraries**: Link against gtest_main, LLVM (if needed)
- **WASM Files**: Copy test files to build directory with POST_BUILD commands

**Phase 3 Completion**:   
Update TODO list marking tasks 3.1-3.6 as completed, display progress percentage, declare Phase 4 as next.

### PHASE 4: Build & Test Execution

Execute the build and test process systematically:

**Build Commands:**
```bash
cd tests/unit/
cmake -S enhanced_opcode/{CATEGORY} -B build/enhanced_opcode/{CATEGORY} -DCOLLECT_CODE_COVERAGE=1
cmake --build build/enhanced_opcode/{CATEGORY} --parallel $(nproc)
ctest --test-dir build/enhanced_opcode/{CATEGORY} --output-on-failure --verbose
```

**Validation Criteria:**
- All source files compile without warnings
- All unit tests pass (0 failures)
- No runtime crashes or memory leaks
- Coverage improvement is measurable

**Phase 4 Completion**: Update TODO list marking tasks 4.1-4.6 as completed. If successful, proceed to Phase 6. If failed, declare Phase 5 as next.

### PHASE 5: Issue Detection & Resolution
**(Conditional - Apply only if Phase 4 fails)**

**Resolution Process:**
1. **Issue Detection**: Analyze compilation errors, runtime crashes, or assertion failures
2. **Root Cause Analysis**: Categorize as compilation, runtime, test logic, or coverage issue
3. **Targeted Resolution**: Apply appropriate fixes (missing includes, null checks, correct expected values)
4. **Verification**: Re-run build and test to confirm resolution
5. **Iteration**: Repeat until all issues are resolved

**Phase 5 Completion**: Update TODO list marking tasks 5.1-5.5 as completed, then declare Phase 6 as next.

### PHASE 6: Code Review & Standardized Commit

**Final Review Process:**
1. **Quality Review**: Verify WAMR coding standards, meaningful test descriptions, proper resource management
2. **Coverage Analysis**: Confirm comprehensive test coverage across all scenarios
3. **Performance Assessment**: Validate reasonable execution time and memory usage
4. **Commit Creation**: Stage files and create commit using exact template

**Commit Process:**
```bash
git add tests/unit/enhanced_opcode/{CATEGORY}/{OPCODE_NAME}* #Add code file
git add tests/unit/enhanced_opcode/{CATEGORY}/wasm-apps/*.wat  #Add wat and wasm files
git add tests/unit/enhanced_opcode/{CATEGORY}/wasm-apps/*.wasm  #Add wasm files
git add tests/unit/enhanced_opcode/{CATEGORY}/CMakeLists.txt #Add CMakeLists if modifiled
git commit -s -m "Enhanced unit tests for {OPCODE_NAME} opcode - Comprehensive test coverage

## Summary
- Opcode: {OPCODE_NAME} (Category: {CATEGORY})
- Test Cases: {TEST_COUNT} comprehensive tests generated
- Files Modified: {FILES_MODIFIED}"
```

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
- [ ] **Documentation**: Function comments with source locations included
- [ ] **Resource Management**: Proper SetUp/TearDown implementation
- [ ] **Repository Integration**: Git commit using EXACT template format

**ENFORCEMENT**: Any unchecked item constitutes IMMEDIATE FAILURE of the generation process.

**EXECUTION ORDER**: Execute all phases sequentially. Only proceed to next phase after current phase completes successfully. Phase 5 is conditional - only execute if Phase 4 reports failures.