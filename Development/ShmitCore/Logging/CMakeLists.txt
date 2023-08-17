# Include directories below this one
add_subdirectory(Data)
add_subdirectory(Diagnostics)
add_subdirectory(Events)

target_sources(ShmitCore
    PRIVATE
    Logger.cpp
    StdoutLogger.cpp
    String.cpp
    VoidLogger.cpp
)