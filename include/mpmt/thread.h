#pragma once
#include <stdlib.h>
#include <stdbool.h>

/* Thread instance handle */
struct Thread;

/*
 * Creates a new thread executing the specified function.
 * Returns thread on success, otherwise null.
 *
 * function - pointer to the valid function that should be invoked.
 * argument - argument that will be passed to the function.
 */
struct Thread* createThread(
	void (*function)(void*),
	void* argument);

/*
 * Destroys specified thread.
 * thread - pointer to the valid thread or null.
 */
void destroyThread(
	struct Thread* thread);

/*
 * Blocks the current thread until the function execution end.
 * thread - pointer to the valid thread.
 */
void joinThread(
	struct Thread* thread);

/*
 * Returns thread current join status.
 * thread - pointer to the valid thread.
 */
bool getThreadJoined(
	struct Thread* thread);

/*
 * Blocks the execution of the current thread for a specified time.
 * milliseconds - thread sleep delay time.
 */
void sleepThread(
	size_t milliseconds);
