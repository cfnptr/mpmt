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

#pragma once
#include <stdint.h>

#define atomic_int32 volatile int32_t
#define atomic_int64 volatile int64_t

#if __linux__ || __APPLE__
#define atomicFetchAdd32(memory, value) __sync_fetch_and_add(memory, value)
#define atomicFetchAdd64(memory, value) __sync_fetch_and_add(memory, value)
#elif _WIN32
#include <windows.h>
#define atomicFetchAdd32(memory, value) InterlockedExchangeAdd(memory, value)
#define atomicFetchAdd64(memory, value) InterlockedExchangeAdd64(memory, value)
#else
#error Unknown operating system
#endif

// TODO: add exchange, or, xor, compare swap, max, min, increment, decrement
