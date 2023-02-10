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

    template<class Denomination1, class Denomination2>
    friend constexpr Duration<Denomination2> duration_cast(Duration<Denomination1> const& rhs) noexcept;

private:
    Rep m_count {0};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Explicit specializations of Duration            ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline namespace fixed
{

using Nanoseconds  = Duration<Nanosecond<intmax_t>>;
using Microseconds = Duration<Microsecond<intmax_t>>;
using Milliseconds = Duration<Millisecond<intmax_t>>;
using Seconds      = Duration<Second<intmax_t>>;
using Minutes      = Duration<Minute<intmax_t>>;
using Hours        = Duration<Hour<intmax_t>>;
using Days         = Duration<Day<intmax_t>>;

} // namespace fixed

namespace floating
{

using Nanoseconds  = Duration<Nanosecond<double>>;
using Microseconds = Duration<Microsecond<double>>;
using Milliseconds = Duration<Millisecond<double>>;
using Seconds      = Duration<Second<double>>;
using Minutes      = Duration<Minute<double>>;
using Hours        = Duration<Hour<double>>;
using Days         = Duration<Day<double>>;

} // namespace floating

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Namespace function definitions in alphabetical order        ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**!
 * @brief Transforms a Duration from one Denomination to another
 *
 * @tparam Denomination1 Starting denomination
 * @tparam Denomination2 Ending denomination
 * @param[in] duration Duration to cast
 * @return Duration<Denomination2> Cast Duration
 */
template<class Denomination1, class Denomination2>
inline constexpr Duration<Denomination2> duration_cast(Duration<Denomination1> const& duration) noexcept
{
    using ResultRep = typename Denomination2::Rep;
    using Ratio1    = typename Denomination1::ToBase;
    using Ratio2    = typename Denomination2::ToBase;

    // Divide starting Denomination's conversion rate by the end Denomination's to get the transformation
    using Transformation                 = typename math::divide<Ratio1, Ratio2>::Result;
    constexpr math::Ratio transformation = Transformation::value;

    // Cast quantifying value to the end Denomination's representation then perform value transformation
    ResultRep rep_count    = static_cast<ResultRep>(duration.m_count);
    ResultRep result_count = rep_count * transformation;

    return Duration<Denomination2>(result_count);
}

} // namespace time
} // namespace shmit