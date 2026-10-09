cmake_minimum_required(VERSION 3.10)
project(matrix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(matrix
    main.cpp
    Symbol.cpp
    Line.cpp
    AppManager.cpp
    ConsoleHelper.cpp
)