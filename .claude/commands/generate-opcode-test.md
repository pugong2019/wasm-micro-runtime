# Generate Comprehensive WASM Opcode Test Suite

Generate a complete, production-ready WASM opcode test suite using a systematic 6-phase approach with mandatory TODO list management.

## Usage
`/generate-opcode-test <OPCODE_NAME>`

**Examples:**
```
/generate-opcode-test i32.add
/generate-opcode-test br_if
/generate-opcode-test memory.grow
/generate-opcode-test v128.add
```
---
## 🔐 MANDATORY: TODO List Management Protocol

**CRITICAL REQUIREMENT: You MUST create and maintain a structured TODO list before initiating any work.**

**Operational Flow (Non-Negotiable):**
1. Create TODO List → 2. Execute Current Task → 3. Update TODO List → 4. Repeat until completion

### Standardized TODO List Template
```markdown
## WASM Opcode Test Generation TODO List

### Phase 1: Ultra-Deep Opcode Analysis
- [ ] 1.1 Perform semantic analysis of opcode functionality and purpose
- [ ] 1.2 Analyze type system (input/output types, conversions, polymorphism)
- [ ] 1.3 Document stack effect analysis (pop/push behavior, height changes)
- [ ] 1.4 Identify edge cases (boundary values, special numeric values, overflow conditions)
- [ ] 1.5 Map error conditions (traps, type mismatches, stack underflow, out-of-bounds)
- [ ] 1.6 Classify opcode category (numeric, memory, control-flow, variable, reference, extension)

### Phase 2: Strategic Test Planning
- [ ] 2.1 Design main routine test cases (basic functionality with typical values)
- [ ] 2.2 Design corner case tests (boundary conditions, overflow/underflow scenarios)
- [ ] 2.3 Design edge case tests (zero operands, identity operations, extreme values)
- [ ] 2.4 Design error exception tests (invalid operands, stack underflow, type mismatches)
- [ ] 2.5 Plan cross-execution mode validation strategy
- [ ] 2.6 Specify detailed test case descriptions and expected outcomes

### Phase 3: Complete Code Generation
- [ ] 3.1 Create directory structure in tests/unit/enhanced_opcode/{CATEGORY}/
- [ ] 3.2 Generate enhanced_{opcode}_test.cc with GTest framework
- [ ] 3.3 Generate {opcode}_common.h header file (if needed)
- [ ] 3.4 Generate comprehensive WASM test files ({opcode}_test.wat and .wasm)
- [ ] 3.5 Generate CMakeLists.txt with proper dependencies and coverage support
- [ ] 3.6 Implement ALL test cases with meaningful ASSERT_* statements (NO GTEST_SKIP/SUCCEED/FAIL)

### Phase 4: Build & Test Execution
- [ ] 4.1 Navigate to tests/unit/ directory and configure CMake build
- [ ] 4.2 Build test suite with parallel compilation
- [ ] 4.3 Execute test suite and capture detailed output
- [ ] 4.4 Validate all tests pass (0 failures) and no runtime crashes
- [ ] 4.5 Generate coverage report and verify measurable improvement
- [ ] 4.6 Document any build/test failures for Phase 5 resolution

### Phase 5: Issue Detection & Resolution (Conditional - Only if Phase 4 fails)
- [ ] 5.1 Analyze compilation errors, runtime crashes, or assertion failures
- [ ] 5.2 Categorize issues (compilation, runtime, test logic, coverage)
- [ ] 5.3 Apply targeted fixes (missing includes, null checks, correct expected values)
- [ ] 5.4 Re-run build and test to confirm resolution
- [ ] 5.5 Iterate until all issues resolved and tests pass

### Phase 6: Code Review & Standardized Commit
- [ ] 6.1 Perform comprehensive code quality review
- [ ] 6.2 Validate test coverage across all scenarios
- [ ] 6.3 Assess performance and resource usage
- [ ] 6.4 Stage files and create standardized commit message
- [ ] 6.5 Execute commit and validate repository state
```

### TODO Update Protocol (Mandatory Compliance)
After EVERY task completion, you MUST:
1. Mark completed tasks with ✅ checkbox
2. Update current progress status with explicit percentage
3. Display the updated TODO list in its entirety
4. Explicitly declare the next task to be executed
---
## 🚨 Enforcement Policies (Non-Negotiable)

### ABSOLUTE REQUIREMENTS (Mandatory Compliance)

1. **TODO List Creation**: MUST create structured TODO list before initiating any work
2. **Phase Sequential Execution**: MUST complete phases in strict order (1→2→3→4→(5 if needed)→6)
3. **Assertion Standards**: MUST use ASSERT_* assertions exclusively (never EXPECT_*)
4. **Test Skip Prohibition**: MUST NEVER use GTEST_SKIP(), SUCCEED(), or FAIL() - handle unsupported features via early return
5. **Build Location Enforcement**: MUST build exclusively in tests/unit/ directory (never in module directories)
6. **Functional Validation**: MUST validate actual WAMR runtime behavior, not merely code execution paths
7. **Assertion Substance**: MUST include meaningful assertions in every test case (never ASSERT_TRUE(true) or similar)
8. **Naming Convention Compliance**: MUST follow `TEST_F({opcode}_test_suite, Function_Scenario_ExpectedOutcome)` pattern
9. **Resource Management**: MUST implement proper SetUp/TearDown with RAII patterns
10. **Operation Status Validation**: MUST use ASSERT statements to check status of operations (e.g., ASSERT_NE(nullptr, module) after wasm_runtime_load)
11. **Directory Structure**: MUST create files in tests/unit/enhanced_opcode/{CATEGORY}/ following exact patterns
12. **Test Completeness**: MUST implement ALL test cases from Phase 2 strategy (main, corner, edge, error)
13. **Build Success**: MUST achieve zero compilation errors and warnings
14. **Test Success**: MUST achieve 100% test pass rate (zero failures)
15. **Commit Template Compliance**: MUST use EXACT commit message template without additional content

### ABSOLUTE PROHIBITIONS (Zero Tolerance)

1. **Workflow Violations**: Starting work without creating TODO list
2. **Phase Skipping**: Bypassing any required phase or task within phases
3. **Invalid Test Constructs**: Using GTEST_SKIP(), placeholder assertions, or non-substantive validations
4. **Meaningless Assertions**: Using ASSERT(true), ASSERT(false), ASSERT_TRUE(true), ASSERT_FALSE(false), or any similar trivial assertions that provide no validation value
5. **Location Violations**: Building tests outside tests/unit/ directory
6. **Process Shortcuts**: Skipping iterative issue resolution cycles
7. **Template Violations**: Adding content beyond specified commit message template
8. **Unchecked Operation Status**: Using if conditions without ASSERT validation for operation results
9. **Incomplete Implementation**: Generating partial test suites or missing test categories
10. **Quality Compromises**: Accepting compilation warnings, test failures, or runtime crashes
11. **Documentation Shortcuts**: Missing function comments with source location and target coverage goals

### Critical Success Gates

**Gate 1 (Phase 1)**: Comprehensive opcode analysis completed with category classification  
**Gate 2 (Phase 2)**: Complete test strategy documented with all 4 test categories  
**Gate 3 (Phase 3)**: Production-ready code generated with proper directory structure  
**Gate 4 (Phase 4)**: Build successful with 100% test pass rate and measurable coverage  
**Gate 5 (Phase 5)**: All issues resolved (conditional gate - only if Phase 4 fails)  
**Gate 6 (Phase 6)**: Quality review passed and standardized commit created  

**FAILURE ESCALATION**: If any gate fails after 3 attempts, mark task as FAILED and document specific blocking issues.

## Implementation: Sequential 6-Phase Execution

**CRITICAL FIRST STEP: You MUST create the standardized TODO list using the TodoWrite tool before starting any analysis or work.**

For the opcode "${1}", execute this workflow with mandatory TODO list management:

### PHASE 1: Ultra-Deep Opcode Analysis
Perform comprehensive analysis of "${1}":

1. **Semantic Analysis**: Identify the opcode's primary function, purpose, and operation
2. **Type System Analysis**: Determine input/output types, conversions, and polymorphism
3. **Stack Effect Analysis**: Document stack transformations (pop/push counts, positions)
4. **Edge Case Discovery**: Identify boundary values, special numeric values, overflow conditions
5. **Error Condition Mapping**: Map type mismatches, stack underflow, out-of-bounds access, traps
6. **Category Classification**: Classify into numeric, memory, control-flow, variable, reference, or extension

**Automatic Category Detection (Examples):**
- **Numeric**: `i32.add`, `f64.mul`, `i64.eqz` - Arithmetic, comparison, bitwise operations
- **Memory**: `i32.load`, `memory.grow` - Load, store, memory control operations
- **Control Flow**: `br_if`, `call`, `loop` - Branching, loops, function calls
- **Variable**: `local.get`, `global.set` - Local and global variable access
- **Reference**: `ref.null`, `ref.func` - Reference type operations
- **Extension**: `v128.add`, `i32.atomic.load` - SIMD, atomic operations

**Note**: These are sample opcodes. For the complete WebAssembly instruction set, refer to the official specification: https://webassembly.github.io/spec/core/appendix/index-instructions.html

**After Phase 1**: Update TODO list marking tasks 1.1-1.6 as completed, display progress percentage, declare Phase 2 as next.

### PHASE 2: Strategic Test Planning
Generate comprehensive test strategy for "${1}":

1. **Main Routine Tests**: Basic functionality with typical input values
2. **Corner Case Tests**: Boundary conditions, overflow/underflow, signed/unsigned boundaries
3. **Edge Case Tests**: Zero operands, identity operations, extreme values (MIN/MAX, NaN, Infinity)
4. **Error Exception Tests**: Invalid operand types, stack underflow, out-of-bounds access

For each category, specify:
- Detailed test case descriptions
- Specific input conditions and setup requirements
- Expected outcomes and behaviors
- Meaningful assertion statements
- WASM module requirements

**After Phase 2**: Update TODO list marking tasks 2.1-2.6 as completed, display progress percentage, declare Phase 3 as next.

### PHASE 3: Complete Code Generation
Generate production-ready test suite in `tests/unit/enhanced_opcode/{CATEGORY}/`(If not existed previous):

**Directory Structure:**
```
tests/unit/enhanced_opcode/{CATEGORY}/
├── CMakeLists.txt
├── enhanced_{opcode}_test.cc
├── {opcode}_common.h (if needed)
└── wasm-apps/
    ├── {opcode}_test.wat
    └── {opcode}_test.wasm
```

**Requirements:**
- Follow WAMR coding standards
- Use GTest framework with TestWithParam<RunningMode>
- Include proper SetUp/TearDown with WAMR initialization
- Implement ALL test cases from Phase 2 with meaningful ASSERT_* statements
- NO GTEST_SKIP(), SUCCEED(), or FAIL() calls
- Generate comprehensive WASM test files (.wat and .wasm)
- Create proper CMakeLists.txt with dependencies and coverage support
- **CMakeLists.txt Build Structure**: Follow the build structure pattern from `memory64/CMakeLists.txt` including:
  - Proper WAMR build flags (WAMR_BUILD_INTERP, WAMR_BUILD_AOT, etc.)
  - Add or modify related build MACRO flags to enable the opcode feature being tested
  - Include `unit_common.cmake` for shared build configuration
  - Set appropriate definitions (e.g., `-DRUN_ON_LINUX`)
  - Link against required libraries (gtest_main, LLVM if needed)
  - Copy WASM test files to build directory with POST_BUILD commands

**After Phase 3**: Update TODO list marking tasks 3.1-3.6 as completed, display progress percentage, declare Phase 4 as next.

### PHASE 4: Build & Test Execution
Execute the build and test process:

```bash
cd tests/unit/
cmake -S enhanced_opcode/{CATEGORY} -B build/enhanced_opcode/{CATEGORY} -DCOLLECT_CODE_COVERAGE=1
cmake --build build/enhanced_opcode/{CATEGORY} --parallel $(nproc)
ctest --test-dir build/enhanced_opcode/{CATEGORY} --output-on-failure --verbose
```

Validate:
- All source files compile without warnings
- All unit tests pass (0 failures)
- No runtime crashes or memory leaks
- Coverage improvement is measurable

**After Phase 4**: Update TODO list marking tasks 4.1-4.6 as completed. If successful, proceed to Phase 6. If failed, declare Phase 5 as next.

### PHASE 5: Issue Detection & Resolution (Apply only if Phase 4 fails)
If build or tests fail:

1. **Issue Detection**: Analyze compilation errors, runtime crashes, or assertion failures
2. **Root Cause Analysis**: Categorize as compilation, runtime, test logic, or coverage issue
3. **Targeted Resolution**: Apply appropriate fixes (missing includes, null checks, correct expected values)
4. **Verification**: Re-run build and test to confirm resolution
5. **Iteration**: Repeat until all issues are resolved

**After Phase 5**: Update TODO list marking tasks 5.1-5.5 as completed, then declare Phase 6 as next.

### PHASE 6: Code Review & Standardized Commit
Perform final review and commit:

1. **Quality Review**: Verify WAMR coding standards, meaningful test descriptions, proper resource management
2. **Coverage Analysis**: Confirm comprehensive test coverage across all scenarios
3. **Performance Assessment**: Validate reasonable execution time and memory usage
4. **Commit Creation**: Stage files and create commit using EXACT template below

**Commit Process:**
```bash
git add tests/unit/enhanced_opcode/{CATEGORY}/{OPCODE_NAME}*
git add tests/unit/enhanced_opcode/{CATEGORY}/wasm-apps/{OPCODE_NAME}*
git add tests/unit/enhanced_opcode/{CATEGORY}/CMakeLists.txt

git commit -s -m "Enhanced unit tests for {OPCODE_NAME} opcode - Comprehensive test coverage

## Summary
- Opcode: {OPCODE_NAME} (Category: {CATEGORY})
- Test Cases: {TEST_COUNT} comprehensive tests generated
- Files Modified: {FILES_MODIFIED}"
```

**After Phase 6**: Update TODO list marking tasks 6.1-6.5 as completed.

---
## Success Criteria Checklist

Before completion, validate ALL items:

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

Execute all phases sequentially. Only proceed to the next phase after the current phase completes successfully. Phase 5 is conditional - only execute if Phase 4 reports failures.