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
static void* threadFunction(void* argument)
{
	Thread thread = (Thread)argument;
	thread->function(thread->argument);
	return NULL;
}
#elif _WIN32
DWORD threadFunction(LPVOID argument)
{
	Thread thread = (Thread)argument;
	thread->function(thread->argument);
	return 0;
}
#endif

Thread createThread(
	void (*function)(void*),
	void* argument)
{
	assert(function != NULL);

	Thread thread = malloc(
		sizeof(struct Thread));

	if(thread == NULL)
		return NULL;

	thread->joined = false;
	thread->function = function;
	thread->argument = argument;

#if __linux__ || __APPLE__
	int result = pthread_create(
		&thread->handle,
		NULL,
		threadFunction,
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
		threadFunction,
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

void destroyThread(Thread thread)
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

void joinThread(Thread thread)
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

bool isThreadJoined(Thread thread)
{
	assert(thread != NULL);
	return thread->joined;
}

void sleepThread(double _delay)
{
	assert(_delay >= 0.0);

#if __linux__ || __APPLE__
	struct timespec delay;

	delay.tv_sec = (time_t)_delay;

	delay.tv_nsec = (long)(
		(_delay - (double)delay.tv_sec) *
		1000000000.0);

	int result = nanosleep(
		&delay,
		NULL);

	if (result != 0)
		abort();
#elif _WIN32
	Sleep((DWORD)(_delay * 1000.0));
#endif
}

bool yieldThread()
{
#if __linux__ || __APPLE__
	return sched_yield() == 0;
#elif _WIN32
	return SwitchToThread() == TRUE;
#endif
}

double getCurrentClock()
{
#if __linux__ || __APPLE__
	struct timespec time;

	int result = clock_gettime(
		CLOCK_MONOTONIC,
		&time);

	if (result != 0)
		abort();

	return (double)time.tv_sec +
		(double)time.tv_nsec / 1000000000.0;
#elif _WIN32
	LARGE_INTEGER frequency;

	BOOL result = QueryPerformanceFrequency(
		&frequency);

	if (result != TRUE)
		abort();

	LARGE_INTEGER counter;

	result = QueryPerformanceCounter(
		&counter);

	if (result != TRUE)
		abort();

	return (double)counter.QuadPart /
		(double)frequency.QuadPart;
#endif
}
