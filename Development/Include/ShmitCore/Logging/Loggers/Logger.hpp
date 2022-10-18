#pragma once

#include "../Defines.hpp"

namespace shmit
{
namespace log
{

/// @brief Base class and interface for a mechanism which publishes logs
class Logger
{
public:
    /// @brief Logger default constructor
    /// @param[in] filter Log entries below this level will not be published (default 'eTrace')
    Logger(Level filter = Level::eTrace);

    /// @brief Prepares a log entry and, if the level passes the filter, publishes it
    /// @param[in] type Log type
    /// @param[in] level Log level
    /// @param[in] id Log ID
    /// @param[in] context Log context
    /// @param[in] msg Log message
    void LogEntry(Type type, Level level, char const* id, char const* context, char const* msg);

private:
    /// @brief Publishes an entry
    /// @param[in] type Log type
    /// @param[in] level Log level
    /// @param[in] id Log ID
    /// @param[in] context Log context
    /// @param[in] msg Log message
    virtual void PublishEntry(char const* type, char const* level, char const* id, char const* context,
                              char const* msg) = 0;

    Level m_filter;
};

} // namespace log
} // namespace shmit