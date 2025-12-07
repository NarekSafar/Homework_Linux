cmake_minimum_required(VERSION 3.10)
project(prime-calculator)

set(CMAKE_CXX_STANDARD 17)
file(GLOB SOURCES "*.cpp")
add_executable(prime-calculator ${SOURCES})

