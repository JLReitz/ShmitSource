#pragma once

#include "Posit.hpp"

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

    /// Prepares a log entry and, if the Severity passes the threshold, publishes it
    /// @param[in] type Log type
    /// @param[in] Severity Log Severity
    /// @param[in] id Log ID
    /// @param[in] context Log context
    /// @param[in] msg Log message
    virtual void Post(Posit const& posit) = 0;
};

} // namespace log
} // namespace shmit