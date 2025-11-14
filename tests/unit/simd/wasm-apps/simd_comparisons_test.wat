(module
  (func $test_simd_i8x16_eq (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i8x16.eq)
  
  (func $test_simd_i8x16_ne (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i8x16.ne)
  
  (func $test_simd_i8x16_lt_s (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i8x16.lt_s)
  
  (func $test_simd_i8x16_lt_u (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i8x16.lt_u)
  
  (func $test_simd_i16x8_eq (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i16x8.eq)
  
  (func $test_simd_i16x8_ne (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i16x8.ne)
  
  (export "test_simd_i8x16_eq" (func $test_simd_i8x16_eq))
  (export "test_simd_i8x16_ne" (func $test_simd_i8x16_ne))
  (export "test_simd_i8x16_lt_s" (func $test_simd_i8x16_lt_s))
  (export "test_simd_i8x16_lt_u" (func $test_simd_i8x16_lt_u))
  (export "test_simd_i16x8_eq" (func $test_simd_i16x8_eq))
  (export "test_simd_i16x8_ne" (func $test_simd_i16x8_ne)))