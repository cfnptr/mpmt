#include "mpmt/thread.h"
#include <stdlib.h>

#if __linux__ || __APPLE__
#include <pthread.h>
#define THREAD pthread_t
#elif _WIN32
#include <windows.h>
#define THREAD HANDLE
#else
#error Unknown operating system
#endif

struct Thread
{
	bool detached;
	THREAD handle;
};
struct ThreadData
{
	void (*routine)(void*);
	void* argument;
};

#if __linux__ || __APPLE__
void* mpmtThreadRoutine(void* argument)
{
	struct ThreadData* data =
		(struct ThreadData*)argument;

	data->routine(
		data->argument);

	free(data);
	return NULL;
}
#elif _WIN32
DWORD mpmtThreadRoutine(LPVOID argument)
{
	struct ThreadData* data =
		(struct ThreadData*)argument;
	
	data_routine(
		data->argument);

	free(data);
	return 0;
}
#endif

struct Thread* mpmtCreateThread(
	void (*routine)(void*),
	void* argument)
{
	if(!routine)
		return NULL;

	struct Thread* thread =
		malloc(sizeof(struct Thread));

	if (!thread)
		return NULL;

	thread->detached = false;

	struct ThreadData* data =
		malloc(sizeof(struct ThreadData));

	if (!data)
	{
		free(thread);
		return NULL;
	}

	data->routine = routine;
	data->argument = argument;

	THREAD handle;

#if __linux__ || __APPLE__
	int result = pthread_create(
		&handle,
		NULL,
		mpmtThreadRoutine,
		data);

	if (result != 0)
	{
		free(data);
		free(thread);
		return NULL;
	}
#elif _WIN32
	LPDWORD threadID;

	handle = CreateThread(
		NULL, 
		0,
		mpmtThreadRoutine,
		data,
		0,
		&threadID);

	if (handle == NULL)
	{
		free(data);
		free(thread);
		return NULL;
	}
#endif

	thread->handle = handle;
	return thread;
}
void mpmtDestroyThread(
	struct Thread* thread)
{
	if (thread)
	{
		if (!thread->detached) 
		{
#if __linux__ || __APPLE__
			pthread_detach(
				thread->handle);
#elif _WIN32
			CloseHandle(
				thread->handle);
#endif
		}
	}

	free(thread);
}

bool mpmtJoinThread(
	struct Thread* thread)
{
	if(!thread || thread->detached)
		return false;

#if __linux__ || __APPLE__
	pthread_join(
		thread->handle,
		NULL);

	thread->detached = true;
	return true;
#elif _WIN32
	THREAD handle = thread->handle;

	WaitForSingleObject(
		handle,
		INFINITE);
	CloseHandle(
		handle);

	thread->detached = true;
	return true;
#endif
}
bool mpmtDetachThread(
	struct Thread* thread)
{
	if(!thread || thread->detached)
		return false;

#if __linux__ || __APPLE__
	pthread_detach(
		thread->handle);
#elif _WIN32
	CloseHandle(
		thread->handle);
#endif

	thread->detached = true;
	return true;
}
void mpmtExitThread()
{
#if __linux__ || __APPLE__
	pthread_exit(NULL);
#elif _WIN32
	ExitThread(0);
#endif
}
