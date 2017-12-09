cmake_minimum_required(VERSION 3.5)
project(b4m35pag_space_simulator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp main.h gif.h)

add_executable(stars ${SOURCE_FILES})