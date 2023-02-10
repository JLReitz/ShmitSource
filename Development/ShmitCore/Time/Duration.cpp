#include <ShmitCore/Time/Duration.hpp>

namespace shmit
{
namespace time
{

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

} // namespace time
} // namespace shmit