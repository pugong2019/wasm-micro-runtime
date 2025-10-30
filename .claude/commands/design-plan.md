# WAMR Test Case Design Command

## Purpose
Analyzes uncovered WAMR code sections to assess testing complexity, determine feasibility, and provide actionable test strategies or alternative recommendations.

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

### Code Paths
- **Happy Path**: [normal execution flow]
- **Error Paths**: [failure scenarios]
- **Edge Cases**: [boundary conditions]
```

### Step 2: Complexity Assessment
**Objective:** Rate the testing difficulty and effort required

**Actions:**
1. Evaluate setup requirements (1-5 scale)
2. Assess dependency complexity (1-5 scale)
3. Analyze logic complexity (1-5 scale)
4. Identify testability barriers
5. Calculate overall complexity rating

**Complexity Scale:**
- **1 - TRIVIAL**: Simple function, minimal setup (Direct calls, no dependencies)
- **2 - SIMPLE**: Basic WAMR functionality (Standard initialization, limited dependencies)
- **3 - MODERATE**: Complex logic, substantial setup (Multiple dependencies, mocking required)
- **4 - COMPLEX**: Advanced WAMR internals (Deep integration, platform-specific)
- **5 - EXPERT**: System-dependent, extremely difficult to mock(e.g.Root privileges, hardware-specific)

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

**Simple Decision Rule:**
- **Complexity 1-3**: IMPLEMENT
- **Complexity 4-5**: DROP

**Output Template:**
```markdown
## Step 3: Implementation Decision for [function_name]

### Decision: [IMPLEMENT/DROP]
### Complexity: [1-5]
### Rationale: [Brief explanation based on complexity factors]
```

### Step 4: Strategy Design (Only if need IMPLEMENT)
**Objective:** Create specific test implementation plan

**Test Approaches by Complexity:**
- **Complexity 1-2**: Direct Testing
  - Minimal WAMR setup, direct function calls
  - Focus on input validation and return values

- **Complexity 3**: Mock-Assisted Testing
  - Full WAMR environment with mocks
  - Test normal, error, and state transitions

**Actions:**
1. Choose test approach based on complexity level
2. Design specific test cases
3. Provide implementation template

**Key Test Cases**:
- **Happy Path**: [test normal execution]
- **Error Cases**: [test failure scenarios]
- **Edge Cases**: [test boundary conditions]


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
- [ ] Test approach matches complexity level (Direct for 1-2, Mock-Assisted for 3)
- [ ] All key test cases defined: Happy Path, Error Cases, Edge Cases
- [ ] Test implementation template provided with realistic setup steps
- [ ] Test names follow naming convention: `TEST_F(Enhanced[Module]Test, [Function]_[Scenario])`

### 4. Output Deliverable
- [ ] Analysis saved to correct location: `tests/unit/[module]/[source_filename]_test_plan.md`
- [ ] Output follows exact markdown template format
- [ ] File contains only the concise decision summary (not the full analysis process)
- [ ] All placeholders replaced with actual values

### 5. Quality Standards
- [ ] Analysis based on actual code examination (not assumptions)
- [ ] Dependencies mapped accurately to WAMR architecture
- [ ] Test cases cover realistic scenarios for the target function
- [ ] Implementation plan is actionable and specific




