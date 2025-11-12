(module
  ;; f64.gt test module
  ;; Tests f64.gt opcode with comprehensive test cases covering:
  ;; - Basic comparisons (positive, negative, mixed signs)
  ;; - Equal values (including zero cases)
  ;; - Boundary values (max, min, epsilon)
  ;; - Special values (NaN, infinity)

  ;; Test function for f64.gt opcode
  ;; Parameters: two f64 values to compare
  ;; Returns: i32 result (1 if first > second, 0 otherwise)
  (func $f64_gt_test (export "f64_gt_test") (param $a f64) (param $b f64) (result i32)
    local.get $a    ;; Push first operand
    local.get $b    ;; Push second operand
    f64.gt          ;; Execute f64.gt comparison
  )
)