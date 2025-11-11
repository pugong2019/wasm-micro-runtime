;; Enhanced test module for i16x8.ge_s SIMD opcode
;; Tests signed greater-than-or-equal comparison on 8 x 16-bit integer lanes

(module
  ;; Test function for basic i16x8.ge_s functionality
  ;; Takes two v128 vectors, performs i16x8.ge_s, returns result
  (func (export "test_i16x8_ge_s_basic") (param $a0 i32) (param $a1 i32) (param $a2 i32) (param $a3 i32)
                                          (param $a4 i32) (param $a5 i32) (param $a6 i32) (param $a7 i32)
                                          (param $b0 i32) (param $b1 i32) (param $b2 i32) (param $b3 i32)
                                          (param $b4 i32) (param $b5 i32) (param $b6 i32) (param $b7 i32)
                                          (result i32 i32 i32 i32 i32 i32 i32 i32)
    ;; Local variables to hold the vectors and result
    (local $vec_a v128)
    (local $vec_b v128)
    (local $result v128)

    ;; Build vector A: [a0, a1, a2, a3, a4, a5, a6, a7] as i16x8
    ;; Start with splat and replace each lane
    local.get $a0
    i16x8.splat
    local.get $a1
    i16x8.replace_lane 1
    local.get $a2
    i16x8.replace_lane 2
    local.get $a3
    i16x8.replace_lane 3
    local.get $a4
    i16x8.replace_lane 4
    local.get $a5
    i16x8.replace_lane 5
    local.get $a6
    i16x8.replace_lane 6
    local.get $a7
    i16x8.replace_lane 7
    local.set $vec_a

    ;; Build vector B: [b0, b1, b2, b3, b4, b5, b6, b7] as i16x8
    local.get $b0
    i16x8.splat
    local.get $b1
    i16x8.replace_lane 1
    local.get $b2
    i16x8.replace_lane 2
    local.get $b3
    i16x8.replace_lane 3
    local.get $b4
    i16x8.replace_lane 4
    local.get $b5
    i16x8.replace_lane 5
    local.get $b6
    i16x8.replace_lane 6
    local.get $b7
    i16x8.replace_lane 7
    local.set $vec_b

    ;; Perform i16x8.ge_s comparison: vec_a >= vec_b (signed)
    local.get $vec_a
    local.get $vec_b
    i16x8.ge_s
    local.set $result

    ;; Extract result lanes and return them as 8 i32 values
    local.get $result
    i16x8.extract_lane_s 0
    local.get $result
    i16x8.extract_lane_s 1
    local.get $result
    i16x8.extract_lane_s 2
    local.get $result
    i16x8.extract_lane_s 3
    local.get $result
    i16x8.extract_lane_s 4
    local.get $result
    i16x8.extract_lane_s 5
    local.get $result
    i16x8.extract_lane_s 6
    local.get $result
    i16x8.extract_lane_s 7
  )

  ;; Additional test function for edge cases
  (func (export "test_i16x8_ge_s_edge_cases") (param $a0 i32) (param $a1 i32) (param $a2 i32) (param $a3 i32)
                                              (param $a4 i32) (param $a5 i32) (param $a6 i32) (param $a7 i32)
                                              (param $b0 i32) (param $b1 i32) (param $b2 i32) (param $b3 i32)
                                              (param $b4 i32) (param $b5 i32) (param $b6 i32) (param $b7 i32)
                                              (result i32 i32 i32 i32 i32 i32 i32 i32)
    ;; Local variables
    (local $vec_a v128)
    (local $vec_b v128)
    (local $result v128)

    ;; Build vector A
    local.get $a0
    i16x8.splat
    local.get $a1
    i16x8.replace_lane 1
    local.get $a2
    i16x8.replace_lane 2
    local.get $a3
    i16x8.replace_lane 3
    local.get $a4
    i16x8.replace_lane 4
    local.get $a5
    i16x8.replace_lane 5
    local.get $a6
    i16x8.replace_lane 6
    local.get $a7
    i16x8.replace_lane 7
    local.set $vec_a

    ;; Build vector B
    local.get $b0
    i16x8.splat
    local.get $b1
    i16x8.replace_lane 1
    local.get $b2
    i16x8.replace_lane 2
    local.get $b3
    i16x8.replace_lane 3
    local.get $b4
    i16x8.replace_lane 4
    local.get $b5
    i16x8.replace_lane 5
    local.get $b6
    i16x8.replace_lane 6
    local.get $b7
    i16x8.replace_lane 7
    local.set $vec_b

    ;; Perform i16x8.ge_s comparison
    local.get $vec_a
    local.get $vec_b
    i16x8.ge_s
    local.set $result

    ;; Extract and return result lanes
    local.get $result
    i16x8.extract_lane_s 0
    local.get $result
    i16x8.extract_lane_s 1
    local.get $result
    i16x8.extract_lane_s 2
    local.get $result
    i16x8.extract_lane_s 3
    local.get $result
    i16x8.extract_lane_s 4
    local.get $result
    i16x8.extract_lane_s 5
    local.get $result
    i16x8.extract_lane_s 6
    local.get $result
    i16x8.extract_lane_s 7
  )

  ;; Memory exports for potential future use
  (memory (export "memory") 1)
)