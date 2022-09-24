#pragma once

#include "Loggers/LoggerInterface.hpp"

#include <string>

namespace shmit
{
namespace log
{

class StaticEventLoggingInterface
{
private:
    static LoggerInterface* mLogger;

    static void LogEntry(Level level, const char* id, const char* msgStr);

public:
    // TODO add event ID
    template<typename... ARGV>
    static void LogEvent(Level level, const char* id, const char* msgFormat, ARGV... args)
    {
        // Format message
        size_t msgLength = std::snprintf(NULL, 0, msgFormat, args...) + 1;

        // Place in some kind of buffer
        char* msgStr = new char[msgLength];
        std::snprintf(msgStr, msgLength, msgFormat, args...);

        LogEntry(level, id, msgStr);

        // Destruct buffer
        delete[] msgStr;
    }

    static void LoadLogger(LoggerInterface& logger);
};

} // namespace log
} // namespace shmit

#define LOG_DEBUG_EVENT(id, msgFormat, args...) \
    shmit::log::StaticEventLoggingInterface::LogEvent(shmit::log::Level::eInfo, id, msgFormat, args);

#define LOG_INFO_EVENT(id, msgFormat, args...) \
    shmit::log::StaticEventLoggingInterface::LogEvent(shmit::log::Level::eInfo, id, msgFormat, args);

#define LOG_WARNING_EVENT(id, msgFormat, args...) \
    shmit::log::StaticEventLoggingInterface::LogEvent(shmit::log::Level::eWarning, id, msgFormat, args);

#define LOG_ERROR_EVENT(id, msgFormat, args...) \
    shmit::log::StaticEventLoggingInterface::LogEvent(shmit::log::Level::eError, id, msgFormat, args);

#define LOG_FATAL_EVENT(id, msgFormat, args...) \
    shmit::log::StaticEventLoggingInterface::LogEvent(shmit::log::Level::eFatal, id, msgFormat, args);

#define LOG_DEBUG_EVENT_STRING(id, str) \
    shmit::log::StaticEventLoggingInterface::LogEvent(shmit::log::Level::eDebug, id, "%s", str);

#define LOG_INFO_EVENT_STRING(id, str) \
    shmit::log::StaticEventLoggingInterface::LogEvent(shmit::log::Level::eInfo, id, "%s", str);

#define LOG_WARNING_EVENT_STRING(id, str) \
    shmit::log::StaticEventLoggingInterface::LogEvent(shmit::log::Level::eWarning, id, "%s", str);

#define LOG_ERROR_EVENT_STRING(id, str) \
    shmit::log::StaticEventLoggingInterface::LogEvent(shmit::log::Level::eError, id, "%s", str);

#define LOG_FATAL_EVENT_STRING(id, str) \
    shmit::log::StaticEventLoggingInterface::LogEvent(shmit::log::Level::eFatal, id, "%s", str);
