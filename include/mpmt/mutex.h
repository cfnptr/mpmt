#pragma once
#include <stdlib.h>
#include <stdbool.h>

/* Mutex instance handle */
struct Mutex;
/* Condition variable instance handle */
struct ConditionVariable;

/*
 * Creates a new mutex.
 * Returns mutex on success, otherwise null.
 * Mutex create function call is not thread safe.
 */
struct Mutex* createMutex();

/*
 * Destroys the mutex.
 * Mutex should be unlocked on destruction.
 * Mutex destroy function call is not thread safe.Z
 *
 * mutex - pointer to the valid mutex.
 */
void destroyMutex(struct Mutex* mutex);

/*
 * Locks mutex and executes function with blocking.
 * Returns true if mutex was successfully locked.
 *
 * mutex - pointer to the valid mutex.
 * function - pointer to the valid function.
 * argument - pointer to the function argument.
 */
bool lockMutex(
	struct Mutex* mutex,
	void (*function)(void*),
	void* argument);

/*
 * Tries to lock the specified mutex without blocking.
 * Returns true if mutex was successfully locked.
 *
 * mutex - pointer to the valid mutex.
 * function - pointer to the valid function.
 * argument - pointer to the function argument.
 */
bool tryLockMutex(
	struct Mutex* mutex,
	void (*function)(void*),
	void* argument);
