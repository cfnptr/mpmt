# Copyright 2020-2025 Nikita Fediuchin. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.10)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "Prevented in-tree build")
endif()

if(TARGET mpmt-static)
	return()
endif()

project(mpmt VERSION 1.7.5 LANGUAGES C
	DESCRIPTION "Multi-platform multi-threading library"
	HOMEPAGE_URL "https://github.com/cfnptr/mpmt")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

option(MPMT_BUILD_SHARED "Build MPMT shared library" ON)
option(MPMT_BUILD_TESTS "Build MPMT library tests" ON)
option(MPMT_BUILD_EXAMPLES "Build MPMT usage examples" ON)

find_package(Threads REQUIRED)
configure_file(cmake/defines.h.in include/mpmt/defines.h)

set(MPMT_SOURCES source/sync.c source/thread.c source/thread_pool.c)
set(MPMT_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/include ${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/wrappers/cpp ${CMAKE_THREAD_LIBS_INIT})

add_library(mpmt-static STATIC ${MPMT_SOURCES})
target_link_libraries(mpmt-static PUBLIC Threads::Threads)
target_include_directories(mpmt-static PUBLIC ${MPMT_INCLUDE_DIRS})

if(MPMT_BUILD_SHARED)
	add_library(mpmt-shared SHARED ${MPMT_SOURCES})
	set_target_properties(mpmt-shared PROPERTIES OUTPUT_NAME
		"mpmt" WINDOWS_EXPORT_ALL_SYMBOLS ON)
	target_link_libraries(mpmt-shared PUBLIC Threads::Threads)
	target_include_directories(mpmt-shared PUBLIC ${MPMT_INCLUDE_DIRS})
endif()

if(MPMT_BUILD_EXAMPLES)
	add_executable(mpmt-thread-example examples/thread_example.c)
	target_link_libraries(mpmt-thread-example PRIVATE mpmt-static)
	target_include_directories(mpmt-thread-example PRIVATE
		${PROJECT_BINARY_DIR}/include ${PROJECT_SOURCE_DIR}/include)

	add_executable(mpmt-mutex-example examples/mutex_example.c)
	target_link_libraries(mpmt-mutex-example PRIVATE mpmt-static)
	target_include_directories(mpmt-mutex-example PRIVATE
		${PROJECT_BINARY_DIR}/include ${PROJECT_SOURCE_DIR}/include)
endif()

if(MPMT_BUILD_TESTS)
	enable_testing()

	add_executable(TestMpmtSync tests/test_sync.c)
	target_link_libraries(TestMpmtSync PUBLIC mpmt-static)
	add_test(NAME TestMpmtSync COMMAND TestMpmtSync)

	add_executable(TestMpmtThreadPool tests/test_thread_pool.c)
	target_link_libraries(TestMpmtThreadPool PUBLIC mpmt-static)
	add_test(NAME TestMpmtThreadPool COMMAND TestMpmtThreadPool)

	# TODO: test atomics
endif()
