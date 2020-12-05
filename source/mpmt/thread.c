#include "mpmt/thread.h"

#include <stdlib.h>
#include <assert.h>

#if __linux__ || __APPLE__
#include <pthread.h>
#define THREAD pthread_t
#elif _WIN32
#include <windows.h>
#define THREAD HANDLE
#else
#error Unknown operating system
#endif

struct Thread
{
	THREAD handle;
	bool joined;
	void (*function)(void*);
	void* argument;
};

#if __linux__ || __APPLE__
void* mpmtThreadFunction(void* argument)
{
	assert(argument != NULL);

	struct Thread* thread =
		(struct Thread*)argument;

	thread->function(
		thread->argument);

	return NULL;
}
#elif _WIN32
DWORD mpmtThreadFunction(LPVOID argument)
{
	assert(argument != NULL);

	struct Thread* thread =
		(struct Thread*)argument;

	thread->function(
		thread->argument);

	return 0;
}
#endif

struct Thread* createThread(
	void (*function)(void*),
	void* argument)
{
	assert(function != NULL);

	struct Thread* thread =
		malloc(sizeof(struct Thread));

	if(thread == NULL)
		return NULL;

	thread->joined = false;
	thread->function = function;
	thread->argument = argument;

	THREAD handle;

#if __linux__ || __APPLE__
	int result = pthread_create(
		&handle,
		NULL,
		mpmtThreadFunction,
		thread);

	if (result != 0)
	{
		free(thread);
		return NULL;
	}
#elif _WIN32
	handle = CreateThread(
		NULL,
		0,
		mpmtThreadFunction,
		thread,
		0,
		NULL);

	if (handle == NULL)
	{
		free(thread);
		return NULL;
	}
#endif

	thread->handle = handle;
	return thread;
}

void destroyThread(struct Thread* thread)
{
	if(thread != NULL && thread->joined == false)
	{
#if __linux__ || __APPLE__
		int result = pthread_detach(
			thread->handle);

		if (result != 0)
			abort();
#elif _WIN32
		BOOL result = CloseHandle(
			thread->handle);

		if (result != TRUE)
			abort();
#endif
	}

	free(thread);
}

bool joinThread(struct Thread* thread)
{
	assert(thread != NULL);

	if(thread->joined)
		return false;

	thread->joined = true;

#if __linux__ || __APPLE__
	return pthread_join(
		thread->handle,
		NULL) == 0;
#elif _WIN32
	THREAD handle = thread->handle;

	bool result = WaitForSingleObject(
		handle,
		INFINITE) == WAIT_OBJECT_0;

	result &= CloseHandle(
		handle) == TRUE;

	return result;
#endif
}

bool isThreadJoined(struct Thread* thread)
{
	assert(thread != NULL);
	return thread->joined;
}

void sleepThread(size_t milliseconds)
{
#if __linux__ || __APPLE__
	struct timespec delay;

	delay.tv_sec =
		(long)milliseconds / 1000;
	delay.tv_nsec =
		(long)(milliseconds % 1000) * 1000000;

	nanosleep(
		&delay,
		NULL);
#elif _WIN32
	Sleep((DWORD)milliseconds);
#endif
}
