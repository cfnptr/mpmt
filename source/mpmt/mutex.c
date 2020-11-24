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

struct Mutex* mpmtCreateMutex()
{
	MUTEX handle;

#if __linux__ || __APPLE__
	int result = pthread_mutex_init(
		&handle,
		NULL);

	if(result != 0)
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

void mpmtDestroyMutex(
	struct Mutex* mutex)
{
	if(mutex)
	{
#if __linux__ || __APPLE__
		pthread_mutex_destroy(
			&mutex->handle);
#elif _WIN32
		CloseHandle(
			mutex->handle);
#endif
	}

	free(mutex);
}

bool mpmtLockMutex(
	struct Mutex* mutex)
{
	if(!mutex)
		return false;

#if __linux__ || __APPLE__
	return pthread_mutex_lock(
		&mutex->handle) == 0;
#elif _WIN32
	return WaitForSingleObject(
		mutex->handle,
		INFINITE) == WAIT_OBJECT_0;
#endif
}
bool mpmtUnlockMutex(
	struct Mutex* mutex)
{
	if(!mutex)
		return false;
#if __linux__ || __APPLE__
	return pthread_mutex_unlock(
		&mutex->handle) == 0;
#elif _WIN32
	return ReleaseMutex(
		mutex->handle) == TRUE;
#endif
}
bool mpmtTryLockMutex(
	struct Mutex* mutex)
{
	if(!mutex)
		return false;
#if __linux__ || __APPLE__
	return pthread_mutex_trylock(
		&mutex->handle) == 0;
#elif _WIN32
	return WaitForSingleObject(
		mutex->handle,
		0) == WAIT_OBJECT_0;
#endif
}

