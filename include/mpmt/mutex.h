#pragma once
#include <stdlib.h>
#include <stdbool.h>

/* Mutex instance handle */
struct Mutex;

/*
 * Creates a new mutex.
 * Returns mutex on success, otherwise NULL.
 */
struct Mutex* createMutex();

/*
 * Destroys the mutex.
 * mutex - pointer to the mutex or NULL.
 */
void destroyMutex(
	struct Mutex* mutex);

/*
 * Locks mutex with blocking.
 * mutex - pointer to the valid mutex.
 */
void lockMutex(
	struct Mutex* mutex);

/*
 * Unlocks locked mutex.
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
