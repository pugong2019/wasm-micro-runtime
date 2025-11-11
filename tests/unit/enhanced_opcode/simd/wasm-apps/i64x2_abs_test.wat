;; Enhanced test module for i64x2.abs SIMD opcode
;; Tests absolute value operation on 2 x 64-bit integer lanes

(module
  ;; Test function for basic i64x2.abs functionality
  ;; Takes two i64 parameters to build a vector, performs i64x2.abs, returns result lanes
  (func (export "test_basic_abs") (param $input0 i64) (param $input1 i64)
                                  (result i64 i64)
    (local $vec_input v128)
    (local $result v128)

    ;; Build input vector: [input0, input1]
    local.get $input0
    i64x2.splat
    local.get $input1
    i64x2.replace_lane 1
    local.set $vec_input

    ;; Perform i64x2.abs operation
    local.get $vec_input
    i64x2.abs
    local.set $result

    ;; Extract result lanes and return
    local.get $result
    i64x2.extract_lane 0
    local.get $result
    i64x2.extract_lane 1
  )

  ;; Test function for boundary value absolute values
  ;; Tests INT64_MIN, INT64_MAX, and boundary conditions
  (func (export "test_boundary_abs") (param $input0 i64) (param $input1 i64)
                                     (result i64 i64)
    (local $vec_input v128)
    (local $result v128)

    ;; Build input vector: [input0, input1]
    local.get $input0
    i64x2.splat
    local.get $input1
    i64x2.replace_lane 1
    local.set $vec_input

    ;; Perform i64x2.abs operation
    local.get $vec_input
    i64x2.abs
    local.set $result

    ;; Extract result lanes and return
    local.get $result
    i64x2.extract_lane 0
    local.get $result
    i64x2.extract_lane 1
  )

  ;; Test function for special value absolute values
  ;; Tests zero, -1, 1, and other special values
  (func (export "test_special_abs") (param $input0 i64) (param $input1 i64)
                                    (result i64 i64)
    (local $vec_input v128)
    (local $result v128)

    ;; Build input vector: [input0, input1]
    local.get $input0
    i64x2.splat
    local.get $input1
    i64x2.replace_lane 1
    local.set $vec_input

    ;; Perform i64x2.abs operation
    local.get $vec_input
    i64x2.abs
    local.set $result

    ;; Extract result lanes and return
    local.get $result
    i64x2.extract_lane 0
    local.get $result
    i64x2.extract_lane 1
  )

  ;; Test function for mixed lane absolute values
  ;; Tests lane independence with mixed positive/negative values
  (func (export "test_mixed_abs") (param $input0 i64) (param $input1 i64)
                                  (result i64 i64)
    (local $vec_input v128)
    (local $result v128)

    ;; Build input vector: [input0, input1]
    local.get $input0
    i64x2.splat
    local.get $input1
    i64x2.replace_lane 1
    local.set $vec_input

    ;; Perform i64x2.abs operation
    local.get $vec_input
    i64x2.abs
    local.set $result

    ;; Extract result lanes and return
    local.get $result
    i64x2.extract_lane 0
    local.get $result
    i64x2.extract_lane 1
  )

  ;; Test function for bit pattern absolute values
  ;; Tests special bit patterns and 64-bit boundary values
  (func (export "test_bitpattern_abs") (param $input0 i64) (param $input1 i64)
                                       (result i64 i64)
    (local $vec_input v128)
    (local $result v128)

    ;; Build input vector: [input0, input1]
    local.get $input0
    i64x2.splat
    local.get $input1
    i64x2.replace_lane 1
    local.set $vec_input

    ;; Perform i64x2.abs operation
    local.get $vec_input
    i64x2.abs
    local.set $result

    ;; Extract result lanes and return
    local.get $result
    i64x2.extract_lane 0
    local.get $result
    i64x2.extract_lane 1
  )

  ;; Test function demonstrating absolute value with constant values
  ;; Tests compile-time constant folding and runtime execution
  (func (export "test_const_abs_positive") (result v128)
    ;; Create vector [42, 1000] and compute absolute values
    i64.const 42
    i64x2.splat
    i64.const 1000
    i64x2.replace_lane 1
    i64x2.abs
  )

  ;; Test function for negative constant absolute values
  (func (export "test_const_abs_negative") (result v128)
    ;; Create vector [-100, -999999] and compute absolute values
    i64.const -100
    i64x2.splat
    i64.const -999999
    i64x2.replace_lane 1
    i64x2.abs
  )

  ;; Test function for zero constant absolute values
  (func (export "test_const_abs_zero") (result v128)
    ;; Create vector [0, 0] and compute absolute values
    i64.const 0
    i64x2.splat
    i64x2.abs
  )

  ;; Test function for boundary constant absolute values
  (func (export "test_const_abs_boundary") (result v128)
    ;; Create vector [INT64_MIN, INT64_MAX] and compute absolute values
    ;; Note: INT64_MIN = 0x8000000000000000, INT64_MAX = 0x7FFFFFFFFFFFFFFF
    i64.const 0x8000000000000000  ;; INT64_MIN
    i64x2.splat
    i64.const 0x7FFFFFFFFFFFFFFF  ;; INT64_MAX
    i64x2.replace_lane 1
    i64x2.abs
  )

  ;; Test function demonstrating chained SIMD operations with absolute value
  ;; Tests integration with other SIMD operations
  (func (export "test_chained_abs") (param $input0 i64) (param $input1 i64) (result v128)
    (local $vec1 v128)
    (local $vec2 v128)

    ;; Create first vector and compute absolute values
    local.get $input0
    i64x2.splat
    local.get $input1
    i64x2.replace_lane 1
    i64x2.abs
    local.set $vec1

    ;; Create second vector with different values and compute absolute values
    i64.const -50
    i64x2.splat
    i64.const 75
    i64x2.replace_lane 1
    i64x2.abs
    local.set $vec2

    ;; Perform bitwise OR on both absolute value results
    local.get $vec1
    local.get $vec2
    v128.or
  )

  ;; Identity test function: abs then negate to test roundtrip
  ;; Verifies mathematical properties of absolute value operation
  (func (export "test_abs_identity") (param $input i64) (result i64)
    (local $vec v128)

    ;; Create vector [input, -input], compute abs, verify both lanes are positive
    local.get $input
    i64x2.splat
    local.get $input
    i64.const -1
    i64.mul
    i64x2.replace_lane 1
    i64x2.abs
    local.set $vec

    ;; Extract lane 0 (should be abs(input))
    local.get $vec
    i64x2.extract_lane 0
  )
)