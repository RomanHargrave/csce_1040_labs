cmake_minimum_required(VERSION 2.8.4)
project(lab01)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -D__DEBUG")

include_directories(course/)

set(SOURCE_FILES
    course/bubble.c
    lab_one.c)

add_executable(lab01 ${SOURCE_FILES})