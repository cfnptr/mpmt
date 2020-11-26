#pragma once
#include <stdlib.h>
#include <stdbool.h>

struct Thread;

/*
 *
 * Creates a new thread executing the specified function.
 * Returns pointer to the valid thread on success, otherwise null.
 *
 * function - pointer to the valid function that should be invoked.
 * argument - argument that will be passed to the function.
 */
struct Thread* createThread(
	void (*function)(void*),
	void* argument);

/*
 *
 * Destroys specified thread.
 * Thread should be joined or detached before destruction.
 *
 * thread - pointer to the valid thread or null.
 */
void destroyThread(struct Thread* thread);

/*
 *
 * Blocks the current thread until the specified thread finishes execution.
 * Should be called only once, otherwise undefined behavior.
 *
 * thread - pointer to the valid, not previously joined or detached thread.
 */
void joinThread(struct Thread* thread);

/*
 *
 * Detaches the specified thread from the current environment.
 * Should be called only once, otherwise undefined behavior.
 *
 * thread - pointer to the valid, not previously joined or detached thread.
 */
void detachThread(struct Thread* thread);

/*
 *
 * Blocks the execution of the current thread for a specified time.
 */
void sleepThread(size_t milliseconds);
