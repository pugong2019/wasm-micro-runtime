(module
  (func $test_simd_i8x16_add_sat_s (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i8x16.add_sat_s)
  
  (func $test_simd_i8x16_add_sat_u (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i8x16.add_sat_u)
  
  (func $test_simd_i8x16_sub_sat_s (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i8x16.sub_sat_s)
  
  (func $test_simd_i8x16_sub_sat_u (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i8x16.sub_sat_u)
  
  (export "test_simd_i8x16_add_sat_s" (func $test_simd_i8x16_add_sat_s))
  (export "test_simd_i8x16_add_sat_u" (func $test_simd_i8x16_add_sat_u))
  (export "test_simd_i8x16_sub_sat_s" (func $test_simd_i8x16_sub_sat_s))
  (export "test_simd_i8x16_sub_sat_u" (func $test_simd_i8x16_sub_sat_u)))