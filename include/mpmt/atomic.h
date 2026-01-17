// Copyright 2020-2026 Nikita Fediuchin. All rights reserved.
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
 */

// TODO: compare exchange, test/set/clear, thread fences and barriers.
// TODO: relaxed barrier functions.

#pragma once

#if __linux__ || __APPLE__
#include <stdint.h>

/**
 * @brief Integer type for atomic operations. (int8)
 */
#define atomic_int8 volatile int8_t
/**
 * @brief Integer type for atomic operations. (int16)
 */
#define atomic_int16 volatile int16_t
/**
 * @brief Integer type for atomic operations. (int32)
 */
#define atomic_int32 volatile int32_t
/**
 * @brief Integer type for atomic operations. (int64)
 */
#define atomic_int64 volatile int64_t

/**
 * @brief Atomically loads the value from the variable that memory points to.
 * @param[in] memory pointer of a variable from which the value is to be loaded
 * @return The current value of the variable that memory points to.
 */
#define atomicLoad8(memory) __atomic_load_n(memory, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically loads the value from the variable that memory points to.
 * @param[in] memory pointer of a variable from which the value is to be loaded
 * @return The current value of the variable that memory points to.
 */
#define atomicLoad16(memory) __atomic_load_n(memory, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically loads the value from the variable that memory points to.
 * @param[in] memory pointer of a variable from which the value is to be loaded
 * @return The current value of the variable that memory points to.
 */
#define atomicLoad32(memory) __atomic_load_n(memory, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically loads the value from the variable that memory points to.
 * @param[in] memory pointer of a variable from which the value is to be loaded
 * @return The current value of the variable that memory points to.
 */
#define atomicLoad64(memory) __atomic_load_n(memory, __ATOMIC_SEQ_CST)

/***********************************************************************************************************************
 * @brief Atomically stores the value to the variable that memory points to.
 *
 * @param[out] memory pointer of a variable to which the value is to be stored
 * @param value variable whose value is to be stored to the variable that memory points to
 */
#define atomicStore8(memory, value) __atomic_store_n(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically stores the value to the variable that memory points to.
 *
 * @param[out] memory pointer of a variable to which the value is to be stored
 * @param value variable whose value is to be stored to the variable that memory points to
 */
#define atomicStore16(memory, value) __atomic_store_n(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically stores the value to the variable that memory points to.
 *
 * @param[out] memory pointer of a variable to which the value is to be stored
 * @param value variable whose value is to be stored to the variable that memory points to
 */
#define atomicStore32(memory, value) __atomic_store_n(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically stores the value to the variable that memory points to.
 *
 * @param[out] memory pointer of a variable to which the value is to be stored
 * @param value variable whose value is to be stored to the variable that memory points to
 */
#define atomicStore64(memory, value) __atomic_store_n(memory, value, __ATOMIC_SEQ_CST)

/***********************************************************************************************************************
 * @brief Atomically exchanges the value of the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the value is to be written
 * @param value variable whose value is to be written to the variable that memory points to
 */
#define atomicExchange8(memory, value) __atomic_exchange_n(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically exchanges the value of the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the value is to be written
 * @param value variable whose value is to be written to the variable that memory points to
 */
#define atomicExchange16(memory, value) __atomic_exchange_n(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically exchanges the value of the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the value is to be written
 * @param value variable whose value is to be written to the variable that memory points to
 */
#define atomicExchange32(memory, value) __atomic_exchange_n(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically exchanges the value of the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the value is to be written
 * @param value variable whose value is to be written to the variable that memory points to
 */
#define atomicExchange64(memory, value) __atomic_exchange_n(memory, value, __ATOMIC_SEQ_CST)

/***********************************************************************************************************************
 * @brief Atomically performs AND operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the AND operation is to be performed
 * @param value variable whose value is to be used for an AND operation
 */
#define atomicFetchAnd8(memory, value) __atomic_fetch_and(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically performs AND operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the AND operation is to be performed
 * @param value variable whose value is to be used for an AND operation
 */
#define atomicFetchAnd16(memory, value) __atomic_fetch_and(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically performs AND operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the AND operation is to be performed
 * @param value variable whose value is to be used for an AND operation
 */
#define atomicFetchAnd32(memory, value) __atomic_fetch_and(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically performs AND operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the AND operation is to be performed
 * @param value variable whose value is to be used for an AND operation
 */
#define atomicFetchAnd64(memory, value) __atomic_fetch_and(memory, value, __ATOMIC_SEQ_CST)

/***********************************************************************************************************************
 * @brief Atomically performs OR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the OR operation is to be performed
 * @param value variable whose value is to be used for an OR operation
 */
#define atomicFetchOr8(memory, value) __atomic_fetch_or(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically performs OR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the OR operation is to be performed
 * @param value variable whose value is to be used for an OR operation
 */
#define atomicFetchOr16(memory, value) __atomic_fetch_or(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically performs OR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the OR operation is to be performed
 * @param value variable whose value is to be used for an OR operation
 */
#define atomicFetchOr32(memory, value) __atomic_fetch_or(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically performs OR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the OR operation is to be performed
 * @param value variable whose value is to be used for an OR operation
 */
#define atomicFetchOr64(memory, value) __atomic_fetch_or(memory, value, __ATOMIC_SEQ_CST)

/***********************************************************************************************************************
 * @brief Atomically performs XOR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the XOR operation is to be performed
 * @param value variable whose value is to be used for an XOR operation
 */
#define atomicFetchXor8(memory, value) __atomic_fetch_xor(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically performs XOR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the XOR operation is to be performed
 * @param value variable whose value is to be used for an XOR operation
 */
#define atomicFetchXor16(memory, value) __atomic_fetch_xor(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically performs XOR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the XOR operation is to be performed
 * @param value variable whose value is to be used for an XOR operation
 */
#define atomicFetchXor32(memory, value) __atomic_fetch_xor(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically performs XOR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the XOR operation is to be performed
 * @param value variable whose value is to be used for an XOR operation
 */
#define atomicFetchXor64(memory, value) __atomic_fetch_xor(memory, value, __ATOMIC_SEQ_CST)

/***********************************************************************************************************************
 * @brief Atomically adds the value to the variable that memory points to.
 * @return The initial value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the value is to be added
 * @param value variable whose value is to be added to the variable that memory points to
 */
#define atomicFetchAdd8(memory, value) __atomic_fetch_add(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically adds the value to the variable that memory points to.
 * @return The initial value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the value is to be added
 * @param value variable whose value is to be added to the variable that memory points to
 */
#define atomicFetchAdd16(memory, value) __atomic_fetch_add(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically adds the value to the variable that memory points to.
 * @return The initial value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the value is to be added
 * @param value variable whose value is to be added to the variable that memory points to
 */
#define atomicFetchAdd32(memory, value) __atomic_fetch_add(memory, value, __ATOMIC_SEQ_CST)
/**
 * @brief Atomically adds the value to the variable that memory points to.
 * @return The initial value of the variable that memory points to.
 * 
 * @param[in,out] memory pointer of a variable to which the value is to be added
 * @param value variable whose value is to be added to the variable that memory points to
 */
#define atomicFetchAdd64(memory, value) __atomic_fetch_add(memory, value, __ATOMIC_SEQ_CST)

#elif _WIN32
#include <intrin.h>
#include <windows.h>

/***********************************************************************************************************************
 * @brief Integer type for atomic operations. (int8)
 */
#define atomic_int8 volatile CHAR
/**
 * @brief Integer type for atomic operations. (int16)
 */
#define atomic_int16 volatile SHORT
/**
 * @brief Integer type for atomic operations. (int32)
 */
#define atomic_int32 volatile LONG
/**
 * @brief Integer type for atomic operations. (int64)
 */
#define atomic_int64 volatile LONG64

/**
 * @brief Atomically loads the value from the variable that memory points to.
 * @param[in] memory pointer of a variable from which the value is to be loaded
 * @return The current value of the variable that memory points to.
 */
static inline atomic_int8 atomicLoad8(atomic_int8* memory)
{
	atomic_int8 value = *memory;
	MemoryBarrier();
	return value;
}
/**
 * @brief Atomically loads the value from the variable that memory points to.
 * @param[in] memory pointer of a variable from which the value is to be loaded
 * @return The current value of the variable that memory points to.
 */
static inline atomic_int16 atomicLoad16(atomic_int16* memory)
{
	atomic_int16 value = *memory;
	MemoryBarrier();
	return value;
}
/**
 * @brief Atomically loads the value from the variable that memory points to.
 * @param[in] memory pointer of a variable from which the value is to be loaded
 * @return The current value of the variable that memory points to.
 */
static inline atomic_int32 atomicLoad32(atomic_int32* memory)
{
	atomic_int32 value = *memory;
	MemoryBarrier();
	return value;
}
/**
 * @brief Atomically loads the value from the variable that memory points to.
 * @param[in] memory pointer of a variable from which the value is to be loaded
 * @return The current value of the variable that memory points to.
 */
static inline atomic_int64 atomicLoad64(atomic_int64* memory)
{
	atomic_int64 value = *memory;
	MemoryBarrier();
	return value;
}

/***********************************************************************************************************************
 * @brief Atomically stores the value to the variable that memory points to.
 *
 * @param[out] memory pointer of a variable to which the value is to be stored
 * @param value variable whose value is to be stored to the variable that memory points to
 */
static inline void atomicStore8(atomic_int8* memory, atomic_int8 value)
{
	*memory = value;
	MemoryBarrier();
}
/**
 * @brief Atomically stores the value to the variable that memory points to.
 *
 * @param[out] memory pointer of a variable to which the value is to be stored
 * @param value variable whose value is to be stored to the variable that memory points to
 */
static inline void atomicStore16(atomic_int16* memory, atomic_int16 value)
{
	*memory = value;
	MemoryBarrier();
}
/**
 * @brief Atomically stores the value to the variable that memory points to.
 *
 * @param[out] memory pointer of a variable to which the value is to be stored
 * @param value variable whose value is to be stored to the variable that memory points to
 */
static inline void atomicStore32(atomic_int32* memory, atomic_int32 value)
{
	*memory = value;
	MemoryBarrier();
}
/**
 * @brief Atomically stores the value to the variable that memory points to.
 *
 * @param[out] memory pointer of a variable to which the value is to be stored
 * @param value variable whose value is to be stored to the variable that memory points to
 */
static inline void atomicStore64(atomic_int64* memory, atomic_int64 value)
{
	*memory = value;
	MemoryBarrier();
}

/***********************************************************************************************************************
 * @brief Atomically exchanges the value of the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the value is to be written
 * @param value variable whose value is to be written to the variable that memory points to
 */
#define atomicExchange8(memory, value) _InterlockedExchange8(memory, value)
/**
 * @brief Atomically exchanges the value of the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the value is to be written
 * @param value variable whose value is to be written to the variable that memory points to
 */
#define atomicExchange16(memory, value) _InterlockedExchange16(memory, value)
/**
 * @brief Atomically exchanges the value of the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the value is to be written
 * @param value variable whose value is to be written to the variable that memory points to
 */
#define atomicExchange32(memory, value) _InterlockedExchange(memory, value)
/**
 * @brief Atomically exchanges the value of the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the value is to be written
 * @param value variable whose value is to be written to the variable that memory points to
 */
#define atomicExchange64(memory, value) _InterlockedExchange64(memory, value)

/***********************************************************************************************************************
 * @brief Atomically performs AND operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the AND operation is to be performed
 * @param value variable whose value is to be used for an AND operation
 */
#define atomicFetchAnd8(memory, value) _InterlockedAnd8(memory, value)
/**
 * @brief Atomically performs AND operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the AND operation is to be performed
 * @param value variable whose value is to be used for an AND operation
 */
#define atomicFetchAnd16(memory, value) _InterlockedAnd16(memory, value)
/**
 * @brief Atomically performs AND operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the AND operation is to be performed
 * @param value variable whose value is to be used for an AND operation
 */
#define atomicFetchAnd32(memory, value) _InterlockedAnd(memory, value)
/**
 * @brief Atomically performs AND operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the AND operation is to be performed
 * @param value variable whose value is to be used for an AND operation
 */
#define atomicFetchAnd64(memory, value) _InterlockedAnd64(memory, value)

/***********************************************************************************************************************
 * @brief Atomically performs OR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the OR operation is to be performed
 * @param value variable whose value is to be used for an OR operation
 */
#define atomicFetchOr8(memory, value) _InterlockedOr8(memory, value)
/**
 * @brief Atomically performs OR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the OR operation is to be performed
 * @param value variable whose value is to be used for an OR operation
 */
#define atomicFetchOr16(memory, value) _InterlockedOr16(memory, value)
/**
 * @brief Atomically performs OR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the OR operation is to be performed
 * @param value variable whose value is to be used for an OR operation
 */
#define atomicFetchOr32(memory, value) _InterlockedOr(memory, value)
/**
 * @brief Atomically performs OR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the OR operation is to be performed
 * @param value variable whose value is to be used for an OR operation
 */
#define atomicFetchOr64(memory, value) _InterlockedOr64(memory, value)

/***********************************************************************************************************************
 * @brief Atomically performs XOR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the XOR operation is to be performed
 * @param value variable whose value is to be used for an XOR operation
 */
#define atomicFetchXor8(memory, value) _InterlockedXor8(memory, value)
/**
 * @brief Atomically performs XOR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the XOR operation is to be performed
 * @param value variable whose value is to be used for an XOR operation
 */
#define atomicFetchXor16(memory, value) _InterlockedXor16(memory, value)
/**
 * @brief Atomically performs XOR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the XOR operation is to be performed
 * @param value variable whose value is to be used for an XOR operation
 */
#define atomicFetchXor32(memory, value) _InterlockedXor(memory, value)
/**
 * @brief Atomically performs XOR operation to the variable that memory points to.
 * @return The current value of the variable that memory points to.
 *
 * @param[in,out] memory pointer of a variable to which the XOR operation is to be performed
 * @param value variable whose value is to be used for an XOR operation
 */
#define atomicFetchXor64(memory, value) _InterlockedXor64(memory, value)

/***********************************************************************************************************************
 * @brief Atomically adds the value to the variable that memory points to.
 * @return The initial value of the variable that memory points to.
 * 
 * @param[in,out] memory pointer of a variable to which the value is to be added
 * @param value variable whose value is to be added to the variable that memory points to
 */
#define atomicFetchAdd8(memory, value) _InterlockedExchangeAdd8(memory, value)
/**
 * @brief Atomically adds the value to the variable that memory points to.
 * @return The initial value of the variable that memory points to.
 * 
 * @param[in,out] memory pointer of a variable to which the value is to be added
 * @param value variable whose value is to be added to the variable that memory points to
 */
#define atomicFetchAdd16(memory, value) _InterlockedExchangeAdd16(memory, value)
/**
 * @brief Atomically adds the value to the variable that memory points to.
 * @return The initial value of the variable that memory points to.
 * 
 * @param[in,out] memory pointer of a variable to which the value is to be added
 * @param value variable whose value is to be added to the variable that memory points to
 */
#define atomicFetchAdd32(memory, value) _InterlockedExchangeAdd(memory, value)
/**
 * @brief Atomically adds the value to the variable that memory points to.
 * @return The initial value of the variable that memory points to.
 * 
 * @param[in,out] memory pointer of a variable to which the value is to be added
 * @param value variable whose value is to be added to the variable that memory points to
 */
#define atomicFetchAdd64(memory, value) _InterlockedExchangeAdd64(memory, value)

#else
#error Unknown operating system
#endif