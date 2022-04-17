cmake_minimum_required (VERSION 3.16)
project (ShmitSource)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

# TODO see if these are acceptable
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
  -Wall
  #-Wextra
  #-Wconversion
  -Wsign-conversion
  -Og
)

# Build ShmitCore
add_subdirectory(Development/ShmitCore)