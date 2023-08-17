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

/**!
 * @brief 1,000 milliseconds in 1 second
 *
 */
using SecondsToMilliseconds = math::StaticRatio<1'000, 1>;

/**!
 * @brief 1,000 microseconds in 1 millisecond
 *
 */
using SecondsToMicroSeconds = typename math::multiply<SecondsToMilliseconds, math::StaticRatio<1'000, 1>>::type;

/**!
 * @brief 1,000 nanoseconds in 1 microsecond
 *
 */
using SecondsToNanoSeconds = typename math::multiply<SecondsToMicroSeconds, math::StaticRatio<1'000, 1>>::type;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Super-second conversions                            ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**!
 * @brief 60 seconds in 1 minute
 *
 */
using SecondsToMinutes = math::StaticRatio<1, 60>;

/**!
 * @brief 60 minutes in 1 hour
 *
 */
using SecondsToHours = typename math::multiply<SecondsToMinutes, math::StaticRatio<1, 60>>::type;

/**!
 * @brief 24 hours in 1 day
 *
 */
using SecondsToDays = typename math::multiply<SecondsToHours, math::StaticRatio<1, 24>>::type;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Time denomination declarations           ////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline namespace fixed
{

/**!
 * @brief Representative data type for fixed point time
 *
 */
using TimeRep = intmax_t;

/**!
 * @brief SI base unit and denomination for quantifying time
 *
 */
using Second = math::BaseDenomination<TimeRep>;

/**!
 * @brief 1 billionth of a second
 *
 */
using Nanosecond = math::Denomination<TimeRep, SecondsToNanoSeconds>;

/**!
 * @brief 1 millionth of a second
 *
 */
using Microsecond = math::Denomination<TimeRep, SecondsToMicroSeconds>;

/**!
 * @brief 1 thousandth of a second
 *
 */
using Millisecond = math::Denomination<TimeRep, SecondsToMilliseconds>;

/**!
 * @brief Comprised of 60 seconds
 *
 */
using Minute = math::Denomination<TimeRep, SecondsToMinutes>;

/**!
 * @brief Comprised of 60 minutes
 *
 */
using Hour = math::Denomination<TimeRep, SecondsToHours>;

/**!
 * @brief Comprised of 24 hours
 *
 */
using Day = math::Denomination<TimeRep, SecondsToDays>;

} // namespace fixed

namespace floating
{

/**!
 * @brief Representative data type for floating point time
 *
 */
using TimeRep = double;

/**!
 * @brief SI base unit and denomination for quantifying time
 *
 */
using Second = math::BaseDenomination<TimeRep>;

/**!
 * @brief 1 billionth of a second
 *
 */
using Nanosecond = math::Denomination<TimeRep, SecondsToNanoSeconds>;

/**!
 * @brief 1 millionth of a second
 *
 */
using Microsecond = math::Denomination<TimeRep, SecondsToMicroSeconds>;

/**!
 * @brief 1 thousandth of a second
 *
 */
using Millisecond = math::Denomination<TimeRep, SecondsToMilliseconds>;

/**!
 * @brief Comprised of 60 seconds
 *
 */
using Minute = math::Denomination<TimeRep, SecondsToMinutes>;

/**!
 * @brief Comprised of 60 minutes
 *
 */
using Hour = math::Denomination<TimeRep, SecondsToHours>;

/**!
 * @brief Comprised of 24 hours
 *
 */
using Day = math::Denomination<TimeRep, SecondsToDays>;

} // namespace floating

} // namespace time
} // namespace shmit