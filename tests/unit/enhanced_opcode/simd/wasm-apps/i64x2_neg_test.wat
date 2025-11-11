(module
  ;; i64x2.neg comprehensive test module
  ;; Tests two's complement negation operation on 2-lane i64 SIMD vectors

  ;; Test i64x2.neg with basic mixed values
  ;; Input: [42, -100]
  ;; Expected result: [-42, 100]
  (func (export "test_basic_neg") (result v128)
    ;; Create v128 with mixed positive/negative values using i64x2 lanes
    v128.const i64x2 42 -100
    ;; Apply i64x2.neg
    i64x2.neg
  )

  ;; Test i64x2.neg with large values
  ;; Input: [-999999999, 1000000000]
  ;; Expected result: [999999999, -1000000000]
  (func (export "test_large_neg") (result v128)
    ;; Create v128 with large negative and positive values
    v128.const i64x2 -999999999 1000000000
    ;; Apply i64x2.neg
    i64x2.neg
  )

  ;; Test i64x2.neg with boundary values including INT64_MIN overflow
  ;; Input: [INT64_MIN, INT64_MAX]
  ;; Expected result: [INT64_MIN, -INT64_MAX] (INT64_MIN wraps to itself)
  (func (export "test_boundary_neg") (result v128)
    ;; Create v128 with boundary values
    ;; INT64_MIN: -9223372036854775808, INT64_MAX: 9223372036854775807
    v128.const i64x2 -9223372036854775808 9223372036854775807
    ;; Apply i64x2.neg - INT64_MIN (-9223372036854775808) wraps to itself due to overflow
    i64x2.neg
  )

  ;; Test i64x2.neg with reversed boundary values
  ;; Input: [INT64_MAX, INT64_MIN]
  ;; Expected result: [-INT64_MAX, INT64_MIN] (INT64_MIN wraps to itself)
  (func (export "test_boundary_reversed_neg") (result v128)
    ;; Create v128 with reversed boundary values
    v128.const i64x2 9223372036854775807 -9223372036854775808
    ;; Apply i64x2.neg
    i64x2.neg
  )

  ;; Test i64x2.neg with zero values
  ;; Input: [0, 0]
  ;; Expected result: [0, 0] (negation of zero is zero)
  (func (export "test_zero_neg") (result v128)
    ;; Create v128 with all zeros
    v128.const i64x2 0 0
    ;; Apply i64x2.neg to zeros (should remain zero)
    i64x2.neg
  )

  ;; Test i64x2.neg with signed unity values
  ;; Input: [-1, 1]
  ;; Expected result: [1, -1]
  (func (export "test_unity_neg") (result v128)
    ;; Create v128 with negative one and positive one
    v128.const i64x2 -1 1
    ;; Apply i64x2.neg to signed unity
    i64x2.neg
  )

  ;; Test i64x2.neg with mixed sign pattern for lane independence
  ;; Input: [42, -42]
  ;; Expected result: [-42, 42]
  (func (export "test_mixed_neg") (result v128)
    ;; Create v128 with mixed sign pattern across lanes
    v128.const i64x2 42 -42
    ;; Apply i64x2.neg to mixed pattern
    i64x2.neg
  )

  ;; Test i64x2.neg with larger mixed sign pattern
  ;; Input: [-123456789, 987654321]
  ;; Expected result: [123456789, -987654321]
  (func (export "test_mixed_large_neg") (result v128)
    ;; Create v128 with large mixed sign pattern
    v128.const i64x2 -123456789 987654321
    ;; Apply i64x2.neg to large mixed pattern
    i64x2.neg
  )

  ;; Test i64x2.neg with alternating bit pattern
  ;; Input: [0x5555555555555555, -0x5555555555555555]
  ;; Expected result: [-0x5555555555555555, 0x5555555555555555]
  (func (export "test_bitpattern_neg") (result v128)
    ;; Create v128 with alternating bit pattern
    ;; 0x5555555555555555 = 6148914691236517205
    v128.const i64x2 6148914691236517205 -6148914691236517205
    ;; Apply i64x2.neg to bit patterns
    i64x2.neg
  )

  ;; Lane extraction helper function
  ;; Extracts specific lane value for validation (debugging purposes)
  (func (export "extract_lane") (param $vector v128) (param $lane_index i32) (result i64)
    (local $result i64)

    ;; Extract lane based on index using if-else structure
    (if (i32.eq (local.get $lane_index) (i32.const 0))
      (then
        (local.set $result (i64x2.extract_lane 0 (local.get $vector)))
      )
      (else
        (local.set $result (i64x2.extract_lane 1 (local.get $vector)))
      )
    )

    (local.get $result)
  )

  ;; Helper function for manual negation testing (takes two i64 parameters)
  ;; Creates i64x2 vector from two separate i64 values and applies negation
  (func (export "manual_neg") (param $lane0 i64) (param $lane1 i64) (result v128)
    ;; Create v128 from two i64 parameters
    (v128.const i64x2 0 0)
    (i64x2.replace_lane 0 (local.get $lane0))
    (i64x2.replace_lane 1 (local.get $lane1))
    ;; Apply negation
    i64x2.neg
  )
)