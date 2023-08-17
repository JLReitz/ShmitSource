#include <ShmitCore/Logging/Detail/Interface.hpp>
#include <ShmitCore/Platform/Time.hpp>

namespace shmit
{
namespace log
{
namespace detail
{

void Interface::LoadTimestamp(Posit& posit)
{
    // Get timestamp and load in to posit
    time::Microseconds runtime_us = platform::Clock::Now().DurationSinceEpoch<time::Microsecond>();
    posit.timestamp               = runtime_us.Count();
}

} // namespace detail
} // namespace log
} // namespace shmit