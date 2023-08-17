#pragma once

#include <ShmitCore/Time/Instant.hpp>
#include <ShmitCore/Time/Timer.hpp>

namespace shmit
{
namespace platform
{

class Clock final
{
public:
    using TimeRep = time::TimeRep;
    using Instant = time::Instant<Clock>;

    static constexpr uintmax_t kFrequencyHz {static_cast<uintmax_t>(time::SecondsToMicroSeconds::value)};

    static Instant Now();
};

using Instant = time::Instant<Clock>;
using Timer   = time::BasicTimer<Clock>;

} // namespace platform
} // namespace shmit
