#pragma once
#include <stdbool.h>

struct Thread;

struct Thread* mpmtCreateThread(
	void (*routine)(void*),
	void* argument);
void mpmtDestroyThread(
	struct Thread* thread);

bool mpmtJoinThread(
	struct Thread* thread);
bool mpmtDetachThread(
	struct Thread* thread);
void mpmtExitThread(
	void* result);
