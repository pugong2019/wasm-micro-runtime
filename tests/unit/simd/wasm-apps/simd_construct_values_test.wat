(module
  (func $test_simd_i8x16_splat (param $v1 i32) (result v128)
    local.get $v1
    i8x16.splat)
  
  (func $test_simd_i16x8_splat (param $v1 i32) (result v128)
    local.get $v1
    i16x8.splat)
  
  (func $test_simd_i32x4_splat (param $v1 i32) (result v128)
    local.get $v1
    i32x4.splat)
  
  (func $test_simd_i64x2_splat (param $v1 i64) (result v128)
    local.get $v1
    i64x2.splat)
  
  (func $test_simd_f32x4_splat (param $v1 f32) (result v128)
    local.get $v1
    f32x4.splat)
  
  (func $test_simd_f64x2_splat (param $v1 f64) (result v128)
    local.get $v1
    f64x2.splat)
  
  (export "test_simd_i8x16_splat" (func $test_simd_i8x16_splat))
  (export "test_simd_i16x8_splat" (func $test_simd_i16x8_splat))
  (export "test_simd_i32x4_splat" (func $test_simd_i32x4_splat))
  (export "test_simd_i64x2_splat" (func $test_simd_i64x2_splat))
  (export "test_simd_f32x4_splat" (func $test_simd_f32x4_splat))
  (export "test_simd_f64x2_splat" (func $test_simd_f64x2_splat)))