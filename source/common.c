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

#include "mpmt/common.h"
#include <stdlib.h>
#include <string.h>

#if __linux__ || __APPLE__
#include <time.h>
#include <unistd.h>
	#if __x86_64__ || __i386__
	#include <cpuid.h>
	#endif
#elif _WIN32
#include <intrin.h>
#include <windows.h>
#else
#error Unknown operating system
#endif

#if __linux__
#include <sys/sysinfo.h>
#elif __APPLE__
#include <sys/sysctl.h>
#endif

#if !__x86_64__ && !_M_X64 && !__i386__
#include <stdio.h>
#endif

double getCurrentClock()
{
#if __linux__ || __APPLE__
	struct timespec time;

	if (clock_gettime(CLOCK_MONOTONIC, &time) != 0)
		abort();

	return (double)time.tv_sec + (double)time.tv_nsec / 1000000000.0;
#elif _WIN32
	LARGE_INTEGER frequency;
	BOOL result = QueryPerformanceFrequency(&frequency);

	if (result != TRUE)
		abort();

	LARGE_INTEGER counter;

	if (QueryPerformanceCounter(&counter) != TRUE)
		abort();

	return (double)counter.QuadPart / (double)frequency.QuadPart;
#endif
}

int getCpuCount()
{
#if __linux__ || __APPLE__
	return (int)sysconf(_SC_NPROCESSORS_ONLN);
#elif _WIN32
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	return (int)systemInfo.dwNumberOfProcessors;
#endif
}

int64_t getRamSize()
{
#if __linux__
	struct sysinfo info;

	if (sysinfo(&info) != 0)
		return 0;

	return info.totalram;
#elif __APPLE__
	int mib [] = { CTL_HW, HW_MEMSIZE };
	int64_t value = 0;
	size_t length = sizeof(int64_t);

	if(sysctl(mib, 2, &value, &length, NULL, 0) != 0)
		return 0;

	return value;
#elif _WIN32
	ULONGLONG value = 0;

	if (GetPhysicallyInstalledSystemMemory(&value) != TRUE)
		return 0;

	return value * 1024;
#endif
}

static char cpuBrand[65];

const char* getCpuName()
{
#if __x86_64__ || _M_X64 || __i386__
	unsigned int cpuInfo[4] = { 0, 0, 0, };

#if __linux__ || __APPLE__
	__cpuid(0x80000000, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#elif _WIN32
	__cpuid((int*)cpuInfo, 0x80000000);
#endif

	unsigned int nExIds = cpuInfo[0];
	memset(cpuBrand, 0, sizeof(cpuBrand));

	for (unsigned int i = 0x80000000; i <= nExIds; ++i)
	{
#if __linux__ || __APPLE__
		__cpuid(i, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#elif _WIN32
		__cpuid((int*)cpuInfo, i);
#endif

		if (i == 0x80000002)
			memcpy(cpuBrand, cpuInfo, sizeof(cpuInfo));
		else if (i == 0x80000003)
			memcpy(cpuBrand + 16, cpuInfo, sizeof(cpuInfo));
		else if (i == 0x80000004)
			memcpy(cpuBrand + 32, cpuInfo, sizeof(cpuInfo));
	}
#else
	memcpy(cpuBrand, "Unknown\0", 8);
	FILE* file = fopen("/proc/cpuinfo", "r");

	if (!file)
		return cpuBrand;

	char buffer[256];

	while (fgets(buffer, 256, file))
	{
		if (memcmp(buffer, "model name", 10) != 0 &&
			memcmp(buffer, "Model", 5) != 0)
		{
			continue;
		}

		char* pointer = memchr(buffer, ':', 256);

		if (!pointer)
			continue;

		size_t index = (pointer - buffer) + 2;

		if (index >= 256)
			continue;

		for (size_t i = index; i < 256; i++)
		{
			char value = buffer[i];

			if (value != '\n' && value != '\0')
				continue;

			size_t count = i - index;

			if (i > 64)
				return cpuBrand;

			memcpy(cpuBrand, buffer + index, count);
			cpuBrand[count] = '\0';
			break;
		}
	}
#endif
	return cpuBrand;
}