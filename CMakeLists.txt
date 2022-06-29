cmake_minimum_required(VERSION 3.16)
project(ShmitSource)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

# TODO see if these are acceptable
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
  -pedantic

  # -Wall
  # -Wextra
  # -Wconversion
  -Wsign-conversion
  -Og
)

# Include google test using FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.8.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Build ShmitCore
add_subdirectory(Development/ShmitCore)