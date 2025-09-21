(module
  ;; Simple test module for AOT loader testing
  (type $t0 (func (param i32) (result i32)))
  (type $t1 (func))
  
  ;; Import a function for indirect call testing
  (import "env" "print_i32" (func $print_i32 (type $t0)))
  
  ;; Memory for testing memory operations
  (memory $mem 1 2)
  
  ;; Global for testing
  (global $g0 (mut i32) (i32.const 42))
  
  ;; Table for indirect calls
  (table $table 2 funcref)
  
  ;; Data segment for memory init testing
  (data (i32.const 0) "Hello")
  
  ;; Test functions
  (func $add (type $t0)
    local.get 0
    i32.const 1
    i32.add
  )
  
  (func $multiply (type $t0)
    local.get 0
    i32.const 2
    i32.mul
  )
  
  ;; Function that calls indirect
  (func $test_indirect (param i32) (result i32)
    local.get 0
    i32.const 0
    call_indirect (type $t0)
  )
  
  ;; Function that grows memory
  (func $test_memory_grow (param i32) (result i32)
    local.get 0
    memory.grow
  )
  
  ;; Function that initializes memory
  (func $test_memory_init
    i32.const 10
    i32.const 0
    i32.const 5
    memory.init 0
    data.drop 0
  )
  
  ;; Initialize table
  (elem (i32.const 0) $add $multiply)
  
  ;; Export functions
  (export "add" (func $add))
  (export "multiply" (func $multiply))
  (export "test_indirect" (func $test_indirect))
  (export "test_memory_grow" (func $test_memory_grow))
  (export "test_memory_init" (func $test_memory_init))
  (export "memory" (memory $mem))
  (export "global" (global $g0))
)