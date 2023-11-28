// Copyright 2020-2023 Nikita Fediuchin. All rights reserved.
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

/*
 * Mutual exclusion structure.
 */
typedef struct Mutex_T Mutex_T;
/*
 * Mutual exclusion instance.
 */
typedef Mutex_T* Mutex;

/*
 * Condition variable structure.
 */
typedef struct Cond_T Cond_T;
/*
 * Condition variable instance.
 */
typedef Cond_T* Cond;

/*
 * Create a new mutex instance.
 * Returns mutex on success, otherwise NULL.
 */
Mutex createMutex();

/*
 * Destroys mutex instance.
 * mutex - mutex instance or NULL.
 */
void destroyMutex(Mutex mutex);

/*
 * Locks mutex with blocking.
 * mutex - mutex instance.
 */
void lockMutex(Mutex mutex);

/*
 * Unlocks locked mutex.
 * mutex - mutex instance.
 */
void unlockMutex(Mutex mutex);

/*
 * Try to lock the specified mutex without blocking.
 * Returns true if mutex was successfully locked.
 *
 * mutex - mutex instance.
 */
bool tryLockMutex(Mutex mutex);

/*
 * Create a new condition variable instance.
 * Returns cond on success, otherwise NULL.
 */
Cond createCond();

/*
 * Destroys condition variable instance.
 * cond - cond instance or NULL.
 */
void destroyCond(Cond cond);

/*
 * Unblocks one of the waiting threads.
 * cond - cond instance.
 */
void signalCond(Cond cond);

/*
 * Unblocks all waiting threads.
 * cond - cond instance.
 */
void broadcastCond(Cond cond);

/*
 * Blocks the current thread until the
 * condition variable is woken up or
 * a spurious wakeup occurs.
 *
 * cond - cond instance.
 * mutex - mutex instance.
 */
void waitCond(Cond cond, Mutex mutex);

/*
 * Blocks the current thread until the
 * condition variable is woken up or
 * after the specified timeout or
 * a spurious wakeup occurred.
 *
 * cond - cond instance.
 * mutex - mutex instance.
 * timeout - timeout time.
 */
void waitCondFor(Cond cond, Mutex mutex, double timeout);