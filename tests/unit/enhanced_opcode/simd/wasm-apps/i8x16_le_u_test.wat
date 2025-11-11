;; i8x16_le_u_test.wat - WebAssembly Text Format for i8x16.le_u opcode testing

(module
  ;; Function: test_i8x16_le_u
  ;; Description: Performs i8x16.le_u operation on two input v128 vectors
  ;; Parameters: Eight i32 values representing two v128 inputs (4+4 for each 128-bit vector)
  ;; Returns: Four i32 values representing v128 result with unsigned less-than-or-equal mask
  (func $test_i8x16_le_u (export "test_i8x16_le_u")
    (param $in1_0 i32) (param $in1_1 i32) (param $in1_2 i32) (param $in1_3 i32)
    (param $in2_0 i32) (param $in2_1 i32) (param $in2_2 i32) (param $in2_3 i32)
    (result i32 i32 i32 i32)
    (local $vector1 v128)
    (local $vector2 v128)
    (local $result v128)

    ;; Create first v128 vector from four i32 values
    (v128.const i32x4 0 0 0 0)
    (local.get $in1_0)
    (i32x4.replace_lane 0)
    (local.get $in1_1)
    (i32x4.replace_lane 1)
    (local.get $in1_2)
    (i32x4.replace_lane 2)
    (local.get $in1_3)
    (i32x4.replace_lane 3)
    (local.set $vector1)

    ;; Create second v128 vector from four i32 values
    (v128.const i32x4 0 0 0 0)
    (local.get $in2_0)
    (i32x4.replace_lane 0)
    (local.get $in2_1)
    (i32x4.replace_lane 1)
    (local.get $in2_2)
    (i32x4.replace_lane 2)
    (local.get $in2_3)
    (i32x4.replace_lane 3)
    (local.set $vector2)

    ;; Perform i8x16.le_u operation
    (local.get $vector1)
    (local.get $vector2)
    (i8x16.le_u)
    (local.set $result)

    ;; Extract result as four i32 values
    (local.get $result)
    (i32x4.extract_lane 0)
    (local.get $result)
    (i32x4.extract_lane 1)
    (local.get $result)
    (i32x4.extract_lane 2)
    (local.get $result)
    (i32x4.extract_lane 3)
  )

  ;; Additional test functions for comprehensive validation

  ;; Function: test_i8x16_le_u_boundary
  ;; Description: Tests i8x16.le_u with boundary values (0x00, 0xFF)
  (func $test_i8x16_le_u_boundary (export "test_i8x16_le_u_boundary")
    (param $in1_0 i32) (param $in1_1 i32) (param $in1_2 i32) (param $in1_3 i32)
    (param $in2_0 i32) (param $in2_1 i32) (param $in2_2 i32) (param $in2_3 i32)
    (result i32 i32 i32 i32)
    (local $vector1 v128)
    (local $vector2 v128)
    (local $result v128)

    ;; Create vectors with boundary values
    (v128.const i32x4 0 0 0 0)
    (local.get $in1_0)
    (i32x4.replace_lane 0)
    (local.get $in1_1)
    (i32x4.replace_lane 1)
    (local.get $in1_2)
    (i32x4.replace_lane 2)
    (local.get $in1_3)
    (i32x4.replace_lane 3)
    (local.set $vector1)

    (v128.const i32x4 0 0 0 0)
    (local.get $in2_0)
    (i32x4.replace_lane 0)
    (local.get $in2_1)
    (i32x4.replace_lane 1)
    (local.get $in2_2)
    (i32x4.replace_lane 2)
    (local.get $in2_3)
    (i32x4.replace_lane 3)
    (local.set $vector2)

    ;; Perform i8x16.le_u operation with boundary values
    (local.get $vector1)
    (local.get $vector2)
    (i8x16.le_u)
    (local.set $result)

    ;; Extract result as four i32 values
    (local.get $result)
    (i32x4.extract_lane 0)
    (local.get $result)
    (i32x4.extract_lane 1)
    (local.get $result)
    (i32x4.extract_lane 2)
    (local.get $result)
    (i32x4.extract_lane 3)
  )

  ;; Function: test_i8x16_le_u_equal
  ;; Description: Tests i8x16.le_u with equal values (should all return true)
  (func $test_i8x16_le_u_equal (export "test_i8x16_le_u_equal")
    (param $in_0 i32) (param $in_1 i32) (param $in_2 i32) (param $in_3 i32)
    (result i32 i32 i32 i32)
    (local $vector v128)
    (local $result v128)

    ;; Create vector from i32 values
    (v128.const i32x4 0 0 0 0)
    (local.get $in_0)
    (i32x4.replace_lane 0)
    (local.get $in_1)
    (i32x4.replace_lane 1)
    (local.get $in_2)
    (i32x4.replace_lane 2)
    (local.get $in_3)
    (i32x4.replace_lane 3)
    (local.set $vector)

    ;; Compare vector with itself (all should be <= and return true)
    (local.get $vector)
    (local.get $vector)
    (i8x16.le_u)
    (local.set $result)

    ;; Extract result as four i32 values
    (local.get $result)
    (i32x4.extract_lane 0)
    (local.get $result)
    (i32x4.extract_lane 1)
    (local.get $result)
    (i32x4.extract_lane 2)
    (local.get $result)
    (i32x4.extract_lane 3)
  )

  ;; Function: test_i8x16_le_u_unsigned_behavior
  ;; Description: Tests i8x16.le_u with values that differ in signed vs unsigned interpretation
  (func $test_i8x16_le_u_unsigned_behavior (export "test_i8x16_le_u_unsigned_behavior")
    (param $in1_0 i32) (param $in1_1 i32) (param $in1_2 i32) (param $in1_3 i32)
    (param $in2_0 i32) (param $in2_1 i32) (param $in2_2 i32) (param $in2_3 i32)
    (result i32 i32 i32 i32)
    (local $vector1 v128)
    (local $vector2 v128)
    (local $result v128)

    ;; Create vectors targeting unsigned vs signed differences
    (v128.const i32x4 0 0 0 0)
    (local.get $in1_0)
    (i32x4.replace_lane 0)
    (local.get $in1_1)
    (i32x4.replace_lane 1)
    (local.get $in1_2)
    (i32x4.replace_lane 2)
    (local.get $in1_3)
    (i32x4.replace_lane 3)
    (local.set $vector1)

    (v128.const i32x4 0 0 0 0)
    (local.get $in2_0)
    (i32x4.replace_lane 0)
    (local.get $in2_1)
    (i32x4.replace_lane 1)
    (local.get $in2_2)
    (i32x4.replace_lane 2)
    (local.get $in2_3)
    (i32x4.replace_lane 3)
    (local.set $vector2)

    ;; Perform i8x16.le_u operation (unsigned comparison)
    (local.get $vector1)
    (local.get $vector2)
    (i8x16.le_u)
    (local.set $result)

    ;; Extract result as four i32 values
    (local.get $result)
    (i32x4.extract_lane 0)
    (local.get $result)
    (i32x4.extract_lane 1)
    (local.get $result)
    (i32x4.extract_lane 2)
    (local.get $result)
    (i32x4.extract_lane 3)
  )
)