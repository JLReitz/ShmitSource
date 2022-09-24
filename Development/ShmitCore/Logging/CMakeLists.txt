# Include directories below this one
include(Logging/Loggers/CMakeLists.txt)

# Target source files located within this subdirectory
target_sources(ShmitCore
    PRIVATE
    Logging/src/Level.cpp
    Logging/src/StaticEventLoggingInterface.cpp
)