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

#include "mpmt/sync.h"
#include "mpmt/thread.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct ThreadData
{
	Mutex mutex;
	int threadIndex;
} ThreadData;

static void threadFunction(void* argument)
{
	ThreadData* threadData = (ThreadData*)argument;

	lockMutex(threadData->mutex);

	printf("Thread index: %d\n",
		threadData->threadIndex);
	fflush(stdout);

	unlockMutex(threadData->mutex);
}

int main()
{
	Mutex mutex = createMutex();

	if (mutex == NULL)
		return EXIT_FAILURE;

	ThreadData* firstThreadData = malloc(sizeof(ThreadData));

	if (firstThreadData == NULL)
		return EXIT_FAILURE;

	firstThreadData->mutex = mutex;
	firstThreadData->threadIndex = 1;

	ThreadData* secondThreadData = malloc(sizeof(ThreadData));

	if (secondThreadData == NULL)
		return EXIT_FAILURE;

	secondThreadData->mutex = mutex;
	secondThreadData->threadIndex = 2;

	Thread firstThread = createThread(threadFunction, firstThreadData);

	if (firstThread == NULL)
		return EXIT_FAILURE;

	Thread secondThread = createThread(threadFunction, secondThreadData);

	if (secondThread == NULL)
		return EXIT_FAILURE;

	joinThread(secondThread);
	destroyThread(secondThread);

	joinThread(firstThread);
	destroyThread(firstThread);

	free(secondThreadData);
	free(firstThreadData);
	destroyMutex(mutex);
	return EXIT_SUCCESS;
}
