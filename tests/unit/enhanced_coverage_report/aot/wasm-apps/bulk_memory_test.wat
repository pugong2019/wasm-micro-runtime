(module
  ;; Memory for bulk memory operations testing
  (memory 2 5)
  
  ;; Multiple data segments for testing data operations
  (data (i32.const 0x000) "DataSegment0")
  (data (i32.const 0x100) "DataSegment1_TestData")
  (data (i32.const 0x200) "DataSegment2_MoreTestData")
  
  ;; Test function for memory.init - exercises aot_memory_init
  (func $test_memory_init_seg0 (export "test_memory_init_seg0") (param $dst i32) (param $offset i32) (param $len i32)
    local.get $dst
    local.get $offset
    local.get $len
    memory.init 0
  )
  
  (func $test_memory_init_seg1 (export "test_memory_init_seg1") (param $dst i32) (param $offset i32) (param $len i32)
    local.get $dst
    local.get $offset
    local.get $len
    memory.init 1
  )
  
  (func $test_memory_init_seg2 (export "test_memory_init_seg2") (param $dst i32) (param $offset i32) (param $len i32)
    local.get $dst
    local.get $offset
    local.get $len
    memory.init 2
  )
  
  ;; Test function for data.drop - exercises aot_data_drop
  (func $test_data_drop_seg0 (export "test_data_drop_seg0")
    data.drop 0
  )
  
  (func $test_data_drop_seg1 (export "test_data_drop_seg1")
    data.drop 1
  )
  
  (func $test_data_drop_seg2 (export "test_data_drop_seg2")
    data.drop 2
  )
  
  ;; Test function that drops and then tries to init (should fail)
  (func $test_drop_then_init (export "test_drop_then_init") (result i32)
    ;; Drop segment 0
    data.drop 0
    
    ;; Try to init from dropped segment (should cause exception)
    i32.const 0x300
    i32.const 0
    i32.const 5
    memory.init 0
    
    ;; Return success if no exception
    i32.const 1
  )
  
  ;; Test function for memory bounds checking
  (func $test_memory_init_out_of_bounds (export "test_memory_init_out_of_bounds")
    ;; Try to init beyond segment bounds
    i32.const 0x400
    i32.const 100  ;; Offset beyond segment size
    i32.const 10
    memory.init 1
  )
  
  ;; Test function for memory growth with bulk operations
  (func $test_grow_and_init (export "test_grow_and_init") (result i32)
    ;; Grow memory first
    i32.const 1
    memory.grow
    drop
    
    ;; Then initialize in the new area
    i32.const 0x10000  ;; Start of second page
    i32.const 0
    i32.const 10
    memory.init 1
    
    ;; Return current memory size
    memory.size
  )
  
  ;; Utility function to read memory content
  (func $read_memory_i32 (export "read_memory_i32") (param $addr i32) (result i32)
    local.get $addr
    i32.load
  )
  
  ;; Utility function to write memory content
  (func $write_memory_i32 (export "write_memory_i32") (param $addr i32) (param $val i32)
    local.get $addr
    local.get $val
    i32.store
  )
  
  ;; Export memory for external access
  (export "memory" (memory 0))
)