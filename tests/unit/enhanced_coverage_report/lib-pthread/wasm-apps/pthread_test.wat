(module
  ;; Memory: 1 page (64KB) for basic operations
  (memory 1)
  
  ;; Export memory for host access
  (export "memory" (memory 0))
  
  ;; Simple test function: adds 42 to input parameter
  (func $test_fun (param $input i32) (result i32)
    local.get $input
    i32.const 42
    i32.add
  )
  (export "test_fun" (func $test_fun))
  
  ;; Thread function that simulates pthread work
  (func $thread_func (param $arg i32) (result i32)
    local.get $arg
    i32.const 100
    i32.add
  )
  (export "thread_func" (func $thread_func))
  
  ;; Memory test function for pthread operations
  (func $memory_test (param $addr i32) (param $value i32) (result i32)
    local.get $addr
    local.get $value
    i32.store
    local.get $addr
    i32.load
  )
  (export "memory_test" (func $memory_test))
)