#pragma once

#include <ShmitCore/Time/Instant.hpp>

namespace shmit
{
namespace platform
{

class Clock final
{
public:
    using TimeRep = uint64_t;
    static constexpr uintmax_t kFrequencyHz {static_cast<uintmax_t>(time::SecondsToMicroSeconds::value)};

    static time::Instant<Clock> now();
};

using TimeInstant = time::Instant<Clock>;

} // namespace platform
} // namespace shmit
