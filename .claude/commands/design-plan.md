# WAMR Test Case Design Command

## Purpose
Analyzes uncovered WAMR code sections to prioritize common routine code paths for maximum coverage improvement. Focuses on frequently executed code patterns and main execution flows rather than edge cases or error handling.

## Required Input Format
- **module**: [aot|interpreter|runtime-common|libraries|...]  
- **file**: [source_filename.c]  
- **Lines**: [line ranges]  

## Required Output
**tests/unit/[module]/[source_filename]_test_plan.md**

## MUST Strictly Follow Step-by-Step Analysis Process

Follow these 4 steps sequentially for each code analysis request:

### Step 1: Code Structure Analysis
**Objective:** Understand what you're testing

**Actions:**
1. Read and analyze the target code section
2. Identify function signatures and parameters
3. Map out all dependencies (internal/external)
4. Document required setup and initialization
5. Identify all error paths and edge cases

**Output Template:**
```markdown
## Step 1: Code Structure Analysis for [function_name]

### Function Overview
- **Signature**: [function signature]
- **Purpose**: [brief description]
- **Location**: [file:line_numbers]

### Dependencies
- **Internal**: [WAMR modules/functions required]
- **External**: [system calls, libraries, etc.]
- **Global State**: [shared variables, static data]

### Code Paths Analysis Priority
- **Primary Flow**: [main execution path - highest priority for coverage]
- **Common Branches**: [frequently executed conditional paths]
- **Routine Operations**: [standard processing patterns]
- **Secondary Paths**: [less frequent but valid execution flows]
```

### Step 2: Complexity Assessment
**Objective:** Rate the testing difficulty and effort required

**Actions:**
1. Evaluate setup requirements (1-5 scale)
2. Assess dependency complexity (1-5 scale)
3. Analyze logic complexity (1-5 scale)
4. Identify testability barriers
5. Calculate overall complexity rating

**Complexity Scale (Routine Code Focus):**
- **1 - TRIVIAL**: Common utility functions, simple operations (Direct calls, no dependencies)
- **2 - SIMPLE**: Standard WAMR operations, typical processing paths (Basic initialization, common patterns)
- **3 - MODERATE**: Multi-step routines, standard WAMR workflows (Normal dependencies, routine setup)
- **4 - COMPLEX**: Advanced internals or platform-specific code (Focus only if covers many lines)
- **5 - EXPERT**: System-dependent or extremely specialized code (Generally avoid unless critical routine)

**Output Template:**
```markdown
## Step 2: Complexity Assessment for [function_name]

### Complexity Factors
- **Setup Requirements**: [1-5] - [justification]
- **Dependency Complexity**: [1-5] - [justification]
- **Logic Complexity**: [1-5] - [justification]
- **Testability Barriers**: [list specific barriers]

### Overall Complexity Rating: [1-5]
**Justification**: [explain the rating choice]
```

### Step 3: Implementation Decision
**Objective:** Make decision based on complexity assessment

**Routine Code Decision Rule:**
- **Complexity 1-3**: IMPLEMENT (Focus on maximum line coverage)
- **Complexity 4**: IMPLEMENT if high-value routine covering 10+ lines, otherwise DROP
- **Complexity 5**: DROP (unless absolutely critical common routine)

**Output Template:**
```markdown
## Step 3: Implementation Decision for [function_name]

### Decision: [IMPLEMENT/DROP]
### Complexity: [1-5]
### Rationale: [Brief explanation based on complexity factors]
```

### Step 4: Strategy Design (Only if need IMPLEMENT)
**Objective:** Create specific test implementation plan

**Test Approaches for Routine Code Coverage:**
- **Complexity 1-2**: Direct Routine Testing
  - Minimal WAMR setup, direct function calls
  - Focus on common input patterns and typical return values
  - Test main execution paths only

- **Complexity 3**: Standard Workflow Testing
  - Standard WAMR environment setup
  - Test normal operation flows and common variations
  - Focus on routine processing patterns

**Actions:**
1. Choose test approach based on complexity level
2. Design specific test cases
3. Provide implementation template

**Key Test Cases for Maximum Coverage**:
- **Primary Flow**: [test main execution path - covers most lines]
- **Common Variations**: [test frequent code branches]
- **Routine Operations**: [test standard processing patterns]
- **Secondary Flows**: [test alternative paths only if they cover significant lines]


### Required Output Format

After completing the analysis, provide ONLY this concise output into file **tests/unit/[module]/[source_filename]_test_plan.md**:

```markdown
# Decision for [function_name] ([source_filename:lines])

**Module**: [aot|interpreter|runtime-common|libraries|...]  
**File**: [source_filename.c]  
**Lines**: [line ranges]  

## DECISION: [IMPLEMENT/DROP]
**Complexity**: [1-5]
**Rationale**: [Brief justification based on complexity]

## IMPLEMENTATION STRATEGY (Only if IMPLEMENT)
### Test Approach: [Direct/Mock-Assisted]

### Implementation Plan:
```cpp
TEST_F(Enhanced[Module]Test, [Function]_[Scenario_1]) {
    // Step 1: [Setup description]
    // Step 2: [Execute function]
    // Step 3: [Assert results]
    // Step 4: [Cleanup if needed]
}
...
TEST_F(Enhanced[Module]Test, [Function]_[Scenario_N]) {
    // Step 1: [Setup description]
    // Step 2: [Execute function]
    // Step 3: [Assert results]
    // Step 4: [Cleanup if needed]
}
```
## Success Criteria

A successful test case design analysis must meet ALL of the following criteria:

### 1. Analysis Completeness
- [ ] All 4 steps completed sequentially (Code Structure → Complexity → Decision → Strategy)
- [ ] Function signature, dependencies, and code paths documented
- [ ] All error paths and edge cases identified
- [ ] Complexity rating justified with specific evidence

### 2. Decision Quality
- [ ] Complexity rating (1-5) aligns with assessment criteria
- [ ] Implementation decision follows the rule: Complexity 1-3 = IMPLEMENT, 4-5 = DROP
- [ ] Rationale clearly explains the decision based on complexity factors
- [ ] Testability barriers accurately identified

### 3. Implementation Strategy (for IMPLEMENT decisions only)
- [ ] Test approach matches complexity level (Direct Routine for 1-2, Standard Workflow for 3)
- [ ] Key test cases prioritized: Primary Flow, Common Variations, Routine Operations
- [ ] Test implementation template provided focusing on line coverage maximization
- [ ] Test names follow naming convention: `TEST_F(Enhanced[Module]Test, [Function]_[Scenario])`

### 4. Output Deliverable
- [ ] Analysis saved to correct location: `tests/unit/[module]/[source_filename]_test_plan.md`
- [ ] Output follows exact markdown template format
- [ ] File contains only the concise decision summary (not the full analysis process)
- [ ] All placeholders replaced with actual values

### 5. Quality Standards
- [ ] Analysis based on actual code examination (not assumptions)
- [ ] Dependencies mapped accurately to WAMR architecture
- [ ] Test cases prioritize routine scenarios with maximum line coverage potential
- [ ] Implementation plan focuses on common code paths rather than edge cases




