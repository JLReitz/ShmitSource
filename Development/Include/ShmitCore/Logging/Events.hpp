#pragma once

#include "detail/StaticInterface.hpp"

namespace shmit
{
namespace log
{
namespace detail
{

using EventsCategoryLabel = decltype("events"_ssc);
using EventsCategory      = Category<EventsCategoryLabel>;

} // namespace detail

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Events logging interface                                ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using Events = detail::StaticInterface<detail::EventsCategory>;

} // namespace log
} // namespace shmit
