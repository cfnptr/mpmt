cmake_minimum_required(VERSION 3.10)

if (${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "Prevented in-tree build")
endif ()

project(mpmt
	VERSION 0.8.1
	DESCRIPTION "Multi-platform multi-threading library"
	HOMEPAGE_URL "https://github.com/cfnptr/mpmt"
	LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

option(MPMT_BUILD_EXAMPLES "Build MPMT examples" ON)

find_package(Threads REQUIRED)

configure_file(
	cmake/defines.h.in
	mpmt/defines.h)

add_library(mpmt STATIC
	source/sync.c
	source/thread.c)
target_link_libraries(mpmt PUBLIC
	Threads::Threads)
target_include_directories(mpmt PUBLIC
	${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}/include
	${CMAKE_THREAD_LIBS_INIT})

if (MPMT_BUILD_EXAMPLES)
	add_executable(mpmt-thread-example
		examples/thread_example.c)
	target_link_libraries(mpmt-thread-example PRIVATE
		mpmt)
	target_include_directories(mpmt-thread-example PRIVATE
		${PROJECT_BINARY_DIR}
		${PROJECT_SOURCE_DIR}/include)

	add_executable(mpmt-mutex-example
		examples/mutex_example.c)
	target_link_libraries(mpmt-mutex-example PRIVATE
		mpmt)
	target_include_directories(mpmt-mutex-example PRIVATE
		${PROJECT_BINARY_DIR}
		${PROJECT_SOURCE_DIR}/include)
endif ()
