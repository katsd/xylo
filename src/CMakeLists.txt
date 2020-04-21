cmake_minimum_required(VERSION 3.1)

file(GLOB SOURCE ./*.cpp ./.hpp)

#add_executable(xylo ${SOURCE})
add_executable(xylo main.cpp xylo.hpp)

target_link_libraries(xylo ast)
target_link_libraries(xylo codegen)
target_link_libraries(xylo token)
target_link_libraries(xylo parser)
target_link_libraries(xylo vm)

add_subdirectory(AST)
add_subdirectory(CodeGen)
add_subdirectory(Token)
add_subdirectory(Parser)
add_subdirectory(VM)