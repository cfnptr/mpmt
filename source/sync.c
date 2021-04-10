#include "mpmt/sync.h"

#include <stdlib.h>
#include <assert.h>

#if __linux__ || __APPLE__
#include <pthread.h>
#define MUTEX pthread_mutex_t
#define CONDVAR pthread_cond_t
#elif _WIN32
#include <windows.h>
#define MUTEX CRITICAL_SECTION
#define CONDVAR CONDITION_VARIABLE
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

struct Cond
{
	CONDVAR handle;
};

Mutex* createMutex()
{
	Mutex* mutex = malloc(sizeof(Mutex));

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

void destroyMutex(Mutex* mutex)
{
	if (mutex == NULL)
		return;

#ifndef NDEBUG
	assert(mutex->isLocked == false);
#endif

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

void lockMutex(Mutex* mutex)
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

void unlockMutex(Mutex* mutex)
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

bool tryLockMutex(Mutex* mutex)
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

Cond* createCond()
{
	Cond* cond = malloc(sizeof(Cond));

	if (cond == NULL)
		return NULL;

#if __linux__ || __APPLE__
	int result = pthread_cond_init(
		&cond->handle,
		NULL);

	if (result != 0)
	{
		free(cond);
		return NULL;
	}
#elif _WIN32
	InitializeConditionVariable(
		&cond->handle);
#endif
	return cond;
}

void destroyCondvar(Cond* cond)
{
	if (cond == NULL)
		return;

#if __linux__ || __APPLE__
	int result = pthread_cond_destroy(
		&cond->handle);

	if (result != 0)
		abort();
#endif

	free(cond);
}

void signalCond(Cond* cond)
{
	assert(cond != NULL);

#if __linux__ || __APPLE__
	int result = pthread_cond_signal(
		&cond->handle);

	if (result != 0)
		abort();
#elif _WIN32
	WakeConditionVariable(
		&cond->handle);
#endif
}

void broadcastCond(Cond* cond)
{
	assert(cond != NULL);

#if __linux__ || __APPLE__
	int result = pthread_cond_broadcast(
		&cond->handle);

	if (result != 0)
		abort();
#elif _WIN32
	WakeAllConditionVariable(
		&cond->handle);
#endif
}

void waitCond(
	Cond* cond,
	Mutex* mutex)
{
	assert(cond != NULL);
	assert(mutex != NULL);
	assert(mutex->isLocked == true);

#if __linux__ || __APPLE__
	int result = pthread_cond_wait(
		&cond->handle,
		&mutex->handle);

	if (result != 0)
		abort();
#elif _WIN32
	BOOL result = SleepConditionVariableCS(
  		&cond->handle,
  		&mutex->handle,
  		INFINITE);

	if (result != TRUE)
		abort();
#endif
}

void waitCondFor(
	Cond* cond,
	Mutex* mutex,
	double timeout)
{
	assert(cond != NULL);
	assert(mutex != NULL);
	assert(timeout >= 0.0);
	assert(mutex->isLocked == true);

#if __linux__ || __APPLE__
	struct timespec delay;

	delay.tv_sec = time(NULL) + (time_t)timeout;

	delay.tv_nsec = (long)(
		(timeout - (double)delay.tv_sec) *
		1000000000.0);

	int result = pthread_cond_timedwait(
		&cond->handle,
		&mutex->handle,
		&delay);

	if (result != 0)
		abort();
#elif _WIN32
	BOOL result = SleepConditionVariableCS(
  		&cond->handle,
  		&mutex->handle,
  		(DWORD)(timeout * 1000.0));

	if (result != TRUE)
		abort();
#endif
}
