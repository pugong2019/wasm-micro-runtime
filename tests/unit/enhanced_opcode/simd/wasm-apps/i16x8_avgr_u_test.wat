(module
  ;; Test function for i16x8.avgr_u opcode validation
  ;; Takes 16 individual i32 values representing two input vector lanes (8 each)
  ;; Returns 8 i32 values representing the averaged result vector lanes

  (func (export "test_i16x8_avgr_u")
    ;; First vector parameters (vec_a)
    (param $vec_a_lane0 i32) (param $vec_a_lane1 i32) (param $vec_a_lane2 i32) (param $vec_a_lane3 i32)
    (param $vec_a_lane4 i32) (param $vec_a_lane5 i32) (param $vec_a_lane6 i32) (param $vec_a_lane7 i32)

    ;; Second vector parameters (vec_b)
    (param $vec_b_lane0 i32) (param $vec_b_lane1 i32) (param $vec_b_lane2 i32) (param $vec_b_lane3 i32)
    (param $vec_b_lane4 i32) (param $vec_b_lane5 i32) (param $vec_b_lane6 i32) (param $vec_b_lane7 i32)

    ;; Unused parameters for compatibility
    (param $unused1 i32) (param $unused2 i32)

    ;; Return 8 i32 values representing result lanes
    (result i32 i32 i32 i32 i32 i32 i32 i32)

    (local $vec_a v128)
    (local $vec_b v128)
    (local $result v128)

    ;; Construct first input vector (vec_a) from individual lane parameters
    (local.set $vec_a
      (i16x8.replace_lane 7
        (i16x8.replace_lane 6
          (i16x8.replace_lane 5
            (i16x8.replace_lane 4
              (i16x8.replace_lane 3
                (i16x8.replace_lane 2
                  (i16x8.replace_lane 1
                    (i16x8.splat (local.get $vec_a_lane0))
                    (local.get $vec_a_lane1))
                  (local.get $vec_a_lane2))
                (local.get $vec_a_lane3))
              (local.get $vec_a_lane4))
            (local.get $vec_a_lane5))
          (local.get $vec_a_lane6))
        (local.get $vec_a_lane7))
    )

    ;; Construct second input vector (vec_b) from individual lane parameters
    (local.set $vec_b
      (i16x8.replace_lane 7
        (i16x8.replace_lane 6
          (i16x8.replace_lane 5
            (i16x8.replace_lane 4
              (i16x8.replace_lane 3
                (i16x8.replace_lane 2
                  (i16x8.replace_lane 1
                    (i16x8.splat (local.get $vec_b_lane0))
                    (local.get $vec_b_lane1))
                  (local.get $vec_b_lane2))
                (local.get $vec_b_lane3))
              (local.get $vec_b_lane4))
            (local.get $vec_b_lane5))
          (local.get $vec_b_lane6))
        (local.get $vec_b_lane7))
    )

    ;; Perform i16x8.avgr_u operation: (vec_a[i] + vec_b[i] + 1) / 2
    (local.set $result
      (i16x8.avgr_u (local.get $vec_a) (local.get $vec_b))
    )

    ;; Extract all 8 lanes from result vector using unsigned extraction
    (i16x8.extract_lane_u 0 (local.get $result))   ;; lane 0
    (i16x8.extract_lane_u 1 (local.get $result))   ;; lane 1
    (i16x8.extract_lane_u 2 (local.get $result))   ;; lane 2
    (i16x8.extract_lane_u 3 (local.get $result))   ;; lane 3
    (i16x8.extract_lane_u 4 (local.get $result))   ;; lane 4
    (i16x8.extract_lane_u 5 (local.get $result))   ;; lane 5
    (i16x8.extract_lane_u 6 (local.get $result))   ;; lane 6
    (i16x8.extract_lane_u 7 (local.get $result))   ;; lane 7
  )

  ;; Additional test function for basic i16x8.avgr_u validation
  (func (export "test_basic_avgr_u")
    (param $vec_a v128) (param $vec_b v128)
    (result v128)
    (i16x8.avgr_u (local.get $vec_a) (local.get $vec_b))
  )

  ;; Test function for boundary values (0, 65535)
  (func (export "test_boundary_avgr_u")
    (result i32)
    ;; Test average of 0 and 65535: (0 + 65535 + 1) / 2 = 32768
    (i16x8.extract_lane_u 0
      (i16x8.avgr_u
        (i16x8.splat (i32.const 0))
        (i16x8.splat (i32.const 65535))
      )
    )
  )

  ;; Test function for maximum values (65535, 65535)
  (func (export "test_max_avgr_u")
    (result i32)
    ;; Test average of 65535 and 65535: (65535 + 65535 + 1) / 2 = 65535
    (i16x8.extract_lane_u 0
      (i16x8.avgr_u
        (i16x8.splat (i32.const 65535))
        (i16x8.splat (i32.const 65535))
      )
    )
  )

  ;; Test function for zero values (0, 0)
  (func (export "test_zero_avgr_u")
    (result i32)
    ;; Test average of 0 and 0: (0 + 0 + 1) / 2 = 0
    (i16x8.extract_lane_u 0
      (i16x8.avgr_u
        (i16x8.splat (i32.const 0))
        (i16x8.splat (i32.const 0))
      )
    )
  )

  ;; Test function for rounding behavior (odd sums)
  (func (export "test_rounding_avgr_u")
    (result i32)
    ;; Test average of 1 and 2: (1 + 2 + 1) / 2 = 2 (rounds up)
    (i16x8.extract_lane_u 0
      (i16x8.avgr_u
        (i16x8.splat (i32.const 1))
        (i16x8.splat (i32.const 2))
      )
    )
  )

  ;; Test function for typical mid-range values
  (func (export "test_midrange_avgr_u")
    (param $val_a i32) (param $val_b i32)
    (result i32)
    (i16x8.extract_lane_u 0
      (i16x8.avgr_u
        (i16x8.splat (local.get $val_a))
        (i16x8.splat (local.get $val_b))
      )
    )
  )

  ;; Test function for lane independence verification
  (func (export "test_lane_independence")
    (result v128)
    ;; Create alternating pattern to test lane independence
    (i16x8.avgr_u
      ;; vec_a: alternating 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000
      (v128.const i16x8 1000 2000 3000 4000 5000 6000 7000 8000)
      ;; vec_b: alternating 9000, 8000, 7000, 6000, 5000, 4000, 3000, 2000
      (v128.const i16x8 9000 8000 7000 6000 5000 4000 3000 2000)
    )
  )

  ;; Test function for overflow prevention (large values)
  (func (export "test_overflow_prevention")
    (result i32)
    ;; Test that (65534 + 65534 + 1) / 2 = 65534, not overflow
    (i16x8.extract_lane_u 0
      (i16x8.avgr_u
        (i16x8.splat (i32.const 65534))
        (i16x8.splat (i32.const 65534))
      )
    )
  )

  ;; Test function for mixed positive values
  (func (export "test_mixed_values")
    (param $val1_a i32) (param $val1_b i32) (param $val2_a i32) (param $val2_b i32)
    (result i32 i32)
    (local $result v128)
    (local.set $result
      (i16x8.avgr_u
        (v128.const i16x8 0 0 0 0 0 0 0 0)  ;; Initialize with zeros
        (v128.const i16x8 0 0 0 0 0 0 0 0)   ;; Initialize with zeros
      )
    )
    ;; Replace lanes with test values and perform operation
    (local.set $result
      (i16x8.avgr_u
        (i16x8.replace_lane 1
          (i16x8.replace_lane 0 (v128.const i16x8 0 0 0 0 0 0 0 0) (local.get $val1_a))
          (local.get $val2_a))
        (i16x8.replace_lane 1
          (i16x8.replace_lane 0 (v128.const i16x8 0 0 0 0 0 0 0 0) (local.get $val1_b))
          (local.get $val2_b))
      )
    )
    (i16x8.extract_lane_u 0 (local.get $result))
    (i16x8.extract_lane_u 1 (local.get $result))
  )
)