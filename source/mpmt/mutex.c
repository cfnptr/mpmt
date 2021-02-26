#include "mpmt/mutex.h"

#include <stdlib.h>
#include <assert.h>

#if __linux__ || __APPLE__
#include <pthread.h>
#define MUTEX pthread_mutex_t
#elif _WIN32
#include <windows.h>
#define MUTEX CRITICAL_SECTION
#else
#error Unknown operating system
#endif

struct Mutex
{
	MUTEX handle;
#ifndef NDEBUG
	bool isLocked;
#endif
};

struct Mutex* createMutex()
{
	struct Mutex* mutex = malloc(
		sizeof(struct Mutex));

	if (mutex == NULL)
		return NULL;

#if __linux__ || __APPLE__
	int result = pthread_mutex_init(
		&mutex->handle,
		NULL);

	if (result != 0)
	{
		free(mutex);
		return NULL;
	}
#elif _WIN32
	InitializeCriticalSection(
		&mutex->handle);
#endif

#ifndef NDEBUG
	mutex->isLocked = false;
#endif
	return mutex;
}

void destroyMutex(struct Mutex* mutex)
{
#ifndef NDEBUG
	assert(mutex->isLocked == false);
#endif

	if (mutex == NULL)
		return;

#if __linux__ || __APPLE__
	int result = pthread_mutex_destroy(
		&mutex->handle);

	if (result != 0)
		abort();
#elif _WIN32
	DeleteCriticalSection(
		&mutex->handle);
#endif

	free(mutex);
}

void lockMutex(
	struct Mutex* mutex)
{
	assert(mutex != NULL);

#if __linux__ || __APPLE__
	int result = pthread_mutex_lock(
		&mutex->handle);

	if (result != 0)
		abort();
#elif _WIN32
	EnterCriticalSection(
		&mutex->handle);
#endif

#ifndef NDEBUG
	mutex->isLocked = true;
#endif
}

void unlockMutex(
	struct Mutex* mutex)
{
	assert(mutex != NULL);

#if __linux__ || __APPLE__
	int result = pthread_mutex_unlock(
		&mutex->handle);

	if (result != 0)
		abort();
#elif _WIN32
	LeaveCriticalSection(
		&mutex->handle);
#endif

#ifndef NDEBUG
	mutex->isLocked = false;
#endif
}

bool tryLockMutex(
	struct Mutex* mutex)
{
	assert(mutex != NULL);

#if __linux__ || __APPLE__
	bool result = pthread_mutex_trylock(
		&mutex->handle) == 0;
#elif _WIN32
	bool result = TryEnterCriticalSection(
		&mutex->handle) == TRUE;
#endif

#ifndef NDEBUG
	if (result == true)
		mutex->isLocked = true;
#endif
	return result;
}
