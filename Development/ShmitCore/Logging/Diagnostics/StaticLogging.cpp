#include <ShmitCore/Logging/Diagnostics/StaticLogging.hpp>

#include <string.h>

#ifdef BARE_METAL_SHMIT

// The default initialized global logger for bare-metal platforms is VoidLogger
#include <ShmitCore/Logging/VoidLogger.hpp>

using DefaultLoggerType = shmit::log::VoidLogger;

#else

// The default initialized global logger for OS hosted systems is StdoutLogger
#include <ShmitCore/Logging/StdoutLogger.hpp>

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  StaticLogging method definitions in alphabetical order      ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

void StaticLogging::LoadLogger(Logger& logger) // Static method
{
    // Cache current logger's threshold
    Level threshold = m_logger.Threshold();

    // Update logger reference and configure its threshold
    m_logger = logger;
    m_logger.SetThreshold(threshold);
}

void StaticLogging::SetThreshold(Level threshold) // Static method
{
    m_logger.SetThreshold(threshold);
}

//  Private ============================================================================================================

void StaticLogging::LogEntry(Level level, char const* tag, char const* context, char const* diagnostic_str) // Static
                                                                                                            // method
{
    m_logger.LogEntry(Type::eDiagnostics, level, tag, context, diagnostic_str);
}

} // namespace diagnostics
} // namespace log
} // namespace shmit