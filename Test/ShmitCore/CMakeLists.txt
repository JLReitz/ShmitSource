# Set ShmitSource include path
set(SHMITCORE_INCLUDES
  ${CMAKE_SOURCE_DIR}/Development/Include
)

set(SHMITCORE_TEST_INCLUDES
  ${CMAKE_SOURCE_DIR}/Test/Include
)

# This step marks headers as source files for the benefit of other IDEs (Visual Studio, XCode, etc.).
# They should pick up on it and add them automatically to the file list.
# Admittedly, globbing these files is probably not the best solution. This may need to be re-done sometime.
file(GLOB_RECURSE SHMITCORE_HEADERS
  "${SHMITCORE_INCLUDE_PATH}/ShmitCore/*.h"
  "${SHMITCORE_INCLUDE_PATH}/ShmitCore/*.hpp"
)

# Finally, include all subdirectories
add_subdirectory(Containers)
add_subdirectory(Math)