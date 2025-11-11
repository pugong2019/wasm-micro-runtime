(module
  ;; Import memory for test data if needed
  (memory 1)

  ;; Export memory for external access
  (export "memory" (memory 0))

  ;; Main test function for i32x4.neg operation
  ;; Takes four i32 parameters and returns v128 result of i32x4.neg
  ;; Used by: BasicNegation_TypicalValues_ReturnsCorrectNegation
  ;;          BoundaryValues_IntegerLimits_HandlesOverflow
  ;;          PowersOfTwo_BitPatterns_ReturnsCorrectNegation
  ;;          LaneIndependence_MixedValues_IndependentNegation
  (func $test_i32x4_neg_basic (param $a i32) (param $b i32) (param $c i32) (param $d i32) (result v128)
    ;; Create v128 from four i32 values using i32x4.splat and i32x4.replace_lane
    local.get $a
    i32x4.splat
    local.get $b
    i32x4.replace_lane 1
    local.get $c
    i32x4.replace_lane 2
    local.get $d
    i32x4.replace_lane 3
    ;; Apply i32x4.neg to the constructed vector
    i32x4.neg
  )
  (export "test_i32x4_neg_basic" (func $test_i32x4_neg_basic))

  ;; Double negation test function: neg(neg(vector)) should return original
  ;; Verifies that double negation preserves original values (identity property)
  ;; Used by: DoubleNegation_Identity_RestoresOriginalValues
  (func $test_i32x4_neg_double (param $a i32) (param $b i32) (param $c i32) (param $d i32) (result v128)
    ;; Create v128 from four i32 values
    local.get $a
    i32x4.splat
    local.get $b
    i32x4.replace_lane 1
    local.get $c
    i32x4.replace_lane 2
    local.get $d
    i32x4.replace_lane 3
    ;; Apply double negation: neg(neg(x)) = x
    i32x4.neg
    i32x4.neg
  )
  (export "test_i32x4_neg_double" (func $test_i32x4_neg_double))

  ;; Test function for INT32_MAX boundary value
  ;; Used by: BoundaryValues_IntegerLimits_HandlesOverflow
  (func $test_i32x4_neg_max (result v128)
    i32.const 0x7FFFFFFF  ;; INT32_MAX (2147483647)
    i32x4.splat
    i32x4.neg  ;; Should become -2147483647
  )
  (export "test_i32x4_neg_max" (func $test_i32x4_neg_max))

  ;; Test function for INT32_MIN boundary value (overflow case)
  ;; Used by: BoundaryValues_IntegerLimits_HandlesOverflow
  (func $test_i32x4_neg_min (result v128)
    i32.const 0x80000000  ;; INT32_MIN (-2147483648)
    i32x4.splat
    i32x4.neg  ;; Overflow: should remain INT32_MIN
  )
  (export "test_i32x4_neg_min" (func $test_i32x4_neg_min))

  ;; Test function for zero value
  ;; Used by: SpecialValues_ZeroAndSignedOnes_ReturnsCorrectNegation
  (func $test_i32x4_neg_zero (result v128)
    i32.const 0
    i32x4.splat
    i32x4.neg  ;; Should remain 0
  )
  (export "test_i32x4_neg_zero" (func $test_i32x4_neg_zero))

  ;; Test function for negative one value
  ;; Used by: SpecialValues_ZeroAndSignedOnes_ReturnsCorrectNegation
  (func $test_i32x4_neg_neg_one (result v128)
    i32.const -1
    i32x4.splat
    i32x4.neg  ;; Should become 1
  )
  (export "test_i32x4_neg_neg_one" (func $test_i32x4_neg_neg_one))

  ;; Test function for positive one value
  ;; Used by: SpecialValues_ZeroAndSignedOnes_ReturnsCorrectNegation
  (func $test_i32x4_neg_one (result v128)
    i32.const 1
    i32x4.splat
    i32x4.neg  ;; Should become -1
  )
  (export "test_i32x4_neg_one" (func $test_i32x4_neg_one))

  ;; Test function for positive constant value (42)
  (func $test_i32x4_neg_const_42 (result v128)
    i32.const 42
    i32x4.splat
    i32x4.neg  ;; Should become -42 in all lanes
  )
  (export "test_i32x4_neg_const_42" (func $test_i32x4_neg_const_42))

  ;; Test function for negative constant value (-100)
  (func $test_i32x4_neg_const_neg100 (result v128)
    i32.const -100
    i32x4.splat
    i32x4.neg  ;; Should become 100 in all lanes
  )
  (export "test_i32x4_neg_const_neg100" (func $test_i32x4_neg_const_neg100))

  ;; Test function for power of 2 value (1024)
  (func $test_i32x4_neg_power2_1024 (result v128)
    i32.const 1024  ;; 2^10
    i32x4.splat
    i32x4.neg  ;; Should become -1024 in all lanes
  )
  (export "test_i32x4_neg_power2_1024" (func $test_i32x4_neg_power2_1024))

  ;; Test function for larger power of 2 (2048)
  (func $test_i32x4_neg_power2_2048 (result v128)
    i32.const 2048  ;; 2^11
    i32x4.splat
    i32x4.neg  ;; Should become -2048 in all lanes
  )
  (export "test_i32x4_neg_power2_2048" (func $test_i32x4_neg_power2_2048))

  ;; Test function for very large power of 2 (1048576)
  (func $test_i32x4_neg_power2_large (result v128)
    i32.const 1048576  ;; 2^20 (1 << 20)
    i32x4.splat
    i32x4.neg  ;; Should become -1048576 in all lanes
  )
  (export "test_i32x4_neg_power2_large" (func $test_i32x4_neg_power2_large))

  ;; Test function demonstrating chained SIMD operations
  ;; Negation followed by additional SIMD operations to verify compatibility
  (func $test_i32x4_neg_chain (param $value i32) (result v128)
    local.get $value
    i32x4.splat
    i32x4.neg
    ;; Additional SIMD operations can be added here for comprehensive testing
    ;; For now, just return the negated result
  )
  (export "test_i32x4_neg_chain" (func $test_i32x4_neg_chain))

  ;; Test function with multiple negation operations and vector operations
  (func $test_i32x4_neg_multiple (param $a i32) (param $b i32) (result v128)
    ;; Create two vectors, negate both, and perform v128.or
    local.get $a
    i32x4.splat
    i32x4.neg
    local.get $b
    i32x4.splat
    i32x4.neg
    v128.or  ;; Bitwise OR of two negated vectors
  )
  (export "test_i32x4_neg_multiple" (func $test_i32x4_neg_multiple))

  ;; Test function for mixed lane values with different signs
  ;; Demonstrates lane-independent negation
  (func $test_i32x4_neg_mixed_lanes (result v128)
    ;; Create vector [100, -200, 300, -400]
    i32.const 100
    i32x4.splat
    i32.const -200
    i32x4.replace_lane 1
    i32.const 300
    i32x4.replace_lane 2
    i32.const -400
    i32x4.replace_lane 3
    ;; Apply negation: should become [-100, 200, -300, 400]
    i32x4.neg
  )
  (export "test_i32x4_neg_mixed_lanes" (func $test_i32x4_neg_mixed_lanes))

  ;; Test function for alternating positive/negative pattern
  ;; Used for lane independence validation
  (func $test_i32x4_neg_alternating (result v128)
    ;; Create vector [555, -666, 777, -888]
    i32.const 555
    i32x4.splat
    i32.const -666
    i32x4.replace_lane 1
    i32.const 777
    i32x4.replace_lane 2
    i32.const -888
    i32x4.replace_lane 3
    ;; Apply negation: should become [-555, 666, -777, 888]
    i32x4.neg
  )
  (export "test_i32x4_neg_alternating" (func $test_i32x4_neg_alternating))

  ;; Test function demonstrating negation preserves vector structure
  ;; Utility function for comprehensive SIMD operation testing
  (func $test_i32x4_neg_preserve_structure (param $value i32) (result i32)
    ;; Create vector, negate it, then extract first lane
    ;; This demonstrates that negation preserves the v128 structure
    local.get $value
    i32x4.splat
    i32x4.neg
    i32x4.extract_lane 0  ;; Extract negated value from lane 0
  )
  (export "test_i32x4_neg_preserve_structure" (func $test_i32x4_neg_preserve_structure))

  ;; Test function for extreme value differences in lanes
  ;; Validates independent processing of vastly different magnitudes
  (func $test_i32x4_neg_extreme_diff (result v128)
    ;; Create vector [1, 1000000, -1, -1000000]
    i32.const 1
    i32x4.splat
    i32.const 1000000
    i32x4.replace_lane 1
    i32.const -1
    i32x4.replace_lane 2
    i32.const -1000000
    i32x4.replace_lane 3
    ;; Apply negation: should become [-1, -1000000, 1, 1000000]
    i32x4.neg
  )
  (export "test_i32x4_neg_extreme_diff" (func $test_i32x4_neg_extreme_diff))
)