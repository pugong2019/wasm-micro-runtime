;; WebAssembly Text Format (.wat) for i32.add Opcode Testing
;; Comprehensive test module for 32-bit integer addition operation
;;
;; This module provides a simple wrapper function to test the i32.add opcode
;; with two parameters and return the addition result.
;;
;; Function signature: test_i32_add(i32, i32) -> i32
;; Implementation: Returns the sum of two 32-bit signed integers using i32.add

(module
  ;; Export the test function to make it callable from host
  (func (export "test_i32_add") (param $operand1 i32) (param $operand2 i32) (result i32)
    ;; Load first operand onto stack
    local.get $operand1
    ;; Load second operand onto stack
    local.get $operand2
    ;; Execute i32.add opcode
    ;; Stack before: [..., operand1, operand2]
    ;; Stack after:  [..., result]
    i32.add
    ;; Return result (automatically consumed from stack)
  )
)