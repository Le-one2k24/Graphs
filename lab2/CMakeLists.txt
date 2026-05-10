cmake_minimum_required(VERSION 3.10)
project(lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC_FILES
     ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(lab2 ${SRC_FILES})

target_compile_options(lab2 PRIVATE -Wall -Wextra -Wpedantic)
