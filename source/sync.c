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

struct Condvar
{
	CONDVAR handle;
	// TODO: debug lock flag
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

Condvar* createCondvar()
{
	Condvar* condvar = malloc(sizeof(Condvar));

	if (condvar == NULL)
		return NULL;

#if __linux__ || __APPLE__
	int result = pthread_cond_init(
		&condvar->handle,
		NULL);

	if (result != 0)
	{
		free(condvar);
		return NULL;
	}
#elif _WIN32
	InitializeConditionVariable(
		&condvar->handle);
#endif
	return condvar;
}

void destroyCondvar(Condvar* condvar)
{
	if (condvar == NULL)
		return;

#if __linux__ || __APPLE__
	int result = pthread_cond_destroy(
		&condvar->handle);

	if (result != 0)
		abort();
#endif

	free(condvar);
}

void signalCondvar(Condvar* condvar)
{
	assert(condvar != NULL);

#if __linux__ || __APPLE__
	int result = pthread_cond_signal(
		&condvar->handle);

	if (result != 0)
		abort();
#elif _WIN32
	WakeConditionVariable(
		&condvar->handle);
#endif
}

void broadcastCondvar(Condvar* condvar)
{
	assert(condvar != NULL);

#if __linux__ || __APPLE__
	int result = pthread_cond_broadcast(
		&condvar->handle);

	if (result != 0)
		abort();
#elif _WIN32
	WakeAllConditionVariable(
		&condvar->handle);
#endif
}

void waitCondvar(
	Condvar* condvar,
	Mutex* mutex)
{
	assert(condvar != NULL);
	assert(mutex != NULL);
	assert(mutex->isLocked == true);

#if __linux__ || __APPLE__
	int result = pthread_cond_wait(
		&condvar->handle,
		&mutex->handle);

	if (result != 0)
		abort();
#elif _WIN32
	BOOL result = SleepConditionVariableCS(
  		&condvar->handle,
  		&mutex->handle,
  		INFINITE);

	if (result != TRUE)
		abort();
#endif
}

void waitCondvarFor(
	Condvar* condvar,
	Mutex* mutex,
	double timeout)
{
	assert(condvar != NULL);
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
		&condvar->handle,
		&mutex->handle,
		&delay);

	if (result != 0)
		abort();
#elif _WIN32
	BOOL result = SleepConditionVariableCS(
  		&condvar->handle,
  		&mutex->handle,
  		(DWORD)(timeout * 1000.0));

	if (result != TRUE)
		abort();

#endif
}
