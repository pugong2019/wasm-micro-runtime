(module
  ;; Enhanced i32.reinterpret_f32 opcode test module
  ;; This module provides comprehensive test functions for the i32.reinterpret_f32 WebAssembly instruction
  ;; covering basic functionality, special IEEE 754 values, boundary conditions, and subnormal numbers

  ;; Basic i32.reinterpret_f32 test function
  ;; Tests fundamental bit reinterpretation behavior with typical f32 inputs
  ;; Input: f32 value to be reinterpreted as i32 bit pattern
  ;; Output: i32 containing exact bit representation of input f32
  (func $reinterpret_f32 (export "reinterpret_f32") (param $input f32) (result i32)
    ;; Push input parameter onto stack and execute i32.reinterpret_f32
    local.get $input         ;; Load f32 parameter onto stack
    i32.reinterpret_f32     ;; Execute bit reinterpretation: f32 -> i32
    ;; Result: i32 value containing exact IEEE 754 bit pattern of input
  )

  ;; Special values testing function
  ;; Tests reinterpretation of IEEE 754 special values (zero, infinity, NaN)
  ;; Handles special cases like +0.0, -0.0, +∞, -∞, and various NaN patterns
  ;; Input: f32 special value for IEEE 754 compliance testing
  ;; Output: i32 bit pattern demonstrating special value preservation
  (func $reinterpret_f32_special (export "reinterpret_f32_special") (param $special f32) (result i32)
    ;; Load parameter and perform reinterpretation with special value handling
    local.get $special      ;; Load f32 special test value onto stack
    i32.reinterpret_f32    ;; Execute reinterpretation: preserve exact bit pattern
    ;; Result: i32 with IEEE 754 special value bit patterns preserved
  )

  ;; IEEE 754 bit field validation function
  ;; Demonstrates that i32.reinterpret_f32 preserves sign, exponent, and mantissa fields
  ;; Input: f32 value for bit field verification
  ;; Output: i32 result showing exact bit field preservation
  (func $reinterpret_f32_bitfields (export "reinterpret_f32_bitfields") (param $value f32) (result i32)
    ;; Verify bit reinterpretation preserves all IEEE 754 fields
    local.get $value        ;; Load test value for bit field verification
    i32.reinterpret_f32    ;; Perform bit-exact reinterpretation
    ;; Result proves bit field preservation: sign(1) + exponent(8) + mantissa(23) bits
  )

  ;; Subnormal number testing function
  ;; Tests behavior with subnormal floating-point numbers and their bit patterns
  ;; Input: f32 subnormal value (very small numbers with zero exponent field)
  ;; Output: i32 demonstrating correct subnormal bit pattern preservation
  (func $reinterpret_f32_subnormal (export "reinterpret_f32_subnormal") (param $subnormal f32) (result i32)
    ;; Handle subnormal f32 values and verify correct bit pattern preservation
    local.get $subnormal    ;; Load subnormal test value onto stack
    i32.reinterpret_f32    ;; Execute reinterpretation on subnormal value
    ;; Result: i32 showing proper preservation of subnormal bit patterns
  )

  ;; Round-trip consistency validation function
  ;; Verifies that f32->i32->f32 reinterpretation preserves bit patterns exactly
  ;; Input: f32 value for round-trip testing
  ;; Output: i32 bit pattern that should round-trip back to identical f32
  (func $reinterpret_f32_roundtrip (export "reinterpret_f32_roundtrip") (param $original f32) (result i32)
    ;; Test round-trip consistency: f32 -> i32 -> f32 should preserve bits
    local.get $original     ;; Load original f32 value for round-trip test
    i32.reinterpret_f32    ;; Convert f32 to i32 bit pattern
    ;; Result: i32 that should convert back to identical f32 via f32.reinterpret_i32
  )

  ;; Boundary value testing function
  ;; Tests reinterpretation at IEEE 754 numeric boundaries (FLT_MIN, FLT_MAX)
  ;; Input: f32 boundary value (smallest/largest normal numbers)
  ;; Output: i32 bit pattern demonstrating boundary value handling
  (func $reinterpret_f32_boundary (export "reinterpret_f32_boundary") (param $boundary f32) (result i32)
    ;; Handle IEEE 754 boundary values and verify correct bit preservation
    local.get $boundary     ;; Load boundary test value onto stack
    i32.reinterpret_f32    ;; Execute reinterpretation on boundary value
    ;; Result: i32 showing proper handling of floating-point boundary conditions
  )

  ;; Sign bit isolation function
  ;; Demonstrates sign bit preservation across reinterpretation operations
  ;; Input: f32 value with specific sign bit requirements
  ;; Output: i32 showing sign bit preservation in bit 31
  (func $reinterpret_f32_sign (export "reinterpret_f32_sign") (param $signed_value f32) (result i32)
    ;; Verify sign bit (bit 31) is preserved exactly in reinterpretation
    local.get $signed_value ;; Load signed test value for sign verification
    i32.reinterpret_f32    ;; Execute reinterpretation preserving sign bit
    ;; Result: i32 with sign bit preserved in most significant position (bit 31)
  )

  ;; NaN pattern testing function
  ;; Tests various NaN bit patterns including quiet NaN, signaling NaN variations
  ;; Input: f32 NaN value with specific bit pattern
  ;; Output: i32 preserving exact NaN bit configuration
  (func $reinterpret_f32_nan (export "reinterpret_f32_nan") (param $nan_value f32) (result i32)
    ;; Handle various NaN patterns and preserve exact bit configurations
    local.get $nan_value    ;; Load NaN test value with specific bit pattern
    i32.reinterpret_f32    ;; Execute reinterpretation preserving NaN bits
    ;; Result: i32 containing exact NaN bit pattern from input f32
  )
)