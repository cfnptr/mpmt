#include "mpmt/mutex.h"
#include <stdlib.h>

#if __linux__ || __APPLE__
#include <pthread.h>
#define MUTEX pthread_mutex_t
#elif _WIN32
#include <windows.h>
#define MUTEX HANDLE
#else
#error Unknown operating system
#endif

struct Mutex
{
	MUTEX handle;
};

struct Mutex* createMutex()
{
	MUTEX handle;

#if __linux__ || __APPLE__
	int result = pthread_mutex_init(
		&handle,
		NULL);

	if (result != 0)
		return NULL;
#elif _WIN32
	handle = CreateMutex(
		NULL,
		FALSE,
		NULL);

	if (handle == NULL)
		return NULL;
#endif

	struct Mutex* mutex =
		malloc(sizeof(struct Mutex));

	if (!mutex)
		return NULL;

	mutex->handle = handle;
	return mutex;
}

void destroyMutex(struct Mutex* mutex)
{
	if (mutex)
	{
#if __linux__ || __APPLE__
		int result = pthread_mutex_destroy(&mutex->handle);

		if(result != 0)
			abort();
#elif _WIN32
		BOOL result = CloseHandle(mutex->handle);

		if(result != TRUE)
			abort();
#endif
	}

	free(mutex);
}

void lockMutex(struct Mutex* mutex)
{
	if (!mutex)
		abort();

#if __linux__ || __APPLE__
	int result = pthread_mutex_lock(&mutex->handle);

	if(result != 0)
		abort();
#elif _WIN32
	DWORD result = WaitForSingleObject(
		mutex->handle,
		INFINITE);

	if(result != WAIT_OBJECT_0)
		abort();
#endif
}

void unlockMutex(struct Mutex* mutex)
{
	if (!mutex)
		abort();

#if __linux__ || __APPLE__
	int result = pthread_mutex_unlock(&mutex->handle);

	if(result != 0)
		abort();
#elif _WIN32
	BOOL result = ReleaseMutex(mutex->handle);

	if(result != TRUE)
		abort();
#endif
}

bool tryLockMutex(struct Mutex* mutex)
{
	if (!mutex)
		abort();

#if __linux__ || __APPLE__
	return pthread_mutex_trylock(
		&mutex->handle) == 0;
#elif _WIN32
	return WaitForSingleObject(
		mutex->handle,
		0) == WAIT_OBJECT_0;
#endif
}
