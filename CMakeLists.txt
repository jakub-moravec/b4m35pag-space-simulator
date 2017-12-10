cmake_minimum_required(VERSION 3.5)
project(b4m35pag_space_simulator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp main.h gif.h )

SET(CMAKE_CXX_FLAGS "-march=native -Ofast -fopenmp -fopt-info-vec")

add_executable(stars ${SOURCE_FILES})
#add_executable(stars generator.cpp)