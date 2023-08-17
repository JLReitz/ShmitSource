#pragma once

#include "Clock.hpp"
#include "Duration.hpp"

#include <limits>

namespace shmit
{
namespace time
{

/**!
 * @brief A single point in time
 *
 * @tparam Clock Time source. Must meet the ShmitCore named requirements for Clock.
 */
template<class Clock>
class Instant
{
public:
    static_assert(is_clock_v<Clock>, "Clock type must comply with ShmitCore named requirements");

    using Rep = typename Clock::TimeRep;                               /*! Type used to quantify the point in time */

    using TicksPerSecond = math::StaticRatio<Clock::kFrequencyHz, 1>;  /*! Clock source frequency */
    using ClockDenomination = math::Denomination<Rep, TicksPerSecond>; /*! Denomination of the clock source's frequency*/

    /**!
     * @brief Constructs an Instant to represent the epoch (starting point) of its time source
     *
     */
    Instant() = default;

    /**!
     * @brief Constructs an Instant to represent some time after the epoch of its time source
     *
     * @param ticks Time in clock ticks
     */
    Instant(Rep const& ticks) noexcept;

    /**!
     * @brief Return the duration between the Instant and the epoch of its time source
     *
     * @tparam Denomination Units of time
     * @param (Anonymous) Instance of denomination
     * @return Duration<Denomination>
     */
    template<class Denomination>
    Duration<Denomination> DurationSinceEpoch(Denomination) const noexcept;

    /**!
     * @brief Maximum possible point in time that an Instant can represent
     *
     * @return Instant
     */
    static Instant Max() noexcept;

    /**!
     * @brief Minimum possible point in time that an Instant can represent
     *
     * @return Instant
     */
    static Instant Min() noexcept;

    bool operator==(Instant const& rhs);
    bool operator!=(Instant const& rhs);

    bool operator>(Instant const& rhs) const noexcept;
    bool operator>=(Instant const& rhs) const noexcept;
    bool operator<(Instant const& rhs) const noexcept;
    bool operator<=(Instant const& rhs) const noexcept;

    Instant  operator++() noexcept;
    Instant& operator++(int) noexcept;
    Instant  operator--() noexcept;
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
    Rep m_ticks {0}; /*! Measurement taken from the Clock in "ticks" */
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Instant constructor definitions                     ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

template<class Clock>
Instant<Clock>::Instant(typename Instant<Clock>::Rep const& ticks) noexcept : m_ticks {ticks}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Instant method definitions in alphabetical order                        ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

template<class Clock>
template<class Denomination>
Duration<Denomination> Instant<Clock>::DurationSinceEpoch(Denomination) const noexcept
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Namespace function definitions in alphabetical order        ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

/**!
 * @brief Transforms an Instant from one clock source to another
 *
 * @tparam ClockOut Resulting clock source
 * @tparam ClockIn Starting clock source
 * @param instant Instant to cast
 * @return Instant<ClockOut> Cast Instant
 */
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