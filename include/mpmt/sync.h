#pragma once
#include <stdlib.h>
#include <stdbool.h>

/* Mutual exclusion instance handle */
typedef struct Mutex Mutex;
/* Condition variable instance handle */
typedef struct Condvar Condvar;

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
 * Creates a new condvar.
 * Returns condvar on success, otherwise NULL.
 */
Condvar* createCondvar();

/*
 * Destroys the condvar.
 * condvar - pointer to the condvar or NULL.
 */
void destroyCondvar(Condvar* condvar);

/*
 * Unblocks one of the waiting threads.
 * condvar - pointer to the valid condvar.
 */
void signalCondvar(Condvar* condvar);

/*
 * Unblocks all of the waiting threads.
 * condvar - pointer to the valid condvar.
 */
void broadcastCondvar(Condvar* condvar);

/*
 * Blocks the current thread until the
 * condition variable is woken up or
 * a spurious wakeup occurs.
 *
 * condvar - pointer to the valid condvar.
 * mutex - pointer to the valid mutex.
 */
void waitCondvar(
	Condvar* condvar,
	Mutex* mutex);

/*
 * Blocks the current thread until the
 * condition variable is woken up or
 * after the specified timeout or
 * a spurious wakeup occurred.
 *
 * condvar - pointer to the valid condvar.
 * mutex - pointer to the valid mutex.
 * timeout - condvar timeout delay time.
 */
void waitCondvarFor(
	Condvar* condvar,
	Mutex* mutex,
	double timeout);
