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

/***********************************************************************************************************************
 * @file
 * @brief Thread pool functions.
 * 
 * @details
 * A thread pool is a programming concept and design pattern used in concurrent programming to 
 * efficiently manage and reuse a pool of worker threads. Instead of creating a new thread for each task, 
 * a thread pool maintains a fixed number of threads that are created and initialized upfront, 
 * ready to execute tasks concurrently. This helps to reduce the overhead associated with creating and 
 * destroying threads for every individual task, resulting in better performance and resource utilization.
 */

#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Task order types.
 */
typedef enum TaskOrder_T
{
	STACK_TASK_ORDER = 0, // Faster than queue
	QUEUE_TASK_ORDER = 1,
	TASK_ORDER_COUNT = 2,
} TaskOrder_T;
/**
 * @brief Task order type.
 */
typedef uint8_t TaskOrder;

/**
 * @brief Thread pool task structure.
 */
typedef struct ThreadPoolTask
{
	void (*function)(void*);
	void* argument;
} ThreadPoolTask;

/**
 * @brief Thread pool structure.
 */
typedef struct ThreadPool_T ThreadPool_T;
/**
 * @brief Thread pool instance.
 */
typedef ThreadPool_T* ThreadPool;

/***********************************************************************************************************************
 * @brief Creates a new thread pool instance.
 * @note You should destroy created thread pool instance manually.
 * 
 * @details
 * Internally it allocates the necessary mutexes, condvars and arrays.
 * It also creates and starts the specified number of threads.
 *
 * @param threadCount target thread count in the pool
 * @param taskCapacity task buffer size
 * @param taskOrder task order type
 * 
 * @return Thread pool instance on success, otherwise NULL.
 */
ThreadPool createThreadPool(size_t threadCount, size_t taskCapacity, TaskOrder taskOrder);

/**
 * @brief Destroys thread pool instance. (Blocking)
 * @param threadPool thread pool instance or NULL
 */
void destroyThreadPool(ThreadPool threadPool);

/**
 * @brief Returns thread pool thread count.
 * @param threadPool thread pool instance
 */
size_t getThreadPoolThreadCount(ThreadPool threadPool);

/**
 * @brief Returns thread pool task capacity.
 * @param threadPool thread pool instance
 */
size_t getThreadPoolTaskCapacity(ThreadPool threadPool);

/**
 * @brief Returns true if any thread is running task.
 * @param threadPool thread pool instance
 */
bool isThreadPoolRunning(ThreadPool threadPool);

/**
 * @brief Returns thread pool task order type.
 * @param threadPool thread pool instance
 */
TaskOrder getThreadPoolTaskOrder(ThreadPool threadPool);

/**
 * @brief Sets thread pool task order type. (Blocking)
 *
 * @param threadPool thread pool instance
 * @param taskOrder task order type
 */
void setThreadPoolTaskOrder(ThreadPool threadPool, TaskOrder taskOrder);

/**
 * @brief Resize thread pool task buffer. (Blocking)
 *
 * @param threadPool thread pool instance
 * @param taskCapacity task buffer size
 * 
 * @return True on success, otherwise false.
 */
bool resizeThreadPoolTasks(ThreadPool threadPool, size_t taskCapacity);

/***********************************************************************************************************************
 * @brief Adds a new task to the thread pool, if enough space.
 * 
 * @param threadPool thread pool instance
 * @param task target thread pool task
 * 
 * @return True if task successfully added, otherwise false.
 */
bool tryAddThreadPoolTask(ThreadPool threadPool, ThreadPoolTask task);

/**
 * @brief Adds a new task to the thread pool. (Blocking)
 *
 * @param threadPool thread pool instance
 * @param task target thread pool task
 */
void addThreadPoolTask(ThreadPool threadPool, ThreadPoolTask task);

/**
 * @brief Adds a new tasks to the thread pool. (Blocking)
 *
 * @param threadPool thread pool instance
 * @param[in,out] tasks target thread pool tasks
 * @param taskCount task array size
 */
void addThreadPoolTasks(ThreadPool threadPool, ThreadPoolTask* tasks, size_t taskCount);

/**
 * @brief Adds a new tasks to the thread pool. (Blocking)
 *
 * @param threadPool thread pool instance.
 * @param task target thread pool task.
 * @param taskCount task count
 */
void addThreadPoolTaskNumber(ThreadPool threadPool, ThreadPoolTask task, size_t taskCount);

/**
 * @brief Waits until the thread pool has completed all tasks. (Blocking)
 * @param threadPool thread pool instance.
 */
void waitThreadPool(ThreadPool threadPool);