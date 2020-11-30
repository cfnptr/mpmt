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
};

void* xmalloc(size_t size)
{
	void* p = malloc(size);
	if (p == NULL) abort();
	return p;
}

struct Mutex* createMutex()
{
	struct Mutex* mutex =
		xmalloc(sizeof(struct Mutex));

#if __linux__ || __APPLE__
	int result = pthread_mutex_init(
		&mutex->handle,
		NULL);

	if (result != 0)
		abort();
#elif _WIN32
	InitializeCriticalSection(
		&mutex->handle);
#endif

	return mutex;
}

void destroyMutex(struct Mutex* mutex)
{
	if (mutex != NULL)
	{
#if __linux__ || __APPLE__
		int result = pthread_mutex_destroy(
			&mutex->handle);

		if (result != 0)
			abort();
#elif _WIN32
		DeleteCriticalSection(
			&mutex->handle);
#endif
	}

	free(mutex);
}

void lockMutex(struct Mutex* mutex)
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
}

void unlockMutex(struct Mutex* mutex)
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
}

bool tryLockMutex(struct Mutex* mutex)
{
	assert(mutex != NULL);

#if __linux__ || __APPLE__
	return pthread_mutex_trylock(
		&mutex->handle) == 0;
#elif _WIN32
	return TryEnterCriticalSection(
		&mutex->handle) == TRUE;
#endif
}
