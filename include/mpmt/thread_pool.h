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
#include <stdint.h>
#include <stdbool.h>

/*
 * Task order types.
 */
typedef enum TaskOrder_T
{
	STACK_TASK_ORDER = 0, // Faster than queue
	QUEUE_TASK_ORDER = 1,
	TASK_ORDER_COUNT = 2,
} TaskOrder_T;
/*
 * Task order type.
 */
typedef uint8_t TaskOrder;

/*
 * Thread pool task structure.
 */
typedef struct ThreadPoolTask
{
	void (*function)(void*);
	void* argument;
} ThreadPoolTask;

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
 * taskCapacity - task array size.
 * taskOrder - task order type.
 */
ThreadPool createThreadPool(
	size_t threadCount,
	size_t taskCapacity,
	TaskOrder taskOrder);
/*
 * Destroys thread pool instance. (Blocking)
 * threadPool - thread pool instance or NULL.
 */
void destroyThreadPool(ThreadPool threadPool);

/*
 * Returns thread pool thread count.
 * threadPool - thread pool instance.
 */
size_t getThreadPoolThreadCount(ThreadPool threadPool);
/*
 * Returns thread pool task capacity.
 * threadPool - thread pool instance.
 */
size_t getThreadPoolTaskCapacity(ThreadPool threadPool);
/*
 * Returns true if thread is running tasks.
 * threadPool - thread pool instance.
 */
bool isThreadPoolRunning(ThreadPool threadPool);

/*
 * Returns thread pool task order type.
 * threadPool - thread pool instance.
 */
TaskOrder getThreadPoolTaskOrder(
	ThreadPool threadPool);
/*
 * Sets thread pool task order type. (Blocking)
 *
 * threadPool - thread pool instance.
 * taskOrder - task order type.
 */
void setThreadPoolTaskOrder(
	ThreadPool threadPool,
	TaskOrder taskOrder);

/*
 * Resize thread pool task array. (Blocking)
 * Returns true on success.
 *
 * threadPool - thread pool instance.
 * taskCapacity - task array size.
 */
bool resizeThreadPoolTasks(
	ThreadPool threadPool,
	size_t taskCapacity);

/*
 * Try to add a new task to the thread pool.
 * Returns true if task successfully added.
 *
 * threadPool - thread pool instance.
 * task - thread pool task.
 */
bool tryAddThreadPoolTask(
	ThreadPool threadPool,
	ThreadPoolTask task);
/*
 * Adds a new task to the thread pool. (Blocking)
 *
 * threadPool - thread pool instance.
 * task - thread pool task.
 */
void addThreadPoolTask(
	ThreadPool threadPool,
	ThreadPoolTask task);
/*
 * Adds a new tasks to the thread pool. (Blocking)
 *
 * threadPool - thread pool instance.
 * task - thread pool task array.
 * taskCount - task array size.
 */
void addThreadPoolTasks(
	ThreadPool threadPool,
	ThreadPoolTask* tasks,
	size_t taskCount);
/*
 * Adds a new task number to the thread pool. (Blocking)
 *
 * threadPool - thread pool instance.
 * task - thread pool task.
 * taskCount - task count.
 */
void addThreadPoolTaskNumber(
	ThreadPool threadPool,
	ThreadPoolTask task,
	size_t taskCount);

/*
 * Wait until thread pool has completed all tasks. (Blocking)
 * threadPool - thread pool instance.
 */
void waitThreadPool(ThreadPool threadPool);
