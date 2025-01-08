// Copyright 2020-2025 Nikita Fediuchin. All rights reserved.
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

#include "mpmt/thread.h"
#include "mpmt/thread_pool.h"

#include <stdio.h>
#include <stdlib.h>

#define TEST_THREAD_COUNT 4

static void onBlockingTest(void* argument)
{
	sleepThread(0.1);
}

inline static bool testAddBlocking()
{
	ThreadPool threadPool = createThreadPool(
		TEST_THREAD_COUNT, TEST_THREAD_COUNT, STACK_TASK_ORDER);

	if (!threadPool)
	{
		printf("testAddBlocking: failed to create thread pool.");
		return false;
	}

	ThreadPoolTask task;
	task.argument = NULL;

	for (size_t i = 0; i < TEST_THREAD_COUNT * 2; i++)
	{
		task.function = onBlockingTest;
		addThreadPoolTask(threadPool, task);
	}

	destroyThreadPool(threadPool);
	return true;
}
inline static bool testTryAdd()
{
	ThreadPool threadPool = createThreadPool(1, 1, STACK_TASK_ORDER);

	if (!threadPool)
	{
		printf("testTryAdd: failed to create thread pool.");
		return false;
	}

	ThreadPoolTask task = { onBlockingTest, NULL };
	bool result = tryAddThreadPoolTask(threadPool, task);

	if (!result)
	{
		printf("testTryAdd: failed to try add thread pool task.");
		destroyThreadPool(threadPool);
		return false;
	}

	result = tryAddThreadPoolTask(threadPool, task);
	destroyThreadPool(threadPool);

	if (result)
	{
		printf("testTryAdd: added task to already full thread pool.");
		return false;
	}

	return true;
}

int main()
{
	bool result = testAddBlocking();
	result &= testTryAdd();
	return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
