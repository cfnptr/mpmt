# MPMT 
A small library providing a generic interface for multithreading across different platforms.<br>
Created due to the fact that MacOS does not support <threads.h> in C11.

![CI](https://github.com/cfnptr/mpmt/actions/workflows/cmake.yml/badge.svg)

## Features
* Thread
* Mutex (Mutual exclusion)
* Cond (Condition variable)
* (sleep, yield, getTime, etc.)

## Supported operating systems
* Ubuntu
* MacOS
* Windows

## Build requirements
* C99 compiler
* [CMake 3.10+](https://cmake.org/)

## Cloning
```
git clone https://github.com/cfnptr/mpmt
cd mpmt
```

## Building
```
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build/
cmake --build build/
```

### CMake options
| Name                | Description               | Default value |
|---------------------|---------------------------|---------------|
| MPMT_BUILD_EXAMPLES | Build MPMT usage examples | ON            |

## Usage
Thread example: [examples/thread_example.c](https://github.com/cfnptr/mpmt/blob/main/examples/thread_example.c)<br>
Mutex example: [examples/mutex_example.c](https://github.com/cfnptr/mpmt/blob/main/examples/mutex_example.c)
