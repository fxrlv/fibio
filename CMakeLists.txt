cmake_minimum_required (VERSION 2.8)
project(fibio)

set(CMAKE_BUILD_TYPE Debug)

include_directories(include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)
add_subdirectory(coro)
add_subdirectory(example)
