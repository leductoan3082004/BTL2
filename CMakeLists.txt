cmake_minimum_required(VERSION 3.25)
project(A2_initial)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(__MACOSX)

add_executable(A2_initial
        __MACOSX/._knight2.cpp
        __MACOSX/._knight2.h
        knight2.cpp
        knight2.h
        main.cpp
        main.h
        test.cpp)
