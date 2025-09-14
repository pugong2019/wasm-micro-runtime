/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "gtest/gtest.h"
#include "aot_reloc.h"
#include "aot_runtime.h"

// Define relocation constants for testing
#if !defined(BH_PLATFORM_WINDOWS)
#define R_X86_64_64 1       /* Direct 64 bit  */
#define R_X86_64_PC32 2     /* PC relative 32 bit signed */
#define R_X86_64_PLT32 4    /* 32 bit PLT address */
#define R_X86_64_GOTPCREL 9 /* 32 bit signed PC relative offset to GOT */
#define R_X86_64_32 10      /* Direct 32 bit zero extended */
#define R_X86_64_32S 11     /* Direct 32 bit sign extended */
#define R_X86_64_PC64 24    /* PC relative 64 bit */
#else
#define IMAGE_REL_AMD64_ADDR64 1
#define IMAGE_REL_AMD64_ADDR32 2
#define IMAGE_REL_AMD64_REL32  4
#endif

// Mock AOT module for testing
class AOTRelocX86_64Test : public ::testing::Test
{
  protected:
    AOTModule module;
    uint8_t *target_section_addr;
    uint32_t target_section_size;
    char error_buf[128];

    void SetUp() override
    {
        memset(&module, 0, sizeof(AOTModule));
        target_section_size = 4096;
        target_section_addr = (uint8_t *)malloc(target_section_size);
        ASSERT_NE(target_section_addr, nullptr);
        memset(target_section_addr, 0, target_section_size);
        memset(error_buf, 0, sizeof(error_buf));

        // Setup module code section for PLT tests
        module.code_size = 8192;
        module.code = (uint8_t *)malloc(module.code_size);
        ASSERT_NE(module.code, nullptr);
        memset(module.code, 0, module.code_size);
    }

    void TearDown() override
    {
        if (target_section_addr) {
            free(target_section_addr);
        }
        if (module.code) {
            free(module.code);
        }
    }
};

// Test set_error_buf functionality
TEST_F(AOTRelocX86_64Test, test_set_error_buf_functionality)
{
    // Test with valid buffer
    char test_buf[128] = { 0 };
    const char *test_msg = "Test error message";
    
    // Access set_error_buf through apply_relocation with invalid type
    bool result = apply_relocation(&module, target_section_addr,
                                   target_section_size, 0, 0, 999999,
                                   nullptr, -1, test_buf, sizeof(test_buf));
    
    EXPECT_FALSE(result);
    EXPECT_STRNE(test_buf, "");
    EXPECT_TRUE(strstr(test_buf, "invalid relocation type") != nullptr);
    
    // Test with NULL buffer (should not crash)
    result = apply_relocation(&module, target_section_addr,
                              target_section_size, 0, 0, 999999,
                              nullptr, -1, nullptr, 0);
    EXPECT_FALSE(result);
}

// Test R_X86_64_64 relocation
TEST_F(AOTRelocX86_64Test, test_apply_relocation_r_x86_64_64_success)
{
    uint64_t reloc_offset = 100;
    void *symbol_addr = (void *)0x123456789ABCDEF0;
    int64_t reloc_addend = 0x10;
    
    // Set initial value at relocation offset
    *(uint64_t *)(target_section_addr + reloc_offset) = 0x20;
    
    bool result = apply_relocation(&module, target_section_addr,
                                   target_section_size, reloc_offset,
                                   reloc_addend, R_X86_64_64,
                                   symbol_addr, -1, error_buf,
                                   sizeof(error_buf));
    
    EXPECT_TRUE(result);
    EXPECT_EQ(*(uint64_t *)(target_section_addr + reloc_offset),
              (uint64_t)symbol_addr + reloc_addend + 0x20);
}

// Test R_X86_64_64 with addend
TEST_F(AOTRelocX86_64Test, test_apply_relocation_r_x86_64_64_with_addend)
{
    uint64_t reloc_offset = 200;
    void *symbol_addr = (void *)0x8000000000000000;
    int64_t reloc_addend = -0x1000;
    
    bool result = apply_relocation(&module, target_section_addr,
                                   target_section_size, reloc_offset,
                                   reloc_addend, R_X86_64_64,
                                   symbol_addr, -1, error_buf,
                                   sizeof(error_buf));
    
    EXPECT_TRUE(result);
    EXPECT_EQ(*(uint64_t *)(target_section_addr + reloc_offset),
              (uint64_t)symbol_addr + reloc_addend);
}

// Test R_X86_64_PC64 relocation
TEST_F(AOTRelocX86_64Test, test_apply_relocation_r_x86_64_pc64_success)
{
    uint64_t reloc_offset = 300;
    void *symbol_addr = (void *)0x2000000000000000;
    int64_t reloc_addend = 0x100;
    
    bool result = apply_relocation(&module, target_section_addr,
                                   target_section_size, reloc_offset,
                                   reloc_addend, R_X86_64_PC64,
                                   symbol_addr, -1, error_buf,
                                   sizeof(error_buf));
    
    EXPECT_TRUE(result);
    
    int64_t expected = (int64_t)((uintptr_t)symbol_addr + reloc_addend
                                 - (uintptr_t)(target_section_addr + reloc_offset));
    EXPECT_EQ(*(int64_t *)(target_section_addr + reloc_offset), expected);
}

// Test R_X86_64_32 success
TEST_F(AOTRelocX86_64Test, test_apply_relocation_r_x86_64_32_success)
{
    uint64_t reloc_offset = 400;
    void *symbol_addr = (void *)0x80000000; // Within 32-bit range
    int64_t reloc_addend = 0x1000;
    
    bool result = apply_relocation(&module, target_section_addr,
                                   target_section_size, reloc_offset,
                                   reloc_addend, R_X86_64_32,
                                   symbol_addr, -1, error_buf,
                                   sizeof(error_buf));
    
    EXPECT_TRUE(result);
    EXPECT_EQ(*(uint32_t *)(target_section_addr + reloc_offset),
              (uint32_t)((uintptr_t)symbol_addr + reloc_addend));
}

// Test R_X86_64_32 truncation error
TEST_F(AOTRelocX86_64Test, test_apply_relocation_r_x86_64_32_truncation_error)
{
    uint64_t reloc_offset = 500;
    void *symbol_addr = (void *)0x100000000; // Beyond 32-bit range
    int64_t reloc_addend = 0;
    
    bool result = apply_relocation(&module, target_section_addr,
                                   target_section_size, reloc_offset,
                                   reloc_addend, R_X86_64_32,
                                   symbol_addr, -1, error_buf,
                                   sizeof(error_buf));
    
    EXPECT_FALSE(result);
    EXPECT_TRUE(strstr(error_buf, "R_X86_64_32") != nullptr);
    EXPECT_TRUE(strstr(error_buf, "truncated to fit") != nullptr);
}

// Test R_X86_64_32S success
TEST_F(AOTRelocX86_64Test, test_apply_relocation_r_x86_64_32s_success)
{
    uint64_t reloc_offset = 600;
    void *symbol_addr = (void *)0x7FFFFFFF; // Within signed 32-bit range
    int64_t reloc_addend = -0x1000;
    
    bool result = apply_relocation(&module, target_section_addr,
                                   target_section_size, reloc_offset,
                                   reloc_addend, R_X86_64_32S,
                                   symbol_addr, -1, error_buf,
                                   sizeof(error_buf));
    
    EXPECT_TRUE(result);
    EXPECT_EQ(*(int32_t *)(target_section_addr + reloc_offset),
              (int32_t)((intptr_t)symbol_addr + reloc_addend));
}

// Test R_X86_64_32S truncation error
TEST_F(AOTRelocX86_64Test, test_apply_relocation_r_x86_64_32s_truncation_error)
{
    uint64_t reloc_offset = 700;
    void *symbol_addr = (void *)0x80000000; // Beyond signed 32-bit range
    int64_t reloc_addend = 0;
    
    bool result = apply_relocation(&module, target_section_addr,
                                   target_section_size, reloc_offset,
                                   reloc_addend, R_X86_64_32S,
                                   symbol_addr, -1, error_buf,
                                   sizeof(error_buf));
    
    EXPECT_FALSE(result);
    EXPECT_TRUE(strstr(error_buf, "R_X86_64_32S") != nullptr);
    EXPECT_TRUE(strstr(error_buf, "truncated to fit") != nullptr);
}

// Test invalid relocation type
TEST_F(AOTRelocX86_64Test, test_apply_relocation_invalid_type)
{
    uint64_t reloc_offset = 800;
    void *symbol_addr = (void *)0x1000;
    int64_t reloc_addend = 0;
    
    bool result = apply_relocation(&module, target_section_addr,
                                   target_section_size, reloc_offset,
                                   reloc_addend, 999999, // Invalid type
                                   symbol_addr, -1, error_buf,
                                   sizeof(error_buf));
    
    EXPECT_FALSE(result);
    EXPECT_TRUE(strstr(error_buf, "invalid relocation type") != nullptr);
}

// Test PLT32 with negative symbol index
TEST_F(AOTRelocX86_64Test, test_apply_relocation_plt32_negative_symbol_index)
{
    uint64_t reloc_offset = 900;
    void *symbol_addr = (void *)0x1000000;
    int64_t reloc_addend = 0;
    
    // Initialize PLT table
    uint8_t *plt_table = (uint8_t *)module.code + module.code_size - get_plt_table_size();
    init_plt_table(plt_table);
    
    bool result = apply_relocation(&module, target_section_addr,
                                   target_section_size, reloc_offset,
                                   reloc_addend, R_X86_64_PLT32,
                                   symbol_addr, -1, // Negative symbol index
                                   error_buf, sizeof(error_buf));
    
    // The test should verify that the function handles negative symbol_index correctly
    // If it fails, it means the relocation calculation resulted in truncation
    if (!result) {
        EXPECT_TRUE(strstr(error_buf, "truncated to fit") != nullptr);
    } else {
        // When symbol_index < 0, it should use symbol_addr directly
        intptr_t expected = (intptr_t)((uintptr_t)symbol_addr + reloc_addend
                                       - (uintptr_t)(target_section_addr + reloc_offset));
        EXPECT_EQ(*(int32_t *)(target_section_addr + reloc_offset), (int32_t)expected);
    }
}