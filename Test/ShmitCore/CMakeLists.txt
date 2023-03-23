include(GoogleTest)

add_library(ShmitCore-Test INTERFACE)
target_include_directories(ShmitCore-Test
  INTERFACE
  ${CMAKE_SOURCE_DIR}/Test/Include
)

target_link_libraries(ShmitCore-Test
  INTERFACE
  ShmitCore
  ShmitCore-PlatformClock-Native
)

# Finally, include all subdirectories
add_subdirectory(Containers)
add_subdirectory(Math)