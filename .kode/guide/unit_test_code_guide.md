  # Key Principles for Writing Good Unit Tests

  1. Comprehensive Test Coverage

  - Test all major code paths: The test file covers object file emission with various
  optimization levels, LLVM file emission, and temp file generation
  - Parameter range testing: Tests iterate through valid ranges (opt_level 0-3,
  size_level 0-3, bounds_checks 0-2)
  - Feature flag testing: Tests with different feature combinations (SIMD, bulk
  memory, ref types enabled/disabled)

  2. Proper Test Structure

  // Standard test fixture pattern
  class aot_compiler_test_suit : public testing::Test {
  protected:
      static void SetUpTestCase() {
          // One-time setup for all tests
          CWD = get_binary_path();
          WASM_FILE = strdup((CWD + MAIN_WASM).c_str());
      }

      static void TearDownTestCase() {
          // One-time cleanup
          free(WASM_FILE);
      }

      WAMRRuntimeRAII<512 * 1024> runtime; // RAII resource management
  };

  3. Resource Management

  - RAII patterns: Use WAMRRuntimeRAII for automatic cleanup
  - File cleanup: Tests create temporary files and ensure proper cleanup
  - Memory management: Proper allocation/deallocation in setup/teardown

  4. Error Handling Validation

  // Test error conditions and edge cases
  TEST_F(aot_compiler_test_suit, aot_generate_tempfile_name) {
      // Test common case
      aot_generate_tempfile_name("wamrc-obj", "o", obj_file_name,
  sizeof(obj_file_name));
      EXPECT_NE(nullptr, strstr(obj_file_name, ".o"));

      // Test abnormal cases
      EXPECT_EQ(nullptr, aot_generate_tempfile_name("wamrc-obj", "o", obj_file_name,
  0));

      // Test buffer overflow scenarios
      char obj_file_name_1[20];
      EXPECT_EQ(nullptr, aot_generate_tempfile_name("wamrc-obj",
  "12345678901234567890",
                                                   obj_file_name_1,
  sizeof(obj_file_name_1)));
  }

  5. Integration Testing Approach

  - End-to-end workflow: Load WASM → Create comp data → Create context → Compile →
  Emit
  - Real WASM modules: Use actual WASM files rather than mocked data
  - Configuration testing: Test with different compiler options

  6. Assertion Best Practices

  - Use EXPECT_NE for non-null checks: EXPECT_NE(wasm_module, nullptr)
  - Validate error states: EXPECT_STREQ(aot_get_last_error(), "")
  - Check boolean results: EXPECT_TRUE(aot_compile_wasm(comp_ctx))

  7. Test Organization

  - Logical grouping: Related tests in the same test fixture
  - Shared helper functions: test_aot_emit_object_file_with_option() for code reuse
  - Clear test names: Descriptive names indicating what's being tested

  8. Platform and Configuration Awareness

  - Conditional compilation: Handle platform-specific behavior
  - Feature flags: Test with different WASM_ENABLE_* configurations
  - Architecture considerations: Account for different target architectures

  9. Performance Considerations

  - Reuse expensive resources: WASM module loading in setup
  - Avoid redundant operations: Share compiled data between related tests
  - Cleanup properly: Ensure no resource leaks between test runs

  10. Documentation and Maintainability

  - Clear test purpose: Each test should have obvious intent
  - Minimal dependencies: Tests should be self-contained where possible
  - Consistent patterns: Follow established WAMR testing conventions

  Key Takeaways from the Implementation

  1. Focus on integration points where different components interact
  2. Test both success and failure paths comprehensively
  3. Use real data when practical rather than excessive mocking
  4. Follow the WAMR testing patterns established in existing tests
  5. Ensure proper resource cleanup to prevent test pollution

  These principles ensure tests are reliable, maintainable, and provide meaningful
  validation of the AOT compiler functionality.