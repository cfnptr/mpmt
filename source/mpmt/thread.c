#include "mpmt/thread.h"
#include <stdlib.h>

#if __linux__ || __APPLE__
#include <pthread.h>
#define THREAD pthread_t
#elif _WIN32
// TODO:
#else
#error Unknown operating system
#endif

struct Thread
{
	THREAD handle;
};

struct Thread* mpmtCreateThread(
	void* (*routine)(void*),
	void* argument)
{
	if(!routine)
		return NULL;

	THREAD handle;

	int result = pthread_create(
		&handle,
		NULL,
		routine,
		argument);

	if(result != 0)
		return NULL;

	struct Thread* thread =
		malloc(sizeof(struct Thread));
	thread->handle = handle;
	return thread;
}
void mpmtDestroyThread(
	struct Thread* thread)
{
	free(thread);
}

bool mpmtJoinThread(
	struct Thread* thread,
	void** _result)
{
	if(!thread)
		return false;

	if(_result)
	{
		void* threadResult;

		int result = pthread_join(
			thread->handle,
			&threadResult);

		if(result != 0)
			return false;

		*_result = threadResult;
	}
	else
	{
		int result = pthread_join(
			thread->handle,
			NULL);

		if(result != 0)
			return false;
	}

	return true;
}
bool mpmtDetachThread(
	struct Thread* thread)
{
	if(!thread)
		return false;

	return pthread_detach(
		thread->handle);
}
void mpmtExitThread(
	void* result)
{
	pthread_exit(result);
}
