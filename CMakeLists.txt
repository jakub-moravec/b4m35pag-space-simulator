cmake_minimum_required(VERSION 3.5)
project(b4m35pag_space_simulator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        generator.cpp)

add_executable(generator ${SOURCE_FILES})