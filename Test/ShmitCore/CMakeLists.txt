# Set ShmitSource include path
set(SHMITCORE_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/Development/Include)

# This step marks headers as source files for the benefit of other IDEs (Visual Studio, XCode, etc.).
# They should pick up on it and add them automatically to the file list.
# Admittedly, globbing these files is probably not the best solution. This may need to be re-done sometime.
file(GLOB_RECURSE SHMITCORE_HEADERS
  "${SHMITCORE_INCLUDE_PATH}/ShmitCore/*.h"
  "${SHMITCORE_INCLUDE_PATH}/ShmitCore/*.hpp"
)

# Finally, include all subdirectories
include(Types/CMakeLists.txt)