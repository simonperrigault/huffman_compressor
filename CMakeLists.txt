cmake_minimum_required(VERSION 3.0)

project(compressor)

set(CMAKE_CXX_STANDARD 11)

add_executable(compressor main.cpp Compressor.cpp Compressor.h)

target_compile_options(compressor PRIVATE -Wall -Wextra -g)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests tests.cpp Compressor.cpp Compressor.h)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)