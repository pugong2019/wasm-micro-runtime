(module
  ;; Test module for native symbol resolution
  
  ;; Import native functions to test symbol resolution
  (import "env" "native_func1" (func $native_func1 (param i32) (result i32)))
  (import "env" "native_func2" (func $native_func2 (param f32) (result f32)))
  (import "wasi_snapshot_preview1" "fd_write" (func $fd_write (param i32 i32 i32 i32) (result i32)))
  
  ;; Memory
  (memory 1)
  
  ;; Function that calls native functions
  (func $call_native_functions (param i32) (result i32)
    ;; Call first native function
    local.get 0
    call $native_func1
    
    ;; Call second native function with float conversion
    f32.convert_i32_s
    call $native_func2
    i32.trunc_f32_s
    
    ;; Add original parameter
    local.get 0
    i32.add
  )
  
  ;; Function that uses WASI function
  (func $test_wasi_call (result i32)
    ;; Write "test" to stdout
    i32.const 1    ;; fd (stdout)
    i32.const 8    ;; iovs pointer
    i32.const 1    ;; iovs count
    i32.const 16   ;; nwritten pointer
    call $fd_write
  )
  
  ;; Initialize some data for WASI call
  (data (i32.const 8) "\08\00\00\00")  ;; iov.iov_base = 20
  (data (i32.const 12) "\04\00\00\00") ;; iov.iov_len = 4
  (data (i32.const 20) "test")         ;; actual data
  
  ;; Export functions
  (export "call_native_functions" (func $call_native_functions))
  (export "test_wasi_call" (func $test_wasi_call))
  (export "memory" (memory 0))
)