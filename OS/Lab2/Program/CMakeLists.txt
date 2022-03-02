cmake_minimum_required(VERSION 3.21)
project(Program)

set(CMAKE_CXX_STANDARD 17)

add_executable(Program main.cpp)

include_directories(includes)
find_library(NUMBER_LIBRARY NAMES Number PATHS libraries)
find_library(VECTOR_LIBRARY NAMES Vector PATHS libraries)
target_link_libraries(Program ${NUMBER_LIBRARY} ${VECTOR_LIBRARY})
