// Copyright 2020-2024 Nikita Fediuchin. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "mpmt/sync.h"
#include <assert.h>
#include <stdlib.h>

#if __linux__ || __APPLE__
#include <pthread.h>
#define MUTEX pthread_mutex_t
#define COND pthread_cond_t
#elif _WIN32
#include <windows.h>
#define MUTEX CRITICAL_SECTION
#define COND CONDITION_VARIABLE
#else
#error Unknown operating system
#endif

struct Mutex_T
{
	MUTEX handle;
#ifndef NDEBUG
	volatile bool isLocked;
#endif
};

struct Cond_T
{
	COND handle;
};

Mutex createMutex()
{
	Mutex mutex = malloc(sizeof(Mutex_T));
	if (!mutex) return NULL;

#if __linux__ || __APPLE__
	if (pthread_mutex_init(&mutex->handle, NULL) != 0)
	{
		free(mutex);
		return NULL;
	}
#elif _WIN32
	InitializeCriticalSection(&mutex->handle);
#endif

#ifndef NDEBUG
	mutex->isLocked = false;
#endif
	return mutex;
}

void destroyMutex(Mutex mutex)
{
	if (!mutex) return;

#ifndef NDEBUG
	assert(!mutex->isLocked);
#endif

#if __linux__ || __APPLE__
	if (pthread_mutex_destroy(&mutex->handle) != 0) abort();
#elif _WIN32
	DeleteCriticalSection(&mutex->handle);
#endif

	free(mutex);
}

void lockMutex(Mutex mutex)
{
	assert(mutex);

#if __linux__ || __APPLE__
	if (pthread_mutex_lock(&mutex->handle) != 0) abort();
#elif _WIN32
	EnterCriticalSection(&mutex->handle);
#endif

#ifndef NDEBUG
	mutex->isLocked = true;
#endif
}

void unlockMutex(Mutex mutex)
{
	assert(mutex);

#if __linux__ || __APPLE__
	if (pthread_mutex_unlock(&mutex->handle) != 0) abort();
#elif _WIN32
	LeaveCriticalSection(&mutex->handle);
#endif

#ifndef NDEBUG
	mutex->isLocked = false;
#endif
}

bool tryLockMutex(Mutex mutex)
{
	assert(mutex);

#if __linux__ || __APPLE__
	bool result = pthread_mutex_trylock(&mutex->handle) == 0;
#elif _WIN32
	bool result = TryEnterCriticalSection(&mutex->handle) == TRUE;
#endif

#ifndef NDEBUG
	if (result) mutex->isLocked = true;
#endif
	return result;
}

const void* getMutexNative(Mutex mutex)
{
	return &mutex->handle;
}

Cond createCond()
{
	Cond cond = malloc(sizeof(Cond_T));
	if (!cond) return NULL;

#if __linux__ || __APPLE__
	if (pthread_cond_init(&cond->handle, NULL) != 0)
	{
		free(cond);
		return NULL;
	}
#elif _WIN32
	InitializeConditionVariable(&cond->handle);
#endif
	return cond;
}

void destroyCond(Cond cond)
{
	if (!cond) return;

#if __linux__ || __APPLE__
	if (pthread_cond_destroy(&cond->handle) != 0) abort();
#endif

	free(cond);
}

void signalCond(Cond cond)
{
	assert(cond);

#if __linux__ || __APPLE__
	if (pthread_cond_signal(&cond->handle) != 0) abort();
#elif _WIN32
	WakeConditionVariable(&cond->handle);
#endif
}

void broadcastCond(Cond cond)
{
	assert(cond);

#if __linux__ || __APPLE__
	if (pthread_cond_broadcast(&cond->handle) != 0) abort();
#elif _WIN32
	WakeAllConditionVariable(&cond->handle);
#endif
}

void waitCond(Cond cond, Mutex mutex)
{
	assert(cond);
	assert(mutex);

#if __linux__ || __APPLE__
	if (pthread_cond_wait(&cond->handle, &mutex->handle) != 0) abort();
#elif _WIN32
	if (SleepConditionVariableCS(&cond->handle,
		&mutex->handle, INFINITE) != TRUE) abort();
#endif
}

void waitCondFor(Cond cond, Mutex mutex, double timeout)
{
	assert(cond);
	assert(mutex);
	assert(timeout >= 0.0);
	assert(mutex->isLocked);

#if __linux__ || __APPLE__
	struct timespec delay;
	delay.tv_sec = time(NULL) + (time_t)timeout;
	delay.tv_nsec = (long)((timeout - (double)delay.tv_sec) * 1000000000.0);

	if (pthread_cond_timedwait(&cond->handle, &mutex->handle, &delay) != 0)
		abort();
#elif _WIN32
	if (SleepConditionVariableCS(&cond->handle, &mutex->handle,
		(DWORD)(timeout * 1000.0)) != TRUE) abort();
#endif
}

const void* getCondNative(Cond cond)
{
	return &cond->handle;
}