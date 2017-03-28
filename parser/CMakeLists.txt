cmake_minimum_required(VERSION 3.5)
project(parser)

message(STATUS "corriendo cmake...")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG -g")

set(SOURCE_FILES
    parser/metadata_program.c
    parser/metadata_program.h
    parser/parser.c
    parser/parser.h
    parser/sintax.h)

add_library(parser SHARED ${SOURCE_FILES})
target_link_libraries(parser commons)

install(TARGETS parser
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
