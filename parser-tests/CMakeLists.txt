cmake_minimum_required(VERSION 3.5)
project(tests)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG -g -std=gnu99 -Wall")

set(SOURCE_FILES
        analizadorLineaTest.c
        parserUtils.h
        parserUtils.c)

add_executable(tests ${SOURCE_FILES})
include_directories(${CMAKE_SOURCE_DIR}/parser)
include_directories(${CMAKE_SOURCE_DIR}/cspec)
target_link_libraries(tests cspecs)
target_link_libraries(tests parser)
