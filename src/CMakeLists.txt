cmake_minimum_required(VERSION 3.1)

file(GLOB SOURCE ./*.cpp ./.hpp)

add_executable(xylo ${SOURCE})

target_link_libraries(xylo vm)

add_subdirectory(vm)