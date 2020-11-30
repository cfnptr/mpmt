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
};

struct ThreadData
{
	void (*function)(void*);
	void* argument;
};

inline void* xmalloc(size_t size)
{
	void* p = malloc(size);
	if (p == NULL) abort();
	return p;
}

#if __linux__ || __APPLE__
void* mpmtThreadFunction(void* argument)
{
	assert(argument != NULL);

	struct ThreadData* data =
		(struct ThreadData*)argument;

	data->function(
		data->argument);

	free(data);
	return NULL;
}
#elif _WIN32
DWORD mpmtThreadFunction(LPVOID argument)
{
	assert(argument != NULL);

	struct ThreadData* data =
		(struct ThreadData*)argument;

	data->function(
		data->argument);

	free(data);
	return 0;
}
#endif

struct Thread* createThread(
	void (*function)(void*),
	void* argument)
{
	assert(function != NULL);

	struct Thread* thread =
		xmalloc(sizeof(struct Thread));
	struct ThreadData* data =
		xmalloc(sizeof(struct ThreadData));

	data->function = function;
	data->argument = argument;

	THREAD handle;

#if __linux__ || __APPLE__
	int result = pthread_create(
		&handle,
		NULL,
		mpmtThreadFunction,
		data);

	if (result != 0)
		abort();
#elif _WIN32
	handle = CreateThread(
		NULL,
		0,
		mpmtThreadFunction,
		data,
		0,
		NULL);

	if (handle == NULL)
		abort();
#endif

	thread->handle = handle;
	return thread;
}

void destroyThread(struct Thread* thread)
{
	free(thread);
}

void joinThread(struct Thread* thread)
{
	assert(thread != NULL);

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

	BOOL closeResult = CloseHandle(handle);

	if (closeResult != TRUE)
		abort();
#endif
}

void detachThread(struct Thread* thread)
{
	assert(thread != NULL);

#if __linux__ || __APPLE__
	int result = pthread_detach(thread->handle);

	if (result != 0)
		abort();
#elif _WIN32
	BOOL result = CloseHandle(thread->handle);

	if (result != TRUE)
		abort();
#endif
}

void sleepThread(size_t milliseconds)
{
#if __linux__ || __APPLE__
	struct timespec delay;

	delay.tv_sec =
		(long)milliseconds / 1000;
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
