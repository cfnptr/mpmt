cmake_minimum_required(VERSION 3.10)

if(${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "Prevented in-tree build")
endif()

project(mpmt
	VERSION 0.2.0
	LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

configure_file(
	cmake/defines.h.in
	mpmt/defines.h)

add_library(mpmt STATIC
	source/mpmt/mutex.c
	source/mpmt/thread.c)
target_include_directories(mpmt PUBLIC
	${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}/include)
