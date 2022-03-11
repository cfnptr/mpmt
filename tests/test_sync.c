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

#include "mpmt/sync.h"
#include "mpmt/thread.h"

#include <stdio.h>

#define TEST_BUFFER_SIZE 100000

typedef struct LockingData
{
	size_t index;
	int buffer[TEST_BUFFER_SIZE * 2];
	Mutex mutex;
} LockingData;

static void onLockingTest(void* argument)
{
	LockingData* data = (LockingData*)argument;
	lockMutex(data->mutex);

	int value = 0;

	for (size_t i = 0; i < TEST_BUFFER_SIZE; i++)
	{
		data->buffer[data->index++] = value;
		value += 3;
	}

	unlockMutex(data->mutex);
}

inline static bool testLocking()
{
	LockingData* data = calloc(1, sizeof(LockingData));

	if (!data)
	{
		printf("testLocking: failed to allocate data.");
		return false;
	}

	Mutex mutex = createMutex();

	if (!mutex)
	{
		printf("testLocking: failed to create mutex.");
		free(data);
		return false;
	}

	data->mutex = mutex;

	Thread firstThread = createThread(
		onLockingTest,
		data);

	if (!firstThread)
	{
		printf("testLocking: failed to create first thread.");
		destroyMutex(mutex);
		free(data);
		return false;
	}

	Thread secondThread = createThread(
		onLockingTest,
		data);

	if (!secondThread)
	{
		printf("testLocking: failed to create second thread.");
		joinThread(secondThread);
		destroyThread(secondThread);
		destroyMutex(mutex);
		free(data);
		return false;
	}

	joinThread(secondThread);
	destroyThread(secondThread);
	joinThread(firstThread);
	destroyThread(firstThread);
	destroyMutex(mutex);

	for (size_t i = 0; i < 2; i++)
	{
		int value = 0;

		for (size_t j = 0; j < TEST_BUFFER_SIZE; j++)
		{
			if (data->buffer[j] != value)
			{
				printf("testLocking: incorrect buffer value. (index: %zu)", j);
				return false;
			}

			value += 3;
		}
	}

	free(data);
	return true;
}
inline static bool testTryLock()
{
	Mutex mutex = createMutex();

	if (!mutex)
	{
		printf("testTryLock: failed to create mutex.");
		return false;
	}

	bool result = tryLockMutex(mutex);

	if (!result)
	{
		printf("testTryLock: failed to try lock mutex.");
		return false;
	}

	result = tryLockMutex(mutex);

	if (result)
	{
		printf("testTryLock: locked already locked mutex.");
		return false;
	}

	unlockMutex(mutex);
	destroyMutex(mutex);
	return true;
}

int main()
{
	bool result = testLocking();
	result &= testTryLock();
	return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
