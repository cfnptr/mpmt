# MPMT  

A small [library](https://github.com/cfnptr/mpmt) providing generic interface for **multithreading** across different platforms.<br/>
Created due to the fact that macOS does not support `<threads.h>` in C11.

See the [documentation](https://cfnptr.github.io/mpmt).

## Features

* Mutex (Mutual exclusion)
* Cond (Condition variable)
* Thread (sleep, yield, etc.)
* Thread pool (tasks)
* Atomics (fetch add)

## Usage example

```c
void mutexExample()
{
    Mutex mutex = createMutex();

    if (!mutex)
        abort();

    lockMutex(mutex);
    // Do some synchronized work...
    unlockMutex(mutex);

    destroyMutex(mutex);
}

// ========================================

static void onUpdate(void* arument)
{
    volatile bool* isRunning = argument;
    
    while (*isRunning)
    {
        // Do some parallel work...
        sleepThread(0.001);
    }
}

void threadExample()
{
    volatile bool isRunning = true;

    Thread thread = createThread(
        onUpdate, &isRunning);

    if (!thread)
        abort();

    isRunning = false;
    joinThread(thread);
    destroyThread(thread);
}
```

## Supported operating systems

* Windows (10/11)
* Ubuntu (22.04/24.04)
* macOS (14/15)

This list includes only those systems on which functionality testing is conducted.
However, you can also compile it under any other Linux distribution or operating system.

## Build requirements

* C99 compiler
* C++17 compiler (optional)
* [Git 2.30+](https://git-scm.com/)
* [CMake 3.10+](https://cmake.org/)

Use building [instructions](BUILDING.md) to install all required tools and libraries.

### CMake options

| Name                | Description               | Default value |
|---------------------|---------------------------|---------------|
| MPMT_BUILD_SHARED   | Build MPMT shared library | `ON`          |
| MPMT_BUILD_TESTS    | Build MPMT library tests  | `ON`          |
| MPMT_BUILD_EXAMPLES | Build MPMT usage examples | `ON`          |

### CMake targets

| Name        | Description          | Windows | macOS    | Linux |
|-------------|----------------------|---------|----------|-------|
| mpmt-static | Static MPMT library  | `.lib`  | `.a`     | `.a`  |
| mpmt-shared | Dynamic MPMT library | `.dll`  | `.dylib` | `.so` |

## Cloning

```
git clone https://github.com/cfnptr/mpmt
```

## Building ![CI](https://github.com/cfnptr/mpmt/actions/workflows/cmake.yml/badge.svg)

* Windows: ```./scripts/build-release.bat```
* macOS / Ubuntu: ```./scripts/build-release.sh```

## Usage

Thread example: [examples/thread_example.c](examples/thread_example.c)<br/>
Mutex example: [examples/mutex_example.c](examples/mutex_example.c)