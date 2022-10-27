# Include directories below this one
include(Logging/Data/CMakeLists.txt)
include(Logging/Diagnostics/CMakeLists.txt)
include(Logging/Events/CMakeLists.txt)

target_sources(ShmitCore
    PRIVATE
    Logging/Logger.cpp
    Logging/StdoutLogger.cpp
    Logging/VoidLogger.cpp
)