cmake_minimum_required(VERSION 3.5)
project(dummy)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG -g")

set(SOURCE_FILES
        dummy_ansisop.c
        dummy_ansisop.h
        main.c)

add_executable(dummy ${SOURCE_FILES})
include_directories(${CMAKE_SOURCE_DIR}/parser)
target_link_libraries(dummy parser)

install(TARGETS dummy RUNTIME DESTINATION bin)