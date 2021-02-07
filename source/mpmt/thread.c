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
	struct Thread* thread =
		(struct Thread*)argument;

	thread->function(
		thread->argument);

	return NULL;
}
#elif _WIN32
DWORD mpmtThreadFunction(LPVOID argument)
{
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

#if __linux__ || __APPLE__
	int result = pthread_create(
		&thread->handle,
		NULL,
		mpmtThreadFunction,
		thread);

	if (result != 0)
	{
		free(thread);
		return NULL;
	}
#elif _WIN32
	thread->handle = CreateThread(
		NULL,
		0,
		mpmtThreadFunction,
		thread,
		0,
		NULL);

	if (thread->handle == NULL)
	{
		free(thread);
		return NULL;
	}
#endif

	return thread;
}

void destroyThread(struct Thread* thread)
{
	if (thread == NULL)
		return;

	if (thread->joined == false)
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

void joinThread(struct Thread* thread)
{
	assert(thread != NULL);

	if (thread->joined == true)
		abort();

	thread->joined = true;

#if __linux__ || __APPLE__
	int result = pthread_join(
		thread->handle,
		NULL);

	if (result != 0)
		abort();
#elif _WIN32
	THREAD handle = thread->handle;

	DWORD waitResult = WaitForSingleObject(
		handle,
		INFINITE);

	if (waitResult != WAIT_OBJECT_0)
		abort();

	BOOL closeResult = CloseHandle(
		handle);

	if (closeResult != TRUE)
		abort();
#endif
}

bool isThreadJoined(
	struct Thread* thread)
{
	assert(thread != NULL);
	return thread->joined;
}

void sleepThread(
	size_t milliseconds)
{
#if __linux__ || __APPLE__
	struct timespec delay;

	delay.tv_sec =
		milliseconds / 1000;
	delay.tv_nsec =
		(long)(milliseconds % 1000) * 1000000;

	int result = nanosleep(
		&delay,
		NULL);

	if (result != 0)
		abort();
#elif _WIN32
	Sleep((DWORD)milliseconds);
#endif
}

uint64_t getCurrentClock()
{
#if __linux__ || __APPLE__
	struct timespec time;

	int result = clock_gettime(
		CLOCK_MONOTONIC,
		&time);

	if (result != 0)
		abort();

	return (time.tv_sec) * 1000 +
		(time.tv_nsec) / 1000000;
#elif _WIN32
	abort();

	// TODO: QueryPerformanceFrequency
#endif
}
