cmake_minimum_required(VERSION 2.8.4)
project(lab_04)

set(SOURCE_FILES binarySearch.c)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -lm")

add_executable(bubble      bubble.c)
add_executable(bsearch     ${SOURCE_FILES} bSearch.c)
add_executable(hash_search hashSearch.c)

