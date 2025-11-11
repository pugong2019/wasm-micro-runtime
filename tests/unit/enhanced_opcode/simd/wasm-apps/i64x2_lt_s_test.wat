;; Enhanced test module for i64x2.lt_s SIMD opcode
;; Tests signed less-than comparison on 2 x 64-bit integer lanes

(module
  ;; Test function for basic i64x2.lt_s functionality
  ;; Takes two v128 vectors represented as 4 i64 parameters, performs i64x2.lt_s, returns result
  (func (export "test_basic") (param $a0 i64) (param $a1 i64)
                              (param $b0 i64) (param $b1 i64)
                              (result i64 i64)
    ;; Create first vector from individual i64 parameters
    (local $vec_a v128)
    (local $vec_b v128)
    (local $result v128)

    ;; Build vector A: [a0, a1]
    local.get $a0
    i64x2.splat
    local.get $a1
    i64x2.replace_lane 1
    local.set $vec_a

    ;; Build vector B: [b0, b1]
    local.get $b0
    i64x2.splat
    local.get $b1
    i64x2.replace_lane 1
    local.set $vec_b

    ;; Perform i64x2.lt_s comparison
    local.get $vec_a
    local.get $vec_b
    i64x2.lt_s
    local.set $result

    ;; Extract result lanes and return
    local.get $result
    i64x2.extract_lane 0
    local.get $result
    i64x2.extract_lane 1
  )

  ;; Test function for boundary value comparisons
  ;; Tests INT64_MIN, INT64_MAX, and zero boundary conditions
  (func (export "test_boundary") (param $a0 i64) (param $a1 i64)
                                 (param $b0 i64) (param $b1 i64)
                                 (result i64 i64)
    (local $vec_a v128)
    (local $vec_b v128)
    (local $result v128)

    ;; Build vector A: [a0, a1]
    local.get $a0
    i64x2.splat
    local.get $a1
    i64x2.replace_lane 1
    local.set $vec_a

    ;; Build vector B: [b0, b1]
    local.get $b0
    i64x2.splat
    local.get $b1
    i64x2.replace_lane 1
    local.set $vec_b

    ;; Perform i64x2.lt_s comparison
    local.get $vec_a
    local.get $vec_b
    i64x2.lt_s
    local.set $result

    ;; Extract result lanes and return
    local.get $result
    i64x2.extract_lane 0
    local.get $result
    i64x2.extract_lane 1
  )

  ;; Test function for equal value comparisons
  ;; Tests that equal values return false (0x0000000000000000)
  (func (export "test_equal") (param $a0 i64) (param $a1 i64)
                              (param $b0 i64) (param $b1 i64)
                              (result i64 i64)
    (local $vec_a v128)
    (local $vec_b v128)
    (local $result v128)

    ;; Build vector A: [a0, a1]
    local.get $a0
    i64x2.splat
    local.get $a1
    i64x2.replace_lane 1
    local.set $vec_a

    ;; Build vector B: [b0, b1]
    local.get $b0
    i64x2.splat
    local.get $b1
    i64x2.replace_lane 1
    local.set $vec_b

    ;; Perform i64x2.lt_s comparison
    local.get $vec_a
    local.get $vec_b
    i64x2.lt_s
    local.set $result

    ;; Extract result lanes and return
    local.get $result
    i64x2.extract_lane 0
    local.get $result
    i64x2.extract_lane 1
  )

  ;; Test function for cross-lane independence validation
  ;; Tests that comparison results in one lane don't affect the other lane
  (func (export "test_independence") (param $a0 i64) (param $a1 i64)
                                     (param $b0 i64) (param $b1 i64)
                                     (result i64 i64)
    (local $vec_a v128)
    (local $vec_b v128)
    (local $result v128)

    ;; Build vector A: [a0, a1]
    local.get $a0
    i64x2.splat
    local.get $a1
    i64x2.replace_lane 1
    local.set $vec_a

    ;; Build vector B: [b0, b1]
    local.get $b0
    i64x2.splat
    local.get $b1
    i64x2.replace_lane 1
    local.set $vec_b

    ;; Perform i64x2.lt_s comparison
    local.get $vec_a
    local.get $vec_b
    i64x2.lt_s
    local.set $result

    ;; Extract result lanes and return
    local.get $result
    i64x2.extract_lane 0
    local.get $result
    i64x2.extract_lane 1
  )
)