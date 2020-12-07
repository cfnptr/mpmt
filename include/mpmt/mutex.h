#pragma once
#include <stdlib.h>
#include <stdbool.h>

/* Mutex instance handle */
struct Mutex;

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
void destroyMutex(
	struct Mutex* mutex);

/*
 * Locks mutex with blocking.
 * Returns true if mutex was successfully locked.
 *
 * mutex - pointer to the valid mutex.
 */
void lockMutex(
	struct Mutex* mutex);

/*
 * Unlocks locked mutex.
 * Returns true if mutex was successfully locked.
 *
 * mutex - pointer to the valid mutex.
 */
void unlockMutex(
	struct Mutex* mutex);

/*
 * Tries to lock the specified mutex without blocking.
 * Returns true if mutex was successfully locked.
 *
 * mutex - pointer to the valid mutex.
 */
bool tryLockMutex(
	struct Mutex* mutex);
