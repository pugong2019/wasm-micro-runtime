(module
  ;; Memory for auxiliary stack testing
  (memory 1 2)
  
  ;; Global variable for auxiliary stack top (required for aot_set_aux_stack)
  (global $aux_stack_top (mut i32) (i32.const 0x8000))
  
  ;; Test function that uses auxiliary stack operations
  (func $test_aux_stack_usage (export "test_aux_stack_usage") (param $iterations i32) (result i32)
    (local $i i32)
    (local $sum i32)
    
    ;; Initialize local variables
    i32.const 0
    local.set $i
    i32.const 0
    local.set $sum
    
    ;; Loop that uses stack space
    loop $loop
      local.get $i
      local.get $iterations
      i32.lt_u
      if
        ;; Perform some stack-intensive operations
        local.get $sum
        local.get $i
        i32.add
        local.set $sum
        
        ;; Increment counter
        local.get $i
        i32.const 1
        i32.add
        local.set $i
        
        br $loop
      end
    end
    
    local.get $sum
  )
  
  ;; Recursive function to test stack frame operations
  (func $recursive_stack_test (export "recursive_stack_test") (param $depth i32) (result i32)
    local.get $depth
    i32.const 0
    i32.eq
    if (result i32)
      i32.const 42  ;; Base case
    else
      local.get $depth
      i32.const 1
      i32.sub
      call $recursive_stack_test
      local.get $depth
      i32.add
    end
  )
  
  ;; Function that allocates and uses local variables (tests tiny frame allocation)
  (func $test_local_variables (export "test_local_variables") (param $input i32) (result i32)
    (local $var1 i32)
    (local $var2 i32)
    (local $var3 i32)
    (local $var4 i32)
    (local $var5 i32)
    
    ;; Initialize local variables
    local.get $input
    local.set $var1
    
    local.get $var1
    i32.const 10
    i32.add
    local.set $var2
    
    local.get $var2
    i32.const 20
    i32.mul
    local.set $var3
    
    local.get $var3
    i32.const 5
    i32.div_u
    local.set $var4
    
    local.get $var4
    i32.const 3
    i32.rem_u
    local.set $var5
    
    ;; Return sum of all variables
    local.get $var1
    local.get $var2
    i32.add
    local.get $var3
    i32.add
    local.get $var4
    i32.add
    local.get $var5
    i32.add
  )
  
  ;; Function that performs nested calls (tests frame management)
  (func $nested_calls_test (export "nested_calls_test") (param $level i32) (result i32)
    local.get $level
    i32.const 0
    i32.eq
    if (result i32)
      i32.const 1
    else
      local.get $level
      i32.const 1
      i32.sub
      call $helper_function
    end
  )
  
  (func $helper_function (param $n i32) (result i32)
    local.get $n
    i32.const 0
    i32.eq
    if (result i32)
      i32.const 2
    else
      local.get $n
      i32.const 1
      i32.sub
      call $nested_calls_test
      i32.const 1
      i32.add
    end
  )
  
  ;; Function to test auxiliary stack boundary conditions
  (func $test_stack_boundary (export "test_stack_boundary") (param $size i32) (result i32)
    ;; Create array of local variables based on size parameter
    (local $arr_0 i32) (local $arr_1 i32) (local $arr_2 i32) (local $arr_3 i32)
    (local $arr_4 i32) (local $arr_5 i32) (local $arr_6 i32) (local $arr_7 i32)
    (local $sum i32)
    
    ;; Initialize array elements
    local.get $size
    local.set $arr_0
    local.get $size
    i32.const 1
    i32.add
    local.set $arr_1
    local.get $size
    i32.const 2
    i32.add
    local.set $arr_2
    local.get $size
    i32.const 3
    i32.add
    local.set $arr_3
    local.get $size
    i32.const 4
    i32.add
    local.set $arr_4
    local.get $size
    i32.const 5
    i32.add
    local.set $arr_5
    local.get $size
    i32.const 6
    i32.add
    local.set $arr_6
    local.get $size
    i32.const 7
    i32.add
    local.set $arr_7
    
    ;; Sum all elements
    local.get $arr_0
    local.get $arr_1
    i32.add
    local.get $arr_2
    i32.add
    local.get $arr_3
    i32.add
    local.get $arr_4
    i32.add
    local.get $arr_5
    i32.add
    local.get $arr_6
    i32.add
    local.get $arr_7
    i32.add
  )
  
  ;; Export global for auxiliary stack testing
  (export "aux_stack_top" (global $aux_stack_top))
  (export "memory" (memory 0))
)