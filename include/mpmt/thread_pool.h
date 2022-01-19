// Copyright 2020-2022 Nikita Fediuchin. All rights reserved.
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
#include <stdlib.h>
#include <stdbool.h>

/*
 * Thread pool structure.
 */
typedef struct ThreadPool_T ThreadPool_T;
/*
 * Thread pool instance.
 */
typedef ThreadPool_T* ThreadPool;

/*
 * Creates a new thread pool instance.
 * Returns thread pool instance on success, otherwise NULL.
 *
 * threadCount - thread count in the pool.
 * taskCapacity - task pool capacity.
 */
ThreadPool createThreadPool(
	size_t threadCount,
	size_t taskCapacity);
/*
 * Destroys thread pool instance.
 * threadPool - thread pool instance or NULL.
 */
void destroyThreadPool(ThreadPool threadPool);

/*
 * Returns thread pool thread count.
 * threadPool - thread pool instance.
 */
size_t getThreadPoolThreadCount(
	ThreadPool threadPool);
/*
 * Returns thread pool task capacity.
 * threadPool - thread pool instance.
 */
size_t getThreadPoolTaskCapacity(
	ThreadPool threadPool);

/*
 * Add a new task to the thread pool.
 * Returns true if task successfully added.
 *
 * threadPool - thread pool instance.
 * function - pointer to the function that should be invoked.
 * argument - argument that will be passed to the function.
 */
bool addThreadPoolTask(
	ThreadPool threadPool,
	void (*function)(void*),
	void* argument);

/*
 * wait until thread pool has completed all tasks.
 * threadPool - pointer to the thread pool.
 */
void waitThreadPool(ThreadPool threadPool);
