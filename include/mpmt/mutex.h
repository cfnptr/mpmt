#pragma once
#include <stdlib.h>
#include <stdbool.h>

/* Mutex instance handle */
struct Mutex;
/* Condition variable instance handle */
struct ConditionVariable;

/*
 * Creates a new mutex.
 * Returns pointer to the valid mutex.
 */
struct Mutex* createMutex();

/*
 * Destroys the mutex.
 * Mutex should be unlocked before destruction.
 *
 * mutex - pointer to the valid mutex.
 */
void destroyMutex(struct Mutex* mutex);

/*
 * Blocks the current thread until the mutex is locked.
 * Mutex should be locked only once at the same thread.
 *
 * mutex - pointer to the valid mutex.
 */
void lockMutex(struct Mutex* mutex);

/*
 * Unlocks the mutex.
 * Mutex should be unlocked only once at the same thread.
 *
 * mutex - pointer to the valid mutex.
 */
void unlockMutex(struct Mutex* mutex);

/*
 * Tries to lock the specified mutex without blocking.
 * On successful result mutex should be unlocked.
 * Returns true if mutex was successfully locked.
 *
 * mutex - pointer to the valid mutex.
 */
bool tryLockMutex(struct Mutex* mutex);

/*
 * Creates a new condition variable.
 * Returns pointer to the valid condition variable.
 */
struct ConditionVariable* createConditionVariable();

/*
 * Destroys the condition variable.
 * Condition variable should be unlocked before destruction.
 *
 * conditionVariable - pointer to the valid the condition variable.
 */
void destroyConditionVariable(
	struct ConditionVariable* conditionVariable);

/*
 * Unblocks a single thread waiting on the specified condition variable.
 * conditionVariable - pointer to the valid the condition variable.
 */
void signalConditionVariable(
	struct ConditionVariable* conditionVariable);

/*
 * Unblocks all threads waiting on the specified condition variable.
 * conditionVariable - pointer to the valid the condition variable.
 */
void broadcastConditionVariable(
	struct ConditionVariable* conditionVariable);

/*
 * Waits for the specified condition variable and unlocks the mutex.
 * Mutex should be locked by the calling thread before function call.
 *
 * conditionVariable - pointer to the valid the condition variable.
 */
void waitConditionVariable(
	struct ConditionVariable* conditionVariable,
	struct Mutex* mutex);
