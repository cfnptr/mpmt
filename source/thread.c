// Copyright 2020-2022 Nikita Fediuchin. All rights reserved.
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

#include "mpmt/thread.h"

#include <stdlib.h>
#include <assert.h>

#if __linux__ || __APPLE__
#include <errno.h>
#include <pthread.h>
#define THREAD pthread_t
#elif _WIN32
#include <windows.h>
#define THREAD HANDLE
#else
#error Unknown operating system
#endif

struct Thread_T
{
	THREAD handle;
	bool joined;
	void (*function)(void*);
	void* argument;
};

#if __linux__ || __APPLE__
static void* threadFunction(void* argument)
{
	Thread thread = (Thread)argument;
	thread->function(thread->argument);
	return NULL;
}
#elif _WIN32
DWORD threadFunction(LPVOID argument)
{
	Thread thread = (Thread)argument;
	thread->function(thread->argument);
	return 0;
}
#endif

Thread createThread(
	void (*function)(void*),
	void* argument)
{
	assert(function);

	Thread thread = malloc(
		sizeof(Thread_T));

	if(!thread)
		return NULL;

	thread->joined = false;
	thread->function = function;
	thread->argument = argument;

#if __linux__ || __APPLE__
	int result = pthread_create(
		&thread->handle,
		NULL,
		threadFunction,
		thread);

	if (result != 0)
	{
		free(thread);
		return NULL;
	}
#elif _WIN32
	thread->handle = CreateThread(
		NULL,
		0,
		threadFunction,
		thread,
		0,
		NULL);

	if (thread->handle == NULL)
	{
		free(thread);
		return NULL;
	}
#endif

	return thread;
}

void destroyThread(Thread thread)
{
	if (!thread)
		return;

	if (!thread->joined)
	{
#if __linux__ || __APPLE__
		int result = pthread_detach(
			thread->handle);

		if (result != 0)
			abort();
#elif _WIN32
		BOOL result = CloseHandle(
			thread->handle);

		if (result != TRUE)
			abort();
#endif
	}

	free(thread);
}

void joinThread(Thread thread)
{
	assert(thread);

	if (thread->joined)
		abort();

	thread->joined = true;

#if __linux__ || __APPLE__
	int result = pthread_join(
		thread->handle,
		NULL);

	if (result != 0)
		abort();
#elif _WIN32
	THREAD handle = thread->handle;

	DWORD waitResult = WaitForSingleObject(
		handle,
		INFINITE);

	if (waitResult != WAIT_OBJECT_0)
		abort();

	BOOL closeResult = CloseHandle(
		handle);

	if (closeResult != TRUE)
		abort();
#endif
}

bool isThreadJoined(Thread thread)
{
	assert(thread);
	return thread->joined;
}

void sleepThread(double _delay)
{
	assert(_delay >= 0.0);

#if __linux__ || __APPLE__
	struct timespec delay;

	delay.tv_sec = (time_t)_delay;

	delay.tv_nsec = (long)(
		(_delay - (double)delay.tv_sec) *
		1000000000.0);

	while (true)
	{
		int result = nanosleep(
			&delay,
			&delay);

		if (result != 0)
		{
			int error = errno;

			if (error == EINTR)
				continue;
			else
				abort();
		}

		return;
	}
#elif _WIN32
	Sleep((DWORD)(_delay * 1000.0));
#endif
}

bool yieldThread()
{
#if __linux__ || __APPLE__
	return sched_yield() == 0;
#elif _WIN32
	return SwitchToThread() == TRUE;
#endif
}

double getCurrentClock()
{
#if __linux__ || __APPLE__
	struct timespec time;

	int result = clock_gettime(
		CLOCK_MONOTONIC,
		&time);

	if (result != 0)
		abort();

	return (double)time.tv_sec +
		(double)time.tv_nsec / 1000000000.0;
#elif _WIN32
	LARGE_INTEGER frequency;

	BOOL result = QueryPerformanceFrequency(
		&frequency);

	if (result != TRUE)
		abort();

	LARGE_INTEGER counter;

	result = QueryPerformanceCounter(
		&counter);

	if (result != TRUE)
		abort();

	return (double)counter.QuadPart /
		(double)frequency.QuadPart;
#endif
}
