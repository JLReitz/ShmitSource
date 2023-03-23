#pragma once

#include "Clock.hpp"
#include "Duration.hpp"

#include <limits>

namespace shmit
{
namespace time
{

template<class Clock>
class Instant
{
public:
    static_assert(is_clock_v<Clock>, "Clock type must comply with ShmitCore named requirements");

    using Rep = typename Clock::TimeRep;

    Instant() = default;
    Instant(Rep const& ticks) noexcept;

    template<class Denomination>
    Duration<Denomination> DurationSinceEpoch() const noexcept;

    static Instant Max() noexcept;
    static Instant Min() noexcept;

    bool operator==(Instant const& rhs);
    bool operator!=(Instant const& rhs);

    bool operator>(Instant const& rhs) const noexcept;

    bool operator>=(Instant const& rhs) const noexcept;

    bool operator<(Instant const& rhs) const noexcept;

    bool operator<=(Instant const& rhs) const noexcept;

    Instant operator++() noexcept;

    Instant& operator++(int) noexcept;

    Instant operator--() noexcept;

    Instant& operator--(int) noexcept;

    template<class Denomination>
    Instant& operator+=(Duration<Denomination> const& rhs) noexcept;

    template<class Denomination>
    Instant& operator-=(Duration<Denomination> const& rhs) noexcept;

    template<class ClockArg, class Denomination>
    friend Instant<ClockArg> operator+(Duration<Denomination> const& lhs, Instant<ClockArg> const& rhs) noexcept;

    template<class ClockArg, class Denomination>
    friend Instant<ClockArg> operator+(Instant<ClockArg> const& lhs, Duration<Denomination> const& rhs) noexcept;

    template<class ClockArg, class Denomination>
    friend Instant<ClockArg> operator-(Instant<ClockArg> const& lhs, Duration<Denomination> const& rhs) noexcept;

    template<class ClockArg, class Denomination>
    friend Duration<Denomination> operator-(Instant<ClockArg> const& lhs, Instant<ClockArg> const& rhs) noexcept;

    template<class ClockOut>
    friend Instant<ClockOut> instant_cast(Instant const& instant) noexcept;

private:
    using ClockTicksPerSecond = math::StaticRatio<Clock::kFrequencyHz, 1>;
    using ClockDenomination   = math::Denomination<Rep, ClockTicksPerSecond>;

    Rep m_ticks {0};
};

template<class Clock>
Instant<Clock>::Instant(typename Instant<Clock>::Rep const& ticks) noexcept : m_ticks {ticks}
{
}

template<class Clock>
template<class Denomination>
Duration<Denomination> Instant<Clock>::DurationSinceEpoch() const noexcept
{
    using ResultRep = typename Denomination::Rep;

    ResultRep result = math::denomination_cast<Denomination, ClockDenomination>(m_ticks);

    return Duration<Denomination> {result};
}

template<class Clock>
Instant<Clock> Instant<Clock>::Max() noexcept // Static method
{
    Rep max_rep = std::numeric_limits<Rep>::max;
    return Instant<Clock> {max_rep};
}

template<class Clock>
Instant<Clock> Instant<Clock>::Min() noexcept // Static method
{
    Rep min_rep = std::numeric_limits<Rep>::min;
    return Instant<Clock> {min_rep};
}

template<class Clock>
bool Instant<Clock>::operator==(Instant<Clock> const& rhs)
{
    return (m_ticks == rhs.m_ticks);
}

template<class Clock>
bool Instant<Clock>::operator!=(Instant<Clock> const& rhs)
{
    return !(*this == rhs);
}

template<class Clock>
inline bool Instant<Clock>::operator>(Instant<Clock> const& rhs) const noexcept
{
    return (m_ticks > rhs.m_ticks);
}

template<class Clock>
inline bool Instant<Clock>::operator>=(Instant<Clock> const& rhs) const noexcept
{
    return (m_ticks >= rhs.m_ticks);
}

template<class Clock>
inline bool Instant<Clock>::operator<(Instant<Clock> const& rhs) const noexcept
{
    return !(*this >= rhs);
}

template<class Clock>
inline bool Instant<Clock>::operator<=(Instant<Clock> const& rhs) const noexcept
{
    return !(*this > rhs);
}

template<class Clock>
inline Instant<Clock> Instant<Clock>::operator++() noexcept
{
    Instant tmp {*this};
    m_ticks++;
    return tmp;
}

template<class Clock>
inline Instant<Clock>& Instant<Clock>::operator++(int) noexcept
{
    m_ticks++;
    return *this;
}

template<class Clock>
inline Instant<Clock> Instant<Clock>::operator--() noexcept
{
    Instant tmp {*this};
    m_ticks--;
    return tmp;
}

template<class Clock>
inline Instant<Clock>& Instant<Clock>::operator--(int) noexcept
{
    m_ticks--;
    return *this;
}

template<class Clock>
template<class Denomination>
Instant<Clock>& Instant<Clock>::operator+=(Duration<Denomination> const& rhs) noexcept
{
    Rep duration_in_ticks = math::denomination_cast<ClockDenomination, Denomination>(rhs.count());
    m_ticks += duration_in_ticks;
    return *this;
}

template<class Clock>
template<class Denomination>
Instant<Clock>& Instant<Clock>::operator-=(Duration<Denomination> const& rhs) noexcept
{
    Rep duration_in_ticks = math::denomination_cast<ClockDenomination, Denomination>(rhs.count());
    m_ticks -= duration_in_ticks;
    return *this;
}

template<class ClockArg, class Denomination>
Instant<ClockArg> operator+(Duration<Denomination> const& lhs, Instant<ClockArg> const& rhs) noexcept
{
    Instant<ClockArg> tmp {rhs};
    tmp += lhs;
    return tmp;
}

template<class ClockArg, class Denomination>
Instant<ClockArg> operator+(Instant<ClockArg> const& lhs, Duration<Denomination> const& rhs) noexcept
{
    Instant<ClockArg> tmp {lhs};
    tmp += rhs;
    return tmp;
}

template<class ClockArg, class Denomination>
Instant<ClockArg> operator-(Instant<ClockArg> const& lhs, Duration<Denomination> const& rhs) noexcept
{
    Instant<ClockArg> tmp {lhs};
    tmp -= lhs;
    return tmp;
}

template<class ClockArg, class Denomination>
Duration<Denomination> operator-(Instant<ClockArg> const& lhs, Instant<ClockArg> const& rhs) noexcept
{
    using InstantRep        = typename ClockArg::TimeRep;
    using ResultRep         = typename Denomination::Rep;
    using ClockDenomination = typename Instant<ClockArg>::ClockDenomination;

    InstantRep duration_ticks = lhs.m_ticks - rhs.m_ticks;
    ResultRep  result         = math::denomination_cast<Denomination, ClockDenomination>(duration_ticks);

    return Duration<Denomination> {result};
}

template<class ClockOut, class ClockIn>
Instant<ClockOut> instant_cast(Instant<ClockIn> const& instant) noexcept
{
    using ClockOutRep = typename ClockOut::TimeRep;

    using ClockOutDenomination = typename Instant<ClockOut>::ClockDenomination;
    using ClockInDenomination  = typename Instant<ClockIn>::ClockDenomination;

    ClockOutRep result = math::denomination_cast<ClockOutDenomination, ClockInDenomination>(instant.m_ticks);

    return Instant<ClockOut> {result};
}

} // namespace time
} // namespace shmit