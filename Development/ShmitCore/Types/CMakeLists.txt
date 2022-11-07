# Include directories below this one
include(Types/Containers/CMakeLists.txt)
include(Types/Generic/CMakeLists.txt)
include(Types/Memory/CMakeLists.txt)

# Target source files located within this subdirectory
target_sources(ShmitCore
    PRIVATE
    Types/TypeHelp.cpp
)
