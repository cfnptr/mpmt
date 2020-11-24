#include "mpmt/mutex.h"
#include <stdlib.h>

#if __linux__ || __APPLE__
#include <pthread.h>
#define MUTEX pthread_mutex_t
#elif _WIN32
// TODO:
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

	int result = pthread_mutex_init(
		&handle,
		NULL);

	if(result != 0)
		return NULL;

	struct Mutex* mutex =
		malloc(sizeof(struct Mutex));
	mutex->handle = handle;
	return mutex;
}

void mpmtDestroyMutex(
	struct Mutex* mutex)
{
	if(mutex)
	{
		pthread_mutex_destroy(
			&mutex->handle);
	}

	free(mutex);
}

bool mpmtLockMutex(
	struct Mutex* mutex)
{
	if(!mutex)
		return false;

	return pthread_mutex_lock(
		&mutex->handle) == 0;
}
bool mpmtUnlockMutex(
	struct Mutex* mutex)
{
	if(!mutex)
		return false;

	return pthread_mutex_unlock(
		&mutex->handle) == 0;
}
bool mpmtTryLockMutex(
	struct Mutex* mutex)
{
	if(!mutex)
		return false;

	return pthread_mutex_trylock(
		&mutex->handle) == 0;
}

