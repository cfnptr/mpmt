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

/***********************************************************************************************************************
 * @file
 * @brief Atomic operations.
 * 
 * @details
 * Atomic operations are used to perform certain operations in a way that is guaranteed to be indivisible and 
 * not subject to interruption by other threads. This is particularly useful in multithreaded programming to avoid 
 * race conditions and ensure correct behavior when multiple threads are concurrently accessing shared data.
 **********************************************************************************************************************/

#pragma once
#include <stdint.h>

/**
 * @brief Integer type for atomic operations. (int32)
 */
#define atomic_int32 volatile int32_t
/**
 * @brief Integer type for atomic operations. (int64)
 */
#define atomic_int64 volatile int64_t

#if __linux__ || __APPLE__

/**
 * @brief Atomically adds the value to the variable that memory points to.
 * @details Result is stored in the address that is specified by memory.
 * 
 * @param[in] memory pointer of a variable to which value is to be added.
 * @param value variable whose value is to be added to the variable that memory points to.
 * 
 * @return The initial value of the variable that memory points to.
 */
#define atomicFetchAdd32(memory, value) __sync_fetch_and_add(memory, value)

/**
 * @brief Atomically adds the value to the variable that memory points to.
 * @details Result is stored in the address that is specified by memory.
 * 
 * @param[in] memory pointer of a variable to which value is to be added.
 * @param value variable whose value is to be added to the variable that memory points to.
 * 
 * @return The initial value of the variable that memory points to.
 */
#define atomicFetchAdd64(memory, value) __sync_fetch_and_add(memory, value)
#elif _WIN32
#include <windows.h>

/**
 * @brief Atomically adds the value to the variable that memory points to.
 * @details Result is stored in the address that is specified by memory.
 * 
 * @param[in] memory pointer of a variable to which value is to be added.
 * @param value variable whose value is to be added to the variable that memory points to.
 * 
 * @return The initial value of the variable that memory points to.
 */
#define atomicFetchAdd32(memory, value) InterlockedExchangeAdd(memory, value)

/**
 * @brief Atomically adds the value to the variable that memory points to.
 * @details Result is stored in the address that is specified by memory.
 * 
 * @param[in] memory pointer of a variable to which value is to be added.
 * @param value variable whose value is to be added to the variable that memory points to.
 * 
 * @return The initial value of the variable that memory points to.
 */
#define atomicFetchAdd64(memory, value) InterlockedExchangeAdd64(memory, value)

#else
#error Unknown operating system
#endif

// TODO: add exchange, or, xor, compare swap, max, min, increment, decrement