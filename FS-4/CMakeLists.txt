cmake_minimum_required(VERSION 3.10)
project(redirect-cin)

set(CMAKE_CXX_STANDARD 17)
file(GLOB SOURCES "*.cpp")
add_executable(redirect-cin ${SOURCES})

