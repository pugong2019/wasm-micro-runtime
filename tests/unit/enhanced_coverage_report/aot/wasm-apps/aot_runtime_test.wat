(module
  ;; Memory for AOT runtime testing (1 page = 64KB)
  (memory 1 10)
  
  ;; Data segment for memory initialization testing
  (data (i32.const 0x100) "Hello AOT Runtime Test Data")
  
  ;; Table for indirect function calls
  (table 10 funcref)
  
  ;; Type definitions for function signatures
  (type $t0 (func (param i32 i32) (result i32)))
  (type $t1 (func (param i32) (result i32)))
  (type $t2 (func))
  
  ;; Test functions for indirect calls
  (func $add_func (type $t0) (param $a i32) (param $b i32) (result i32)
    local.get $a
    local.get $b
    i32.add
  )
  
  (func $multiply_func (type $t0) (param $a i32) (param $b i32) (result i32)
    local.get $a
    local.get $b
    i32.mul
  )
  
  (func $square_func (type $t1) (param $x i32) (result i32)
    local.get $x
    local.get $x
    i32.mul
  )
  
  ;; Initialize table elements
  (elem (i32.const 0) $add_func $multiply_func $square_func)
  
  ;; Test function for indirect calls - exercises aot_call_indirect
  (func $test_call_indirect (export "test_call_indirect") (param $func_idx i32) (param $a i32) (param $b i32) (result i32)
    local.get $a
    local.get $b
    local.get $func_idx
    call_indirect (type $t0)
  )
  
  ;; Test function for single parameter indirect calls
  (func $test_call_indirect_single (export "test_call_indirect_single") (param $func_idx i32) (param $x i32) (result i32)
    local.get $x
    local.get $func_idx
    call_indirect (type $t1)
  )
  
  ;; Test function that triggers exception for aot_get_exception testing
  (func $trigger_exception (export "trigger_exception")
    ;; Call invalid table index to trigger exception
    i32.const 10
    i32.const 20
    i32.const 99  ;; Invalid function index
    call_indirect (type $t0)
    drop
  )
  
  ;; Test function for memory growth - exercises aot_enlarge_memory_with_idx
  (func $test_memory_grow (export "test_memory_grow") (param $pages i32) (result i32)
    local.get $pages
    memory.grow
  )
  
  ;; Test function for memory operations - exercises aot_memory_init
  (func $test_memory_init (export "test_memory_init") (param $dst i32) (param $src i32) (param $len i32)
    local.get $dst
    local.get $src
    local.get $len
    memory.init 0
  )
  
  ;; Test function for data drop - exercises aot_data_drop
  (func $test_data_drop (export "test_data_drop")
    data.drop 0
  )
  
  ;; Test function to get current memory size
  (func $get_memory_size (export "get_memory_size") (result i32)
    memory.size
  )
  
  ;; Test function for memory copy operations
  (func $test_memory_copy (export "test_memory_copy") (param $dst i32) (param $src i32) (param $len i32)
    local.get $dst
    local.get $src
    local.get $len
    memory.copy
  )
  
  ;; Test function for memory fill operations
  (func $test_memory_fill (export "test_memory_fill") (param $dst i32) (param $val i32) (param $len i32)
    local.get $dst
    local.get $val
    local.get $len
    memory.fill
  )
  
  ;; Test function that causes stack overflow for frame testing
  (func $recursive_func (export "recursive_func") (param $n i32) (result i32)
    local.get $n
    i32.const 0
    i32.eq
    if (result i32)
      i32.const 1
    else
      local.get $n
      i32.const 1
      i32.sub
      call $recursive_func
      local.get $n
      i32.add
    end
  )
  
  ;; Test function for auxiliary stack operations
  (func $test_aux_stack_operations (export "test_aux_stack_operations") (param $size i32) (result i32)
    ;; Simple function that uses stack space
    local.get $size
    i32.const 10
    i32.add
  )
  
  ;; Export memory for external access
  (export "memory" (memory 0))
  (export "table" (table 0))
)