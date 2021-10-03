#pragma once
#include <stdlib.h>
#include <stdbool.h>

/* Mutual exclusion instance handle */
typedef struct Mutex* Mutex;
/* Condition variable instance handle */
typedef struct Cond* Cond;

/*
 * Create a new mutex instance.
 * Returns mutex on success, otherwise NULL.
 */
Mutex createMutex();

/*
 * Destroy mutex instance.
 * mutex - mutex instance or NULL.
 */
void destroyMutex(Mutex mutex);

/*
 * Lock mutex with blocking.
 * mutex - mutex instance.
 */
void lockMutex(Mutex mutex);

/*
 * Unlock locked mutex.
 * mutex - mutex instance.
 */
void unlockMutex(Mutex mutex);

/*
 * Try to lock the specified mutex without blocking.
 * Returns true if mutex was successfully locked.
 *
 * mutex - mutex instance.
 */
bool tryLockMutex(Mutex mutex);

/*
 * Create a new condition variable instance.
 * Returns cond on success, otherwise NULL.
 */
Cond createCond();

/*
 * Destroy condition variable instance.
 * cond - cond instance or NULL.
 */
void destroyCond(Cond cond);

/*
 * Unblock one of the waiting threads.
 * cond - cond instance.
 */
void signalCond(Cond cond);

/*
 * Unblock all waiting threads.
 * cond - cond instance.
 */
void broadcastCond(Cond cond);

/*
 * Block the current thread until the
 * condition variable is woken up or
 * a spurious wakeup occurs.
 *
 * cond - cond instance.
 * mutex - mutex instance.
 */
void waitCond(
	Cond cond,
	Mutex mutex);

/*
 * Block the current thread until the
 * condition variable is woken up or
 * after the specified timeout or
 * a spurious wakeup occurred.
 *
 * cond - cond instance.
 * mutex - mutex instance.
 * timeout - timeout time.
 */
void waitCondFor(
	Cond cond,
	Mutex mutex,
	double timeout);
