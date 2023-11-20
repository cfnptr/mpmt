// Copyright 2020-2023 Nikita Fediuchin. All rights reserved.
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
#elif _WIN32
#include <windows.h>
#else
#error Unknown operating system
#endif

#if __linux__ || __APPLE__
#define THREAD pthread_t
#elif _WIN32
#define THREAD HANDLE
#else
#error Unknown operating system
#endif

struct Thread_T
{
	void (*function)(void*);
	void* argument;
	THREAD handle;
	bool joined;
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

Thread createThread(void(*function)(void*), void* argument)
{
	assert(function);

	Thread thread = malloc(sizeof(Thread_T));

	if(!thread)
		return NULL;

	thread->function = function;
	thread->argument = argument;
	thread->joined = false;

#if __linux__ || __APPLE__
	int result = pthread_create(
		&thread->handle, NULL, threadFunction, thread);

	if (result != 0)
	{
		free(thread);
		return NULL;
	}
#elif _WIN32
	thread->handle = CreateThread(
		NULL, 0, threadFunction, thread, 0, NULL);

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
		if (pthread_detach(thread->handle) != 0)
			abort();
#elif _WIN32
		if (CloseHandle(thread->handle) != TRUE)
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
	if (pthread_join(thread->handle, NULL) != 0)
		abort();
#elif _WIN32
	THREAD handle = thread->handle;

	if (WaitForSingleObject(handle, INFINITE) != WAIT_OBJECT_0)
		abort();
	if (CloseHandle(handle) != TRUE)
		abort();
#endif
}
void sleepThread(double delay)
{
	assert(delay >= 0.0);

#if __linux__ || __APPLE__
	struct timespec spec;
	spec.tv_sec = (time_t)delay;
	spec.tv_nsec = (long)((delay - (double)spec.tv_sec) * 1000000000.0);

	while (true)
	{
		if (nanosleep(&spec, &spec) != 0)
		{
			int error = errno;
			if (error == EINTR) continue;
			else abort();
		}

		return;
	}
#elif _WIN32
	Sleep((DWORD)(delay * 1000.0));
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

bool isThreadJoined(Thread thread)
{
	assert(thread);
	return thread->joined;
}
bool isThreadCurrent(Thread thread)
{
	assert(thread);
#if __linux__ || __APPLE__
	return pthread_equal(pthread_self(), thread->handle) ? true : false;
#elif _WIN32
	return GetCurrentThreadId() == GetThreadId(thread->handle);
#endif
}

static bool isMainThreadSet = false;

#if __linux__ || __APPLE__
static THREAD mainThread;
#elif _WIN32
static DWORD mainThread;
#endif

void setMainThread()
{
	if (isMainThreadSet) abort();
#if __linux__ || __APPLE__
	mainThread = pthread_self();
#elif _WIN32
	mainThread = GetCurrentThreadId();
#endif
	isMainThreadSet = true;
}
bool isCurrentThreadMain()
{
	if (!isMainThreadSet) abort();
#if __linux__ || __APPLE__
	return pthread_equal(mainThread, pthread_self()) ? true : false;
#elif _WIN32
	return mainThread == GetCurrentThreadId();
#endif
}
bool isThreadMain(Thread thread)
{
	if (!isMainThreadSet) abort();
#if __linux__ || __APPLE__
	return pthread_equal(mainThread, thread->handle) ? true : false;
#elif _WIN32
	return mainThread == GetThreadId(thread->handle);
#endif
}
