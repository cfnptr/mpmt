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

struct Mutex* createMutex()
{
	struct Mutex* mutex =
		malloc(sizeof(struct Mutex));

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

bool lockMutex(
	struct Mutex* mutex,
	void (*function)(void*),
	void* argument)
{
	assert(mutex != NULL);

#if __linux__ || __APPLE__
	int result = pthread_mutex_lock(
		&mutex->handle);

	if (result != 0)
		return false;
#elif _WIN32
	EnterCriticalSection(
		&mutex->handle);
#endif

	function(argument);

#if __linux__ || __APPLE__
	result = pthread_mutex_unlock(
		&mutex->handle);

	if (result != 0)
		return false;
#elif _WIN32
	LeaveCriticalSection(
		&mutex->handle);
#endif
	return true;
}

bool tryLockMutex(
	struct Mutex* mutex,
	void (*function)(void*),
	void* argument)
{
	assert(mutex != NULL);

#if __linux__ || __APPLE__
	int result = pthread_mutex_trylock(
		&mutex->handle);

	if (result != 0)
		return false;
#elif _WIN32
	BOOL result = TryEnterCriticalSection(
		&mutex->handle);

	if (resutl != TRUE)
		return false;
#endif

	function(argument);

#if __linux__ || __APPLE__
	result = pthread_mutex_unlock(
		&mutex->handle);

	if (result != 0)
		return false;
#elif _WIN32
	LeaveCriticalSection(
		&mutex->handle);
#endif

	return true;
}
