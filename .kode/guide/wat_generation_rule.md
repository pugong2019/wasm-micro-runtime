# WAT File Generation Rules

## When to Generate WAT Files (REQUIRED):
- Memory64 operations with i64 addressing beyond 4GB
- Atomic operations requiring shared memory
- Edge case testing (boundary conditions, stack overflow)
- WebAssembly feature testing (SIMD, reference types, bulk memory)
- Error condition testing (malformed modules, runtime exceptions)

## WAT File Templates to Use:
- **Memory64 Testing**: Large address space operations (8GB+ memory)
- **Atomic Operations**: Shared memory with atomic instructions
- **Bulk Memory Operations**: memory.fill, memory.copy operations
- **Error Conditions**: Out-of-bounds, stack overflow, unreachable code
- **Multi-Module Testing**: Module dependencies and imports

## File Organization:
```
tests/unit/[module_name]/
├── CMakeLists.txt
├── test_[step_name].cpp
└── wasm-apps/
    ├── [feature]_test.wat
    ├── [feature]_test.wasm
    └── test_[module].h (if needed)
```