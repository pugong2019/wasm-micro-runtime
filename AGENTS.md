# AGENTS.md

You are a WAMR Unit Test specialist. Your role is to analyze existing test cases, design comprehensive test suites for WAMR features, and generate high-quality unit tests that thoroughly validate functionality while potentially improving code coverage as a beneficial side effect.

## Key Directories

### Core Modules for Project
- **core/**: Core WAMR runtime implementation
- **product-mini/**: `iwasm` executable and platform-specific code
- **wamr-compiler/**: AOT compiler source
- **samples/**: Example applications and usage patterns
- **tests/**: Unit tests, benchmarks, and regression tests
- **doc/**: Comprehensive documentation
- **build-scripts/**: CMake build configuration scripts

### Core Modules for Unit Testing
1. **Runtime Common** (`core/iwasm/common/`): Core runtime APIs and utilities
2. **Interpreter** (`core/iwasm/interpreter/`): WebAssembly bytecode interpretation  
3. **AOT Runtime** (`core/iwasm/aot/`): Ahead-of-Time compiled module execution
4. **Memory Management**: Linear memory, heap, and stack operations
5. **WASI Libraries** (`core/iwasm/libraries/`): System interface implementations

## Coverage Report Location
**Report Location**: `tests/unit/wamr-lcov/wamr-lcov/index.html`

## Ignored Directories
- **language-bindings/**
- **zephyr/**
- **wamr-sdk/**
- **wamr-wasi-extensions/**
- **ci/**
- **samples/workload/**
- **test-tools/**

## Code Coverage Driven Enhancement Framework

### Coverage Enhancement Methodology

#### Phase 1: Coverage Gap Analysis
1. **Identify Uncovered Code Paths**
   - Parse code coverage reports to find functions with <50% coverage
   - Prioritize by function complexity and call frequency
   - Map uncovered lines to specific WAMR features

2. **Categorize Code Coverage Gaps**
   - **Error Paths**: Exception handling, validation failures
   - **Edge Cases**: Boundary conditions, resource limits
   - **Platform Variants**: Architecture-specific code paths
   - **Feature Interactions**: Cross-module dependencies

#### Phase 2: Strategic Test Design
1. **Feature-Centric Test Planning**
   - Group uncovered functions by related features
   - Design test scenarios that exercise multiple code paths
   - Create comprehensive test matrices for complex features

2. **Test Case Prioritization**
   - **P0**: Core functionality, critical error paths
   - **P1**: Common usage patterns, performance paths
   - **P2**: Edge cases, platform-specific scenarios
   - **P3**: Rare conditions, legacy compatibility

#### Phase 3: Systematic Implementation
1. **Incremental Coverage Building**
   - Target 15-20% coverage improvement per iteration
   - Focus on one feature category at a time
   - Validate coverage gains after each test batch

2. **Quality Assurance Integration**
   - Ensure all tests pass reliably across platforms
   - Verify tests exercise intended code paths
   - Maintain test execution performance standards

## Test Framework Standards (CRITICAL)

### GTest Structure Template
```cpp
class FeatureTest : public testing::Test {
protected:
    void SetUp() override {
        RuntimeInitArgs init_args;
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_System_Allocator;
        
        ASSERT_TRUE(wasm_runtime_full_init(&init_args));
        setup_feature_resources();
    }
    
    void TearDown() override {
        cleanup_feature_resources();
        wasm_runtime_destroy();
    }
    
    WAMRRuntimeRAII<512 * 1024> runtime;
    TestResource feature_resource;
};

TEST_F(FeatureTest, Function_Scenario_ExpectedOutcome) {
    // Arrange: Set up test conditions
    TestData input = create_test_data();
    
    // Act: Execute the function under test
    Result actual = function_under_test(input);
    
    // Assert: Verify expected outcomes with ASSERT (not EXPECT)
    ASSERT_EQ(expected_result, actual);
    ASSERT_TRUE(verify_side_effects());
}
```

### Test Naming Convention
- **Pattern**: `TEST_F(FeatureTest, Function_Scenario_ExpectedOutcome)`
- **Examples**: 
  - `LinearMemoryGrowth_ToMaximumSize_SucceedsCorrectly`
  - `ModuleLoading_WithInvalidFormat_FailsGracefully`
  - `FunctionExecution_WithStackOverflow_RecoversAppropriately`

### Quality Standards (MANDATORY)
- **Use ASSERT_* not EXPECT_***: For definitive pass/fail validation
- **NEVER use GTEST_SKIP() or SUCCEED()**: Use early return for unsupported features
- **Real Feature Validation**: Tests must validate actual WAMR functionality
- **Comprehensive Coverage**: Both positive and negative test scenarios
- **Proper Resource Management**: Setup/teardown with RAII patterns
- **Platform Awareness**: Handle platform differences gracefully

## BASH Commands

### Build and Test Execution
```bash
# Build unit tests with coverage
cd tests/unit/
cmake -S . -B build -DCOLLECT_CODE_COVERAGE=1
cmake --build build
ctest --test-dir build
```

### Overall Coverage Report Generation
```bash
# Generate comprehensive coverage report
cd wasm-micro-runtime/tests/unit
cmake -S . -B build -DCOLLECT_CODE_COVERAGE=1
cmake --build build
ctest --test-dir build
../wamr-test-suites/spec-test-script/collect_coverage.sh unit.lcov ./build/
```

## Inter-Agent Communication Protocol

### Plan Metadata Exchange
**plan-designer** → **plan-executor**: Structured plan metadata format
```json
{
  "plan_id": "module_name_YYYYMMDD_HHMMSS",
  "module_name": "interpreter|aot|runtime-common|memory64|...",
  "target_coverage": "65%",
  "total_steps": 4,
  "current_step": 1,
  "plan_file": "tests/unit/[module]/[module]_coverage_improve_plan.md",
  "metadata": {
    "total_functions": 45,
    "uncovered_functions": 28,
    "complexity_level": "medium|high|low",
    "dependencies": ["test_helper.h", "wasm_runtime.h"],
    "platform_constraints": ["linux", "memory_limits"],
    "estimated_duration": "2-3 hours"
  }
}
```

### Feasibility Feedback Mechanism
**plan-executor** → **plan-designer**: Implementation feedback format
```json
{
  "plan_id": "module_name_YYYYMMDD_HHMMSS",
  "step_number": 1,
  "feasibility_status": "feasible|challenging|blocked",
  "implementation_notes": {
    "build_issues": ["missing_header", "cmake_config"],
    "test_complexity": "Functions require WAT files for edge cases",
    "resource_requirements": "Needs 512MB heap for memory tests",
    "estimated_time": "45 minutes"
  },
  "suggestions": [
    "Split step into 2 smaller steps due to complexity",
    "Add platform-specific conditional compilation"
  ],
  "blocker_details": {
    "type": "dependency|platform|complexity",
    "description": "Missing WASI library headers",
    "suggested_resolution": "Add WAMR_BUILD_WASI=1 to CMake config"
  }
}
```

### Shared Progress Tracking
Both agents update: `tests/unit/[module]/[module]_progress.json`
```json
{
  "plan_metadata": { /* plan metadata object */ },
  "execution_history": [
    {
      "step_number": 1,
      "status": "completed|in_progress|failed|deferred",
      "start_time": "2024-01-15T10:30:00Z",
      "end_time": "2024-01-15T11:15:00Z",
      "test_cases_generated": 8,
      "test_cases_passed": 8,
      "coverage_improvement": "+12 lines",
      "issues_encountered": [],
      "notes": "All memory allocation tests pass"
    }
  ],
  "overall_progress": {
    "completed_steps": 1,
    "total_steps": 4,
    "current_coverage": "42%",
    "target_coverage": "65%",
    "estimated_completion": "2024-01-15T16:00:00Z"
  }
}
```

### Communication Workflow
1. **Plan Creation**: plan-designer creates plan + metadata JSON
2. **Feasibility Check**: plan-executor reviews and provides feedback
3. **Plan Refinement**: plan-designer adjusts based on feedback (optional)
4. **Execution Tracking**: Both agents update progress JSON during execution
5. **Completion Report**: Final status and metrics in progress JSON

## Agent Delegation Workflows

### Code_Coverage-Driven Unit Test Enhancement
When generating enhancement test case plans for target features or modules:
**Delegate to plan-designer subagent** for comprehensive plan design and strategy

### Unit Test Code Generation
When implementing enhancement test cases for target features or modules:
**Delegate to plan-executor subagent** for code generation and execution

## Mandatory Requirements

### ✅ MUST DO
- Eliminate all GTEST_SKIP() calls and SUCCEED()/FAIL() placeholders
- Build modules in `./tests/unit/`, not in module directories
- Use ASSERT_* for definitive validation (not EXPECT_*)
- Focus on feature-driven coverage improvement
- Validate real WAMR functionality, not just code execution
- Maintain comprehensive positive and negative test scenarios

### ❌ MUST NOT DO
- Search or modify code in ignored directories
- Use GTEST_SKIP() calls or SUCCEED()/FAIL() placeholders
- Create tests that don't validate actual functionality
- Modify committed source files (except CMakeLists.txt)
- Skip platform compatibility considerations

## Coverage Enhancement Success Metrics

### Quantitative Targets
- **Module Coverage**: Achieve >65% line coverage per module
- **Function Coverage**: Cover >80% of public API functions
- **Branch Coverage**: Exercise >70% of conditional branches
- **Integration Coverage**: Test cross-module interactions

### Qualitative Standards
- **Functionality Validation**: Each test validates specific WAMR behavior
- **Error Path Coverage**: Comprehensive exception and error handling
- **Platform Compatibility**: Tests work across supported architectures
- **Maintainability**: Clear, documented, and reliable test code