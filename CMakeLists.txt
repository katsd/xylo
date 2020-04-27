cmake_minimum_required(VERSION 3.1)
project(Xylo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(./include)

add_subdirectory(src)

add_custom_target(run COMMAND "xylo" "../tests/test.xy")
