// Copyright 2020-2026 Nikita Fediuchin. All rights reserved.
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
#include <stddef.h>
#include <stdbool.h>

/***********************************************************************************************************************
 * @file
 * @brief Common thread functions.
 * 
 * @details
 * A thread is the smallest unit of execution within a process. Threads are parallel execution paths 
 * that can run independently but share the same resources, such as memory space, file descriptors, 
 * and other process-specific information. Threads enable concurrent execution and can be used to 
 * perform multiple tasks simultaneously within a single process.
 */

/**
 * @brief Thread structure.
 */
typedef struct Thread_T Thread_T;
/**
 * @brief Thread instance.
 */
typedef Thread_T* Thread;

/**
 * @brief Creates a new thread executing the specified function.
 * 
 * @details
 * Threads begin execution immediately upon construction of the associated 
 * thread object (pending any OS scheduling delays), starting at the 
 * top-level function provided as a constructor argument.
 *
 * @param[in] function pointer to the function that should be invoked
 * @param[in] argument argument that will be passed to the function or NULL
 * 
 * @return Thread instance on success, otherwise NULL.
 */
Thread createThread(void (*function)(void*), void* argument);

/**
 * @brief Destroys thread instance.
 * @param thread thread instance or NULL
 */
void destroyThread(Thread thread);

/**
 * @brief Blocks the current thread until the function execution end.
 * @param thread thread instance
 */
void joinThread(Thread thread);

/**
 * @brief Blocks the execution of the current thread for a specified time.
 * @param delay thread sleep delay time (in seconds)
 */
void sleepThread(double delay);

/**
 * @brief Causes the current thread to yield execution to another thread.
 * @return True on success, otherwise false.
 */
bool yieldThread();

/**
 * @brief Returns thread current join status.
 * @param thread thread instance
 */
bool isThreadJoined(Thread thread);

/**
 * @brief Returns true if thread is currently running one.
 * @param thread thread instance
 */
bool isThreadCurrent(Thread thread);

/**
 * @brief Returns pointer to the native thread handle.
 * @warning You should not free returned pointer or use it after thread destruction.
 *
 * @param thread thread instance
 */
const void* getThreadNative(Thread thread);

/***********************************************************************************************************************
 * @brief Sets current thread as main.
 */
void setMainThread();

/**
 * @brief Returns true if current thread is main.
 */
bool isCurrentThreadMain();

/**
 * @brief Returns true if thread is main.
 * @param thread target thread instance
 */
bool isThreadMain(Thread thread);

/**
 * @brief Returns current thread name.
 * @param[out] name pointer to the thread name string
 * @param size maximal name string capacity (including '\\0')
 */
void getThreadName(char* name, size_t size);

/**
 * @brief Sets current thread name.
 * @param[in] name thread name string
 */
void setThreadName(const char* name);

/**
 * @brief Sets current thread priority to foreground.
 */
void setThreadForegroundPriority();

/**
 * @brief Sets current thread priority to background.
 */
void setThreadBackgroundPriority();