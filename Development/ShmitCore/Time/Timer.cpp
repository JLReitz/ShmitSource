#include <ShmitCore/Time/Timer.hpp>

namespace shmit
{
namespace time
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  PeriodicTimer constructor definitions               ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

PeriodicTimer::PeriodicTimer(Timer& timer) : m_timer {timer}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  PeriodicTimer method definitions in alphabetical order                  ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

inline bool PeriodicTimer::IsExpired() const noexcept
{
    // If the timer expired, reset and return true
    if (m_timer.IsExpired())
    {
        m_timer.Reset();
        return true;
    }

    // Otherwise return false
    return false;
}

inline bool PeriodicTimer::IsOverExpired() const noexcept
{
    return m_timer.IsOverExpired();
}

inline void PeriodicTimer::Reset() noexcept
{
    return m_timer.Reset();
}

} // namespace time
} // namespace shmit