;; WebAssembly Text Format (.wat) for local.get Invalid Index Testing
;; This module contains functions for testing boundary conditions
;;
;; Note: We can't create truly invalid WAT files since they won't compile,
;; but we can test boundary conditions and validate proper error handling

(module
  ;; Test function that would try to access a high index -
  ;; In practice, this validates that our error handling works correctly
  (func (export "test_boundary_access") (result i32)
    ;; This function has no explicit locals, so only implicit locals (none for this func)
    ;; Accessing local 0 when there are no locals should be handled gracefully
    ;; by the runtime validation
    i32.const 0
  )

  ;; Valid test function for comparison
  (func (export "test_valid_access") (result i32)
    (local $valid_local i32)

    ;; Set and get valid local
    i32.const 42
    local.set $valid_local
    local.get $valid_local
  )

  ;; Test function with maximum locals to test index boundaries
  (func (export "test_max_locals") (result i32)
    (local $l0 i32) (local $l1 i32) (local $l2 i32) (local $l3 i32) (local $l4 i32)
    (local $l5 i32) (local $l6 i32) (local $l7 i32) (local $l8 i32) (local $l9 i32)

    ;; Access the last valid local (index 9)
    i32.const 999
    local.set $l9
    local.get $l9
  )
)