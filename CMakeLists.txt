cmake_minimum_required(VERSION 3.0)

project(compressor)

set(CMAKE_CXX_STANDARD 11)

add_executable(compressor main.cpp)

add_library(compressor_lib STATIC Compressor.cpp Compressor.h)
target_link_libraries(compressor compressor_lib)