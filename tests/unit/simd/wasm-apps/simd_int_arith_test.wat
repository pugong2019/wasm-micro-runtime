(module
  (func $test_simd_i8x16_sub (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i8x16.sub)
  
  (func $test_simd_i16x8_sub (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i16x8.sub)
  
  (func $test_simd_i32x4_sub (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i32x4.sub)
  
  (func $test_simd_i64x2_sub (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i64x2.sub)
  
  (export "test_simd_i8x16_sub" (func $test_simd_i8x16_sub))
  (export "test_simd_i16x8_sub" (func $test_simd_i16x8_sub))
  (export "test_simd_i32x4_sub" (func $test_simd_i32x4_sub))
  (export "test_simd_i64x2_sub" (func $test_simd_i64x2_sub)))