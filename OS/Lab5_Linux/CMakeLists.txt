cmake_minimum_required(VERSION 3.21)
project(Lab5_Linux)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Lab5_Linux main.cpp Matrix.cpp Matrix.h MatrixCreator.cpp MatrixCreator.h MatrixMultiplicator.cpp MatrixMultiplicator.h)
