cmake_minimum_required(VERSION 3.1)

file(GLOB SOURCE ./*.cpp ./*.hpp)

add_library(native ${SOURCE})
