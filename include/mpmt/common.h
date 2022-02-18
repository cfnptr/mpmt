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
#include <string.h>

#if __linux__ || __APPLE__
#include <time.h>
#include <unistd.h>
#include <cpuid.h>
#elif _WIN32
#include <windows.h>
#include <intrin.h>
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

/*
 * CPU brand string.
 * Use getCpuName to get CPU name!
 */
static char cpuBrand[0x40];

/*
 * Returns running system CPU name string.
 */
inline static const char* getCpuName()
{
	unsigned int cpuInfo[4] = { 0, 0, 0, };

#if __linux__ || __APPLE__
	__cpuid(0x80000000, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#elif _WIN32
	__cpuid(cpuInfo, 0x80000000);
#endif

	unsigned int nExIds = cpuInfo[0];
	memset(cpuBrand, 0, sizeof(cpuBrand));

	for (unsigned int i = 0x80000000; i <= nExIds; ++i)
	{
#if __linux__ || __APPLE__
		__cpuid(i, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#elif _WIN32
		__cpuid(cpuInfo, i);
#endif

		if (i == 0x80000002)
			memcpy(cpuBrand, cpuInfo, sizeof(cpuInfo));
		else if (i == 0x80000003)
			memcpy(cpuBrand + 16, cpuInfo, sizeof(cpuInfo));
		else if (i == 0x80000004)
			memcpy(cpuBrand + 32, cpuInfo, sizeof(cpuInfo));
	}

	return cpuBrand;
}
