#pragma once
#include <stdbool.h>

struct Mutex;

/*
 *
 * Creates a new mutex.
 * Returns pointer to the valid mutex on success, otherwise null.
 */
struct Mutex* createMutex();

/*
 *
 * Destroys the mutex.
 * Mutex should be unlocked before destruction.
 *
 * mutex - pointer to the valid mutex.
 */
void destroyMutex(struct Mutex* mutex);

/*
 *
 * Blocks the current thread until the mutex is locked.
 * Mutex should be locked only once at the same thread.
 *
 * mutex - pointer to the valid mutex.
 */
void lockMutex(struct Mutex* mutex);

/*
 *
 * Unlocks the mutex.
 * Mutex should be unlocked only once at the same thread.
 *
 * mutex - pointer to the valid mutex.
 */
void unlockMutex(struct Mutex* mutex);

/*
 *
 * Tries to lock the specified mutex without blocking.
 * On successful result mutex should be unlocked.
 * Returns true if mutex was successfully locked.
 *
 * mutex - pointer to the valid mutex.
 */
bool tryLockMutex(struct Mutex* mutex);
