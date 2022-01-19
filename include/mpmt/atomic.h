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
#include <stdint.h>

#define atomic_int32 volatile int32_t
#define atomic_int64 volatile int64_t

#if __linux__ || __APPLE__
#define atomicFetchAdd8(memory, value) __sync_fetch_and_add(memory, value)
#define atomicFetchAdd16(memory, value) __sync_fetch_and_add(memory, value)
#define atomicFetchAdd32(memory, value) __sync_fetch_and_add(memory, value)
#define atomicFetchAdd64(memory, value) __sync_fetch_and_add(memory, value)

#define atomicFetchOr8(memory, value) __sync_fetch_and_or(memory, value)
#define atomicFetchOr16(memory, value) __sync_fetch_and_or(memory, value)
#define atomicFetchOr32(memory, value) __sync_fetch_and_or(memory, value)
#define atomicFetchOr64(memory, value) __sync_fetch_and_or(memory, value)

#define atomicFetchXor8(memory, value) __sync_fetch_and_xor(memory, value)
#define atomicFetchXor16(memory, value) __sync_fetch_and_xor(memory, value)
#define atomicFetchXor32(memory, value) __sync_fetch_and_xor(memory, value)
#define atomicFetchXor64(memory, value) __sync_fetch_and_xor(memory, value)

#define atomicFetchAnd8(memory, value) __sync_fetch_and_and(memory, value)
#define atomicFetchAnd16(memory, value) __sync_fetch_and_and(memory, value)
#define atomicFetchAnd32(memory, value) __sync_fetch_and_and(memory, value)
#define atomicFetchAnd64(memory, value) __sync_fetch_and_and(memory, value)
#elif _WIN32
#include <winnt.h>

#define atomicFetchAdd8(memory, value) InterlockedAdd8(memory, value)
#define atomicFetchAdd16(memory, value) InterlockedAdd16(memory, value)
#define atomicFetchAdd32(memory, value) InterlockedAdd(memory, value)
#define atomicFetchAdd64(memory, value) InterlockedAdd64(memory, value)

#define atomicFetchOr8(memory, value) InterlockedAdd8(memory, value)
#define atomicFetchOr16(memory, value) InterlockedAdd16(memory, value)
#define atomicFetchOr32(memory, value) InterlockedAdd(memory, value)
#define atomicFetchOr64(memory, value) InterlockedAdd64(memory, value)

#define atomicFetchXor8(memory, value) InterlockedAdd8(memory, value)
#define atomicFetchXor16(memory, value) InterlockedAdd16(memory, value)
#define atomicFetchXor32(memory, value) InterlockedAdd(memory, value)
#define atomicFetchXor64(memory, value) InterlockedAdd64(memory, value)

#define atomicFetchAnd8(memory, value) InterlockedAdd8(memory, value)
#define atomicFetchAnd16(memory, value) InterlockedAdd16(memory, value)
#define atomicFetchAnd32(memory, value) InterlockedAdd(memory, value)
#define atomicFetchAnd64(memory, value) InterlockedAdd64(memory, value)
#endif

// TODO: add exchange, compare swap, max, min, increment, decrement
