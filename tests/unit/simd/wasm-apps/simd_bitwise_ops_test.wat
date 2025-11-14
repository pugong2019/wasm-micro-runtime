(module
  ;; Purpose: Test SIMD bitwise operations for WAMR compilation
  ;; Features: SIMD V128 bitwise operations (AND, OR, XOR, ANDNOT, NOT, BITSELECT)
  
  ;; Memory for testing
  (memory 1)
  
  ;; Test data initialization
  (data (i32.const 0) "\00\01\02\03\04\05\06\07\08\09\0A\0B\0C\0D\0E\0F")
  (data (i32.const 16) "\10\11\12\13\14\15\16\17\18\19\1A\1B\1C\1D\1E\1F")
  (data (i32.const 32) "\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF")
  (data (i32.const 48) "\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00")
  
  ;; V128 AND operation test
  (func (export "test_v128_and") (result v128)
    ;; Load two vectors and perform AND
    (v128.load (i32.const 0))
    (v128.load (i32.const 16))
    v128.and
  )
  
  ;; V128 OR operation test
  (func (export "test_v128_or") (result v128)
    ;; Load two vectors and perform OR
    (v128.load (i32.const 0))
    (v128.load (i32.const 16))
    v128.or
  )
  
  ;; V128 XOR operation test
  (func (export "test_v128_xor") (result v128)
    ;; Load two vectors and perform XOR
    (v128.load (i32.const 0))
    (v128.load (i32.const 16))
    v128.xor
  )
  
  ;; V128 ANDNOT operation test
  (func (export "test_v128_andnot") (result v128)
    ;; Load two vectors and perform ANDNOT
    (v128.load (i32.const 0))
    (v128.load (i32.const 16))
    v128.andnot
  )
  
  ;; V128 NOT operation test
  (func (export "test_v128_not") (result v128)
    ;; Load vector and perform NOT
    (v128.load (i32.const 32))
    v128.not
  )
  
  ;; V128 BITSELECT operation test
  (func (export "test_v128_bitselect") (result v128)
    ;; Load three vectors and perform BITSELECT
    ;; v128.or(v128.and(v1, c), v128.and(v2, v128.not(c)))
    (v128.load (i32.const 0))   ;; v1
    (v128.load (i32.const 16))  ;; v2
    (v128.load (i32.const 32))  ;; c (mask)
    v128.bitselect
  )
  
  ;; Comprehensive test with multiple operations
  (func (export "test_v128_comprehensive") (result v128)
    ;; Test sequence: AND -> OR -> XOR -> NOT
    (v128.load (i32.const 0))
    (v128.load (i32.const 16))
    v128.and
    (v128.load (i32.const 32))
    v128.or
    (v128.load (i32.const 48))
    v128.xor
    v128.not
  )
  
  ;; Edge case: All zeros
  (func (export "test_v128_zeros") (result v128)
    (v128.load (i32.const 48))
    (v128.load (i32.const 48))
    v128.and
  )
  
  ;; Edge case: All ones
  (func (export "test_v128_ones") (result v128)
    (v128.load (i32.const 32))
    (v128.load (i32.const 32))
    v128.or
  )
)