cmake_minimum_required(VERSION 3.5)
project(dummy-cpu)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG -g -std=gnu99 -Wall")

set(SOURCE_FILES
        dummy_ansisop.h
        dummy_ansisop.c
        runner.c)

add_executable(dummy-cpu ${SOURCE_FILES})
include_directories(${CMAKE_SOURCE_DIR}/parser)
target_link_libraries(dummy-cpu parser)
