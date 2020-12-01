#include "mpmt/mutex.h"
#include "mpmt/xalloc.h"

#include <stdlib.h>
#include <assert.h>

#if __linux__ || __APPLE__
#include <pthread.h>
#define MUTEX pthread_mutex_t
#define CONDITION_VARIABLE pthread_cond_t
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

struct ConditionVariable
{
	CONDITION_VARIABLE handle;
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
	InitializeCriticalSection(
		&handle);
#endif

	struct Mutex* mutex =
		xmalloc(sizeof(struct Mutex));
	mutex->handle = handle;

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

struct ConditionVariable* createConditionVariable()
{
	CONDITION_VARIABLE handle;

#if __linux__ || __APPLE__
	int result = pthread_cond_init(
		&handle,
		NULL);

	if (result != 0)
		abort();
#elif _WIN32
	InitializeConditionVariable(
		&handle);
#endif

	struct ConditionVariable* conditionVariable =
		xmalloc(sizeof(struct ConditionVariable));
	conditionVariable->handle = handle;

	return conditionVariable;
}

void destroyConditionVariable(
	struct ConditionVariable* conditionVariable)
{
	if (conditionVariable != NULL)
	{
#if __linux__ || __APPLE__
		int result = pthread_cond_destroy(
			&conditionVariable->handle);

		if (result != 0)
			abort();
#endif
	}

	free(conditionVariable);
}

void signalConditionVariable(
	struct ConditionVariable* conditionVariable)
{
	assert(conditionVariable);

#if __linux__ || __APPLE__
	int result = pthread_cond_signal(
		&conditionVariable->handle);

	if (result != 0)
		abort();
#elif _WIN32
	WakeConditionVariable(
		&conditionVariable->handle);
#endif
}

void broadcastConditionVariable(
	struct ConditionVariable* conditionVariable)
{
	assert(conditionVariable);

#if __linux__ || __APPLE__
	int result = pthread_cond_broadcast(
		&conditionVariable->handle);

	if (result != 0)
		abort();
#elif _WIN32
	WakeAllConditionVariable(
		&conditionVariable->handle);
#endif
}

void waitConditionVariable(
	struct ConditionVariable* conditionVariable,
	struct Mutex* mutex)
{
	assert(conditionVariable);

#if __linux__ || __APPLE__
	int result = pthread_cond_wait(
		&conditionVariable->handle,
		&mutex->handle);

	if (result != 0)
		abort();
#elif _WIN32
	BOOL result = SleepConditionVariableCS(
		&conditionVariable->handle,
		&mutex->handle,
		INFINITE);

	if(result != TRUE)
		abort();
#endif
}
