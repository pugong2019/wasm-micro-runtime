(module
  (func $test_simd_i8x16_add (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i8x16.add)
  
  (func $test_simd_i16x8_add (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i16x8.add)
  
  (func $test_simd_i32x4_add (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i32x4.add)
  
  (func $test_simd_i64x2_add (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i64x2.add)
  
  (func $test_simd_f32x4_add (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    f32x4.add)
  
  (func $test_simd_f64x2_add (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    f64x2.add)
  
  (export "test_simd_i8x16_add" (func $test_simd_i8x16_add))
  (export "test_simd_i16x8_add" (func $test_simd_i16x8_add))
  (export "test_simd_i32x4_add" (func $test_simd_i32x4_add))
  (export "test_simd_i64x2_add" (func $test_simd_i64x2_add))
  (export "test_simd_f32x4_add" (func $test_simd_f32x4_add))
  (export "test_simd_f64x2_add" (func $test_simd_f64x2_add)))