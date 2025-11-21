# SIMD Module Coverage Improvement Plan

## 1. Current Coverage Status Analysis

Through analysis of the coverage for three files: `simd_conversions.c`, `simd_floating_point.c`, and `simd_int_arith.c`, it was found that the current coverage is 0%. This is because existing test files only verify that the compilation process can complete successfully, but do not actually execute the compiled WASM modules, thus failing to cover these AOT compilation-related function implementations.

### 1.1 Uncovered File Paths

- `/home/chengnie/works/wasm-micro-runtime/core/iwasm/compilation/simd/simd_conversions.c`
- `/home/chengnie/works/wasm-micro-runtime/core/iwasm/compilation/simd/simd_floating_point.c`
- `/home/chengnie/works/wasm-micro-runtime/core/iwasm/compilation/simd/simd_int_arith.c`

### 1.2 Uncovered Main Function Points

#### simd_conversions.c
- [ ] Integer narrowing operations (narrow)
- [ ] Integer extension operations (extend)
- [ ] Floating-point truncation saturation operations (trunc_sat)
- [ ] Integer conversion operations
- [ ] Floating-point conversion operations
- [ ] Pairwise addition with extension (extadd_pairwise)
- [ ] Q15 multiplication saturation operations (q15mulr_sat)
- [ ] Integer extension multiplication operations (extmul)

#### simd_floating_point.c
- [ ] Floating-point arithmetic operations (add, sub, mul, div)
- [ ] Floating-point negation (neg)
- [ ] Floating-point mathematical functions (abs, sqrt, ceil, floor, trunc, nearest)
- [ ] Floating-point comparison operations (min, max, pmin, pmax)
- [ ] Floating-point type conversions (demote, promote)

#### simd_int_arith.c
- [ ] Integer arithmetic operations (add, sub, mul)
- [ ] Integer negation (neg)
- [ ] Population count (popcnt)
- [ ] Integer comparison operations (min, max)
- [ ] Absolute value operations (abs)
- [ ] Unsigned average operations (avg)
- [ ] Dot product operations (dot)

## 2. Coverage Improvement Plan

### 2.1 Test Framework Modifications

#### 2.1.1 Update Test Base Class

Modify existing test classes to actually execute compiled WASM modules, not just verify the compilation process:

```cpp
class SIMDTestBase : public testing::Test {
protected:
    void SetUp() override {
        // Initialize runtime environment
        RuntimeInitArgs init_args;
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_System_Allocator;
        ASSERT_TRUE(wasm_runtime_full_init(&init_args));
    }

    void TearDown() override {
        // Clean up runtime environment
        wasm_runtime_destroy();
    }

    // Helper method to execute WASM function
    bool execute_wasm_function(const char* wasm_file, const char* func_name, 
                              wasm_value_t* params, wasm_value_t* results) {
        // Load, compile, instantiate and execute WASM module
        unsigned int wasm_file_size = 0;
        unsigned char* wasm_file_buf = (unsigned char*)bh_read_file_to_buffer(
            wasm_file, &wasm_file_size);
        ASSERT_NE(wasm_file_buf, nullptr);

        // 1. Load WASM module
        wasm_module_t wasm_module = wasm_runtime_load(
            wasm_file_buf, wasm_file_size, error_buf, sizeof(error_buf));
        ASSERT_NE(wasm_module, nullptr);

        // 2. Create AOT compilation options
        AOTCompOption option = {0};
        option.opt_level = 3;
        option.size_level = 3;
        option.output_format = AOT_FORMAT_MEMORY;
        option.bounds_checks = 2;
        option.enable_simd = true;

        // 3. Create compilation data and context
        aot_comp_data_t comp_data = aot_create_comp_data(
            wasm_module, NULL, false);
        ASSERT_NE(comp_data, nullptr);
        
        aot_comp_context_t comp_ctx = aot_create_comp_context(
            comp_data, &option);
        ASSERT_NE(comp_ctx, nullptr);

        // 4. Compile WASM module
        bool compile_result = aot_compile_wasm(comp_ctx);
        ASSERT_TRUE(compile_result);

        // 5. Get compiled code
        wasm_module = (wasm_module_t)aot_get_compiled_module(comp_ctx);
        ASSERT_NE(wasm_module, nullptr);

        // 6. Instantiate module
        wasm_module_inst_t module_inst = wasm_runtime_instantiate(
            wasm_module, 64 * 1024, 64 * 1024, error_buf, sizeof(error_buf));
        ASSERT_NE(module_inst, nullptr);

        // 7. Create execution environment
        wasm_exec_env_t exec_env = wasm_runtime_create_exec_env(
            module_inst, 16 * 1024);
        ASSERT_NE(exec_env, nullptr);

        // 8. Execute function
        bool result = wasm_runtime_call_wasm(exec_env, module_inst, 
                                            func_name, results, params);
        
        // 9. Clean up resources
        wasm_runtime_destroy_exec_env(exec_env);
        wasm_runtime_deinstantiate(module_inst);
        aot_destroy_comp_context(comp_ctx);
        aot_destroy_comp_data(comp_data);
        BH_FREE(wasm_file_buf);
        
        return result;
    }

    char error_buf[128];
};
```

### 2.2 Test Case Improvements

#### 2.2.1 simd_conversions_test.cc Improvements

Update test cases to ensure they not only compile WASM modules but also execute functions within the modules:

```cpp
TEST_F(SIMDTestBase, SIMD_Integer_Extension_Test) {
    const char* wasm_file = CWD + "/simd_conversions_test.wasm";
    
    // Test signed extension
    wasm_value_t params[1], results[1];
    params[0].kind = WASM_I32;
    params[0].of.i32 = 0xFFFF8000; // Negative number
    ASSERT_TRUE(execute_wasm_function(wasm_file, "test_i32x4_extend_i16x8_s", 
                                     params, results));
    ASSERT_EQ(results[0].of.i32, 0xFFFFFFFF); // Should preserve sign
    
    // Test unsigned extension
    params[0].of.i32 = 0x00008000;
    ASSERT_TRUE(execute_wasm_function(wasm_file, "test_i32x4_extend_i16x8_u", 
                                     params, results));
    ASSERT_EQ(results[0].of.i32, 0x00008000); // Should pad with zeros
}
```

#### 2.2.2 simd_floating_point_test.cc Improvements

```cpp
TEST_F(SIMDTestBase, SIMD_Float_Arithmetic_Test) {
    const char* wasm_file = CWD + "/simd_floating_point_test.wasm";
    
    // Test floating-point addition
    wasm_value_t params[2], results[1];
    params[0].kind = WASM_F32;
    params[0].of.f32 = 1.5f;
    params[1].kind = WASM_F32;
    params[1].of.f32 = 2.5f;
    ASSERT_TRUE(execute_wasm_function(wasm_file, "test_f32x4_add", 
                                     params, results));
    ASSERT_FLOAT_EQ(results[0].of.f32, 4.0f);
    
    // Test floating-point square root
    params[0].of.f32 = 16.0f;
    ASSERT_TRUE(execute_wasm_function(wasm_file, "test_f32x4_sqrt", 
                                     params, results));
    ASSERT_FLOAT_EQ(results[0].of.f32, 4.0f);
}
```

#### 2.2.3 simd_int_arith_test.cc Improvements

```cpp
TEST_F(SIMDTestBase, SIMD_Int_Arithmetic_Test) {
    const char* wasm_file = CWD + "/simd_int_arith_test.wasm";
    
    // Test integer multiplication
    wasm_value_t params[2], results[1];
    params[0].kind = WASM_I32;
    params[0].of.i32 = 10;
    params[1].kind = WASM_I32;
    params[1].of.i32 = 5;
    ASSERT_TRUE(execute_wasm_function(wasm_file, "test_i32x4_mul", 
                                     params, results));
    ASSERT_EQ(results[0].of.i32, 50);
    
    // Test dot product operation
    params[0].kind = WASM_I32;
    params[0].of.i32 = 0x01020304; // Packed 4 i8 values
    params[1].kind = WASM_I32;
    params[1].of.i32 = 0x05060708;
    ASSERT_TRUE(execute_wasm_function(wasm_file, "test_i32x4_dot_i16x8", 
                                     params, results));
    // Calculate expected value: (1*5)+(2*6)+(3*7)+(4*8) = 5+12+21+32 = 70
    ASSERT_EQ(results[0].of.i32, 70);
}
```

### 2.3 WASM Test Program Updates

Ensure WASM test programs include all required SIMD operations, with corresponding export functions for each operation.

#### simd_conversions_test.wat Example

```webassembly
(module
  (memory 1)
  (export "memory" (memory 0))
  
  ;; Test i32x4 extend i16x8 signed
  (func (export "test_i32x4_extend_i16x8_s") (param i32) (result i32)
    (local i32)
    ;; Load data from memory to v128
    (i32.store (i32.const 0) (local.get 0))
    (v128.store (i32.const 0) (i32x4.splat (i32.const 0)))
    (v128.store (i32.const 0) (i16x8.extend_low_i8x16_s (v128.load (i32.const 0))))
    (i32x4.extract_lane_s 0 (v128.load (i32.const 0)))
  )
  
  ;; Test i32x4 extend i16x8 unsigned
  (func (export "test_i32x4_extend_i16x8_u") (param i32) (result i32)
    ;; Similar implementation...
  )
  
  ;; Add more test functions...
)
```

## 3. Testing Strategy

### 3.1 Critical Path Coverage

1. **Basic Functionality Testing**: Test the basic functionality correctness of each SIMD instruction
2. **Boundary Condition Testing**: Test boundary cases such as extreme values, zero values, NaN values, etc.
3. **Error Handling Testing**: Test error handling paths such as memory allocation failures, LLVM API failures, etc.
4. **Performance Testing**: Test the performance advantages of SIMD operations

### 3.2 Testing Priority

1. **High Priority**: Basic arithmetic operations, type conversions, common mathematical functions
2. **Medium Priority**: Comparison operations, bit operations, data reorganization
3. **Low Priority**: Special operations, edge cases

## 4. Implementation Steps

### 4.1 Short-term Plan (1-2 weeks)

- [ ] Update test base class, add functionality to execute WASM functions
- [ ] Modify existing test files to ensure they execute compiled code
- [ ] Ensure WASM test programs include all necessary export functions
- [ ] Run tests and collect new coverage reports

### 4.2 Medium-term Plan (2-4 weeks)

- [ ] Add boundary condition tests
- [ ] Add error handling tests
- [ ] Optimize test execution efficiency
- [ ] Continuously monitor coverage improvement

### 4.3 Long-term Plan (1-2 months)

- [ ] Add performance tests
- [ ] Ensure all SIMD operations have comprehensive test coverage
- [ ] Establish automated testing workflow
- [ ] Maintain coverage targets (>80% line coverage)

## 5. Expected Outcomes

By implementing the above plan, it is expected that the coverage of the three SIMD-related files can be improved from the current 0% to at least 80%, with specific targets as follows:

- **Line Coverage**: >80%
- **Function Coverage**: >80%
- **Branch Coverage**: >70%

This will significantly improve code quality, reduce potential bugs, and provide better guarantees for future maintenance and extensions.

## 6. Execution Plan Checklist

### Framework Setup
- [ ] Create shared SIMDTestBase class in test_helper.h
- [ ] Implement execute_wasm_function helper method
- [ ] Update CMakeLists.txt to include new test files
- [ ] Configure test environment with proper SIMD support

### Test Implementation

#### simd_conversions.c
- [ ] Update SIMDTestBase in simd_conversions_test.cc
- [ ] Implement tests for integer narrowing operations
- [ ] Implement tests for integer extension operations
- [ ] Implement tests for floating-point truncation operations
- [ ] Implement tests for integer conversion operations
- [ ] Implement tests for floating-point conversion operations
- [ ] Implement tests for pairwise addition with extension
- [ ] Implement tests for Q15 multiplication saturation operations
- [ ] Implement tests for integer extension multiplication operations

#### simd_floating_point.c
- [ ] Update SIMDTestBase in simd_floating_point_test.cc
- [ ] Implement tests for floating-point arithmetic operations
- [ ] Implement tests for floating-point negation
- [ ] Implement tests for mathematical functions (abs, sqrt, etc.)
- [ ] Implement tests for floating-point comparison operations
- [ ] Implement tests for floating-point type conversions

#### simd_int_arith.c
- [ ] Update SIMDTestBase in simd_int_arith_test.cc
- [ ] Implement tests for integer arithmetic operations
- [ ] Implement tests for integer negation
- [ ] Implement tests for population count (popcnt)
- [ ] Implement tests for integer comparison operations
- [ ] Implement tests for absolute value operations
- [ ] Implement tests for unsigned average operations
- [ ] Implement tests for dot product operations

### WASM Test Applications
- [ ] Update simd_conversions_test.wat with all required export functions
- [ ] Update simd_floating_point_test.wat with all required export functions
- [ ] Update simd_int_arith_test.wat with all required export functions
- [ ] Compile updated WASM test applications

### Coverage Verification
- [ ] Run tests with coverage collection enabled
- [ ] Generate coverage reports for SIMD modules
- [ ] Analyze coverage improvements
- [ ] Identify any remaining uncovered code paths

### Optimization and Refinement
- [ ] Optimize test execution speed
- [ ] Add boundary case coverage
- [ ] Enhance error handling tests
- [ ] Document test coverage in README files