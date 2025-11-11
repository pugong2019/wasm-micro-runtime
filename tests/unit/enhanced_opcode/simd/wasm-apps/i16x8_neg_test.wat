(module
  ;; i16x8.neg comprehensive test module
  ;; Tests two's complement negation operation on 8-lane i16 SIMD vectors

  ;; Test i16x8.neg with basic mixed values
  ;; Input: [1, -2, 100, -500, 0, 32767, -32767, 42]
  ;; Expected result: [-1, 2, -100, 500, 0, -32767, 32767, -42]
  (func (export "test_i16x8_neg_basic") (result v128)
    ;; Create v128 with mixed positive/negative values using i16x8 lanes
    v128.const i16x8 1 -2 100 -500 0 32767 -32767 42
    ;; Apply i16x8.neg
    i16x8.neg
  )

  ;; Test i16x8.neg with boundary values including INT16_MIN overflow
  ;; Input: [-32768, 32767, -1, 1, 0, -32768, 32767, -1]
  ;; Expected result: [-32768, -32767, 1, -1, 0, -32768, -32767, 1] (INT16_MIN wraps to itself)
  (func (export "test_i16x8_neg_boundary") (result v128)
    ;; Create v128 with boundary values
    ;; INT16_MIN: -32768, INT16_MAX: 32767
    v128.const i16x8 -32768 32767 -1 1 0 -32768 32767 -1
    ;; Apply i16x8.neg - INT16_MIN (-32768) wraps to itself due to overflow
    i16x8.neg
  )

  ;; Test i16x8.neg with mixed sign pattern
  ;; Input: [-1000, 2000, -30, 4, 0, -12345, 6789, -9]
  ;; Expected result: [1000, -2000, 30, -4, 0, 12345, -6789, 9]
  (func (export "test_i16x8_neg_mixed_pattern") (result v128)
    ;; Create v128 with mixed sign pattern across all lanes
    v128.const i16x8 -1000 2000 -30 4 0 -12345 6789 -9
    ;; Apply i16x8.neg to mixed pattern
    i16x8.neg
  )

  ;; Test i16x8.neg with all zero values
  ;; Input: [0, 0, 0, 0, 0, 0, 0, 0]
  ;; Expected result: [0, 0, 0, 0, 0, 0, 0, 0] (negation of zero is zero)
  (func (export "test_i16x8_neg_zero") (result v128)
    ;; Create v128 with all zeros
    v128.const i16x8 0 0 0 0 0 0 0 0
    ;; Apply i16x8.neg to zeros (should remain zero)
    i16x8.neg
  )

  ;; Test lane independence: only lane 0 has non-zero value
  ;; Input: [100, 0, 0, 0, 0, 0, 0, 0]
  ;; Expected result: [-100, 0, 0, 0, 0, 0, 0, 0]
  (func (export "test_i16x8_neg_lane0_independence") (result v128)
    ;; Create v128 with value only in lane 0
    v128.const i16x8 100 0 0 0 0 0 0 0
    ;; Apply i16x8.neg
    i16x8.neg
  )

  ;; Test lane independence: only lane 1 has non-zero value
  ;; Input: [0, -200, 0, 0, 0, 0, 0, 0]
  ;; Expected result: [0, 200, 0, 0, 0, 0, 0, 0]
  (func (export "test_i16x8_neg_lane1_independence") (result v128)
    ;; Create v128 with value only in lane 1
    v128.const i16x8 0 -200 0 0 0 0 0 0
    ;; Apply i16x8.neg
    i16x8.neg
  )

  ;; Lane extraction helper function
  ;; Extracts specific lane value for validation
  (func (export "extract_lane") (param $vector v128) (param $lane_index i32) (result i32)
    (local $result i32)

    ;; Extract lane based on index
    (local.set $result
      (block (result i32)
        (block
          (block
            (block
              (block
                (block
                  (block
                    (block
                      (block
                        ;; Check lane index and branch accordingly
                        (br_table 0 1 2 3 4 5 6 7 (local.get $lane_index))
                      ) ;; Default case (lane 7)
                      (br 7 (i16x8.extract_lane_s 7 (local.get $vector)))
                    ) ;; Lane 6
                    (br 6 (i16x8.extract_lane_s 6 (local.get $vector)))
                  ) ;; Lane 5
                  (br 5 (i16x8.extract_lane_s 5 (local.get $vector)))
                ) ;; Lane 4
                (br 4 (i16x8.extract_lane_s 4 (local.get $vector)))
              ) ;; Lane 3
              (br 3 (i16x8.extract_lane_s 3 (local.get $vector)))
            ) ;; Lane 2
            (br 2 (i16x8.extract_lane_s 2 (local.get $vector)))
          ) ;; Lane 1
          (br 1 (i16x8.extract_lane_s 1 (local.get $vector)))
        ) ;; Lane 0
        (i16x8.extract_lane_s 0 (local.get $vector))
      )
    )

    (local.get $result)
  )
)