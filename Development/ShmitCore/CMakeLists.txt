# Declare ShmitCore as the build target
add_library(ShmitCore "")

# Add source subdirectories
add_subdirectory(Platform)
add_subdirectory(Types)

# Add ShmitSource includes to target
target_include_directories(ShmitCore PUBLIC ${CMAKE_SOURCE_DIR}/Development/Include)

# This step marks headers as source files for the benefit of other development environments (Visual Studio, XCode, etc.).
# They should pick up on it and add them automatically to the file list.
# Admittedly, globbing these files is probably not the best solution. This may need to be re-done sometime.
file (GLOB_RECURSE SHMITCORE_HEADERS
  "Development/Include/ShmitCore/*.h"
  "Development/Include/ShmitCore/*.hpp"
)
target_sources(ShmitCore
  PUBLIC
    ${SHMITCORE_HEADERS}
)