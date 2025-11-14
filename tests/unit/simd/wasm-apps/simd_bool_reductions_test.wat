(module
  (func $test_simd_i8x16_all_true (param $v1 v128) (result i32)
    local.get $v1
    i8x16.all_true)
  
  (func $test_simd_i16x8_all_true (param $v1 v128) (result i32)
    local.get $v1
    i16x8.all_true)
  
  (func $test_simd_i32x4_all_true (param $v1 v128) (result i32)
    local.get $v1
    i32x4.all_true)
  
  (func $test_simd_i64x2_all_true (param $v1 v128) (result i32)
    local.get $v1
    i64x2.all_true)
  
  (func $test_simd_i8x16_bitmask (param $v1 v128) (result i32)
    local.get $v1
    i8x16.bitmask)
  
  (func $test_simd_i16x8_bitmask (param $v1 v128) (result i32)
    local.get $v1
    i16x8.bitmask)
  
  (export "test_simd_i8x16_all_true" (func $test_simd_i8x16_all_true))
  (export "test_simd_i16x8_all_true" (func $test_simd_i16x8_all_true))
  (export "test_simd_i32x4_all_true" (func $test_simd_i32x4_all_true))
  (export "test_simd_i64x2_all_true" (func $test_simd_i64x2_all_true))
  (export "test_simd_i8x16_bitmask" (func $test_simd_i8x16_bitmask))
  (export "test_simd_i16x8_bitmask" (func $test_simd_i16x8_bitmask)))