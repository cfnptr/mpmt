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

#pragma once
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#if __linux__ || __APPLE__
#include <errno.h>
#include <pthread.h>
#elif _WIN32
#include <windows.h>
#else
#error Unknown operating system
#endif

/*
 * Thread structure.
 */
typedef struct Thread_T Thread_T;
/*
 * Thread instance.
 */
typedef Thread_T* Thread;

/*
 * Creates a new thread executing the specified function.
 * Returns thread instance on success, otherwise NULL.
 *
 * function - pointer to the function that should be invoked.
 * argument - argument that will be passed to the function or NULL.
 */
Thread createThread(
	void (*function)(void*),
	void* argument);
/*
 * Destroys thread instance.
 * thread - thread instance or NULL.
 */
void destroyThread(Thread thread);

/*
 * Blocks the current thread until the function execution end.
 * thread - thread instance.
 */
void joinThread(Thread thread);

/*
 * Returns thread current join status.
 * thread - thread instance.
 */
bool isThreadJoined(Thread thread);

/*
 * Blocks the execution of the current thread for a specified time.
 * delay - thread sleep delay time (s).
 */
inline static void sleepThread(double delay)
{
	assert(delay >= 0.0);

#if __linux__ || __APPLE__
	struct timespec spec;

	spec.tv_sec = (time_t)delay;

	spec.tv_nsec = (long)(
		(delay - (double)spec.tv_sec) *
		1000000000.0);

	while (true)
	{
		int result = nanosleep(
			&spec,
			&spec);

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
	Sleep((DWORD)(delay * 1000.0));
#endif
}

/*
 * Causes the current thread to yield execution to another thread.
 */
inline static bool yieldThread()
{
#if __linux__ || __APPLE__
	return sched_yield() == 0;
#elif _WIN32
	return SwitchToThread() == TRUE;
#endif
}
