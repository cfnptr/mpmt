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
#include <stddef.h>
#include <stdbool.h>

/*
 * Thread structure.
 */
typedef struct Thread_T Thread_T;
/*
 * Thread instance.
 */
typedef Thread_T* Thread;

/*
 * Creates a new thread executing the specified function.
 * Returns thread instance on success, otherwise NULL.
 *
 * function - pointer to the function that should be invoked.
 * argument - argument that will be passed to the function or NULL.
 */
Thread createThread(void (*function)(void*), void* argument);
/*
 * Destroys thread instance.
 * thread - thread instance or NULL.
 */
void destroyThread(Thread thread);

/*
 * Blocks the current thread until the function execution end.
 * thread - thread instance.
 */
void joinThread(Thread thread);
/*
 * Blocks the execution of the current thread for a specified time.
 * delay - thread sleep delay time (s).
 */
void sleepThread(double delay);
/*
 * Causes the current thread to yield execution to another thread.
 */
bool yieldThread();

/*
 * Returns thread current join status.
 * thread - thread instance.
 */
bool isThreadJoined(Thread thread);
/*
 * Returns true if thread is currently running one.
 * thread - thread instance.
 */
bool isThreadCurrent(Thread thread);

/*
 * Sets current thread as main.
 */
void setMainThread();
/*
 * Returns true if current thread is main.
 */
bool isCurrentThreadMain();
/*
 * Returns true if thread is main.
 */
bool isThreadMain(Thread thread);

/*
 * Sets current thread name.
 * name - thread name string.
 * size - name buffer size.
 */
void getThreadName(char* name, size_t size);
/*
 * Returns current thread name.
 * name - thread name string.
 */
void setThreadName(const char* name);

/*
 * Sets current thread priority to foreground.
 */
void setThreadForegroundPriority();
/*
 * Sets current thread priority to background.
 */
void setThreadBackgroundPriority();