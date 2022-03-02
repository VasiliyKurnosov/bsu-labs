cmake_minimum_required(VERSION 3.21)
project(Vector)

set(CMAKE_CXX_STANDARD 17)

add_library(Vector SHARED Vector.cpp Vector.h)

include_directories(includes)
find_library(NUMBER_LIBRARY NAMES Number PATHS libraries)
target_link_libraries(Vector ${NUMBER_LIBRARY})
