cmake_minimum_required(VERSION 3.1)
project(Xylo CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(./include)

add_subdirectory(src)

add_custom_target(run COMMAND "xylo" "../tests/test.xy")
