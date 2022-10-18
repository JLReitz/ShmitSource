#include <ShmitCore/Logging/Data/StaticLogging.hpp>

#include <string.h>

#ifdef BARE_METAL_SHMIT

// The default initialized global logger for bare-metal platforms is VoidLogger
#include <ShmitCore/Logging/Loggers/VoidLogger.hpp>

using DefaultLoggerType = shmit::log::VoidLogger;

#else

// The default initialized global logger for OS hosted builds is StdoutLogger
#include <ShmitCore/Logging/Loggers/StdoutLogger.hpp>

using DefaultLoggerType = shmit::log::StdoutLogger;

#endif

namespace shmit
{
namespace log
{
namespace data
{

static DefaultLoggerType gDefaultLoggerInstance;
Logger*                  StaticLogging::m_logger(&gDefaultLoggerInstance);

void StaticLogging::LogEntry(Level level, char const* id, char const* context, char const* data_str)
{
    if (m_logger)
        m_logger->LogEntry(Type::eDiagnostics, level, id, context, data_str);
}

void StaticLogging::LoadLogger(Logger& logger)
{
    m_logger = &logger;
}

} // namespace data
} // namespace log
} // namespace shmit