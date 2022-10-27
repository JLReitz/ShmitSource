#include <ShmitCore/Logging/Logger.hpp>

namespace shmit
{
namespace log
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Logger method definitions in alphabetical order      ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

Logger::Logger(Level threshold) : m_threshold(threshold)
{
}

void Logger::LogEntry(Type type, Level level, char const* id, char const* context, char const* msg)
{
    char const* type_str  = TypeToString(type);
    char const* level_str = LevelToString(level);

    if (level >= m_threshold)
    {
        PublishEntry(type_str, level_str, id, context, msg);
    }
}

void Logger::SetThreshold(Level threshold)
{
    m_threshold = threshold;
}

Level Logger::Threshold() const
{
    return m_threshold;
}

} // namespace log
} // namespace shmit