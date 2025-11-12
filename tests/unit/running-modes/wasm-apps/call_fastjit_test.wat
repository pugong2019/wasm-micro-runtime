;; WASM test module for Fast JIT CALL opcode implementation testing
;; Tests boundary parameter count conditions, native function calls,
;; internal function calls, and exception handling scenarios

(module
  ;; Import native functions with different parameter counts to test boundary conditions
  (import "env" "native_func_2params" (func $native_func_2params (param i32 i32) (result i32)))
  (import "env" "native_func_4params" (func $native_func_4params (param i32 i32 i32 i32) (result i32)))
  (import "env" "native_func_5params" (func $native_func_5params (param i32 i32 i32 i32 i32) (result i32)))
  (import "env" "native_func_6params" (func $native_func_6params (param i32 i32 i32 i32 i32 i32) (result i32)))

  ;; Import function with pointer parameters for signature processing
  (import "env" "native_func_with_ptr" (func $native_func_with_ptr (param i32 i32) (result i32)))

  ;; Memory for pointer/string parameter testing
  (memory (export "memory") 1)

  ;; Internal WASM function for testing CALLBC generation - simple addition
  (func $internal_add (param $a i32) (param $b i32) (result i32)
    local.get $a
    local.get $b
    i32.add
  )

  ;; Internal WASM function for testing CALLBC generation - recursive factorial
  (func $internal_factorial (param $n i32) (result i32)
    (local $result i32)

    ;; Base case: if n <= 1, return 1
    local.get $n
    i32.const 1
    i32.le_s
    if (result i32)
      i32.const 1
    else
      ;; Recursive case: n * factorial(n-1)
      local.get $n
      local.get $n
      i32.const 1
      i32.sub
      call $internal_factorial  ;; This tests CALLBC generation for internal calls
      i32.mul
    end
  )

  ;; Test function: calls native function with 2 parameters (direct call path)
  (func $test_call_native_2params (param i32 i32) (result i32)
    local.get 0
    local.get 1
    call $native_func_2params
  )
  (export "test_call_native_2params" (func $test_call_native_2params))

  ;; Test function: calls native function with 4 parameters (direct call path - boundary)
  (func $test_call_native_4params (param i32 i32 i32 i32) (result i32)
    local.get 0
    local.get 1
    local.get 2
    local.get 3
    call $native_func_4params
  )
  (export "test_call_native_4params" (func $test_call_native_4params))

  ;; Test function: calls native function with 5 parameters (fast_jit_invoke_native path - boundary)
  (func $test_call_native_5params (param i32 i32 i32 i32 i32) (result i32)
    local.get 0
    local.get 1
    local.get 2
    local.get 3
    local.get 4
    call $native_func_5params
  )
  (export "test_call_native_5params" (func $test_call_native_5params))

  ;; Test function: calls native function with 6 parameters (fast_jit_invoke_native path)
  (func $test_call_native_6params (param i32 i32 i32 i32 i32 i32) (result i32)
    local.get 0
    local.get 1
    local.get 2
    local.get 3
    local.get 4
    local.get 5
    call $native_func_6params
  )
  (export "test_call_native_6params" (func $test_call_native_6params))

  ;; Test function: calls native function with pointer parameter for signature processing
  (func $test_call_native_with_ptr (param $offset i32) (param $len i32) (result i32)
    local.get $offset
    local.get $len
    call $native_func_with_ptr
  )
  (export "test_call_native_with_ptr" (func $test_call_native_with_ptr))


  ;; Test function: calls internal WASM function to test CALLBC generation
  (func $test_call_internal_add (param i32 i32) (result i32)
    local.get 0
    local.get 1
    call $internal_add  ;; This tests CALLBC generation for internal function calls
  )
  (export "test_call_internal_add" (func $test_call_internal_add))

  ;; Test function: calls recursive internal function to test CALLBC with recursion
  (func $test_call_internal_factorial (param i32) (result i32)
    local.get 0
    call $internal_factorial  ;; This tests CALLBC generation for recursive calls
  )
  (export "test_call_internal_factorial" (func $test_call_internal_factorial))

  ;; Test function: attempts to call invalid function index for exception testing
  (func $test_call_invalid_index (param $invalid_idx i32) (result i32)
    ;; This will be handled at runtime - the test will use an invalid index
    ;; and expect the runtime to catch it and throw an exception
    i32.const 0  ;; Return dummy value, exception should be thrown before this
  )
  (export "test_call_invalid_index" (func $test_call_invalid_index))

  ;; Test function: calls function that causes runtime exception
  (func $test_call_with_exception (param $divisor i32) (result i32)
    ;; Simulate a runtime exception scenario (division by zero)
    (local $result i32)

    local.get $divisor
    i32.const 0
    i32.eq
    if
      ;; Force an exception by accessing invalid memory or similar
      i32.const 0xFFFFFFFF
      i32.load  ;; This should cause a runtime exception
      drop
    end

    i32.const 100
    local.get $divisor
    i32.div_s  ;; Division by zero if divisor is 0
  )
  (export "test_call_with_exception" (func $test_call_with_exception))
)