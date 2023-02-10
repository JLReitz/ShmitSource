#pragma once

#include "Duration.hpp"

namespace shmit
{
namespace time
{

template<class Clock>
class Instant
{
public:
    // TODO assert Clock is of correct type
    using TicksPerSecond = typename Clock::TicksPerSecond; /*! Conversion ratio from clock ticks to Seconds */

    template<class Denomination>
    operator Duration<Denomination>() const noexcept;

private:
    uint64_t m_ticks;
};

} // namespace time
} // namespace shmit