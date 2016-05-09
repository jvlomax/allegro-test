cmake_minimum_required(VERSION 2.8.4)
project(allegro_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    test.cpp)

add_executable(allegro_test ${SOURCE_FILES})