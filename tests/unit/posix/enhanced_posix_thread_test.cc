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

/******
 * Test Case: os_cond_wait_ValidParameters_Success
 * Source: core/shared/platform/common/posix/posix_thread.c:204-212
 * Target Lines: 206 (assert(cond)), 207 (assert(mutex)), 209 (pthread_cond_wait check), 212 (return BHT_OK)
 * Functional Purpose: Validates that os_cond_wait() correctly waits on a condition variable
 *                     when signaled by another thread, exercising the success path through
 *                     the function including parameter validation and successful return.
 * Call Path: os_cond_wait() [PUBLIC API - DIRECT CALL]
 * Coverage Goal: Exercise normal successful condition wait path
 ******/
TEST_F(EnhancedPosixThreadTest, os_cond_wait_ValidParameters_Success)
{
    korp_cond condition;
    korp_mutex mutex;

    // Initialize condition variable and mutex
    int cond_init_result = os_cond_init(&condition);
    ASSERT_EQ(BHT_OK, cond_init_result);

    int mutex_init_result = os_mutex_init(&mutex);
    ASSERT_EQ(BHT_OK, mutex_init_result);

    // Lock the mutex before waiting
    int lock_result = os_mutex_lock(&mutex);
    ASSERT_EQ(BHT_OK, lock_result);

    // Create a thread to signal the condition
    pthread_t signal_thread;
    struct CondSignalData {
        korp_cond* cond;
        korp_mutex* mutex;
    } signal_data = { &condition, &mutex };

    auto signal_func = [](void* arg) -> void* {
        CondSignalData* data = static_cast<CondSignalData*>(arg);
        usleep(10000); // Wait 10ms to ensure main thread is waiting

        int lock_result = os_mutex_lock(data->mutex);
        if (lock_result == BHT_OK) {
            int signal_result = os_cond_signal(data->cond);
            int unlock_result = os_mutex_unlock(data->mutex);
        }
        return nullptr;
    };

    int thread_create_result = pthread_create(&signal_thread, nullptr, signal_func, &signal_data);
    ASSERT_EQ(0, thread_create_result);

    // Test os_cond_wait - this should exercise lines 206, 207, 209, 212
    int wait_result = os_cond_wait(&condition, &mutex);
    ASSERT_EQ(BHT_OK, wait_result);

    // Wait for signal thread to complete
    void* thread_result;
    int join_result = pthread_join(signal_thread, &thread_result);
    ASSERT_EQ(0, join_result);

    // Unlock the mutex
    int unlock_result = os_mutex_unlock(&mutex);
    ASSERT_EQ(BHT_OK, unlock_result);

    // Clean up
    int cond_destroy_result = os_cond_destroy(&condition);
    ASSERT_EQ(BHT_OK, cond_destroy_result);

    int mutex_destroy_result = os_mutex_destroy(&mutex);
    ASSERT_EQ(BHT_OK, mutex_destroy_result);
}

/******
 * Test Case: os_cond_wait_MultipleWaiters_Success
 * Source: core/shared/platform/common/posix/posix_thread.c:204-212
 * Target Lines: 206 (assert(cond)), 207 (assert(mutex)), 209 (pthread_cond_wait check), 212 (return BHT_OK)
 * Functional Purpose: Validates that os_cond_wait() works correctly with multiple threads
 *                     waiting on the same condition variable, ensuring the function's
 *                     parameter validation and pthread_cond_wait integration work properly.
 * Call Path: os_cond_wait() [PUBLIC API - DIRECT CALL]
 * Coverage Goal: Exercise condition wait in multi-threaded scenario
 ******/
TEST_F(EnhancedPosixThreadTest, os_cond_wait_MultipleWaiters_Success)
{
    korp_cond condition;
    korp_mutex mutex;

    // Initialize condition variable and mutex
    int cond_init_result = os_cond_init(&condition);
    ASSERT_EQ(BHT_OK, cond_init_result);

    int mutex_init_result = os_mutex_init(&mutex);
    ASSERT_EQ(BHT_OK, mutex_init_result);

    const int NUM_WAITERS = 2;
    pthread_t waiter_threads[NUM_WAITERS];
    volatile int wait_count = 0;

    struct WaiterData {
        korp_cond* cond;
        korp_mutex* mutex;
        volatile int* count;
    } waiter_data = { &condition, &mutex, &wait_count };

    auto waiter_func = [](void* arg) -> void* {
        WaiterData* data = static_cast<WaiterData*>(arg);

        int lock_result = os_mutex_lock(data->mutex);
        if (lock_result == BHT_OK) {
            int wait_result = os_cond_wait(data->cond, data->mutex);
            if (wait_result == BHT_OK) {
                (*data->count)++;
            }
            int unlock_result = os_mutex_unlock(data->mutex);
        }
        return nullptr;
    };

    // Create waiter threads
    for (int i = 0; i < NUM_WAITERS; i++) {
        int thread_create_result = pthread_create(&waiter_threads[i], nullptr, waiter_func, &waiter_data);
        ASSERT_EQ(0, thread_create_result);
    }

    // Give threads time to start waiting
    usleep(20000);

    // Signal all waiters
    int lock_result = os_mutex_lock(&mutex);
    ASSERT_EQ(BHT_OK, lock_result);

    int broadcast_result = os_cond_broadcast(&condition);
    ASSERT_EQ(BHT_OK, broadcast_result);

    int unlock_result = os_mutex_unlock(&mutex);
    ASSERT_EQ(BHT_OK, unlock_result);

    // Wait for all threads to complete
    for (int i = 0; i < NUM_WAITERS; i++) {
        void* thread_result;
        int join_result = pthread_join(waiter_threads[i], &thread_result);
        ASSERT_EQ(0, join_result);
    }

    // Verify all waiters were awakened
    ASSERT_EQ(NUM_WAITERS, wait_count);

    // Clean up
    int cond_destroy_result = os_cond_destroy(&condition);
    ASSERT_EQ(BHT_OK, cond_destroy_result);

    int mutex_destroy_result = os_mutex_destroy(&mutex);
    ASSERT_EQ(BHT_OK, mutex_destroy_result);
}

/******
 * Test Case: os_cond_wait_SequentialWaits_Success
 * Source: core/shared/platform/common/posix/posix_thread.c:204-212
 * Target Lines: 206 (assert(cond)), 207 (assert(mutex)), 209 (pthread_cond_wait check), 212 (return BHT_OK)
 * Functional Purpose: Validates that os_cond_wait() can be called multiple times sequentially
 *                     on the same condition variable, ensuring the function's assertions
 *                     and pthread_cond_wait calls work reliably across multiple invocations.
 * Call Path: os_cond_wait() [PUBLIC API - DIRECT CALL]
 * Coverage Goal: Test repeated successful condition waits
 ******/
TEST_F(EnhancedPosixThreadTest, os_cond_wait_SequentialWaits_Success)
{
    korp_cond condition;
    korp_mutex mutex;

    // Initialize condition variable and mutex
    int cond_init_result = os_cond_init(&condition);
    ASSERT_EQ(BHT_OK, cond_init_result);

    int mutex_init_result = os_mutex_init(&mutex);
    ASSERT_EQ(BHT_OK, mutex_init_result);

    const int NUM_ITERATIONS = 3;

    for (int iteration = 0; iteration < NUM_ITERATIONS; iteration++) {
        // Lock the mutex
        int lock_result = os_mutex_lock(&mutex);
        ASSERT_EQ(BHT_OK, lock_result);

        // Create signaler thread for this iteration
        pthread_t signal_thread;
        struct IterationData {
            korp_cond* cond;
            korp_mutex* mutex;
            int iter;
        } iter_data = { &condition, &mutex, iteration };

        auto signal_func = [](void* arg) -> void* {
            IterationData* data = static_cast<IterationData*>(arg);
            usleep(5000); // Wait 5ms

            int lock_result = os_mutex_lock(data->mutex);
            if (lock_result == BHT_OK) {
                int signal_result = os_cond_signal(data->cond);
                int unlock_result = os_mutex_unlock(data->mutex);
            }
            return nullptr;
        };

        int thread_create_result = pthread_create(&signal_thread, nullptr, signal_func, &iter_data);
        ASSERT_EQ(0, thread_create_result);

        // Wait for condition - exercises target lines each iteration
        int wait_result = os_cond_wait(&condition, &mutex);
        ASSERT_EQ(BHT_OK, wait_result);

        // Clean up this iteration
        void* thread_result;
        int join_result = pthread_join(signal_thread, &thread_result);
        ASSERT_EQ(0, join_result);

        int unlock_result = os_mutex_unlock(&mutex);
        ASSERT_EQ(BHT_OK, unlock_result);
    }

    // Clean up
    int cond_destroy_result = os_cond_destroy(&condition);
    ASSERT_EQ(BHT_OK, cond_destroy_result);

    int mutex_destroy_result = os_mutex_destroy(&mutex);
    ASSERT_EQ(BHT_OK, mutex_destroy_result);
}