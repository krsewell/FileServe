# cmake_minimum_required(VERSION <specify CMake version here>)
project(FileServe)

set(CMAKE_CXX_STANDARD 14)

add_executable(FileServer src/server.cpp src/Connection.cpp src/include/Connection.h )
add_executable(FileClient src/client.cpp src/Connection.cpp src/include/Connection.h )