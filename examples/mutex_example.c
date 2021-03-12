#include "mpmt/mutex.h"
#include "mpmt/thread.h"

#include <stdio.h>

struct ThreadData
{
	struct Mutex* mutex;
	int threadIndex;
};

static void threadFunction(void* argument)
{
	struct ThreadData* threadData =
		(struct ThreadData*)argument;

	lockMutex(threadData->mutex);

	printf("Thread index: %d\n",
		threadData->threadIndex);
	fflush(stdout);

	unlockMutex(threadData->mutex);
}

int main()
{
	struct Mutex* mutex = createMutex();

	if (mutex == NULL)
		return EXIT_FAILURE;

	struct ThreadData* firstThreadData = malloc(
		sizeof(struct ThreadData));

	if (firstThreadData == NULL)
		return EXIT_FAILURE;

	firstThreadData->mutex = mutex;
	firstThreadData->threadIndex = 1;

	struct ThreadData* secondThreadData = malloc(
		sizeof(struct ThreadData));

	if (secondThreadData == NULL)
		return EXIT_FAILURE;

	secondThreadData->mutex = mutex;
	secondThreadData->threadIndex = 2;

	struct Thread* firstThread = createThread(
		threadFunction,
		firstThreadData);

	if (firstThread == NULL)
		return EXIT_FAILURE;

	struct Thread* secondThread = createThread(
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
