#include <ShmitCore/Logging/StaticEventLoggingInterface.hpp>

#ifdef SHMITCORE_EMBEDDED_PLATFORM
#include <ShmitCore/Logging/Loggers/VoidLogger.hpp>
shmit::log::VoidLogger       gDefaultLoggerInstance;
shmit::log::LoggerInterface* shmit::log::StaticEventLoggingInterface::mLogger(&gDefaultLoggerInstance);
#else
#include <ShmitCore/Logging/Loggers/StdoutLogger.hpp>
shmit::log::StdoutLogger     gDefaultLoggerInstance;
shmit::log::LoggerInterface* shmit::log::StaticEventLoggingInterface::mLogger(&gDefaultLoggerInstance);
#endif

namespace shmit
{
namespace log
{

void StaticEventLoggingInterface::LoadLogger(LoggerInterface& logger) // Static method
{
    mLogger = &logger;
}

void StaticEventLoggingInterface::LogEntry(Level level, const char* id, const char* msgStr) // Static method
{
    // Pass to member logger
    if (mLogger)
        mLogger->LogEntry(Type::eEvent, level, id, msgStr);
}

} // namespace log
} // namespace shmit