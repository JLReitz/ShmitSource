#pragma once

#include "Denominations.hpp"

namespace shmit
{
namespace time
{

template<class Denomination>
class Duration
{
public:
    using Rep             = typename Denomination::Rep;
    using CountsPerSecond = typename Denomination::Ratio;

    Duration() = default;

    Duration(Rep const& count) noexcept;

    constexpr Duration(Duration const& rhs) noexcept;
    constexpr Duration(Duration&& rhs) noexcept;

    constexpr Rep Count() const noexcept;

    template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
    constexpr T CountInSeconds() const noexcept;

    constexpr bool operator==(Duration const& rhs) const noexcept;
    constexpr bool operator!=(Duration const& rhs) const noexcept;

    constexpr Duration operator+() const noexcept;
    constexpr Duration operator-() const noexcept;

    constexpr Duration& operator++() noexcept;
    constexpr Duration& operator++(int) noexcept;
    constexpr Duration& operator--() noexcept;
    constexpr Duration& operator--(int) noexcept;

    constexpr Duration operator+(Duration const& rhs) const noexcept;
    constexpr Duration operator-(Duration const& rhs) const noexcept;
    constexpr Duration operator-(intmax_t rhs) const noexcept;
    constexpr Duration operator%(intmax_t rhs) const noexcept;

    template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
    constexpr Duration operator*(T rhs) const noexcept;

    template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
    constexpr Duration operator/(T rhs) const noexcept;

    constexpr Duration& operator+=(Duration const& rhs) noexcept;
    constexpr Duration& operator-=(Duration const& rhs) noexcept;
    constexpr Duration& operator%=(intmax_t rhs) noexcept;

    template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
    constexpr Duration& operator*=(T rhs) noexcept;

    template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
    constexpr Duration& operator/=(T rhs) noexcept;

    constexpr Duration& operator=(Duration const& rhs) noexcept;
    constexpr Duration& operator=(Duration&& rhs) noexcept;

private:
    Rep m_count {0};
};

template<class Denomination1, class Denomination2>
static constexpr Duration<Denomination1> duration_cast(Duration<Denomination2> const& rhs);

using Nanoseconds  = Duration<Nanosecond<intmax_t>>;
using Microseconds = Duration<Microsecond<intmax_t>>;
using Milliseconds = Duration<Millisecond<intmax_t>>;
using Seconds      = Duration<Second<intmax_t>>;
using Minutes      = Duration<Minute<intmax_t>>;
using Hours        = Duration<Hour<intmax_t>>;
using Days         = Duration<Day<intmax_t>>;

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
template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t>>
constexpr T Duration<Denomination>::CountInSeconds() const noexcept
{
    constexpr math::Ratio conversion = CountsPerSecond::Value();

    T count_rep = static_cast<T>(m_count);
    return count_rep * conversion;
}

template<class Denomination>
constexpr bool Duration<Denomination>::operator==(Duration<Denomination> const& rhs) const noexcept
{
    return m_count == rhs.m_count;
}

template<class Denomination>
constexpr bool Duration<Denomination>::operator!=(Duration<Denomination> const& rhs) const noexcept
{
    return !(*this == rhs);
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
    return temp;
}

template<class Denomination>
constexpr Duration<Denomination>& Duration<Denomination>::operator++(int) noexcept
{
    m_count += 1;
    return temp;
}

template<class Denomination>
constexpr Duration<Denomination>& Duration<Denomination>::operator--() noexcept
{
    Duration tmp = *this;
    m_count -= 1;
    return temp;
}

template<class Denomination>
constexpr Duration<Denomination>& Duration<Denomination>::operator--(int) noexcept
{
    m_count -= 1;
    return temp;
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
constexpr Duration<Denomination> Duration<Denomination>::operator-(intmax_t rhs) const noexcept
{
    Duration tmp = *this;
    tmp.m_count -= rhs;
    return tmp;
}

template<class Denomination>
constexpr Duration<Denomination> Duration<Denomination>::operator%(intmax_t rhs) const noexcept
{
    Duration tmp = *this;
    tmp.m_count %= rhs;
    return tmp;
}

template<class Denomination>
template<typename T, typename>
constexpr Duration<Denomination> Duration<Denomination>::operator*(T rhs) const noexcept
{
    Duration tmp = *this;
    tmp.m_count *= rhs;
    return tmp;
}

template<class Denomination>
template<typename T, typename>
constexpr Duration<Denomination> Duration<Denomination>::operator/(T rhs) const noexcept
{
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
constexpr Duration<Denomination>& Duration<Denomination>::operator%=(intmax_t rhs) noexcept
{
    m_count %= rhs;
    return *this;
}

template<class Denomination>
template<typename T, typename>
constexpr Duration<Denomination>& Duration<Denomination>::operator*=(T rhs) noexcept
{
    m_count *= rhs;
    return *this;
}

template<class Denomination>
template<typename T, typename>
constexpr Duration<Denomination>& Duration<Denomination>::operator/=(T rhs) noexcept
{
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
//  External method definitions in alphabetical order       ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Denomination1, class Denomination2>
static constexpr Duration<Denomination2> duration_cast(Duration<Denomination1> const& rhs)
{
    using ResultRep  = typename Duration<Denomination2>::Rep;
    using Ratio1     = typename Denomination1::Ratio;
    using Ratio2     = typename Denomination2::Ratio;
    using Conversion = typename math::Divide<Ratio1, Ratio2>::Result;

    constexpr math::Ratio conversion = Conversion::Value();

    ResultRep rep_count    = static_cast<ResultRep>(rhs.m_count);
    ResultRep result_count = rhs.m_count * conversion;

    return Duration<Denomination2>(result_count);
}

} // namespace time
} // namespace shmit