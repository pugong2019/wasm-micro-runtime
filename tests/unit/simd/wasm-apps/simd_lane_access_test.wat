;; SIMD Lane Access Test - Comprehensive test for SIMD lane extraction and replacement operations
(module
  ;; Memory for testing
  (memory 1)
  
  ;; Test functions for i8x16 lane operations
  (func $test_i8x16_extract_signed (export "test_i8x16_extract_signed") (result i32)
    (local $v v128)
    ;; Create vector [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
    (v128.const i8x16 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16)
    local.set $v
    ;; Extract lane 7 (should be 8)
    (i8x16.extract_lane_s 7 (local.get $v))
  )

  (func $test_i8x16_extract_lane_0 (export "test_i8x16_extract_lane_0") (result i32)
    (local $v v128)
    ;; Create vector [42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    (v128.const i8x16 42 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)
    local.set $v
    ;; Extract first lane
    (i8x16.extract_lane_s 0 (local.get $v))
  )

  (func $test_i8x16_extract_lane_15 (export "test_i8x16_extract_lane_15") (result i32)
    (local $v v128)
    ;; Create vector [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99]
    (v128.const i8x16 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 99)
    local.set $v
    ;; Extract last lane
    (i8x16.extract_lane_s 15 (local.get $v))
  )

  (func $test_i8x16_extract_unsigned (export "test_i8x16_extract_unsigned") (result i32)
    (local $v v128)
    ;; Create vector [255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240]
    (v128.const i8x16 255 254 253 252 251 250 249 248 247 246 245 244 243 242 241 240)
    local.set $v
    ;; Extract lane 0 (should be 255 unsigned)
    (i8x16.extract_lane_u 0 (local.get $v))
  )

  (func $test_i8x16_replace (export "test_i8x16_replace") (result v128)
    (local $v v128)
    ;; Create vector [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    (v128.const i8x16 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)
    local.set $v
    ;; Replace lane 5 with 42
    (i8x16.replace_lane 5 (local.get $v) (i32.const 42))
  )

  ;; Test functions for i16x8 lane operations
  (func $test_i16x8_extract_signed (export "test_i16x8_extract_signed") (result i32)
    (local $v v128)
    ;; Create vector [1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000]
    (v128.const i16x8 1000 2000 3000 4000 5000 6000 7000 8000)
    local.set $v
    ;; Extract lane 3 (should be 4000)
    (i16x8.extract_lane_s 3 (local.get $v))
  )

  (func $test_i16x8_extract_lane_0 (export "test_i16x8_extract_lane_0") (result i32)
    (local $v v128)
    ;; Create vector [12345, 0, 0, 0, 0, 0, 0, 0]
    (v128.const i16x8 12345 0 0 0 0 0 0 0)
    local.set $v
    ;; Extract first lane
    (i16x8.extract_lane_s 0 (local.get $v))
  )

  (func $test_i16x8_extract_lane_7 (export "test_i16x8_extract_lane_7") (result i32)
    (local $v v128)
    ;; Create vector [0, 0, 0, 0, 0, 0, 0, 54321]
    (v128.const i16x8 0 0 0 0 0 0 0 54321)
    local.set $v
    ;; Extract last lane
    (i16x8.extract_lane_s 7 (local.get $v))
  )

  (func $test_i16x8_extract_unsigned (export "test_i16x8_extract_unsigned") (result i32)
    (local $v v128)
    ;; Create vector [65535, 65534, 65533, 65532, 65531, 65530, 65529, 65528]
    (v128.const i16x8 65535 65534 65533 65532 65531 65530 65529 65528)
    local.set $v
    ;; Extract lane 0 (should be 65535 unsigned)
    (i16x8.extract_lane_u 0 (local.get $v))
  )

  (func $test_i16x8_replace (export "test_i16x8_replace") (result v128)
    (local $v v128)
    ;; Create vector [0, 0, 0, 0, 0, 0, 0, 0]
    (v128.const i16x8 0 0 0 0 0 0 0 0)
    local.set $v
    ;; Replace lane 2 with 12345
    (i16x8.replace_lane 2 (local.get $v) (i32.const 12345))
  )

  ;; Test functions for i32x4 lane operations
  (func $test_i32x4_extract (export "test_i32x4_extract") (result i32)
    (local $v v128)
    ;; Create vector [100000, 200000, 300000, 400000]
    (v128.const i32x4 100000 200000 300000 400000)
    local.set $v
    ;; Extract lane 1 (should be 200000)
    (i32x4.extract_lane 1 (local.get $v))
  )

  (func $test_i32x4_replace (export "test_i32x4_replace") (result v128)
    (local $v v128)
    ;; Create vector [0, 0, 0, 0]
    (v128.const i32x4 0 0 0 0)
    local.set $v
    ;; Replace lane 3 with 999999
    (i32x4.replace_lane 3 (local.get $v) (i32.const 999999))
  )

  ;; Test functions for i64x2 lane operations
  (func $test_i64x2_extract (export "test_i64x2_extract") (result i64)
    (local $v v128)
    ;; Create vector [123456789012345, 987654321098765]
    (v128.const i64x2 123456789012345 987654321098765)
    local.set $v
    ;; Extract lane 0 (should be 123456789012345)
    (i64x2.extract_lane 0 (local.get $v))
  )

  (func $test_i64x2_replace (export "test_i64x2_replace") (result v128)
    (local $v v128)
    ;; Create vector [0, 0]
    (v128.const i64x2 0 0)
    local.set $v
    ;; Replace lane 1 with 555555555555555
    (i64x2.replace_lane 1 (local.get $v) (i64.const 555555555555555))
  )

  ;; Test functions for f32x4 lane operations
  (func $test_f32x4_extract (export "test_f32x4_extract") (result f32)
    (local $v v128)
    ;; Create vector [1.5, 2.5, 3.5, 4.5]
    (v128.const f32x4 1.5 2.5 3.5 4.5)
    local.set $v
    ;; Extract lane 2 (should be 3.5)
    (f32x4.extract_lane 2 (local.get $v))
  )

  (func $test_f32x4_zero (export "test_f32x4_zero") (result f32)
    (local $v v128)
    ;; Create vector [0.0, 0.0, 0.0, 0.0]
    (v128.const f32x4 0.0 0.0 0.0 0.0)
    local.set $v
    ;; Extract first lane (should be 0.0)
    (f32x4.extract_lane 0 (local.get $v))
  )

  (func $test_f32x4_replace (export "test_f32x4_replace") (result v128)
    (local $v v128)
    ;; Create vector [0.0, 0.0, 0.0, 0.0]
    (v128.const f32x4 0.0 0.0 0.0 0.0)
    local.set $v
    ;; Replace lane 0 with 7.5
    (f32x4.replace_lane 0 (local.get $v) (f32.const 7.5))
  )

  ;; Test functions for f64x2 lane operations
  (func $test_f64x2_extract (export "test_f64x2_extract") (result f64)
    (local $v v128)
    ;; Create vector [1.23456789, 9.87654321]
    (v128.const f64x2 1.23456789 9.87654321)
    local.set $v
    ;; Extract lane 1 (should be 9.87654321)
    (f64x2.extract_lane 1 (local.get $v))
  )

  (func $test_f64x2_replace (export "test_f64x2_replace") (result v128)
    (local $v v128)
    ;; Create vector [0.0, 0.0]
    (v128.const f64x2 0.0 0.0)
    local.set $v
    ;; Replace lane 0 with 3.14159
    (f64x2.replace_lane 0 (local.get $v) (f64.const 3.14159))
  )

  ;; Test shuffle operations
  (func $test_shuffle_identity (export "test_shuffle_identity") (result v128)
    (local $v1 v128)
    (local $v2 v128)
    ;; Create two vectors
    (v128.const i8x16 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16)
    local.set $v1
    (v128.const i8x16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32)
    local.set $v2
    ;; Identity shuffle - take all from first vector
    (i8x16.shuffle 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 (local.get $v1) (local.get $v2))
  )

  (func $test_shuffle_reverse (export "test_shuffle_reverse") (result v128)
    (local $v1 v128)
    (local $v2 v128)
    ;; Create two vectors
    (v128.const i8x16 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16)
    local.set $v1
    (v128.const i8x16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32)
    local.set $v2
    ;; Reverse shuffle
    (i8x16.shuffle 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 (local.get $v1) (local.get $v2))
  )

  (func $test_shuffle_interleave (export "test_shuffle_interleave") (result v128)
    (local $v1 v128)
    (local $v2 v128)
    ;; Create two vectors
    (v128.const i8x16 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16)
    local.set $v1
    (v128.const i8x16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32)
    local.set $v2
    ;; Interleave elements from both vectors
    (i8x16.shuffle 0 16 1 17 2 18 3 19 4 20 5 21 6 22 7 23 (local.get $v1) (local.get $v2))
  )

  ;; Test swizzle operations
  (func $test_swizzle_basic (export "test_swizzle_basic") (result v128)
    (local $vector v128)
    (local $mask v128)
    ;; Create vector [10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160]
    (v128.const i8x16 10 20 30 40 50 60 70 80 90 100 110 120 130 140 150 160)
    local.set $vector
    ;; Create mask [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15] (identity)
    (v128.const i8x16 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15)
    local.set $mask
    ;; Apply swizzle
    (i8x16.swizzle (local.get $vector) (local.get $mask))
  )

  (func $test_swizzle_out_of_range (export "test_swizzle_out_of_range") (result v128)
    (local $vector v128)
    (local $mask v128)
    ;; Create vector [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
    (v128.const i8x16 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16)
    local.set $vector
    ;; Create mask with out-of-range indices [16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    (v128.const i8x16 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31)
    local.set $mask
    ;; Apply swizzle with out-of-range indices (should return zeros for out-of-range)
    (i8x16.swizzle (local.get $vector) (local.get $mask))
  )

  ;; Test stack operations
  (func $test_stack_operations (export "test_stack_operations") (result i32)
    (local $v1 v128)
    (local $v2 v128)
    ;; Create vectors and test stack operations
    (v128.const i8x16 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16)
    local.set $v1
    (v128.const i8x16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32)
    local.set $v2
    ;; Test shuffle operation that uses stack
    (i8x16.shuffle 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 (local.get $v1) (local.get $v2))
    drop
    ;; Return success
    (i32.const 1)
  )

  ;; Test boundary conditions
  (func $test_extract_first_lane (export "test_extract_first_lane") (result i32)
    (local $v v128)
    ;; Create vector [42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    (v128.const i8x16 42 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)
    local.set $v
    ;; Extract first lane
    (i8x16.extract_lane_s 0 (local.get $v))
  )

  (func $test_extract_last_lane (export "test_extract_last_lane") (result i32)
    (local $v v128)
    ;; Create vector [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99]
    (v128.const i8x16 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 99)
    local.set $v
    ;; Extract last lane
    (i8x16.extract_lane_s 15 (local.get $v))
  )

  (func $test_replace_first_lane (export "test_replace_first_lane") (result v128)
    (local $v v128)
    ;; Create vector [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    (v128.const i8x16 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)
    local.set $v
    ;; Replace first lane with 77
    (i8x16.replace_lane 0 (local.get $v) (i32.const 77))
  )

  (func $test_replace_last_lane (export "test_replace_last_lane") (result v128)
    (local $v v128)
    ;; Create vector [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    (v128.const i8x16 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)
    local.set $v
    ;; Replace last lane with 88
    (i8x16.replace_lane 15 (local.get $v) (i32.const 88))
  )
  
  ;; Additional swizzle tests for coverage
  (func $test_swizzle_mixed_indices (export "test_swizzle_mixed_indices") (result v128)
    (local $vector v128)
    (local $mask v128)
    ;; Create vector [10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160]
    (v128.const i8x16 10 20 30 40 50 60 70 80 90 100 110 120 130 140 150 160)
    local.set $vector
    ;; Create mask with mixed valid and invalid indices
    (v128.const i8x16 7 15 31 16 0 8 24 11 5 9 19 1 3 12 28 14)
    local.set $mask
    ;; Apply swizzle with mixed indices
    (i8x16.swizzle (local.get $vector) (local.get $mask))
  )
  
  (func $test_swizzle_i16x8 (export "test_swizzle_i16x8") (result v128)
    (local $vector v128)
    (local $mask v128)
    ;; Create vector [100, 200, 300, 400, 500, 600, 700, 800]
    (v128.const i16x8 100 200 300 400 500 600 700 800)
    local.set $vector
    ;; Create mask for i16x8 swizzle
    (v128.const i8x16 0 0 1 1 2 2 3 3 4 4 5 5 6 6 7 7)
    local.set $mask
    ;; Apply swizzle
    (i16x8.swizzle (local.get $vector) (local.get $mask))
  )
  
  (func $test_swizzle_i32x4 (export "test_swizzle_i32x4") (result v128)
    (local $vector v128)
    (local $mask v128)
    ;; Create vector [1000, 2000, 3000, 4000]
    (v128.const i32x4 1000 2000 3000 4000)
    local.set $vector
    ;; Create mask for i32x4 swizzle
    (v128.const i8x16 0 0 0 0 1 1 1 1 2 2 2 2 3 3 3 3)
    local.set $mask
    ;; Apply swizzle
    (i32x4.swizzle (local.get $vector) (local.get $mask))
  )
  
  (func $test_swizzle_f32x4 (export "test_swizzle_f32x4") (result v128)
    (local $vector v128)
    (local $mask v128)
    ;; Create vector [1.1, 2.2, 3.3, 4.4]
    (v128.const f32x4 1.1 2.2 3.3 4.4)
    local.set $vector
    ;; Create mask for f32x4 swizzle
    (v128.const i8x16 0 0 0 0 1 1 1 1 2 2 2 2 3 3 3 3)
    local.set $mask
    ;; Apply swizzle
    (f32x4.swizzle (local.get $vector) (local.get $mask))
  )
  
  ;; Tests for signed/unsigned extension
  (func $test_extract_signed_negative (export "test_extract_signed_negative") (result i32)
    (local $v v128)
    ;; Create vector with negative values
    (v128.const i8x16 -1 -2 -3 -4 -5 -6 -7 -8 -9 -10 -11 -12 -13 -14 -15 -16)
    local.set $v
    ;; Extract lane 0 (should be -1, extended with sign)
    (i8x16.extract_lane_s 0 (local.get $v))
  )
  
  (func $test_extract_unsigned_negative (export "test_extract_unsigned_negative") (result i32)
    (local $v v128)
    ;; Create vector with negative values (which become large unsigned values)
    (v128.const i8x16 -1 -2 -3 -4 -5 -6 -7 -8 -9 -10 -11 -12 -13 -14 -15 -16)
    local.set $v
    ;; Extract lane 0 as unsigned (should be 255)
    (i8x16.extract_lane_u 0 (local.get $v))
  )
  
  (func $test_i16x8_extract_signed_negative (export "test_i16x8_extract_signed_negative") (result i32)
    (local $v v128)
    ;; Create vector with negative values
    (v128.const i16x8 -1000 -2000 -3000 -4000 -5000 -6000 -7000 -8000)
    local.set $v
    ;; Extract lane 3 (should be -4000, extended with sign)
    (i16x8.extract_lane_s 3 (local.get $v))
  )
  
  (func $test_i16x8_extract_unsigned_negative (export "test_i16x8_extract_unsigned_negative") (result i32)
    (local $v v128)
    ;; Create vector with negative values (which become large unsigned values)
    (v128.const i16x8 -1000 -2000 -3000 -4000 -5000 -6000 -7000 -8000)
    local.set $v
    ;; Extract lane 3 as unsigned (should be 61536 for -4000)
    (i16x8.extract_lane_u 3 (local.get $v))
  )
  
  ;; Tests for truncation operations
  (func $test_replace_truncate_i32_to_i8 (export "test_replace_truncate_i32_to_i8") (result v128)
    (local $v v128)
    ;; Create vector of zeros
    (v128.const i8x16 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)
    local.set $v
    ;; Replace lane with value that will be truncated
    (i8x16.replace_lane 0 (local.get $v) (i32.const 300)) ;; 300 truncates to 44 (0x12C -> 0x2C)
  )
  
  (func $test_replace_truncate_i32_to_i16 (export "test_replace_truncate_i32_to_i16") (result v128)
    (local $v v128)
    ;; Create vector of zeros
    (v128.const i16x8 0 0 0 0 0 0 0 0)
    local.set $v
    ;; Replace lane with value that will be truncated
    (i16x8.replace_lane 0 (local.get $v) (i32.const 70000)) ;; 70000 truncates to 4464 (0x11170 -> 0x1170)
  )
  
  (func $test_replace_truncate_negative (export "test_replace_truncate_negative") (result v128)
    (local $v v128)
    ;; Create vector of zeros
    (v128.const i8x16 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)
    local.set $v
    ;; Replace lane with negative value that will be truncated
    (i8x16.replace_lane 0 (local.get $v) (i32.const -257)) ;; -257 truncates to -1 (0xFF01 -> 0xFF)
  )
  
  ;; Edge case for swizzle with all invalid indices
  (func $test_swizzle_all_invalid (export "test_swizzle_all_invalid") (result v128)
    (local $vector v128)
    (local $mask v128)
    ;; Create vector
    (v128.const i8x16 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16)
    local.set $vector
    ;; Create mask with all invalid indices
    (v128.const i8x16 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31)
    local.set $mask
    ;; Apply swizzle (should return all zeros)
    (i8x16.swizzle (local.get $vector) (local.get $mask))
  )
  
  ;; Edge case for swizzle with zero vector
  (func $test_swizzle_zero_vector (export "test_swizzle_zero_vector") (result v128)
    (local $vector v128)
    (local $mask v128)
    ;; Create zero vector
    (v128.const i8x16 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)
    local.set $vector
    ;; Create mask
    (v128.const i8x16 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15)
    local.set $mask
    ;; Apply swizzle
    (i8x16.swizzle (local.get $vector) (local.get $mask))
  )
)