# Add ShmitSource includes to target
set(SHMITCORE_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/Development/Include)
target_include_directories(ShmitCore PUBLIC ${SHMITCORE_INCLUDE_PATH})

# This step marks headers as source files for the benefit of other IDEs (Visual Studio, XCode, etc.).
# They should pick up on it and add them automatically to the file list.
# Admittedly, globbing these files is probably not the best solution. This may need to be re-done sometime.
file(GLOB_RECURSE SHMITCORE_HEADERS
  "${SHMITCORE_INCLUDE_PATH}/ShmitCore/*.h"
  "${SHMITCORE_INCLUDE_PATH}/ShmitCore/*.hpp"
)
target_sources(ShmitCore
  PUBLIC
  ${SHMITCORE_HEADERS}
)

# Finally, add source subdirectories
# These will either add to the 'ShmitCore' target or create their own
# Using 'include' instead of 'add_subdirectory' to lay out a paradigm, get off my back
add_subdirectory(Containers)
add_subdirectory(Logging)
add_subdirectory(Platform)
add_subdirectory(Time)
add_subdirectory(Types)