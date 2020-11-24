#pragma once
#include <stdbool.h>

struct Mutex;

struct Mutex* mpmtCreateMutex();
void mpmtDestroyMutex(
	struct Mutex* mutex);

bool mpmtLockMutex(
	struct Mutex* mutex);
bool mpmtUnlockMutex(
	struct Mutex* mutex);
bool mpmtTryLockMutex(
	struct Mutex* mutex);
