(module
  ;; Import functions MUST come first
  (import "env" "test_import_add" (func $test_import_add (param i32 i32) (result i32)))
  (import "env" "test_import_mul" (func $test_import_mul (param i32 i32) (result i32)))
  (import "env" "malloc" (func $malloc (param i32) (result i32)))
  (import "env" "free" (func $free (param i32)))
  
  ;; Type definitions for function signatures
  (type $void_to_void (func))
  (type $i32_to_i32 (func (param i32) (result i32)))
  (type $i64_to_i64 (func (param i64) (result i64)))
  (type $malloc_sig (func (param i32) (result i32)))
  (type $free_sig (func (param i32)))
  
  ;; Memory for testing malloc/free operations
  (memory 1)
  
  ;; Table for indirect function calls
  (table 4 funcref)
  
  ;; Local functions for table initialization
  (func $add_func (type $i32_to_i32) (param $x i32) (result i32)
    local.get $x
    i32.const 10
    i32.add)
  
  (func $mul_func (type $i32_to_i32) (param $x i32) (result i32)
    local.get $x
    i32.const 2
    i32.mul)
  
  (func $identity_func (type $i32_to_i32) (param $x i32) (result i32)
    local.get $x)
  
  (func $void_func (type $void_to_void))
  
  ;; Initialize table with function references
  (elem (i32.const 0) $add_func $mul_func $identity_func $void_func)
  
  ;; Test functions for call_indirect scenarios
  (func (export "test_call_indirect_valid") (param $idx i32) (param $val i32) (result i32)
    ;; Valid indirect call with type checking
    local.get $val
    local.get $idx
    call_indirect (type $i32_to_i32))
  
  (func (export "test_call_indirect_invalid_index") (param $val i32) (result i32)
    ;; Invalid table index (>= table size)
    local.get $val
    i32.const 10  ;; Invalid index
    call_indirect (type $i32_to_i32))
  
  (func (export "test_call_indirect_null_ref") (param $val i32) (result i32)
    ;; Call with uninitialized table element (would be NULL_REF)
    local.get $val
    i32.const 3  ;; Index with void function (type mismatch)
    call_indirect (type $i32_to_i32))
  
  (func (export "test_call_indirect_type_mismatch") (param $val i32) (result i32)
    ;; Type signature mismatch
    local.get $val
    i32.const 3  ;; void_func index
    call_indirect (type $i32_to_i32))
  
  ;; Test functions for import function calls
  (func (export "test_import_function_call") (param $a i32) (param $b i32) (result i32)
    ;; Test wasm_interp_call_func_import with valid import
    local.get $a
    local.get $b
    call $test_import_add)
  
  (func (export "test_import_function_mul") (param $a i32) (param $b i32) (result i32)
    ;; Test multiple import function calls
    local.get $a
    local.get $b
    call $test_import_mul)
  
  ;; Test functions for malloc/free operations
  (func (export "test_malloc_operation") (param $size i32) (result i32)
    ;; Test execute_malloc_function
    local.get $size
    call $malloc)
  
  (func (export "test_free_operation") (param $ptr i32)
    ;; Test execute_free_function
    local.get $ptr
    call $free)
  
  (func (export "test_malloc_free_cycle") (param $size i32) (result i32)
    ;; Test malloc followed by free
    (local $ptr i32)
    local.get $size
    call $malloc
    local.set $ptr
    
    ;; Write some data to verify allocation
    local.get $ptr
    i32.const 42
    i32.store
    
    ;; Read back the data
    local.get $ptr
    i32.load
    
    ;; Free the memory
    local.get $ptr
    call $free
    
    ;; Return the value that was stored
    )
  
  ;; Test function for stack value copying scenarios
  (func (export "test_stack_operations") (param $val1 i32) (param $val2 i32) (result i32)
    ;; Create scenario that requires copy_stack_values
    ;; This involves function calls with multiple parameters and return values
    local.get $val1
    local.get $val2
    call $test_import_add
    
    ;; Chain multiple calls to exercise stack copying
    i32.const 5
    call $add_func
    
    local.get $val2
    i32.const 3
    call $test_import_mul
    i32.add)
  
  ;; Test complex indirect call scenarios
  (func (export "test_complex_indirect_calls") (param $selector i32) (param $value i32) (result i32)
    ;; Test multiple indirect calls based on selector
    local.get $selector
    i32.const 0
    i32.eq
    if (result i32)
      ;; Call add_func
      local.get $value
      i32.const 0
      call_indirect (type $i32_to_i32)
    else
      local.get $selector
      i32.const 1
      i32.eq
      if (result i32)
        ;; Call mul_func
        local.get $value
        i32.const 1
        call_indirect (type $i32_to_i32)
      else
        ;; Call identity_func
        local.get $value
        i32.const 2
        call_indirect (type $i32_to_i32)
      end
    end)
  
  ;; Test function with large parameter count for stack operations
  (func (export "test_large_param_stack") 
    (param $p1 i32) (param $p2 i32) (param $p3 i32) (param $p4 i32)
    (param $p5 i32) (param $p6 i32) (param $p7 i32) (param $p8 i32)
    (result i32)
    ;; Exercise copy_stack_values with many parameters
    local.get $p1
    local.get $p2
    call $test_import_add
    
    local.get $p3
    local.get $p4
    call $test_import_add
    i32.add
    
    local.get $p5
    local.get $p6
    call $test_import_add
    i32.add
    
    local.get $p7
    local.get $p8
    call $test_import_add
    i32.add)
)