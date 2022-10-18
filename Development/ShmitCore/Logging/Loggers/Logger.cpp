#include <ShmitCore/Logging/Loggers/Logger.hpp>

namespace shmit
{
namespace log
{

Logger::Logger(Level filter) : m_filter(filter)
{
}

void Logger::LogEntry(Type type, Level level, char const* id, char const* context, char const* msg)
{
    char const* type_str  = TypeToString(type);
    char const* level_str = LevelToString(level);

    if (level >= m_filter)
    {
        PublishEntry(type_str, level_str, id, context, msg);
    }
}

} // namespace log
} // namespace shmit