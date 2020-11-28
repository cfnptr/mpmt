#pragma once
#include "mpmt/mutex.h"

struct ConditionVariable;

/*
 *
 * Creates a new condition variable.
 * Returns pointer to the valid condition variable.
 */
struct ConditionVariable* createConditionVariable();

/*
 *
 * Destroys the condition variable.
 * Condition variable should be unlocked before destruction.
 *
 * conditionVariable - pointer to the valid the condition variable.
 */
void destroyConditionVariable(
	struct ConditionVariable* conditionVariable);

/*
 *
 * Unblocks a single thread waiting on the specified condition variable.
 * conditionVariable - pointer to the valid the condition variable.
 */
void signalConditionVariable(
	struct ConditionVariable* conditionVariable);

/*
 *
 * Unblocks all threads waiting on the specified condition variable.
 * conditionVariable - pointer to the valid the condition variable.
 */
void broadcastConditionVariable(
	struct ConditionVariable* conditionVariable);

/*
 *
 * Waits for the specified condition variable and unlocks the mutex.
 * Mutex should be locked by the calling thread before function call.
 *
 * conditionVariable - pointer to the valid the condition variable.
 */
void waitConditionVariable(
	struct ConditionVariable* conditionVariable,
	struct Mutex* mutex);
