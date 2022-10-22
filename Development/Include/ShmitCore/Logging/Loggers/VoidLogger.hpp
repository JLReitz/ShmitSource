#pragma once

#include "Logger.hpp"

namespace shmit
{
namespace log
{

/// @brief Logger that publishes to the void
class VoidLogger : public Logger
{
    /*****************************************
     * Default constructor implicitly defined
     *****************************************/

    /// @brief Yeets that entry in to the void
    /// @param[in] type Log type
    /// @param[in] level Log level
    /// @param[in] id Log ID
    /// @param[in] context Log context
    /// @param[in] msg Log message
    void PublishEntry(const char* type, const char* level, const char* id, const char* context, const char* msg) final;
};

} // namespace log
} // namespace shmit