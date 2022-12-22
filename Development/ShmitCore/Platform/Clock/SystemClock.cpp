#include <ShmitCore/Platform/Help.hpp>
#include <ShmitCore/Platform/SystemClock.hpp>

namespace shmit
{
namespace platform
{
namespace clock
{

std::chrono::time_point<SystemClock> SystemClock::now() noexcept
{
    if (m_clock)
    {
    }
    else
    {
        if constexpr (shmit::platform::is_bare_metal())
        {
            // Return time point at epoch, there is nothing else we can do but hope the plebs who forgot to load the
            // system clock notice the lack of timestamps in their logs
            return std::chrono::time_point<SystemClock> {};
        }
        else
        {
            // If not bare metal, use the chrono system clock as the source of time points
                }
    }
}

} // namespace clock
} // namespace platform
} // namespace shmit