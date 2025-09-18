(module
  ;; Memory definition: 8 GB = 131072 pages for comprehensive testing
  (memory (;0;) i64 131072 131072)

  ;; Initialize memory with test data at specific locations
  (data (i64.const 0x1000) "\01\02\03\04\05\06\07\08\09\0a\0b\0c\0d\0e\0f\10\11\12\13\14\15\16\17\18")
  (data (i64.const 0x100000000) "\aa\bb\cc\dd\ee\ff\00\11")

  ;; Test basic i64.load operation
  (func (export "test_i64_load_basic") (param $addr i64) (result i64)
    (i64.load (local.get $addr))
  )

  ;; Test basic i64.store operation
  (func (export "test_i64_store_basic") (param $addr i64) (param $value i64)
    (i64.store (local.get $addr) (local.get $value))
  )

  ;; Test i64.store and verify with load
  (func (export "test_i64_store_and_load") (param $addr i64) (param $value i64) (result i64)
    (i64.store (local.get $addr) (local.get $value))
    (i64.load (local.get $addr))
  )

  ;; Test memory.size64 operation
  (func (export "test_memory_size64") (result i64)
    (memory.size)
  )

  ;; Test memory.grow64 with success case
  (func (export "test_memory_grow64_success") (param $pages i64) (result i64)
    (memory.grow (local.get $pages))
  )

  ;; Test memory.grow64 with failure case (too many pages)
  (func (export "test_memory_grow64_failure") (param $pages i64) (result i64)
    (memory.grow (local.get $pages))
  )

  ;; Test i64.load with offset - Fixed to load from initialized data
  (func (export "test_i64_load_offset") (param $addr i64) (param $offset i32) (result i64)
    (if (result i64) (i32.eq (local.get $offset) (i32.const 8))
      (then (i64.load offset=8 (local.get $addr)))
      (else (if (result i64) (i32.eq (local.get $offset) (i32.const 16))
        (then (i64.load offset=16 (local.get $addr)))
        (else (i64.load (local.get $addr)))
      ))
    )
  )

  ;; Test i64.store with offset - Fixed return type
  (func (export "test_i64_store_offset") (param $addr i64) (param $offset i32) (param $value i64) (result i32)
    (if (i32.eq (local.get $offset) (i32.const 8))
      (then (i64.store offset=8 (local.get $addr) (local.get $value)))
      (else (if (i32.eq (local.get $offset) (i32.const 16))
        (then (i64.store offset=16 (local.get $addr) (local.get $value)))
        (else (i64.store (local.get $addr) (local.get $value)))
      ))
    )
    (i32.const 1) ;; Return success
  )

  ;; Test address validation with large addresses
  (func (export "test_address_validation") (param $addr i64) (result i32)
    (local $result i32)
    (local.set $result (i32.const 1))
    (block $catch
      ;; Try to access the address
      (i64.load (local.get $addr))
      drop
    )
    (local.get $result)
  )

  ;; Test boundary conditions at 4GB mark
  (func (export "test_boundary_4gb") (param $value i64) (result i64)
    (local $addr i64)
    (local.set $addr (i64.const 0x100000000)) ;; Exactly 4GB
    (i64.store (local.get $addr) (local.get $value))
    (i64.load (local.get $addr))
  )

  ;; Test alignment checks with different alignments
  (func (export "test_alignment_check") (param $addr i64) (param $align_type i32) (result i64)
    (if (result i64) (i32.eq (local.get $align_type) (i32.const 1))
      (then (i64.load align=1 (local.get $addr)))
      (else (if (result i64) (i32.eq (local.get $align_type) (i32.const 2))
        (then (i64.load align=2 (local.get $addr)))
        (else (if (result i64) (i32.eq (local.get $align_type) (i32.const 4))
          (then (i64.load align=4 (local.get $addr)))
          (else (i64.load align=8 (local.get $addr)))
        ))
      ))
    )
  )

  ;; Test memory operations at various sizes
  (func (export "test_different_load_sizes") (param $addr i64) (param $size_type i32) (result i64)
    (if (result i64) (i32.eq (local.get $size_type) (i32.const 8))
      (then (i64.extend_i32_u (i32.load8_u (local.get $addr))))
      (else (if (result i64) (i32.eq (local.get $size_type) (i32.const 16))
        (then (i64.extend_i32_u (i32.load16_u (local.get $addr))))
        (else (if (result i64) (i32.eq (local.get $size_type) (i32.const 32))
          (then (i64.extend_i32_u (i32.load (local.get $addr))))
          (else (i64.load (local.get $addr)))
        ))
      ))
    )
  )

  ;; Test memory initialization verification
  (func (export "test_memory_initialization") (result i64)
    ;; Load from initialized data section at 0x1000
    (i64.load (i64.const 0x1000))
  )

  ;; Test large address operations beyond 4GB
  (func (export "test_large_address_ops") (param $offset i64) (param $value i64) (result i64)
    (local $base_addr i64)
    (local.set $base_addr (i64.const 0x100000000)) ;; 4GB base
    (local.set $base_addr (i64.add (local.get $base_addr) (local.get $offset)))
    (i64.store (local.get $base_addr) (local.get $value))
    (i64.load (local.get $base_addr))
  )

  ;; Test memory grow and size operations together - Fixed to use separate calls
  (func (export "test_memory_grow_and_size") (param $grow_pages i64) (result i64 i64)
    (local $size_before i64)
    (local.set $size_before (memory.size))
    (memory.grow (local.get $grow_pages))
    drop ;; Drop the grow result since we return sizes
    (local.get $size_before)
    (memory.size)
  )
)