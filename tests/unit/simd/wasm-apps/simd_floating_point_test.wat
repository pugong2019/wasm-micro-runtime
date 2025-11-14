(module
  (func $test_simd_f32x4_abs (param $v1 v128) (result v128)
    local.get $v1
    f32x4.abs)
  
  (func $test_simd_f32x4_neg (param $v1 v128) (result v128)
    local.get $v1
    f32x4.neg)
  
  (func $test_simd_f32x4_sqrt (param $v1 v128) (result v128)
    local.get $v1
    f32x4.sqrt)
  
  (func $test_simd_f64x2_abs (param $v1 v128) (result v128)
    local.get $v1
    f64x2.abs)
  
  (func $test_simd_f64x2_neg (param $v1 v128) (result v128)
    local.get $v1
    f64x2.neg)
  
  (func $test_simd_f64x2_sqrt (param $v1 v128) (result v128)
    local.get $v1
    f64x2.sqrt)
  
  (export "test_simd_f32x4_abs" (func $test_simd_f32x4_abs))
  (export "test_simd_f32x4_neg" (func $test_simd_f32x4_neg))
  (export "test_simd_f32x4_sqrt" (func $test_simd_f32x4_sqrt))
  (export "test_simd_f64x2_abs" (func $test_simd_f64x2_abs))
  (export "test_simd_f64x2_neg" (func $test_simd_f64x2_neg))
  (export "test_simd_f64x2_sqrt" (func $test_simd_f64x2_sqrt)))