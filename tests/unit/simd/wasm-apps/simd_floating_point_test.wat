(module
  ;; 32位浮点数算术运算
  (func $f32x4_add_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    f32x4.add)
  
  (func $f32x4_sub_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    f32x4.sub)
  
  (func $f32x4_mul_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    f32x4.mul)
  
  (func $f32x4_div_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    f32x4.div)
  
  ;; 64位浮点数算术运算
  (func $f64x2_add_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    f64x2.add)
  
  (func $f64x2_sub_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    f64x2.sub)
  
  (func $f64x2_mul_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    f64x2.mul)
  
  (func $f64x2_div_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    f64x2.div)
  
  ;; 32位浮点数比较操作
  (func $f32x4_eq_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    f32x4.eq)
  
  (func $f32x4_lt_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    f32x4.lt)
  
  ;; 64位浮点数比较操作
  (func $f64x2_eq_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    f64x2.eq)
  
  (func $f64x2_lt_test (param $v1 v128) (param $v2 v128) (result v128)
    local.get $v1
    local.get $v2
    f64x2.lt)
  
  ;; 32位浮点数绝对值
  (func $f32x4_abs_test (param $v1 v128) (result v128)
    local.get $v1
    f32x4.abs)
  
  ;; 64位浮点数绝对值
  (func $f64x2_abs_test (param $v1 v128) (result v128)
    local.get $v1
    f64x2.abs)
  
  ;; 导出所有函数
  (export "f32x4_add_test" (func $f32x4_add_test))
  (export "f32x4_sub_test" (func $f32x4_sub_test))
  (export "f32x4_mul_test" (func $f32x4_mul_test))
  (export "f32x4_div_test" (func $f32x4_div_test))
  (export "f64x2_add_test" (func $f64x2_add_test))
  (export "f64x2_sub_test" (func $f64x2_sub_test))
  (export "f64x2_mul_test" (func $f64x2_mul_test))
  (export "f64x2_div_test" (func $f64x2_div_test))
  (export "f32x4_eq_test" (func $f32x4_eq_test))
  (export "f32x4_lt_test" (func $f32x4_lt_test))
  (export "f64x2_eq_test" (func $f64x2_eq_test))
  (export "f64x2_lt_test" (func $f64x2_lt_test))
  (export "f32x4_abs_test" (func $f32x4_abs_test))
  (export "f64x2_abs_test" (func $f64x2_abs_test))
)