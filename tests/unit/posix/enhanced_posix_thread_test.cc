/*
 * Copyright (C) 2025 WAMR Community. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "test_helper.h"
#include "gtest/gtest.h"
#include "platform_api_vmcore.h"
#include "platform_api_extension.h"
#include "posix_test_helper.h"
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

class EnhancedPosixThreadTest : public testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}

  public:
    WAMRRuntimeRAII<512 * 1024> runtime;
};

/******
 * Test Case: os_recursive_mutex_init_ValidMutex_Success
 * Source: core/shared/platform/common/posix/posix_thread.c:130-145
 * Target Lines: 132 (int ret), 134 (pthread_mutexattr_t mattr), 136 (assert(mutex)),
 *               137 (pthread_mutexattr_init), 141 (pthread_mutexattr_settype),
 *               142 (pthread_mutex_init), 143 (pthread_mutexattr_destroy), 145 (return)
 * Functional Purpose: Validates that os_recursive_mutex_init() correctly initializes
 *                     a recursive mutex with proper pthread attributes and returns
 *                     BHT_OK on successful initialization.
 * Call Path: os_recursive_mutex_init() [PUBLIC API - DIRECT CALL]
 * Coverage Goal: Exercise normal successful initialization path
 ******/
TEST_F(EnhancedPosixThreadTest, os_recursive_mutex_init_ValidMutex_Success)
{
    korp_mutex mutex;

    // Test successful recursive mutex initialization
    int result = os_recursive_mutex_init(&mutex);
    ASSERT_EQ(BHT_OK, result);

    // Verify the mutex was properly initialized by testing recursive locking
    int lock_result1 = os_mutex_lock(&mutex);
    ASSERT_EQ(BHT_OK, lock_result1);

    // Test recursive lock (should succeed with recursive mutex)
    int lock_result2 = os_mutex_lock(&mutex);
    ASSERT_EQ(BHT_OK, lock_result2);

    // Unlock both times
    int unlock_result1 = os_mutex_unlock(&mutex);
    ASSERT_EQ(BHT_OK, unlock_result1);

    int unlock_result2 = os_mutex_unlock(&mutex);
    ASSERT_EQ(BHT_OK, unlock_result2);

    // Clean up
    int destroy_result = os_mutex_destroy(&mutex);
    ASSERT_EQ(BHT_OK, destroy_result);
}

/******
 * Test Case: os_recursive_mutex_init_ValidMutex_VerifyRecursiveBehavior
 * Source: core/shared/platform/common/posix/posix_thread.c:130-145
 * Target Lines: 132, 134, 136, 137, 141, 142, 143, 145
 * Functional Purpose: Validates that the recursive mutex created by
 *                     os_recursive_mutex_init() actually exhibits recursive
 *                     locking behavior, confirming the pthread_mutexattr_settype
 *                     call on line 141 worked correctly.
 * Call Path: os_recursive_mutex_init() [PUBLIC API - DIRECT CALL]
 * Coverage Goal: Verify functional correctness of recursive mutex behavior
 ******/
TEST_F(EnhancedPosixThreadTest, os_recursive_mutex_init_ValidMutex_VerifyRecursiveBehavior)
{
    korp_mutex mutex;

    // Initialize recursive mutex
    int init_result = os_recursive_mutex_init(&mutex);
    ASSERT_EQ(BHT_OK, init_result);

    // Lock multiple times (should work with recursive mutex)
    for (int i = 0; i < 5; i++) {
        int lock_result = os_mutex_lock(&mutex);
        ASSERT_EQ(BHT_OK, lock_result);
    }

    // Unlock the same number of times
    for (int i = 0; i < 5; i++) {
        int unlock_result = os_mutex_unlock(&mutex);
        ASSERT_EQ(BHT_OK, unlock_result);
    }

    // Clean up
    int destroy_result = os_mutex_destroy(&mutex);
    ASSERT_EQ(BHT_OK, destroy_result);
}

/******
 * Test Case: os_recursive_mutex_init_MultipleInitializations_Success
 * Source: core/shared/platform/common/posix/posix_thread.c:130-145
 * Target Lines: 132, 134, 136, 137, 141, 142, 143, 145
 * Functional Purpose: Validates that os_recursive_mutex_init() can be called
 *                     multiple times on different mutex objects successfully,
 *                     ensuring the function's internal attribute setup and
 *                     cleanup (lines 137, 141, 142, 143) work reliably.
 * Call Path: os_recursive_mutex_init() [PUBLIC API - DIRECT CALL]
 * Coverage Goal: Test function reliability with multiple invocations
 ******/
TEST_F(EnhancedPosixThreadTest, os_recursive_mutex_init_MultipleInitializations_Success)
{
    const int NUM_MUTEXES = 3;
    korp_mutex mutexes[NUM_MUTEXES];

    // Initialize multiple recursive mutexes
    for (int i = 0; i < NUM_MUTEXES; i++) {
        int result = os_recursive_mutex_init(&mutexes[i]);
        ASSERT_EQ(BHT_OK, result);
    }

    // Test that all mutexes work independently
    for (int i = 0; i < NUM_MUTEXES; i++) {
        int lock_result = os_mutex_lock(&mutexes[i]);
        ASSERT_EQ(BHT_OK, lock_result);

        // Test recursive locking on each
        int recursive_lock = os_mutex_lock(&mutexes[i]);
        ASSERT_EQ(BHT_OK, recursive_lock);

        int unlock_result1 = os_mutex_unlock(&mutexes[i]);
        ASSERT_EQ(BHT_OK, unlock_result1);

        int unlock_result2 = os_mutex_unlock(&mutexes[i]);
        ASSERT_EQ(BHT_OK, unlock_result2);
    }

    // Clean up all mutexes
    for (int i = 0; i < NUM_MUTEXES; i++) {
        int destroy_result = os_mutex_destroy(&mutexes[i]);
        ASSERT_EQ(BHT_OK, destroy_result);
    }
}

/******
 * Test Case: os_recursive_mutex_init_AttributeValidation_Success
 * Source: core/shared/platform/common/posix/posix_thread.c:130-145
 * Target Lines: 137 (pthread_mutexattr_init), 141 (pthread_mutexattr_settype),
 *               142 (pthread_mutex_init), 143 (pthread_mutexattr_destroy)
 * Functional Purpose: Validates the complete attribute initialization sequence:
 *                     pthread_mutexattr_init -> pthread_mutexattr_settype ->
 *                     pthread_mutex_init -> pthread_mutexattr_destroy
 * Call Path: os_recursive_mutex_init() [PUBLIC API - DIRECT CALL]
 * Coverage Goal: Exercise all pthread attribute manipulation lines
 ******/
TEST_F(EnhancedPosixThreadTest, os_recursive_mutex_init_AttributeValidation_Success)
{
    korp_mutex mutex;

    // Test the complete attribute sequence by initializing
    int result = os_recursive_mutex_init(&mutex);
    ASSERT_EQ(BHT_OK, result);

    // Validate the mutex was created with recursive attributes
    // by attempting nested locks from the same thread
    int first_lock = os_mutex_lock(&mutex);
    ASSERT_EQ(BHT_OK, first_lock);

    int second_lock = os_mutex_lock(&mutex);
    ASSERT_EQ(BHT_OK, second_lock);

    int third_lock = os_mutex_lock(&mutex);
    ASSERT_EQ(BHT_OK, third_lock);

    // Unlock in reverse order
    int third_unlock = os_mutex_unlock(&mutex);
    ASSERT_EQ(BHT_OK, third_unlock);

    int second_unlock = os_mutex_unlock(&mutex);
    ASSERT_EQ(BHT_OK, second_unlock);

    int first_unlock = os_mutex_unlock(&mutex);
    ASSERT_EQ(BHT_OK, first_unlock);

    // Clean up
    int destroy_result = os_mutex_destroy(&mutex);
    ASSERT_EQ(BHT_OK, destroy_result);
}