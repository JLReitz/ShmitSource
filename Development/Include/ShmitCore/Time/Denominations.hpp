#pragma once

#include <ShmitCore/Math/Denomination.hpp>
#include <ShmitCore/Math/Ratio.hpp>

namespace shmit
{
namespace time
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Sub-second order of magnitude conversions           ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using SecondsToMilliseconds = math::StaticRatio<1'000, 1>; /*! 1,000 milliseconds in 1 second */
using SecondsToMicroSeconds =
    typename math::multiply<SecondsToMilliseconds, math::StaticRatio<1'000, 1>>::type; /*! 1,000 microseconds in 1 millisecond */
using SecondsToNanoSeconds =
    typename math::multiply<SecondsToMicroSeconds, math::StaticRatio<1'000, 1>>::type; /*! 1,000 nanoseconds in 1 microsecond */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Super-second conversions                            ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using SecondsToMinutes = math::StaticRatio<1, 60>; /*! 60 seconds in 1 minute */
using SecondsToHours = typename math::multiply<SecondsToMinutes, math::StaticRatio<1, 60>>::type; /*! 60 minutes in 1 hour */
using SecondsToDays = typename math::multiply<SecondsToHours, math::StaticRatio<1, 24>>::type; /*! 24 hours in 1 day */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Time denomination declarations           ////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline namespace fixed
{

/**
 * @brief SI base unit and denomination for quantifying time
 */
using Second = math::BaseDenomination<intmax_t>;

/**
 * @brief 1 billionth of a second
 */
using Nanosecond = math::Denomination<intmax_t, SecondsToNanoSeconds>;

/**
 * @brief 1 millionth of a second
 */
using Microsecond = math::Denomination<intmax_t, SecondsToMicroSeconds>;

/**
 * @brief 1 thousandth of a second
 */
using Millisecond = math::Denomination<intmax_t, SecondsToMilliseconds>;

/**
 * @brief Comprised of 60 seconds
 */
using Minute = math::Denomination<intmax_t, SecondsToMinutes>;

/**
 * @brief Comprised of 60 minutes
 */
using Hour = math::Denomination<intmax_t, SecondsToHours>;

/**
 * @brief Comprised of 24 hours
 */
using Day = math::Denomination<intmax_t, SecondsToDays>;

} // namespace fixed

namespace floating
{

/**
 * @brief SI base unit and denomination for quantifying time
 */
using Second = math::BaseDenomination<double>;

/**
 * @brief 1 billionth of a second
 */
using Nanosecond = math::Denomination<double, SecondsToNanoSeconds>;

/**
 * @brief 1 millionth of a second
 */
using Microsecond = math::Denomination<double, SecondsToMicroSeconds>;

/**
 * @brief 1 thousandth of a second
 */
using Millisecond = math::Denomination<double, SecondsToMilliseconds>;

/**
 * @brief Comprised of 60 seconds
 */
using Minute = math::Denomination<double, SecondsToMinutes>;

/**
 * @brief Comprised of 60 minutes
 */
using Hour = math::Denomination<double, SecondsToHours>;

/**
 * @brief Comprised of 24 hours
 */
using Day = math::Denomination<double, SecondsToDays>;

} // namespace floating

} // namespace time
} // namespace shmit