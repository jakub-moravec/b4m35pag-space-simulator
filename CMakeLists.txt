cmake_minimum_required(VERSION 3.5)
project(stars)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)

SET(CMAKE_CXX_FLAGS "-march=native -Ofast -fopenmp -fopt-info-vec")
#SET(CMAKE_CXX_FLAGS "-march=native -Ofast -fopt-info-vec")
#SET(CMAKE_CXX_FLAGS "-march=native -Ofast")

add_executable(stars ${SOURCE_FILES})