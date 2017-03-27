cmake_minimum_required(VERSION 3.5)
project(dummy)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG -g -std=gnu99")

set(SOURCE_FILES
        dummy_ansisop.h
        tests/analizadorLineaTest.c
        tests/utils/parserUtils.h tests/utils/parserUtils.h.c)

add_executable(dummy ${SOURCE_FILES})
include_directories(${CMAKE_SOURCE_DIR}/parser)
include_directories(${CMAKE_SOURCE_DIR}/cspec)
target_link_libraries(dummy cspecs)
target_link_libraries(dummy parser)
