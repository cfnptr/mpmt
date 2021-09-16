## Features
* Thread
* Mutex (Mutual exclusion)
* Cond (Condition variable)

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
| ------------------- | ------------------------- | ------------- |
| MPMT_BUILD_EXAMPLES | Build MPMT usage examples | ON            |
