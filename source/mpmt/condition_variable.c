#include "mpmt/condition_variable.h"

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

void* xmalloc(size_t size)
{
	void* p = malloc(size);
	if (p == NULL) abort();
	return p;
}

struct ConditionVariable* createConditionVariable()
{
	struct ConditionVariable* conditionVariable =
		xmalloc(sizeof(struct ConditionVariable));

#if __linux__ || __APPLE__
	int result = pthread_cond_init(
		&conditionVariable->handle,
		NULL);

	if (result != 0)
		abort();
#elif _WIN32
	InitializeConditionVariable(
		&conditionVariable->handle);
#endif

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
