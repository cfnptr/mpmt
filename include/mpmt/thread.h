#pragma once
#include <stdlib.h>
#include <stdbool.h>

/* Thread instance handle */
struct Thread;

/*
 * Creates a new thread executing the specified function.
 * Returns thread on success, otherwise null.
 * Thread create function call is not thread safe.
 *
 * function - pointer to the valid function that should be invoked.
 * argument - argument that will be passed to the function.
 */
struct Thread* createThread(
	void (*function)(void*),
	void* argument);

/*
 * Destroys specified thread.
 * Thread destroy function call is not thread safe.
 *
 * thread - pointer to the valid thread or null.
 */
void destroyThread(
	struct Thread* thread);

/*
 * Blocks the current thread until the function execution end.
 * Thread join function call is not thread safe.
 *
 * thread - pointer to the valid thread.
 */
bool joinThread(
	struct Thread* thread);

/*
 * Returns true if thread was previously joined.
 * Is thread joined function call is not thread safe.
 *
 * thread - pointer to the valid thread.
 */
bool isThreadJoined(
	struct Thread* thread);

/*
 * Blocks the execution of the current thread for a specified time.
 * milliseconds - thread sleep delay time.
 */
void sleepThread(
	size_t milliseconds);
