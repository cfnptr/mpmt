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
#include "mpmt/atomic.h"

typedef struct PoolTask
{
	void (*function)(void*);
	void* argument;
} PoolTask;

struct ThreadPool_T
{
	Mutex mutex;
	Cond workCond;
	Cond workingCond;
	PoolTask* tasks;
	size_t taskCapacity;
	size_t taskCount;
	Thread* threads;
	size_t threadCount;
	size_t workingCount;
	bool isRunning;
};

static void onThreadUpdate(void* argument)
{
	ThreadPool threadPool = argument;
	Mutex mutex = threadPool->mutex;
	Cond workCond = threadPool->workCond;
	Cond workingCond = threadPool->workingCond;
	PoolTask* tasks = threadPool->tasks;

	lockMutex(mutex);

	while (true)
	{
		lockMutex(mutex);

		while (!threadPool->taskCount)
		{
			if (!threadPool->isRunning)
			{
				unlockMutex(mutex);
				return;
			}

			waitCond(workCond, mutex);
		}

		threadPool->taskCount--;
		threadPool->workingCount++;
		PoolTask task = tasks[threadPool->taskCount];

		unlockMutex(mutex);

		task.function(task.argument);

		lockMutex(mutex);

		threadPool->workingCount--;

		if (!threadPool->isRunning)
		{
			unlockMutex(mutex);
			return;
		}

		if (threadPool->workingCount == 0)
			broadcastCond(workingCond);

		unlockMutex(mutex);
	}
}
ThreadPool createThreadPool(
	size_t threadCount,
	size_t taskCapacity)
{
	assert(threadCount);
	assert(taskCapacity);

	ThreadPool threadPool = calloc(1,
		sizeof(ThreadPool_T));

	if (!threadPool)
		return NULL;

	threadPool->workingCount = 0;
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

	PoolTask* tasks = malloc(
		taskCapacity * sizeof(PoolTask));

	if (!tasks)
	{
		destroyThreadPool(threadPool);
		return NULL;
	}

	threadPool->tasks = tasks;
	threadPool->taskCapacity = taskCapacity;
	threadPool->taskCount = 0;

	Thread* threads = calloc(1,
		threadCount * sizeof(Thread));

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

bool addThreadPoolTask(
	ThreadPool threadPool,
	void (*function)(void*),
	void* argument)
{
	assert(threadPool);
	assert(function);

	Mutex mutex = threadPool->mutex;
	lockMutex(mutex);

	size_t taskCount = threadPool->taskCount;

	if (taskCount == threadPool->taskCapacity)
	{
		unlockMutex(mutex);
		return false;
	}

	PoolTask task = {
		function,
		argument,
	};

	threadPool->tasks[taskCount] = task;
	threadPool->taskCount++;

	signalCond(threadPool->workCond);
	unlockMutex(mutex);
	return true;
}

void waitThreadPool(ThreadPool threadPool)
{
	assert(threadPool);

	Mutex mutex = threadPool->mutex;
	Cond workingCond = threadPool->workingCond;

	lockMutex(mutex);

	while (true)
	{
		if (!threadPool->taskCount && !threadPool->workingCount)
			break;

		waitCond(workingCond, mutex);
	}

	unlockMutex(mutex);
}
