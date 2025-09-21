(module
  ;; Test module for import globals functionality
  
  ;; Import global for testing
  (import "env" "imported_global" (global $imported_i32 i32))
  (import "env" "imported_mut_global" (global $imported_mut_i32 (mut i32)))
  
  ;; Local globals
  (global $local_global i32 (i32.const 100))
  (global $local_mut_global (mut i32) (global.get $imported_i32))
  
  ;; Memory
  (memory 1)
  
  ;; Function that uses imported globals
  (func $use_imported_globals (result i32)
    global.get $imported_i32
    global.get $imported_mut_i32
    i32.add
    global.get $local_global
    i32.add
  )
  
  ;; Function that modifies mutable global
  (func $modify_global (param i32)
    local.get 0
    global.set $imported_mut_i32
    
    local.get 0
    i32.const 10
    i32.add
    global.set $local_mut_global
  )
  
  ;; Export functions
  (export "use_imported_globals" (func $use_imported_globals))
  (export "modify_global" (func $modify_global))
  (export "local_global" (global $local_global))
  (export "local_mut_global" (global $local_mut_global))
)