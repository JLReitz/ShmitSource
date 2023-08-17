#include <ShmitCore/Platform/Time.hpp>

#include <chrono>

namespace shmit
{
namespace platform
{

using stl_duration_ratio = typename std::ratio<1LL, Clock::kFrequencyHz>::type;
using stl_duration       = std::chrono::duration<Clock::TimeRep, stl_duration_ratio>;

struct ProgramStartTime
{
    ProgramStartTime()
        : posix {static_cast<Clock::TimeRep>(
              std::chrono::duration_cast<stl_duration>(std::chrono::system_clock::now().time_since_epoch()).count())}
    {
    }

    Clock::TimeRep posix {0};
} gProgramStartTime;

Clock::Instant Clock::Now() // Static method
{
    stl_duration duration_since_posix_epoch {
        std::chrono::duration_cast<stl_duration>(std::chrono::system_clock::now().time_since_epoch())};
    TimeRep program_run_duration = duration_since_posix_epoch.count() - gProgramStartTime.posix;
    return Instant {program_run_duration};
}

} // namespace platform
} // namespace shmit
