#pragma once
#include <stdlib.h>
#include <stdbool.h>

struct Thread;

struct Thread* createThread(
	void (*routine)(void*),
	void* argument);
void destroyThread(
	struct Thread* thread);

bool joinThread(
	struct Thread* thread);
bool detachThread(
	struct Thread* thread);
void sleepThread(
	size_t milliseconds);
