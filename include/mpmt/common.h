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

#if __linux__ || __APPLE__
#include <time.h>
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#else
#error Unknown operating system
#endif

/*
 * Returns current clock value (s).
 */
inline static double getCurrentClock()
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

/*
 * Returns running system logical CPU count.
 */
inline static int getCpuCount()
{
#if __linux__ || __APPLE__
	return (int)sysconf(_SC_NPROCESSORS_ONLN);
#elif _WIN32
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	return (int)systemInfo.dwNumberOfProcessors;
#endif
}
