#pragma once

#include <ShmitCore/Math/Denomination.hpp>
#include <ShmitCore/Math/Ratio.hpp>

namespace shmit
{
namespace time
{

/**
 * @brief SI base unit and denomination for quantifying time
 *
 * @tparam Representation Type used to quantify Seconds
 */
template<typename Representation>
using Second = math::BaseDenomination<Representation>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Sub-second order of magnitude conversions           ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using SecondsToMilliseconds = math::StaticRatio<1'000, 1>; /*! 1,000 milliseconds in 1 second */
using SecondsToMicroSeconds =
    math::multiply<SecondsToMilliseconds, math::StaticRatio<1'000, 1>>; /*! 1,000 microseconds in 1 millisecond */
using SecondsToNanoSeconds =
    math::multiply<SecondsToMicroSeconds, math::StaticRatio<1'000, 1>>; /*! 1,000 nanoseconds in 1 microsecond */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Super-second conversions                            ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using SecondsToMinutes = math::StaticRatio<1, 60>;                                   /*! 60 seconds in 1 minute */
using SecondsToHours   = math::multiply<SecondsToMinutes, math::StaticRatio<1, 60>>; /*! 60 minutes in 1 hour */
using SecondsToDays    = math::multiply<SecondsToHours, math::StaticRatio<1, 24>>;   /*! 24 hours in 1 day */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Time denomination declarations           ////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief 1 billionth of a second
 *
 * @tparam Representation Type used to quantify Nanoseconds
 */
template<typename Representation>
using Nanosecond = math::Denomination<Representation, SecondsToNanoSeconds>;

/**
 * @brief 1 millionth of a second
 *
 * @tparam Representation Type used to quantify Microseconds
 */
template<typename Representation>
using Microsecond = math::Denomination<Representation, SecondsToMicroSeconds>;

/**
 * @brief 1 thousandth of a second
 *
 * @tparam Representation Type used to quantify Milliseconds
 */
template<typename Representation>
using Millisecond = math::Denomination<Representation, SecondsToMilliseconds>;

/**
 * @brief Comprised of 60 seconds
 *
 * @tparam Representation Type used to quantify Minutes
 */
template<typename Representation>
using Minute = math::Denomination<Representation, SecondsToMinutes>;

/**
 * @brief Comprised of 60 minutes
 *
 * @tparam Representation Type used to quantify Hours
 */
template<typename Representation>
using Hour = math::Denomination<Representation, SecondsToHours>;

/**
 * @brief Comprised of 24 hours
 *
 * @tparam Representation Type used to quantify Days
 */
template<typename Representation>
using Day = math::Denomination<Representation, SecondsToDays>;

} // namespace time
} // namespace shmit