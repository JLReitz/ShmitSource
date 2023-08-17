#pragma once

#include "Denominations.hpp"

namespace shmit
{
namespace time
{

/**
 * @brief Represents a length of time
 *
 * @tparam Denomination
 */
template<class Denomination>
class Duration
{
public:
    static_assert(math::is_denomination_v<Denomination>, "Denomination must be a shmit::math::Denomination "
                                                         "specialization");

    using Rep             = typename Denomination::Rep;    /*! Type used to quantify the length of the duration */
    using CountsPerSecond = typename Denomination::ToBase; /*! Conversion ratio to Seconds */

    Duration() = default;

    /**
     * @brief Constructs a nonzero Duration
     *
     * @param count Number of Denominations to represent
     */
    constexpr Duration(Rep const& count) noexcept;

    constexpr Duration(Duration const& rhs) noexcept;
    constexpr Duration(Duration&& rhs) noexcept;

    /**!
     * @brief Get the Duration's length in the specified Denomination
     *
     * @return Rep Duration length
     */
    constexpr Rep Count() const noexcept;

    /**!
     * @brief Get the Duration's length in seconds
     *
     * @tparam T Arithmetic type, can be different from Rep
     * @return T Duration length
     */
    template<typename T>
    constexpr T CountInSeconds() const noexcept;

    constexpr bool operator==(Duration const& rhs) const noexcept;
    constexpr bool operator!=(Duration const& rhs) const noexcept;

    constexpr bool operator>(Duration const& rhs) const noexcept;
    constexpr bool operator>=(Duration const& rhs) const noexcept;
    constexpr bool operator<(Duration const& rhs) const noexcept;
    constexpr bool operator<=(Duration const& rhs) const noexcept;

    constexpr Duration operator+() const noexcept;
    constexpr Duration operator-() const noexcept;

    constexpr Duration& operator++() noexcept;
    constexpr Duration& operator++(int) noexcept;
    constexpr Duration& operator--() noexcept;
    constexpr Duration& operator--(int) noexcept;

    constexpr Duration operator+(Duration const& rhs) const noexcept;
    constexpr Duration operator-(Duration const& rhs) const noexcept;
    constexpr Duration operator%(Duration const& rhs) const noexcept;

    template<typename T>
    constexpr Duration operator*(T rhs) const noexcept;

    template<typename T>
    constexpr Duration operator/(T rhs) const noexcept;

    constexpr Duration& operator+=(Duration const& rhs) noexcept;
    constexpr Duration& operator-=(Duration const& rhs) noexcept;
    constexpr Duration& operator%=(Duration const& rhs) noexcept;

    template<typename T>
    constexpr Duration& operator*=(T rhs) noexcept;

    template<typename T>
    constexpr Duration& operator/=(T rhs) noexcept;

    constexpr Duration& operator=(Duration const& rhs) noexcept;
    constexpr Duration& operator=(Duration&& rhs) noexcept;

    template<class DenominationOut, class DenominationIn>
    friend constexpr Duration<DenominationOut> duration_cast(Duration<DenominationIn> const& rhs) noexcept;

private:
    Rep m_count {0};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Explicit specializations of Duration            ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline namespace fixed
{

using Nanoseconds  = Duration<fixed::Nanosecond>;
using Microseconds = Duration<fixed::Microsecond>;
using Milliseconds = Duration<fixed::Millisecond>;
using Seconds      = Duration<fixed::Second>;
using Minutes      = Duration<fixed::Minute>;
using Hours        = Duration<fixed::Hour>;
using Days         = Duration<fixed::Day>;

} // namespace fixed

namespace floating
{

using Nanoseconds  = Duration<floating::Nanosecond>;
using Microseconds = Duration<floating::Microsecond>;
using Milliseconds = Duration<floating::Millisecond>;
using Seconds      = Duration<floating::Second>;
using Minutes      = Duration<floating::Minute>;
using Hours        = Duration<floating::Hour>;
using Days         = Duration<floating::Day>;

} // namespace floating

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Duration constructor definitions            ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

template<class Denomination>
constexpr Duration<Denomination>::Duration(typename Duration<Denomination>::Rep const& count) noexcept : m_count {count}
{
}

template<class Denomination>
constexpr Duration<Denomination>::Duration(Duration<Denomination> const& rhs) noexcept : Duration(rhs.m_count)
{
}

template<class Denomination>
constexpr Duration<Denomination>::Duration(Duration&& rhs) noexcept : Duration(rhs.m_count)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Duration method definitions in alphabetical order       ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

template<class Denomination>
constexpr typename Duration<Denomination>::Rep Duration<Denomination>::Count() const noexcept
{
    return m_count;
}

template<class Denomination>
template<typename T>
constexpr T Duration<Denomination>::CountInSeconds() const noexcept
{
    static_assert(std::is_arithmetic_v<T>, "T must be arithmetic fundamental type");

    constexpr math::Ratio conversion = CountsPerSecond::Value();

    T rep_count = static_cast<T>(m_count) * conversion;
    return rep_count;
}

template<class Denomination>
constexpr bool Duration<Denomination>::operator==(Duration<Denomination> const& rhs) const noexcept
{
    return (m_count == rhs.m_count);
}

template<class Denomination>
constexpr bool Duration<Denomination>::operator!=(Duration<Denomination> const& rhs) const noexcept
{
    return !(*this == rhs);
}

template<class Denomination>
constexpr bool Duration<Denomination>::operator>(Duration const& rhs) const noexcept
{
    return (m_count > rhs.m_count);
}

template<class Denomination>
constexpr bool Duration<Denomination>::operator>=(Duration const& rhs) const noexcept
{
    return (m_count >= rhs.m_count);
}

template<class Denomination>
constexpr bool Duration<Denomination>::operator<(Duration const& rhs) const noexcept
{
    return !(*this >= rhs);
}

template<class Denomination>
constexpr bool Duration<Denomination>::operator<=(Duration const& rhs) const noexcept
{
    return !(*this > rhs);
}

template<class Denomination>
constexpr Duration<Denomination> Duration<Denomination>::operator+() const noexcept
{
    return *this;
}

template<class Denomination>
constexpr Duration<Denomination> Duration<Denomination>::operator-() const noexcept
{
    return Duration(-m_count);
}

template<class Denomination>
constexpr Duration<Denomination>& Duration<Denomination>::operator++() noexcept
{
    Duration tmp = *this;
    m_count += 1;
    return tmp;
}

template<class Denomination>
constexpr Duration<Denomination>& Duration<Denomination>::operator++(int) noexcept
{
    m_count += 1;
    return *this;
}

template<class Denomination>
constexpr Duration<Denomination>& Duration<Denomination>::operator--() noexcept
{
    Duration tmp = *this;
    m_count -= 1;
    return tmp;
}

template<class Denomination>
constexpr Duration<Denomination>& Duration<Denomination>::operator--(int) noexcept
{
    m_count -= 1;
    return *this;
}

template<class Denomination>
constexpr Duration<Denomination> Duration<Denomination>::operator+(Duration<Denomination> const& rhs) const noexcept
{
    Duration tmp = *this;
    tmp.m_count += rhs.m_count;
    return tmp;
}

template<class Denomination>
constexpr Duration<Denomination> Duration<Denomination>::operator-(Duration<Denomination> const& rhs) const noexcept
{
    Duration tmp = *this;
    tmp.m_count -= rhs.m_count;
    return tmp;
}

template<class Denomination>
constexpr Duration<Denomination> Duration<Denomination>::operator%(Duration<Denomination> const& rhs) const noexcept
{
    Duration tmp = *this;
    tmp.m_count %= rhs.m_count;
    return tmp;
}

template<class Denomination>
template<typename T>
constexpr Duration<Denomination> Duration<Denomination>::operator*(T rhs) const noexcept
{
    static_assert(std::is_arithmetic_v<T>, "'T' type must be arithmetic fundamental type");

    Duration tmp = *this;
    tmp.m_count *= rhs;
    return tmp;
}

template<class Denomination>
template<typename T>
constexpr Duration<Denomination> Duration<Denomination>::operator/(T rhs) const noexcept
{
    static_assert(std::is_arithmetic_v<T>, "'T' must be arithmetic fundamental type");

    Duration tmp = *this;
    tmp.m_count /= rhs;
    return tmp;
}

template<class Denomination>
constexpr Duration<Denomination>& Duration<Denomination>::operator+=(Duration<Denomination> const& rhs) noexcept
{
    m_count += rhs.m_count;
    return *this;
}

template<class Denomination>
constexpr Duration<Denomination>& Duration<Denomination>::operator-=(Duration<Denomination> const& rhs) noexcept
{
    m_count -= rhs.m_count;
    return *this;
}

template<class Denomination>
constexpr Duration<Denomination>& Duration<Denomination>::operator%=(Duration<Denomination> const& rhs) noexcept
{
    m_count %= rhs.m_count;
    return *this;
}

template<class Denomination>
template<typename T>
constexpr Duration<Denomination>& Duration<Denomination>::operator*=(T rhs) noexcept
{
    static_assert(std::is_arithmetic_v<T>, "'T' type must be arithmetic fundamental type");

    m_count *= rhs;
    return *this;
}

template<class Denomination>
template<typename T>
constexpr Duration<Denomination>& Duration<Denomination>::operator/=(T rhs) noexcept
{
    static_assert(std::is_arithmetic_v<T>, "'T' type must be arithmetic fundamental type");

    m_count /= rhs;
    return *this;
}

template<class Denomination>
constexpr Duration<Denomination>& Duration<Denomination>::operator=(Duration<Denomination> const& rhs) noexcept
{
    m_count = rhs.m_count;
    return *this;
}

template<class Denomination>
constexpr Duration<Denomination>& Duration<Denomination>::operator=(Duration<Denomination>&& rhs) noexcept
{
    m_count = rhs.m_count;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Namespace function definitions in alphabetical order        ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**!
 * @brief Transforms a Duration from one Denomination to another
 *
 * @tparam DenominationOut Resulting denomination
 * @tparam DenominationIn Starting denomination
 * @param[in] duration Duration to cast
 * @return Duration<DenominationOut> Cast Duration
 */
template<class DenominationOut, class DenominationIn>
inline constexpr Duration<DenominationOut> duration_cast(Duration<DenominationIn> const& duration) noexcept
{
    using ResultRep  = typename DenominationOut::Rep;
    ResultRep result = math::denomination_cast<DenominationOut, DenominationIn>(duration.m_count);
    return Duration<DenominationOut> {result};
}

} // namespace time
} // namespace shmit