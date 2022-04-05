cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(merdog)

include_directories("./src/includes")

add_executable(merdog
    ./src/sources/binary_op.cpp
    ./src/sources/block.cpp
    ./src/sources/branch_loop.cpp
    ./src/sources/decl.cpp 
    ./src/sources/environment.cpp 
    ./src/sources/error.cpp 
    ./src/sources/expr.cpp 
    ./src/sources/lexer.cpp 
    ./src/sources/main.cpp
    ./src/sources/memory.cpp 
    ./src/sources/namespace.cpp
    ./src/sources/symbols.cpp  
    ./src/sources/operator.cpp 
    ./src/sources/type.cpp 
    ./src/sources/unary_op.cpp 
    ./src/sources/unit.cpp 
)