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

### Existing Unit Test Modules
Current test directories in `tests/unit/`:
- `aot/`, `aot-stack-frame/`: AOT runtime testing
- `interpreter/`: Interpreter functionality  
- `runtime-common/`: Common runtime operations
- `memory64/`, `linear-memory-*/`: Memory system testing
- `shared-heap/`, `shared-utils/`: Memory sharing features
- `gc/`: Garbage collection (experimental)
- `compilation/`: AOT compilation pipeline

## Ignored Directories
- **language-bindings/**
- **zephyr/**
- **wamr-sdk/**
- **wamr-wasi-extensions/**
- **ci/**
- **samples/workload/**
- **test-tools/**

## Key Conduct Principles (CRITICAL)

1. ### Test Framework
    - GTEST Framework 
    - Test Case Structure Template
        ```cpp
        class ModuleTest : public testing::Test {
        protected:
            void SetUp() override {
                // Initialize test environment
                // Acquire resources
                // Set up test data
            }
            
            void TearDown() override {
                // Clean up resources
                // Reset state
                // Remove temporary files
            }
            
            // Test fixtures and helper data
            WAMRRuntimeRAII<512 * 1024> runtime;
            TestResource resource;
        };

        TEST_F(ModuleTest, FunctionName_Scenario_ExpectedOutcome) {
            // Arrange: Set up test conditions
            TestData input = create_test_data();
            
            // Act: Execute the function under test
            Result actual = function_under_test(input);
            
            // Assert: Verify expected outcomes
            EXPECT_EQ(expected_result, actual);
            EXPECT_TRUE(verify_side_effects());
        }
        ```

2. ### Unit Test Structure
    - #### File Structure
        - All unit tests use **Google Test (GTest)** framework
        - Test files located in `tests/unit/[ModuleName]/`
        - If module directory doesn't exist, create it following the pattern
        - Common test utilities in `tests/unit/common/test_helper.h`

    - #### Creating New Unit Test Modules Workflow
        1. Create directory: `tests/unit/[ModuleName]/`
        2. Add `CMakeLists.txt` following existing patterns
        3. Create test files: `test_[feature].cc`
        4. Include in main unit test build via `tests/unit/CMakeLists.txt`

3. ### Code Convention 
    - Strictly Follow Current Unit Code Convention
    - Test Naming
        ```cpp
        // Test naming: TEST_F(TestClass, TestName)
        TEST_F(WasmRuntimeTest, LoadValidModule) {
            // Test implementation
            WAMRModule module(test_wasm_buffer, sizeof(test_wasm_buffer));
            EXPECT_NE(module.get(), nullptr);
        }

        // Use WAMR test utilities:
        // - WAMRRuntimeRAII: Auto-manage runtime lifecycle
        // - WAMRModule: RAII wrapper for wasm_module_t
        // - WAMRInstance: RAII wrapper for wasm_module_inst_t  
        // - WAMRExecEnv: RAII wrapper for wasm_exec_env_t
        // - DummyExecEnv: Complete test environment setup
        ```

## BASH Commands
1. ### Build and Run Unit Tests
    Build single test module:
    ```bash
    # Build unit tests
    cd tests/unit/
    cmake -S . -B build -DCOLLECT_CODE_COVERAGE=1
    cmake --build build
    ctest --test-dir build    
    ```
2. ###  Final Coverage Collection
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

## Feature-Driven Unit Test Enhancement
When need to generate enhancement test case plan for the target feature or module,
Ask **plan-designer** subagent to help implemente the plan design and generate work

## Generate Unit Test Code
When need to generate enhancement test case for the target feature or module,
Ask **plan-executor** subagent to do the code generate work

**YOU MUST NOT:**
- Search any codes in the **Ignored Directories**