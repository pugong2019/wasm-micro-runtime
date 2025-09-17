# GTest Failure Fix Collection

This document collects common GTest test failures and their solutions for the WAMR project.

## 1. WASM File Not Found During Test Execution

### Problem Description
```
Failed to open file: aot_runtime_test.wasm
/path/to/test.cc:XX: Failure
Value of: load_wasm_file("aot_runtime_test.wasm")
  Actual: false
Expected: true
```

### Root Cause
- Test executable runs from build directory but WASM files are in source directory
- WASM test files are not copied to the build directory where tests execute
- Working directory mismatch between test execution and file location

### Solution Steps

#### Step 1: Add POST_BUILD Command to CMakeLists.txt
Add the following to your module's `CMakeLists.txt` after the `add_executable` command:

```cmake
add_custom_command(TARGET your_test_target POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/wasm-apps/*.wasm
        ${CMAKE_CURRENT_BINARY_DIR}/
        COMMENT "Copy test wasm files to the directory of google test"
        )
memory64
```

#### Step 2: Verify File Loading Method
Ensure your test uses proper file loading. Example working pattern:

```cpp
bool load_wasm_file(const char *wasm_file)
{
    std::ifstream file(wasm_file, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return false;
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        return false;
    }
    
    module = wasm_runtime_load(buffer.data(), buffer.size(), error_buf, sizeof(error_buf));
    return module != nullptr;
}
```

#### Step 3: Run Tests from Correct Directory
Execute tests from the build subdirectory where WASM files are copied:

```bash
cd /path/to/build/module_name
./test_executable
```

### Working Examples
- `tests/unit/memory64/CMakeLists.txt` - Reference implementation
- `tests/unit/shared-heap/CMakeLists.txt` - Another working example

### Prevention
- Always add `POST_BUILD` command when tests require WASM files
- Follow existing working module patterns
- Test file loading in isolation before running full test suite

---

## Template for Future Fixes

### Problem Description
[Brief description of the test failure and error messages]

### Root Cause
[Explanation of why the test fails]

### Solution Steps
[Step-by-step fix instructions with code examples]

### Working Examples
[References to existing working implementations]

### Prevention
[How to avoid this issue in future test development]

---