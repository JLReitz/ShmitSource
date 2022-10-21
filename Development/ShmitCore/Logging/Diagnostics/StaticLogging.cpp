#include <ShmitCore/Logging/Diagnostics/StaticLogging.hpp>

#include <string.h>

#ifdef BARE_METAL_SHMIT

// The default initialized global logger for bare-metal platforms is VoidLogger
#include <ShmitCore/Logging/Loggers/VoidLogger.hpp>

using DefaultLoggerType = shmit::log::VoidLogger;

#else

// The default initialized global logger for OS hosted systems is StdoutLogger
#include <ShmitCore/Logging/Loggers/StdoutLogger.hpp>

using DefaultLoggerType = shmit::log::StdoutLogger;

#endif

namespace shmit
{
namespace log
{
namespace diagnostics
{

static DefaultLoggerType gDefaultLoggerInstance;
Logger&                  StaticLogging::m_logger = gDefaultLoggerInstance;

void StaticLogging::LoadLogger(Logger& logger)
{
    m_logger = logger;
}

void StaticLogging::LogEntry(Level level, char const* tag, char const* context, char const* diagnostic_str)
{
    m_logger.LogEntry(Type::eDiagnostics, level, tag, context, diagnostic_str);
}

} // namespace diagnostics
} // namespace log
} // namespace shmit