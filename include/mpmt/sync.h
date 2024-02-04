// Copyright 2020-2024 Nikita Fediuchin. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include <stdbool.h>

/***********************************************************************************************************************
 * @file
 * @brief Common synchronization primitives.
 * 
 * @details
 * Synchronization primitives are used for managing concurrent access to 
 * shared resources, preventing race conditions, and ensuring thread safety.
 **********************************************************************************************************************/

/**
 * @brief Mutual exclusion structure.
 */
typedef struct Mutex_T Mutex_T;
/**
 * @brief Mutual exclusion instance.
 */
typedef Mutex_T* Mutex;

/**
 * @brief Condition variable structure.
 */
typedef struct Cond_T Cond_T;
/**
 * @brief Condition variable instance.
 */
typedef Cond_T* Cond;

/***********************************************************************************************************************
 * @brief Create a new mutex instance.
 * @note You should destroy created mutex instance manually.
 * 
 * @details
 * The mutex is a synchronization primitive that can be used to protect 
 * shared data from being simultaneously accessed by multiple threads.
 *  
 * @return A new mutex instance on success, otherwise NULL.
 */
Mutex createMutex();

/**
 * @brief Destroys mutex instance.
 * @param mutex mutex instance or NULL
 */
void destroyMutex(Mutex mutex);

/**
 * @brief Locks the mutex, blocks if the mutex is not available.
 * 
 * @details
 * Locks the mutex. If another thread has already locked the mutex, 
 * a call to lock will block execution until the lock is acquired.
 * 
 * @warning If lock is called by a thread that already owns the mutex, the behavior is undefined.
 * 
 * @param mutex mutex instance
 */
void lockMutex(Mutex mutex);

/**
 * @brief Unlocks locked mutex.
 * @warning The mutex must be locked by the current thread of execution, otherwise, the behavior is undefined.
 * @param mutex mutex instance
 */
void unlockMutex(Mutex mutex);

/**
 * @brief Tries to lock the mutex.
 * 
 * @details
 * This function is allowed to fail spuriously and return false 
 * even if the mutex is not currently locked by any other thread. 
 *
 * @param mutex mutex instance
 * @return True on successful lock acquisition, otherwise false. 
 */
bool tryLockMutex(Mutex mutex);

/**
 * @brief Returns pointer to the native mutex handle.
 * @warning You should not free returned pointer or use it after mutex destruction.
 *
 * @param mutex mutex instance
 */
const void* getMutexNative(Mutex mutex);

/***********************************************************************************************************************
 * @brief Create a new condition variable instance.
 * @note You should destroy created condition variable instance manually.
 * 
 * @details
 * The condition variable is a synchronization primitive used with a @ref Mutex to block one or more threads until 
 * another thread both modifies a shared variable (the condition) and notifies the condition variable.
 *  
 * @return A new condition variable instance on success, otherwise NULL.
 */
Cond createCond();

/**
 * @brief Destroys condition variable instance.
 * @param cond condition variable instance or NULL
 */
void destroyCond(Cond cond);

/**
 * @brief Notifies one waiting thread.
 * @details If any threads are waiting on this cond, calling signal() unblocks one of the waiting threads.
 * @param cond condition variable instance
 */
void signalCond(Cond cond);

/**
 * @brief Notifies all waiting threads.
 * @details Unblocks all threads currently waiting for this cond.
 * @param cond condition variable instance
 */
void broadcastCond(Cond cond);

/**
 * @brief Blocks the current thread until the condition variable is awakened.
 *
 * @details
 * Wait causes the current thread to block until the 
 * condition variable is notified or a spurious wakeup occurs.
 * 
 * @param cond condition variable instance
 * @param mutex mutex instance
 */
void waitCond(Cond cond, Mutex mutex);

/**
 * @brief Blocks the current thread until the condition variable is 
 * awakened or after the specified timeout duration.
 * 
 *
 * @param cond condition variable instance
 * @param mutex mutex instance
 * @param timeout timeout time (in seconds)
 */
void waitCondFor(Cond cond, Mutex mutex, double timeout);

/**
 * @brief Returns pointer to the native condition variable handle.
 * @warning You should not free returned pointer or use it after cond destruction.
 *
 * @param cond condition variable instance
 */
const void* getCondNative(Cond cond);