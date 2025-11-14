(module
  (func $test_simd_i8x16_shl (param $v1 v128) (param $v2 i32) (result v128)
    local.get $v1
    local.get $v2
    i8x16.shl)
  
  (func $test_simd_i8x16_shr_s (param $v1 v128) (param $v2 i32) (result v128)
    local.get $v1
    local.get $v2
    i8x16.shr_s)
  
  (func $test_simd_i8x16_shr_u (param $v1 v128) (param $v2 i32) (result v128)
    local.get $v1
    local.get $v2
    i8x16.shr_u)
  
  (func $test_simd_i16x8_shl (param $v1 v128) (param $v2 i32) (result v128)
    local.get $v1
    local.get $v2
    i16x8.shl)
  
  (func $test_simd_i16x8_shr_s (param $v1 v128) (param $v2 i32) (result v128)
    local.get $v1
    local.get $v2
    i16x8.shr_s)
  
  (func $test_simd_i16x8_shr_u (param $v1 v128) (param $v2 i32) (result v128)
    local.get $v1
    local.get $v2
    i16x8.shr_u)
  
  (export "test_simd_i8x16_shl" (func $test_simd_i8x16_shl))
  (export "test_simd_i8x16_shr_s" (func $test_simd_i8x16_shr_s))
  (export "test_simd_i8x16_shr_u" (func $test_simd_i8x16_shr_u))
  (export "test_simd_i16x8_shl" (func $test_simd_i16x8_shl))
  (export "test_simd_i16x8_shr_s" (func $test_simd_i16x8_shr_s))
  (export "test_simd_i16x8_shr_u" (func $test_simd_i16x8_shr_u)))