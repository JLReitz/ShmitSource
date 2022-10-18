#pragma once

#include <ShmitCore/Logging/Loggers/LoggerInterface.hpp>

#include <cstdio>

namespace shmit
{
namespace log
{
namespace events
{

class StaticLogging
{
public:
    // TODO add event ID
    template<typename... ARGV>
    static void Log(Level level, char const* id, char const* context, char const* msg_format, ARGV... args);

    static void LoadLogger(Logger& logger);

private:
    static void LogEntry(Level level, char const* id, char const* context, char const* event_str);

    static Logger* m_logger;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Method definitions in alphabetical order    ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename... ARGV>
void StaticLogging::Log(Level level, char const* id, char const* context, char const* msg_format, ARGV... args)
{
    // Format message
    size_t msg_length = std::snprintf(NULL, 0, msg_format, args...) + 1;

    // Place in some kind of buffer
    char* event_str = new char[msg_length];
    std::snprintf(event_str, msg_length, msg_format, args...);

    LogEntry(level, id, context, event_str);

    // Destruct buffer
    delete[] event_str;
}

} // namespace events
} // namespace log
} // namespace shmit
