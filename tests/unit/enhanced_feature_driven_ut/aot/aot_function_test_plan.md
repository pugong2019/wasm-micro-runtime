# Function-Based Test Plan for AOT Module

## Function Analysis Summary
- Total Functions: 24
- Public Functions: 21 (direct testing)
- Static Functions: 3 (indirect testing via callers)
- Functions per Step: ≤10 (optimized for LLM generation)
- Total Steps Required: 3

## Function Classification

### Public Functions (Direct Testing)
1. **aot_call_indirect** - `aot_runtime.c:3288`
   - **Accessibility**: Public API
   - **Testing Strategy**: Direct function calls with comprehensive input validation
   - **Parameters**: WASMExecEnv *exec_env, uint32 tbl_idx, uint32 table_elem_idx, uint32 argc, uint32 *argv
   - **Return Type**: bool
   - **Test Scenarios**: Valid indirect calls, invalid table indices, null parameters, argc/argv mismatches

2. **aot_data_drop** - `aot_runtime.c:3582`
   - **Accessibility**: Public API
   - **Testing Strategy**: Direct function calls with comprehensive input validation
   - **Parameters**: AOTModuleInstance *module_inst, uint32 seg_index
   - **Return Type**: void
   - **Test Scenarios**: Valid segment drops, invalid segment indices, null module instance

3. **aot_enlarge_memory_with_idx** - `aot_runtime.c:3192`
   - **Accessibility**: Public API
   - **Testing Strategy**: Direct function calls with comprehensive input validation
   - **Parameters**: AOTModuleInstance *module_inst, uint32 inc_page_count, uint32 memidx
   - **Return Type**: bool
   - **Test Scenarios**: Valid memory enlargement, invalid memory indices, boundary conditions

4. **aot_free_tiny_frame** - `aot_runtime.c:4197`
   - **Accessibility**: Public API
   - **Testing Strategy**: Direct function calls with comprehensive input validation
   - **Parameters**: WASMExecEnv *exec_env
   - **Return Type**: void
   - **Test Scenarios**: Valid frame deallocation, null execution environment, frame stack consistency

5. **aot_get_aux_stack** - `aot_runtime.c:3629`
   - **Accessibility**: Public API
   - **Testing Strategy**: Direct function calls with comprehensive input validation
   - **Parameters**: WASMExecEnv *exec_env, uint64 *start_offset, uint32 *size
   - **Return Type**: bool
   - **Test Scenarios**: Valid aux stack retrieval, null parameters, uninitialized aux stack

6. **aot_get_function_instance** - `aot_runtime.c:1461`
   - **Accessibility**: Public API
   - **Testing Strategy**: Direct function calls with comprehensive input validation
   - **Parameters**: AOTModuleInstance *module_inst, uint32 func_idx
   - **Return Type**: AOTFunctionInstance*
   - **Test Scenarios**: Valid function lookup, invalid function indices, null module instance

7. **aot_get_memory_with_idx** - `aot_runtime.c:1206`
   - **Accessibility**: Public API
   - **Testing Strategy**: Direct function calls with comprehensive input validation
   - **Parameters**: AOTModuleInstance *module_inst, uint32 mem_idx
   - **Return Type**: AOTMemoryInstance*
   - **Test Scenarios**: Valid memory retrieval, invalid memory indices, null module instance

8. **aot_get_module_inst_mem_consumption** - `aot_runtime.c:3744`
   - **Accessibility**: Public API
   - **Testing Strategy**: Direct function calls with comprehensive input validation
   - **Parameters**: const AOTModuleInstance *module_inst, uint64 *mem_consumption
   - **Return Type**: uint64
   - **Test Scenarios**: Valid memory consumption calculation, null parameters, complex module instances

9. **aot_get_module_mem_consumption** - `aot_runtime.c:3661`
   - **Accessibility**: Public API
   - **Testing Strategy**: Direct function calls with comprehensive input validation
   - **Parameters**: const AOTModule *module, uint64 *mem_consumption, uint32 *const_str_set_size
   - **Return Type**: uint64
   - **Test Scenarios**: Valid module memory calculation, null parameters, various module sizes

10. **aot_lookup_function_with_idx** - `aot_runtime.c:1408`
    - **Accessibility**: Public API
    - **Testing Strategy**: Direct function calls with comprehensive input validation
    - **Parameters**: AOTModuleInstance *module_inst, uint32 func_idx
    - **Return Type**: void*
    - **Test Scenarios**: Valid function lookups, invalid indices, exported vs imported functions

11. **aot_lookup_memory** - `aot_runtime.c:1180`
    - **Accessibility**: Public API
    - **Testing Strategy**: Direct function calls with comprehensive input validation
    - **Parameters**: AOTModuleInstance *module_inst, char const *name
    - **Return Type**: AOTMemoryInstance*
    - **Test Scenarios**: Valid memory lookup by name, invalid names, null parameters

12. **aot_memory_init** - `aot_runtime.c:3541`
    - **Accessibility**: Public API
    - **Testing Strategy**: Direct function calls with comprehensive input validation
    - **Parameters**: AOTModuleInstance *module_inst, uint32 seg_index, uint32 offset, uint32 len, size_t dst
    - **Return Type**: bool
    - **Test Scenarios**: Valid memory initialization, boundary conditions, invalid segments

13. **aot_resolve_function** - `aot_runtime.c:5540`
    - **Accessibility**: Public API
    - **Testing Strategy**: Direct function calls with comprehensive input validation
    - **Parameters**: const AOTModule *module, const char *function_name, void **p_func, void **p_signature
    - **Return Type**: bool
    - **Test Scenarios**: Valid function resolution, invalid names, null parameters

14. **aot_resolve_function_ex** - `aot_runtime.c:5545`
    - **Accessibility**: Public API
    - **Testing Strategy**: Direct function calls with comprehensive input validation
    - **Parameters**: const char *module_name, const char *function_name, void **p_func, void **p_signature
    - **Return Type**: bool
    - **Test Scenarios**: Extended function resolution, cross-module resolution, invalid names

15. **aot_resolve_symbols** - `aot_runtime.c:5520`
    - **Accessibility**: Public API
    - **Testing Strategy**: Direct function calls with comprehensive input validation
    - **Parameters**: AOTModule *module
    - **Return Type**: bool
    - **Test Scenarios**: Valid symbol resolution, missing symbols, platform-specific symbols

16. **aot_set_aux_stack** - `aot_runtime.c:3595`
    - **Accessibility**: Public API
    - **Testing Strategy**: Direct function calls with comprehensive input validation
    - **Parameters**: WASMExecEnv *exec_env, uint64 start_offset, uint32 size
    - **Return Type**: bool
    - **Test Scenarios**: Valid aux stack setup, boundary conditions, overlapping regions

17. **aot_alloc_tiny_frame** - `aot_runtime.c:4136`
    - **Accessibility**: Public API
    - **Testing Strategy**: Direct function calls with comprehensive input validation
    - **Parameters**: WASMExecEnv *exec_env, uint32 func_index
    - **Return Type**: AOTFrame*
    - **Test Scenarios**: Valid frame allocation, memory exhaustion, invalid function indices

18. **aot_frame_update_profile_info** - `aot_runtime.c:4215`
    - **Accessibility**: Public API
    - **Testing Strategy**: Direct function calls with comprehensive input validation
    - **Parameters**: WASMExecEnv *exec_env, bool alloc_frame
    - **Return Type**: void
    - **Test Scenarios**: Profile info updates, frame allocation/deallocation tracking

19. **execute_free_function** - `aot_runtime.c:2912`
    - **Accessibility**: Public API
    - **Testing Strategy**: Direct function calls with comprehensive input validation
    - **Parameters**: AOTModuleInstance *module_inst, WASMExecEnv *exec_env, uint32 func_idx, uint32 argc, uint32 *argv
    - **Return Type**: bool
    - **Test Scenarios**: Valid free function execution, invalid parameters, memory consistency

20. **execute_malloc_function** - `aot_runtime.c:2807`
    - **Accessibility**: Public API
    - **Testing Strategy**: Direct function calls with comprehensive input validation
    - **Parameters**: AOTModuleInstance *module_inst, WASMExecEnv *exec_env, uint32 func_idx, uint32 argc, uint32 *argv
    - **Return Type**: bool
    - **Test Scenarios**: Valid malloc function execution, allocation failures, parameter validation

21. **const_string_node_size_cb** - `aot_runtime.c:3652`
    - **Accessibility**: Public API (callback function)
    - **Testing Strategy**: Direct function calls with comprehensive input validation
    - **Parameters**: void *key, void *value, void *p_const_string_size
    - **Return Type**: bool
    - **Test Scenarios**: Valid callback execution, null parameters, size calculation accuracy

### Static Functions (Indirect Testing via Callers)
1. **set_error_buf_v** - `aot_runtime.c:103`
   - **Accessibility**: Static (internal)
   - **Public Caller**: Various AOT functions that call `set_error_buf_v()`
   - **Testing Strategy**: Test through public APIs that trigger error conditions
   - **Call Chain**: Multiple public functions → `set_error_buf_v()`
   - **Test Scenarios**: Create error conditions in public APIs that exercise error buffer formatting
   - **Validation Method**: Assert on error message content and format

2. **check_global_init_expr** - `aot_runtime.c:192`
   - **Accessibility**: Static (internal)
   - **Public Caller**: Module instantiation functions that validate global expressions
   - **Testing Strategy**: Test through module loading with various global initialization expressions
   - **Call Chain**: Module instantiation → `check_global_init_expr()`
   - **Test Scenarios**: Valid/invalid global expressions, boundary conditions
   - **Validation Method**: Assert on module instantiation success/failure based on global expr validity

3. **cmp_export_func_map** - `aot_runtime.c:1400`
   - **Accessibility**: Static (internal)
   - **Public Caller**: Function lookup operations that use sorting/comparison
   - **Testing Strategy**: Test through function lookup operations that trigger sorting
   - **Call Chain**: Function lookup → sorting → `cmp_export_func_map()`
   - **Test Scenarios**: Multiple exported functions requiring sorting, function name comparisons
   - **Validation Method**: Assert on correct function lookup results that depend on proper sorting

## Test Implementation Plan

### Step 1: Memory & Function Management (10 functions)
**Target Functions**:
1. aot_get_memory_with_idx - aot_runtime.c:1206 (Public - Direct Testing)
2. aot_enlarge_memory_with_idx - aot_runtime.c:3192 (Public - Direct Testing)
3. aot_memory_init - aot_runtime.c:3541 (Public - Direct Testing)
4. aot_lookup_memory - aot_runtime.c:1180 (Public - Direct Testing)
5. aot_get_function_instance - aot_runtime.c:1461 (Public - Direct Testing)
6. aot_lookup_function_with_idx - aot_runtime.c:1408 (Public - Direct Testing)
7. aot_resolve_function - aot_runtime.c:5540 (Public - Direct Testing)
8. aot_resolve_function_ex - aot_runtime.c:5545 (Public - Direct Testing)
9. aot_resolve_symbols - aot_runtime.c:5520 (Public - Direct Testing)
10. cmp_export_func_map - aot_runtime.c:1400 (Static - Test via function lookup operations)

**Test Cases for Each Function**:

#### Direct Testing (Public Functions)
- [ ] **test_aot_get_memory_with_idx_basic_functionality**
  - **Test Target**: Validate aot_get_memory_with_idx() basic operation
  - **Test Steps**: 
    1. Create AOT module instance with valid memory configuration
    2. Call aot_get_memory_with_idx() with valid memory index
    3. Verify returned memory instance is not null and correctly configured
    4. Check memory instance properties match expected values
  - **Expected Outcomes**: Function returns valid memory instance for valid indices
  - **Edge Cases**: Invalid memory indices, null module instance, zero memory count

- [ ] **test_aot_enlarge_memory_with_idx_boundary_conditions**
  - **Test Target**: Validate aot_enlarge_memory_with_idx() memory growth
  - **Test Steps**:
    1. Create AOT module with limited memory configuration
    2. Call aot_enlarge_memory_with_idx() with various page counts
    3. Verify memory growth succeeds within limits
    4. Test failure cases with excessive page requests
  - **Expected Outcomes**: Memory enlargement succeeds within bounds, fails appropriately
  - **Edge Cases**: Maximum memory limits, zero page increments, invalid memory indices

- [ ] **test_aot_memory_init_data_segment_operations**
  - **Test Target**: Validate aot_memory_init() data segment initialization
  - **Test Steps**:
    1. Create AOT module with data segments
    2. Call aot_memory_init() with valid segment parameters
    3. Verify memory is correctly initialized with segment data
    4. Test boundary conditions and error scenarios
  - **Expected Outcomes**: Memory initialization succeeds with valid parameters
  - **Edge Cases**: Invalid segment indices, out-of-bounds offsets, overlapping segments

- [ ] **test_aot_lookup_memory_by_name**
  - **Test Target**: Validate aot_lookup_memory() name-based lookup
  - **Test Steps**:
    1. Create AOT module with named memory exports
    2. Call aot_lookup_memory() with valid memory names
    3. Verify correct memory instance is returned
    4. Test with invalid names and null parameters
  - **Expected Outcomes**: Correct memory instance returned for valid names
  - **Edge Cases**: Non-existent names, null name parameter, case sensitivity

- [ ] **test_aot_get_function_instance_validation**
  - **Test Target**: Validate aot_get_function_instance() function retrieval
  - **Test Steps**:
    1. Create AOT module with multiple functions
    2. Call aot_get_function_instance() with valid function indices
    3. Verify returned function instance is correct
    4. Test with invalid indices and boundary conditions
  - **Expected Outcomes**: Valid function instance returned for existing functions
  - **Edge Cases**: Invalid function indices, imported vs exported functions

- [ ] **test_aot_lookup_function_with_idx_comprehensive**
  - **Test Target**: Validate aot_lookup_function_with_idx() function pointer lookup
  - **Test Steps**:
    1. Create AOT module with indexed functions
    2. Call aot_lookup_function_with_idx() with various indices
    3. Verify correct function pointers are returned
    4. Test with invalid indices and null module
  - **Expected Outcomes**: Correct function pointers for valid indices
  - **Edge Cases**: Out-of-range indices, uninitialized functions

- [ ] **test_aot_resolve_function_symbol_resolution**
  - **Test Target**: Validate aot_resolve_function() symbol resolution
  - **Test Steps**:
    1. Create AOT module with exported functions
    2. Call aot_resolve_function() with function names
    3. Verify function pointers and signatures are correctly resolved
    4. Test with non-existent function names
  - **Expected Outcomes**: Successful resolution for exported functions
  - **Edge Cases**: Invalid names, null parameters, signature mismatches

- [ ] **test_aot_resolve_function_ex_extended_resolution**
  - **Test Target**: Validate aot_resolve_function_ex() extended resolution
  - **Test Steps**:
    1. Setup multiple AOT modules with cross-references
    2. Call aot_resolve_function_ex() with module and function names
    3. Verify cross-module function resolution works
    4. Test with invalid module/function combinations
  - **Expected Outcomes**: Successful cross-module function resolution
  - **Edge Cases**: Non-existent modules, circular dependencies

- [ ] **test_aot_resolve_symbols_complete_resolution**
  - **Test Target**: Validate aot_resolve_symbols() complete symbol resolution
  - **Test Steps**:
    1. Create AOT module with various symbol dependencies
    2. Call aot_resolve_symbols() to resolve all symbols
    3. Verify all required symbols are successfully resolved
    4. Test with missing symbols and platform differences
  - **Expected Outcomes**: All resolvable symbols are found and linked
  - **Edge Cases**: Missing platform symbols, circular dependencies

#### Indirect Testing (Static Functions)
- [ ] **test_function_lookup_exercises_cmp_export_func_map**
  - **Test Target**: Validate cmp_export_func_map() through function lookup operations
  - **Static Function**: cmp_export_func_map() in aot_runtime.c:1400
  - **Public Caller**: aot_lookup_function_with_idx() and related lookup functions
  - **Test Steps**:
    1. Create AOT module with multiple exported functions requiring sorting
    2. Call function lookup operations that trigger internal sorting
    3. Verify function lookups return correct results (proving proper sorting)
    4. Test with edge cases that stress comparison logic
  - **Expected Outcomes**: Function lookups succeed with correct results
  - **Validation Method**: Assert on lookup results that depend on proper function map sorting
  - **Edge Cases**: Duplicate function names, case variations, special characters

**Status**: PENDING
**Coverage Target**: Functions 1-10 from input list

### Step 2: Execution & Stack Management (8 functions)
**Target Functions**:
1. aot_call_indirect - aot_runtime.c:3288 (Public - Direct Testing)
2. aot_data_drop - aot_runtime.c:3582 (Public - Direct Testing)
3. aot_set_aux_stack - aot_runtime.c:3595 (Public - Direct Testing)
4. aot_get_aux_stack - aot_runtime.c:3629 (Public - Direct Testing)
5. aot_alloc_tiny_frame - aot_runtime.c:4136 (Public - Direct Testing)
6. aot_free_tiny_frame - aot_runtime.c:4197 (Public - Direct Testing)
7. aot_frame_update_profile_info - aot_runtime.c:4215 (Public - Direct Testing)
8. set_error_buf_v - aot_runtime.c:103 (Static - Test via error-triggering operations)

**Test Cases for Each Function**:

#### Direct Testing (Public Functions)
- [ ] **test_aot_call_indirect_function_dispatch**
  - **Test Target**: Validate aot_call_indirect() indirect function calls
  - **Test Steps**:
    1. Create AOT module with function table and indirect call targets
    2. Call aot_call_indirect() with valid table and element indices
    3. Verify correct function is called with proper arguments
    4. Test with invalid indices and parameter mismatches
  - **Expected Outcomes**: Correct indirect function dispatch
  - **Edge Cases**: Invalid table indices, type mismatches, null function pointers

- [ ] **test_aot_data_drop_segment_management**
  - **Test Target**: Validate aot_data_drop() data segment cleanup
  - **Test Steps**:
    1. Create AOT module with active data segments
    2. Call aot_data_drop() to drop specific segments
    3. Verify segments are properly marked as dropped
    4. Test attempts to use dropped segments
  - **Expected Outcomes**: Data segments properly dropped and inaccessible
  - **Edge Cases**: Invalid segment indices, already dropped segments, active segments

- [ ] **test_aot_set_aux_stack_configuration**
  - **Test Target**: Validate aot_set_aux_stack() auxiliary stack setup
  - **Test Steps**:
    1. Create execution environment with memory configuration
    2. Call aot_set_aux_stack() with various stack parameters
    3. Verify auxiliary stack is correctly configured
    4. Test boundary conditions and invalid parameters
  - **Expected Outcomes**: Auxiliary stack properly configured within bounds
  - **Edge Cases**: Overlapping regions, zero size, invalid offsets

- [ ] **test_aot_get_aux_stack_retrieval**
  - **Test Target**: Validate aot_get_aux_stack() auxiliary stack info retrieval
  - **Test Steps**:
    1. Setup execution environment with configured auxiliary stack
    2. Call aot_get_aux_stack() to retrieve stack information
    3. Verify returned stack parameters match configuration
    4. Test with uninitialized and null parameters
  - **Expected Outcomes**: Correct auxiliary stack information returned
  - **Edge Cases**: Uninitialized aux stack, null output parameters

- [ ] **test_aot_alloc_tiny_frame_memory_management**
  - **Test Target**: Validate aot_alloc_tiny_frame() frame allocation
  - **Test Steps**:
    1. Create execution environment with stack space
    2. Call aot_alloc_tiny_frame() with function indices
    3. Verify frame is properly allocated and initialized
    4. Test memory exhaustion and invalid function indices
  - **Expected Outcomes**: Frame allocation succeeds with valid parameters
  - **Edge Cases**: Stack overflow, invalid function indices, memory exhaustion

- [ ] **test_aot_free_tiny_frame_deallocation**
  - **Test Target**: Validate aot_free_tiny_frame() frame cleanup
  - **Test Steps**:
    1. Allocate tiny frames using aot_alloc_tiny_frame()
    2. Call aot_free_tiny_frame() to deallocate frames
    3. Verify frames are properly cleaned up and stack is restored
    4. Test with unbalanced allocation/deallocation
  - **Expected Outcomes**: Frames properly deallocated and stack restored
  - **Edge Cases**: Double free, unallocated frames, stack underflow

- [ ] **test_aot_frame_update_profile_info_tracking**
  - **Test Target**: Validate aot_frame_update_profile_info() profiling updates
  - **Test Steps**:
    1. Enable profiling in execution environment
    2. Call aot_frame_update_profile_info() with allocation/deallocation events
    3. Verify profiling information is correctly updated
    4. Test with profiling disabled and invalid parameters
  - **Expected Outcomes**: Profiling information accurately tracks frame operations
  - **Edge Cases**: Profiling disabled, null execution environment, timing accuracy

#### Indirect Testing (Static Functions)
- [ ] **test_error_conditions_exercise_set_error_buf_v**
  - **Test Target**: Validate set_error_buf_v() through error-triggering operations
  - **Static Function**: set_error_buf_v() in aot_runtime.c:103
  - **Public Caller**: Various AOT functions that encounter error conditions
  - **Test Steps**:
    1. Create conditions that trigger errors in AOT operations
    2. Call public AOT functions that will encounter these errors
    3. Verify error messages are properly formatted and stored
    4. Test with various error buffer sizes and null buffers
  - **Expected Outcomes**: Error messages properly formatted and stored
  - **Validation Method**: Assert on error message content and format consistency
  - **Edge Cases**: Null error buffer, zero buffer size, format string variations

**Status**: PENDING
**Coverage Target**: Functions 11-18 from input list

### Step 3: Memory Consumption & Utility Functions (6 functions)
**Target Functions**:
1. aot_get_module_inst_mem_consumption - aot_runtime.c:3744 (Public - Direct Testing)
2. aot_get_module_mem_consumption - aot_runtime.c:3661 (Public - Direct Testing)
3. execute_malloc_function - aot_runtime.c:2807 (Public - Direct Testing)
4. execute_free_function - aot_runtime.c:2912 (Public - Direct Testing)
5. const_string_node_size_cb - aot_runtime.c:3652 (Public - Direct Testing)
6. check_global_init_expr - aot_runtime.c:192 (Static - Test via module instantiation)

**Test Cases for Each Function**:

#### Direct Testing (Public Functions)
- [ ] **test_aot_get_module_inst_mem_consumption_calculation**
  - **Test Target**: Validate aot_get_module_inst_mem_consumption() memory usage calculation
  - **Test Steps**:
    1. Create AOT module instances with various configurations
    2. Call aot_get_module_inst_mem_consumption() to calculate memory usage
    3. Verify calculated consumption matches actual allocation
    4. Test with complex module instances and null parameters
  - **Expected Outcomes**: Accurate memory consumption calculation
  - **Edge Cases**: Empty modules, large modules, null parameters

- [ ] **test_aot_get_module_mem_consumption_static_analysis**
  - **Test Target**: Validate aot_get_module_mem_consumption() static memory analysis
  - **Test Steps**:
    1. Create AOT modules with different memory requirements
    2. Call aot_get_module_mem_consumption() to analyze static memory needs
    3. Verify calculation includes all module components
    4. Test with modules containing constant strings and complex structures
  - **Expected Outcomes**: Comprehensive static memory analysis
  - **Edge Cases**: Modules with no constants, large constant pools, null parameters

- [ ] **test_execute_malloc_function_allocation**
  - **Test Target**: Validate execute_malloc_function() memory allocation execution
  - **Test Steps**:
    1. Create AOT module with malloc function implementation
    2. Call execute_malloc_function() with various allocation sizes
    3. Verify memory is properly allocated and accessible
    4. Test allocation failures and boundary conditions
  - **Expected Outcomes**: Successful memory allocation through WASM malloc
  - **Edge Cases**: Zero size allocation, large allocations, allocation failures

- [ ] **test_execute_free_function_deallocation**
  - **Test Target**: Validate execute_free_function() memory deallocation execution
  - **Test Steps**:
    1. Allocate memory using execute_malloc_function()
    2. Call execute_free_function() to deallocate memory
    3. Verify memory is properly freed and not accessible
    4. Test double-free and invalid pointer scenarios
  - **Expected Outcomes**: Proper memory deallocation through WASM free
  - **Edge Cases**: Double free, invalid pointers, null pointer free

- [ ] **test_const_string_node_size_cb_callback_execution**
  - **Test Target**: Validate const_string_node_size_cb() callback functionality
  - **Test Steps**:
    1. Create hash table with constant string nodes
    2. Use const_string_node_size_cb() as callback in hash table operations
    3. Verify callback correctly calculates string node sizes
    4. Test with various string lengths and null parameters
  - **Expected Outcomes**: Accurate string node size calculation
  - **Edge Cases**: Empty strings, very long strings, null parameters

#### Indirect Testing (Static Functions)
- [ ] **test_module_instantiation_exercises_check_global_init_expr**
  - **Test Target**: Validate check_global_init_expr() through module instantiation
  - **Static Function**: check_global_init_expr() in aot_runtime.c:192
  - **Public Caller**: Module instantiation functions that validate globals
  - **Test Steps**:
    1. Create AOT modules with various global initialization expressions
    2. Attempt module instantiation to trigger global expression validation
    3. Verify modules with valid expressions instantiate successfully
    4. Verify modules with invalid expressions fail appropriately
  - **Expected Outcomes**: Proper validation of global initialization expressions
  - **Validation Method**: Assert on module instantiation success/failure based on global expr validity
  - **Edge Cases**: Complex constant expressions, imported globals, circular dependencies

**Status**: PENDING
**Coverage Target**: Functions 19-24 from input list

## Call Chain Analysis Results

### Static Function Call Chains Discovered
1. **set_error_buf_v** → Called by multiple AOT functions during error handling
   - **Test Strategy**: Trigger error conditions in public AOT functions
   - **Validation**: Assert on error message format and content consistency

2. **check_global_init_expr** → Called during module instantiation and global validation
   - **Test Strategy**: Create modules with various global initialization expressions
   - **Validation**: Assert on module instantiation success/failure patterns

3. **cmp_export_func_map** → Called during function lookup and sorting operations
   - **Test Strategy**: Exercise function lookup operations requiring sorting
   - **Validation**: Assert on correct function lookup results dependent on proper sorting

### Public Function Direct Testing
All other functions are public APIs that can be tested directly with comprehensive input validation and boundary condition testing.

## Implementation Guidelines

### Test File Organization
- **File Naming**: `test_aot_enhanced_step_[N].cc`
- **Class Naming**: `AOTFunctionTestStep[N]`
- **Test Naming**: `TEST_F(AOTFunctionTestStep[N], Function_Scenario_ExpectedOutcome)`

### Mock Structure Requirements
For functions requiring AOT module structures:
```cpp
class MockAOTModuleHelper {
public:
    static AOTModule* create_valid_aot_module();
    static AOTModuleInstance* create_valid_aot_module_instance();
    static WASMExecEnv* create_valid_exec_env();
    static void cleanup_mock_aot_module(AOTModule* module);
    static void cleanup_mock_aot_module_instance(AOTModuleInstance* inst);
    static void cleanup_mock_exec_env(WASMExecEnv* env);
};
```

### Platform Compatibility
- **Feature Flags**: Check WASM_ENABLE_* macros for AOT feature availability
- **Conditional Testing**: Use early return for unsupported AOT features
- **Build Configuration**: Adapt to current AOT compilation capabilities

## Progress Tracking
- Total Steps: 3
- Completed Steps: 0
- Current Focus: Step 1 (PENDING)
- Functions Tested: 0/24
- Coverage Improvement: TBD

## Quality Criteria
Each step completion requires:
- [ ] All target functions have comprehensive test coverage
- [ ] Static functions tested through verified call chains
- [ ] Public functions tested directly with full input validation
- [ ] Both positive and negative test scenarios for each function
- [ ] Observable validation of function behavior (no tautological assertions)
- [ ] Proper resource management and cleanup
- [ ] Platform compatibility handled gracefully
- [ ] AOT-specific functionality thoroughly validated
- [ ] Memory management and stack operations verified
- [ ] Symbol resolution and function dispatch tested comprehensively