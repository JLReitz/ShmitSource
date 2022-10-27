#pragma once

#include "OStreamLogger.hpp"

#include <iostream>

namespace shmit
{
namespace log
{

/// @brief Logger that publishes to stdout
class StdoutLogger : public OStreamLogger<::std::ostream>
{
public:
    /// @brief Default constructor
    /// @param[in] filter Log entries below this level will not be published (default 'eTrace')
    StdoutLogger(Level filter = Level::eTrace);

    /// @brief StdoutLogger does not allow for a different output stream to be assigned to any instance. Calling this
    /// method override will not do anything.
    /// @param[in] oStream Reference to std::ostream instance (unused)
    void Load(::std::ostream& oStream) final;
};

} // namespace log
} // namespace shmit