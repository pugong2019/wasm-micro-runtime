(module
  ;; 32位整数算术运算
  (func $i32x4_add_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i32x4.add)
  
  (func $i32x4_sub_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i32x4.sub)
  
  (func $i32x4_mul_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i32x4.mul)
  
  ;; 64位整数算术运算
  (func $i64x2_add_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i64x2.add)
  
  (func $i64x2_sub_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i64x2.sub)
  
  (func $i64x2_mul_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i64x2.mul)
  
  ;; 8位整数比较操作
  (func $i8x16_eq_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i8x16.eq)
  
  (func $i8x16_lt_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i8x16.lt_s)
  
  ;; 16位整数比较操作
  (func $i16x8_eq_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i16x8.eq)
  
  (func $i16x8_lt_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i16x8.lt_s)
  
  ;; 32位整数比较操作
  (func $i32x4_eq_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i32x4.eq)
  
  (func $i32x4_lt_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i32x4.lt_s)
  
  ;; 64位整数比较操作
  (func $i64x2_eq_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i64x2.eq)
  
  (func $i64x2_lt_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    i64x2.lt_s)
  
  ;; 导出所有函数
  (export "i32x4_add_test" (func $i32x4_add_test))
  (export "i32x4_sub_test" (func $i32x4_sub_test))
  (export "i32x4_mul_test" (func $i32x4_mul_test))
  (export "i64x2_add_test" (func $i64x2_add_test))
  (export "i64x2_sub_test" (func $i64x2_sub_test))
  (export "i64x2_mul_test" (func $i64x2_mul_test))
  (export "i8x16_eq_test" (func $i8x16_eq_test))
  (export "i8x16_lt_test" (func $i8x16_lt_test))
  (export "i16x8_eq_test" (func $i16x8_eq_test))
  (export "i16x8_lt_test" (func $i16x8_lt_test))
  (export "i32x4_eq_test" (func $i32x4_eq_test))
  (export "i32x4_lt_test" (func $i32x4_lt_test))
  (export "i64x2_eq_test" (func $i64x2_eq_test))
  (export "i64x2_lt_test" (func $i64x2_lt_test))
)