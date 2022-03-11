# MPMT  ![CI](https://github.com/cfnptr/mpmt/actions/workflows/cmake.yml/badge.svg)

A small library providing generic interface for **multithreading** across different platforms.<br/>
Created due to the fact that macOS does not support `<threads.h>` in C11.

## Features

* Mutex (Mutual exclusion)
* Cond (Condition variable)
* Thread (sleep, yield, etc.)
* Thread pool (tasks)
* Atomics

## Supported operating systems

* Ubuntu
* MacOS
* Windows

## Build requirements

* C99 compiler
* [Git 2.30+](https://git-scm.com/)
* [CMake 3.10+](https://cmake.org/)

### CMake options

| Name                | Description               | Default value |
|---------------------|---------------------------|---------------|
| MPMT_BUILD_SHARED   | Build MPMT shared library | `ON`          |
| MPMT_BUILD_TESTS    | Build MPMT library tests  | `ON`          |
| MPMT_BUILD_EXAMPLES | Build MPMT usage examples | `ON`          |

## Cloning

```
git clone --recursive https://github.com/cfnptr/mpmt
```

## Usage

Thread example: [examples/thread_example.c](https://github.com/cfnptr/mpmt/blob/main/examples/thread_example.c)<br/>
Mutex example: [examples/mutex_example.c](https://github.com/cfnptr/mpmt/blob/main/examples/mutex_example.c)
