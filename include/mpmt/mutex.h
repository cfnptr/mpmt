#pragma once
#include <stdlib.h>
#include <stdbool.h>

/* Mutex instance handle */
typedef struct Mutex Mutex;

/*
 * Creates a new mutex.
 * Returns mutex on success, otherwise NULL.
 */
Mutex* createMutex();

/*
 * Destroys the mutex.
 * mutex - pointer to the mutex or NULL.
 */
void destroyMutex(Mutex* mutex);

/*
 * Locks mutex with blocking.
 * mutex - pointer to the valid mutex.
 */
void lockMutex(Mutex* mutex);

/*
 * Unlocks locked mutex.
 * mutex - pointer to the valid mutex.
 */
void unlockMutex(Mutex* mutex);

/*
 * Tries to lock the specified mutex without blocking.
 * Returns true if mutex was successfully locked.
 *
 * mutex - pointer to the valid mutex.
 */
bool tryLockMutex(Mutex* mutex);
