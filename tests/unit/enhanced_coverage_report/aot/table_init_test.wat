(module
  ;; Test module for table operations and initialization
  
  ;; Function types
  (type $t0 (func (param i32) (result i32)))
  (type $t1 (func (result i32)))
  
  ;; Table for function references
  (table $table 4 8 funcref)
  
  ;; Memory
  (memory 1)
  
  ;; Test functions for table
  (func $func0 (type $t0)
    local.get 0
    i32.const 10
    i32.add
  )
  
  (func $func1 (type $t0)
    local.get 0
    i32.const 20
    i32.mul
  )
  
  (func $func2 (type $t1)
    i32.const 42
  )
  
  (func $func3 (type $t0)
    local.get 0
    i32.const 5
    i32.sub
  )
  
  ;; Function to test table.init
  (func $test_table_init
    ;; Initialize table with element segment
    i32.const 0    ;; table index (destination)
    i32.const 0    ;; element segment offset (source)
    i32.const 3    ;; length
    table.init $table 0
    
    ;; Drop element segment after initialization
    elem.drop 0
  )
  
  ;; Function to test call_indirect with table
  (func $test_call_indirect (param i32 i32) (result i32)
    local.get 1    ;; argument for function
    local.get 0    ;; table index
    call_indirect (type $t0)
  )
  
  ;; Function to test table.get
  (func $test_table_get (param i32) (result funcref)
    local.get 0
    table.get $table
  )
  
  ;; Function to test table.set
  (func $test_table_set (param i32 funcref)
    local.get 0
    local.get 1
    table.set $table
  )
  
  ;; Function to test table.size
  (func $test_table_size (result i32)
    table.size $table
  )
  
  ;; Function to test table.grow
  (func $test_table_grow (param i32 funcref) (result i32)
    local.get 1    ;; init value
    local.get 0    ;; delta (number of elements to grow)
    table.grow $table
  )
  
  ;; Element segment for table initialization
  (elem (i32.const 0) $func0 $func1 $func2)
  
  ;; Export functions
  (export "test_table_init" (func $test_table_init))
  (export "test_call_indirect" (func $test_call_indirect))
  (export "test_table_get" (func $test_table_get))
  (export "test_table_set" (func $test_table_set))
  (export "test_table_size" (func $test_table_size))
  (export "test_table_grow" (func $test_table_grow))
  (export "func0" (func $func0))
  (export "func1" (func $func1))
  (export "func2" (func $func2))
  (export "func3" (func $func3))
  (export "table" (table $table))
)