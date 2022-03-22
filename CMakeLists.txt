cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(merdog)

include_directories("./src/includes")
add_executable(merdog
    ./src/sources/main.cpp
)