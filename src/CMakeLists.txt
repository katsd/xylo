cmake_minimum_required(VERSION 3.1)

add_executable(xylo main.cpp)

target_link_libraries(xylo ast)
target_link_libraries(xylo codegen)
target_link_libraries(xylo token)
target_link_libraries(xylo parser)
target_link_libraries(xylo vm)
target_link_libraries(xylo native)

add_subdirectory(ast)
add_subdirectory(codegen)
add_subdirectory(token)
add_subdirectory(parser)
add_subdirectory(vm)
add_subdirectory(native)