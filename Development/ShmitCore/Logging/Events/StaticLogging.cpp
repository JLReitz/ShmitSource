#include <ShmitCore/Logging/Events/StaticLogging.hpp>

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
namespace events
{

static DefaultLoggerType gDefaultLoggerInstance;
Logger&                  StaticLogging::m_logger = gDefaultLoggerInstance;

void StaticLogging::LoadLogger(Logger& logger) // Static method
{
    m_logger = logger;
}

void StaticLogging::LogEntry(Level level, char const* id, char const* context, char const* event_str) // Static method
{
    // Pass to member logger
    m_logger.LogEntry(Type::eEvent, level, id, context, event_str);
}

} // namespace events
} // namespace log
} // namespace shmit