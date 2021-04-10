#include "mpmt/sync.h"
#include "mpmt/thread.h"

#include <stdio.h>

typedef struct ThreadData
{
	Mutex* mutex;
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
	Mutex* mutex = createMutex();

	if (mutex == NULL)
		return EXIT_FAILURE;

	ThreadData* firstThreadData = malloc(
		sizeof(ThreadData));

	if (firstThreadData == NULL)
		return EXIT_FAILURE;

	firstThreadData->mutex = mutex;
	firstThreadData->threadIndex = 1;

	ThreadData* secondThreadData = malloc(
		sizeof(ThreadData));

	if (secondThreadData == NULL)
		return EXIT_FAILURE;

	secondThreadData->mutex = mutex;
	secondThreadData->threadIndex = 2;

	Thread* firstThread = createThread(
		threadFunction,
		firstThreadData);

	if (firstThread == NULL)
		return EXIT_FAILURE;

	Thread* secondThread = createThread(
		threadFunction,
		secondThreadData);

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
