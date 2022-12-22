#pragma once

#include "Clock.hpp"

#include <chrono>

namespace shmit
{
namespace platform
{

class SystemClock
{
public:
    static std::chrono::time_point<SystemClock> now() noexcept;

    static void Load(const Clock& clock) noexcept;

private:
    static Clock* m_clock;
};

} // namespace platform
} // namespace shmit