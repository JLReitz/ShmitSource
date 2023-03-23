cmake_minimum_required(VERSION 3.16)
project(ShmitSource)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

# TODO see if these are acceptable
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_FLAGS -ggdb -pedantic -Wformat -Winline -Wreturn-type -Wsign-compare -Wsign-conversion -Wswitch -Wunused)
set(C_FLAGS -Wstrict-prototypes -Wtraditional)
set(CXX_FLAGS -Wreorder)

# Include google test using FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Declare ShmitCore as the build target
add_library(ShmitCore STATIC)

target_compile_options(ShmitCore
  PRIVATE
  "${COMMON_FLAGS}"
  "$<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>"
  "$<$<COMPILE_LANGUAGE:CXX>:${CXX_FLAGS}>"
)

# Build ShmitCore
add_subdirectory(Development/ShmitCore)

# Build Tests
enable_testing()
add_subdirectory(Test/ShmitCore)