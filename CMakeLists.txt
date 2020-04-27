cmake_minimum_required(VERSION 3.1)
project(Xylo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(./include)

add_subdirectory(src)

add_custom_target(run COMMAND "xylo" "../tests/test.xy")
add_custom_target(emit-ast COMMAND "xylo" "../tests/test.xy" "-emit-ast")
add_custom_target(emit-bytecode COMMAND "xylo" "../tests/test.xy" "-emit-bytecode")
