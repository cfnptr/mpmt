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

#include "mpmt/thread_pool.h"
#include "mpmt/sync.h"
#include "mpmt/thread.h"

struct ThreadPool_T
{
	Mutex mutex;
	Cond workCond;
	Cond workingCond;
	ThreadPoolTask* tasks;
	size_t taskCapacity;
	size_t taskCount;
	Thread* threads;
	size_t threadCount;
	size_t workingCount;
	TaskOrder taskOrder;
	bool isRunning;
};

static void onThreadUpdate(void* argument)
{
	ThreadPool threadPool = argument;
	Mutex mutex = threadPool->mutex;
	Cond workCond = threadPool->workCond;
	Cond workingCond = threadPool->workingCond;

	while (true)
	{
		lockMutex(mutex);

		size_t taskCount = threadPool->taskCount;

		while (taskCount == 0)
		{
			if (!threadPool->isRunning)
			{
				unlockMutex(mutex);
				return;
			}

			waitCond(workCond, mutex);
			taskCount = threadPool->taskCount;
		}

		threadPool->workingCount++;
		threadPool->taskCount--;

		ThreadPoolTask* tasks = threadPool->tasks;
		TaskOrder taskOrder = threadPool->taskOrder;

		ThreadPoolTask task;

		if (taskOrder == STACK_TASK_ORDER_TYPE)
		{
			task = tasks[taskCount - 1];
		}
		else if (taskOrder == QUEUE_TASK_ORDER_TYPE)
		{
			task = tasks[0];

			for (size_t i = 1; i < taskCount; i++)
				tasks[i - 1] = tasks[i];
		}
		else
		{
			abort();
		}

		unlockMutex(mutex);
		task.function(task.argument);
		lockMutex(mutex);

		threadPool->workingCount--;

		if (!threadPool->isRunning)
		{
			unlockMutex(mutex);
			return;
		}

		broadcastCond(workingCond);
		unlockMutex(mutex);
	}
}
ThreadPool createThreadPool(
	size_t threadCount,
	size_t taskCapacity,
	TaskOrder taskOrder)
{
	assert(threadCount);
	assert(taskOrder < TASK_ORDER_TYPE_COUNT);
	assert(taskCapacity >= threadCount);

	ThreadPool threadPool = calloc(1,
		sizeof(ThreadPool_T));

	if (!threadPool)
		return NULL;

	threadPool->workingCount = 0;
	threadPool->taskOrder = taskOrder;
	threadPool->isRunning = true;

	Mutex mutex = createMutex();

	if (!mutex)
	{
		destroyThreadPool(threadPool);
		return NULL;
	}

	threadPool->mutex = mutex;

	Cond workCond = createCond();

	if (!workCond)
	{
		destroyThreadPool(threadPool);
		return NULL;
	}

	threadPool->workCond = workCond;

	Cond workingCond = createCond();

	if (!workingCond)
	{
		destroyThreadPool(threadPool);
		return NULL;
	}

	threadPool->workingCond = workingCond;

	ThreadPoolTask* tasks = malloc(
		taskCapacity * sizeof(ThreadPoolTask));

	if (!tasks)
	{
		destroyThreadPool(threadPool);
		return NULL;
	}

	threadPool->tasks = tasks;
	threadPool->taskCapacity = taskCapacity;
	threadPool->taskCount = 0;

	Thread* threads = calloc(
		threadCount,
		sizeof(Thread));

	if (!threads)
	{
		destroyThreadPool(threadPool);
		return NULL;
	}

	threadPool->threads = threads;
	threadPool->threadCount = threadCount;

	for (size_t i = 0; i < threadCount; i++)
	{
		Thread thread = createThread(
			onThreadUpdate,
			threadPool);

		if (!thread)
		{
			destroyThreadPool(threadPool);
			return NULL;
		}

		threads[i] = thread;
	}

	return threadPool;
}
void destroyThreadPool(ThreadPool threadPool)
{
	if (!threadPool)
		return;

	Thread* threads = threadPool->threads;
	size_t threadCount = threadPool->threadCount;

	if (threads)
	{
		Mutex mutex = threadPool->mutex;
		lockMutex(mutex);
		threadPool->isRunning = false;
		broadcastCond(threadPool->workCond);
		unlockMutex(mutex);

		for (size_t i = 0; i < threadCount; i++)
		{
			Thread thread = threads[i];

			if (!thread)
				continue;

			joinThread(thread);
			destroyThread(thread);
		}

		free(threads);
	}

	free(threadPool->tasks);
	destroyCond(threadPool->workingCond);
	destroyCond(threadPool->workCond);
	destroyMutex(threadPool->mutex);
	free(threadPool);
}

size_t getThreadPoolThreadCount(ThreadPool threadPool)
{
	assert(threadPool);
	return threadPool->threadCount;
}
size_t getThreadPoolTaskCapacity(ThreadPool threadPool)
{
	assert(threadPool);
	return threadPool->taskCapacity;
}
bool isThreadPoolRunning(ThreadPool threadPool)
{
	Mutex mutex = threadPool->mutex;
	lockMutex(mutex);
	bool isRunning = threadPool->taskCount || threadPool->workingCount;
	unlockMutex(mutex);
	return isRunning;
}

TaskOrder getThreadPoolTaskOrder(
	ThreadPool threadPool)
{
	assert(threadPool);
	return threadPool->taskOrder;
}
void setThreadPoolTaskOrder(
	ThreadPool threadPool,
	TaskOrder taskOrder)
{
	assert(threadPool);
	waitThreadPool(threadPool);
	threadPool->taskOrder = taskOrder;
}

bool resizeThreadPoolTasks(
	ThreadPool threadPool,
	size_t taskCapacity)
{
	assert(threadPool);
	assert(taskCapacity > 0);

	waitThreadPool(threadPool);

	ThreadPoolTask* tasks = realloc(
		threadPool->tasks,
		taskCapacity * sizeof(ThreadPoolTask));

	if (!tasks)
		return false;

	threadPool->tasks = tasks;
	threadPool->taskCapacity = taskCapacity;
	return true;
}

bool tryAddThreadPoolTask(
	ThreadPool threadPool,
	ThreadPoolTask task)
{
	assert(threadPool);
	assert(task.function);

	Mutex mutex = threadPool->mutex;
	lockMutex(mutex);

	size_t taskCount = threadPool->taskCount;

	if (taskCount == threadPool->taskCapacity)
	{
		unlockMutex(mutex);
		return false;
	}

	threadPool->tasks[taskCount] = task;
	threadPool->taskCount++;
	signalCond(threadPool->workCond);

	unlockMutex(mutex);
	return true;
}
void addThreadPoolTask(
	ThreadPool threadPool,
	ThreadPoolTask task)
{
	assert(threadPool);
	assert(task.function);

	Mutex mutex = threadPool->mutex;
	Cond workingCond = threadPool->workingCond;
	size_t taskCapacity = threadPool->taskCapacity;

	lockMutex(mutex);

	while (threadPool->taskCount == taskCapacity)
		waitCond(workingCond, mutex);

	threadPool->tasks[threadPool->taskCount++] = task;
	signalCond(threadPool->workCond);

	unlockMutex(mutex);
}
void addThreadPoolTasks(
	ThreadPool threadPool,
	ThreadPoolTask* tasks,
	size_t taskCount)
{
	assert(threadPool);
	assert(tasks);
	assert(taskCount > 0);

#ifndef NDEBUG
	for (size_t i = 0; i < taskCount; i++)
		assert(tasks[i].function);
#endif

	Mutex mutex = threadPool->mutex;
	Cond workingCond = threadPool->workingCond;
	ThreadPoolTask* taskArray = threadPool->tasks;
	size_t taskCapacity = threadPool->taskCapacity;

	lockMutex(mutex);

	for (size_t i = 0; i < taskCount; i++)
	{
		while (threadPool->taskCount == taskCapacity)
			waitCond(workingCond, mutex);

		size_t taskArrayCount = threadPool->taskCount;

		while(taskArrayCount < taskCapacity && i < taskCount)
			taskArray[taskArrayCount++] = tasks[i++];

		threadPool->taskCount = taskArrayCount;
		signalCond(threadPool->workCond);
	}

	unlockMutex(mutex);
}
void addThreadPoolTaskNumber(
	ThreadPool threadPool,
	ThreadPoolTask task,
	size_t taskCount)
{
	assert(threadPool);
	assert(task.function);
	assert(taskCount > 0);

	Mutex mutex = threadPool->mutex;
	Cond workingCond = threadPool->workingCond;
	ThreadPoolTask* taskArray = threadPool->tasks;
	size_t taskCapacity = threadPool->taskCapacity;

	lockMutex(mutex);

	for (size_t i = 0; i < taskCount; i++)
	{
		while (threadPool->taskCount == taskCapacity)
			waitCond(workingCond, mutex);

		size_t taskArrayCount = threadPool->taskCount;

		while(taskArrayCount < taskCapacity && i < taskCount)
		{
			taskArray[taskArrayCount++] = task;
			i++;
		}

		threadPool->taskCount = taskArrayCount;
		signalCond(threadPool->workCond);
	}

	unlockMutex(mutex);
}

void waitThreadPool(ThreadPool threadPool)
{
	assert(threadPool);

	Mutex mutex = threadPool->mutex;
	Cond workingCond = threadPool->workingCond;

	lockMutex(mutex);

	while (threadPool->taskCount || threadPool->workingCount)
		waitCond(workingCond, mutex);

	unlockMutex(mutex);
}
