;; SIMD Type Conversions & Extensions Test Module
;; Purpose: Test basic SIMD conversion operations
;; Features: SIMD, integer extensions, narrowing

(module
  ;; Memory for testing memory operations if needed
  (memory 1)

  ;; Test function: i16x8 extend i8x16 (signed)
  (func (export "i16x8_extend_i8x16_s") (param $v v128) (result v128)
    ;; Extend low 8 lanes from i8x16 to i16x8 (signed)
    (i16x8.extend_low_i8x16_s (local.get $v))
  )

  ;; Test function: i16x8 extend i8x16 (unsigned)
  (func (export "i16x8_extend_i8x16_u") (param $v v128) (result v128)
    ;; Extend low 8 lanes from i8x16 to i16x8 (unsigned)
    (i16x8.extend_low_i8x16_u (local.get $v))
  )

  ;; Test function: i32x4 extend i16x8 (signed)
  (func (export "i32x4_extend_i16x8_s") (param $v v128) (result v128)
    ;; Extend low 4 lanes from i16x8 to i32x4 (signed)
    (i32x4.extend_low_i16x8_s (local.get $v))
  )

  ;; Test function: i32x4 extend i16x8 (unsigned)
  (func (export "i32x4_extend_i16x8_u") (param $v v128) (result v128)
    ;; Extend low 4 lanes from i16x8 to i32x4 (unsigned)
    (i32x4.extend_low_i16x8_u (local.get $v))
  )

  ;; Test function: i8x16 narrow i16x8 (signed)
  (func (export "i8x16_narrow_i16x8_s") (param $a v128) (param $b v128) (result v128)
    ;; Narrow two i16x8 vectors to one i8x16 vector (signed saturation)
    (i8x16.narrow_i16x8_s (local.get $a) (local.get $b))
  )

  ;; Test function: i8x16 narrow i16x8 (unsigned)
  (func (export "i8x16_narrow_i16x8_u") (param $a v128) (param $b v128) (result v128)
    ;; Narrow two i16x8 vectors to one i8x16 vector (unsigned saturation)
    (i8x16.narrow_i16x8_u (local.get $a) (local.get $b))
  )

  ;; Test function: i16x8 narrow i32x4 (signed)
  (func (export "i16x8_narrow_i32x4_s") (param $a v128) (param $b v128) (result v128)
    ;; Narrow two i32x4 vectors to one i16x8 vector (signed saturation)
    (i16x8.narrow_i32x4_s (local.get $a) (local.get $b))
  )

  ;; Test function: i16x8 narrow i32x4 (unsigned)
  (func (export "i16x8_narrow_i32x4_u") (param $a v128) (param $b v128) (result v128)
    ;; Narrow two i32x4 vectors to one i16x8 vector (unsigned saturation)
    (i16x8.narrow_i32x4_u (local.get $a) (local.get $b))
  )

  ;; Helper function: Create i8x16 vector with test pattern
  (func (export "create_i8x16_test") (result v128)
    ;; Create i8x16 vector: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
    v128.const i8x16 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
  )

  ;; Helper function: Create i16x8 vector with test pattern
  (func (export "create_i16x8_test") (result v128)
    ;; Create i16x8 vector: [100, 200, 300, 400, 500, 600, 700, 800]
    v128.const i16x8 100 200 300 400 500 600 700 800
  )

  ;; Helper function: Create i32x4 vector with test pattern
  (func (export "create_i32x4_test") (result v128)
    ;; Create i32x4 vector: [1000, 2000, 3000, 4000]
    v128.const i32x4 1000 2000 3000 4000
  )
)