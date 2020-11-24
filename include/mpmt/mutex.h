#pragma once
#include <stdbool.h>

struct Mutex;

struct Mutex* createMutex();

void destroyMutex(
	struct Mutex* mutex);

bool lockMutex(
	struct Mutex* mutex);
bool unlockMutex(
	struct Mutex* mutex);
bool tryLockMutex(
	struct Mutex* mutex);
