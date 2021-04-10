#pragma once
#include <stdlib.h>
#include <stdbool.h>

/* Mutual exclusion instance handle */
typedef struct Mutex Mutex;
/* Condition variable instance handle */
typedef struct Cond Cond;

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

/*
 * Creates a new cond.
 * Returns cond on success, otherwise NULL.
 */
Cond* createCond();

/*
 * Destroys the cond.
 * cond - pointer to the cond or NULL.
 */
void destroyCondvar(Cond* condvar);

/*
 * Unblocks one of the waiting threads.
 * cond - pointer to the valid cond.
 */
void signalCond(Cond* cond);

/*
 * Unblocks all of the waiting threads.
 * cond - pointer to the valid cond.
 */
void broadcastCond(Cond* cond);

/*
 * Blocks the current thread until the
 * condition variable is woken up or
 * a spurious wakeup occurs.
 *
 * cond - pointer to the valid cond.
 * mutex - pointer to the valid mutex.
 */
void waitCond(
	Cond* cond,
	Mutex* mutex);

/*
 * Blocks the current thread until the
 * condition variable is woken up or
 * after the specified timeout or
 * a spurious wakeup occurred.
 *
 * cond - pointer to the valid cond.
 * mutex - pointer to the valid mutex.
 * timeout - condvar timeout delay time.
 */
void waitCondFor(
	Cond* cond,
	Mutex* mutex,
	double timeout);
