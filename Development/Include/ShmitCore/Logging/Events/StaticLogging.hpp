#pragma once

#include <ShmitCore/Logging/Loggers/Logger.hpp>

#include <cstdio>

namespace shmit
{
namespace log
{
namespace events
{

/// @brief Static event logging interface
class StaticLogging
{
public:
    /// @brief Public entry point for logging an event to the system
    /// @tparam ...ARGV Format arguments type pack
    /// @param level Event log level
    /// @param id Event ID
    /// @param context Event context name
    /// @param msg_format Event message. Standard format string.
    /// @param ...args Format arguments
    template<typename... ARGV>
    static void Log(Level level, char const* id, char const* context, char const* msg_format, ARGV... args);

    /// @brief Loads a logger instance in to the static interface. This replaces the previously loaded Logger.
    /// @param logger Logger instance
    static void LoadLogger(Logger& logger);

private:
    /// @brief Forwards compiled entry fields to the currently loaded Logger
    /// @param level Event log level
    /// @param id Event ID string
    /// @param context Event context name
    /// @param event_str Event message string
    static void LogEntry(Level level, char const* id, char const* context, char const* event_str);

    /// @brief References the currently loaded Logger. By default this is an StdoutLogger on hosted systems and a
    /// VoidLogger when running on bare metal.
    static Logger& m_logger;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Method definitions in alphabetical order    ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename... ARGV>
void StaticLogging::Log(Level level, char const* id, char const* context, char const* msg_format, ARGV... args)
{
    // TODO add timestamp

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
