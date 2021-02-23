#pragma once
#include <stdlib.h>
#include <stdbool.h>

/* Thread instance handle */
struct Thread;

/*
 * Creates a new thread executing the specified function.
 * Returns thread on success, otherwise NULL.
 *
 * function - pointer to the valid function that should be invoked.
 * argument - argument that will be passed to the function.
 */
struct Thread* createThread(
	void (*function)(void*),
	void* argument);

/*
 * Destroys specified thread.
 * thread - pointer to the thread or NULL.
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
bool isThreadJoined(
	struct Thread* thread);

/*
 * Blocks the execution of the current thread for a specified time.
 * delay - thread sleep delay time (s).
 */
void sleepThread(
	double delay);

/*
 * Causes the current thread to yield execution to another thread.
 */
bool yieldThread();

/*
 * Returns current clock value (s).
 */
double getCurrentClock();
