;; SIMD Load/Store Operations Test Module
;; Purpose: Test SIMD memory operations including load, store, splat, extend, and lane operations
;; Features: SIMD v128 operations, memory access patterns

(module
  ;; Memory for SIMD operations
  (memory 1)
  
  ;; Initialize memory with test pattern for SIMD operations
  (data (i32.const 0) "\00\01\02\03\04\05\06\07\08\09\0A\0B\0C\0D\0E\0F")  ;; 16 bytes for v128
  (data (i32.const 16) "\10\11\12\13\14\15\16\17\18\19\1A\1B\1C\1D\1E\1F") ;; Another 16 bytes
  
  ;; Test v128.load operation
  (func (export "test_v128_load") (result v128)
    ;; Load 16 bytes from address 0
    (v128.load (i32.const 0))
  )
  
  ;; Test v128.store operation
  (func (export "test_v128_store") (param $addr i32) (param $value v128)
    ;; Store v128 value at specified address
    (v128.store (local.get $addr) (local.get $value))
  )
  
  ;; Test v128.load8_splat operation
  (func (export "test_v128_load8_splat") (param $addr i32) (result v128)
    ;; Load single byte and splat to all 16 lanes
    (v128.load8_splat (local.get $addr))
  )
  
  ;; Test v128.load16_splat operation
  (func (export "test_v128_load16_splat") (param $addr i32) (result v128)
    ;; Load single 16-bit value and splat to all 8 lanes
    (v128.load16_splat (local.get $addr))
  )
  
  ;; Test v128.load32_splat operation
  (func (export "test_v128_load32_splat") (param $addr i32) (result v128)
    ;; Load single 32-bit value and splat to all 4 lanes
    (v128.load32_splat (local.get $addr))
  )
  
  ;; Test v128.load64_splat operation
  (func (export "test_v128_load64_splat") (param $addr i32) (result v128)
    ;; Load single 64-bit value and splat to both lanes
    (v128.load64_splat (local.get $addr))
  )
  
  ;; Test v128.load32_zero operation
  (func (export "test_v128_load32_zero") (param $addr i32) (result v128)
    ;; Load 32-bit value and zero extend to v128
    (v128.load32_zero (local.get $addr))
  )
  
  ;; Test v128.load64_zero operation
  (func (export "test_v128_load64_zero") (param $addr i32) (result v128)
    ;; Load 64-bit value and zero extend to v128
    (v128.load64_zero (local.get $addr))
  )
  
  ;; Test v128.load8x8_s operation
  (func (export "test_v128_load8x8_s") (param $addr i32) (result v128)
    ;; Load 8 bytes and sign extend to 8x16-bit lanes
    (v128.load8x8_s (local.get $addr))
  )
  
  ;; Test v128.load8x8_u operation
  (func (export "test_v128_load8x8_u") (param $addr i32) (result v128)
    ;; Load 8 bytes and zero extend to 8x16-bit lanes
    (v128.load8x8_u (local.get $addr))
  )
  
  ;; Test v128.load16x4_s operation
  (func (export "test_v128_load16x4_s") (param $addr i32) (result v128)
    ;; Load 4 16-bit values and sign extend to 4x32-bit lanes
    (v128.load16x4_s (local.get $addr))
  )
  
  ;; Test v128.load16x4_u operation
  (func (export "test_v128_load16x4_u") (param $addr i32) (result v128)
    ;; Load 4 16-bit values and zero extend to 4x32-bit lanes
    (v128.load16x4_u (local.get $addr))
  )
  
  ;; Test v128.load32x2_s operation
  (func (export "test_v128_load32x2_s") (param $addr i32) (result v128)
    ;; Load 2 32-bit values and sign extend to 2x64-bit lanes
    (v128.load32x2_s (local.get $addr))
  )
  
  ;; Test v128.load32x2_u operation
  (func (export "test_v128_load32x2_u") (param $addr i32) (result v128)
    ;; Load 2 32-bit values and zero extend to 2x64-bit lanes
    (v128.load32x2_u (local.get $addr))
  )
  
  ;; Test v128.load8_lane operation (lane 0)
  (func (export "test_v128_load8_lane_0") (param $addr i32) (param $src v128) (result v128)
    ;; Load byte and insert into lane 0
    (v128.load8_lane 0 (local.get $addr) (local.get $src))
  )
  
  ;; Test v128.load16_lane operation (lane 0)
  (func (export "test_v128_load16_lane_0") (param $addr i32) (param $src v128) (result v128)
    ;; Load 16-bit value and insert into lane 0
    (v128.load16_lane 0 (local.get $addr) (local.get $src))
  )
  
  ;; Test v128.load32_lane operation (lane 0)
  (func (export "test_v128_load32_lane_0") (param $addr i32) (param $src v128) (result v128)
    ;; Load 32-bit value and insert into lane 0
    (v128.load32_lane 0 (local.get $addr) (local.get $src))
  )
  
  ;; Test v128.load64_lane operation (lane 0)
  (func (export "test_v128_load64_lane_0") (param $addr i32) (param $src v128) (result v128)
    ;; Load 64-bit value and insert into lane 0
    (v128.load64_lane 0 (local.get $addr) (local.get $src))
  )
  
  ;; Test v128.store8_lane operation (lane 0)
  (func (export "test_v128_store8_lane_0") (param $addr i32) (param $src v128)
    ;; Extract byte from lane 0 and store
    (v128.store8_lane 0 (local.get $addr) (local.get $src))
  )
  
  ;; Test v128.store16_lane operation (lane 0)
  (func (export "test_v128_store16_lane_0") (param $addr i32) (param $src v128)
    ;; Extract 16-bit value from lane 0 and store
    (v128.store16_lane 0 (local.get $addr) (local.get $src))
  )
  
  ;; Test v128.store32_lane operation (lane 0)
  (func (export "test_v128_store32_lane_0") (param $addr i32) (param $src v128)
    ;; Extract 32-bit value from lane 0 and store
    (v128.store32_lane 0 (local.get $addr) (local.get $src))
  )
  
  ;; Test v128.store64_lane operation (lane 0)
  (func (export "test_v128_store64_lane_0") (param $addr i32) (param $src v128)
    ;; Extract 64-bit value from lane 0 and store
    (v128.store64_lane 0 (local.get $addr) (local.get $src))
  )
  
  ;; Helper function to create test v128 value
  (func (export "create_test_vector") (result v128)
    ;; Create a test vector with pattern 0x00010203...0F
    (v128.const i32x4 0x03020100 0x07060504 0x0B0A0908 0x0F0E0D0C)
  )
  
  ;; Helper function to read memory at address
  (func (export "read_memory_i32") (param $addr i32) (result i32)
    (i32.load (local.get $addr))
  )
  
  ;; Helper function to read memory byte at address
  (func (export "read_memory_i8") (param $addr i32) (result i32)
    (i32.load8_u (local.get $addr))
  )
)