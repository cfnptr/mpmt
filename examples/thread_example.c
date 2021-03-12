#include "mpmt/thread.h"
#include <stdio.h>

static void threadFunction(void* argument)
{
	printf("Thread index: %s\n",
		(const char*)argument);
	fflush(stdout);
}

int main()
{
	struct Thread* firstThread = createThread(
		threadFunction,
		"1");

	if (firstThread == NULL)
		return EXIT_FAILURE;

	struct Thread* secondThread = createThread(
		threadFunction,
		"2");

	if (secondThread == NULL)
		return EXIT_FAILURE;

	joinThread(secondThread);
	destroyThread(secondThread);

	joinThread(firstThread);
	destroyThread(firstThread);
	return EXIT_SUCCESS;
}
