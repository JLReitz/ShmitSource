#pragma once

#include "Defines.hpp"

namespace shmit
{
namespace log
{

/// @brief Base class and interface for a mechanism which publishes logs
class Logger
{
public:
    /*****************************************
     * Default constructor implicitly defined
     *****************************************/

    /// @brief Constructor overload
    /// @param[in] threshold Log entries below this level will not be published
    Logger(Level threshold);

    /// Prepares a log entry and, if the level passes the threshold, publishes it
    /// @param[in] type Log type
    /// @param[in] level Log level
    /// @param[in] id Log ID
    /// @param[in] context Log context
    /// @param[in] msg Log message
    void LogEntry(Type type, Level level, char const* id, char const* context, char const* msg);

    /// @brief Fetches the level threshold for this Logger
    /// @return Log level
    Level Threshold() const;

    /// @brief Configures the level threshold for this logger
    /// @param[in] threshold Log entries below this level will not be published
    void SetThreshold(Level threshold);

private:
    /// @brief Publishes an entry
    /// @param[in] type Log type
    /// @param[in] level Log level
    /// @param[in] id Log ID
    /// @param[in] context Log context
    /// @param[in] msg Log message
    virtual void PublishEntry(char const* type, char const* level, char const* id, char const* context,
                              char const* msg) = 0;

    Level m_threshold {Level::eTrace}; //!< Logger level threshold (default = 'eTrace')
};

} // namespace log
} // namespace shmit