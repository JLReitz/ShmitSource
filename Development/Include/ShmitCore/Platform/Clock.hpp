#pragma once

#include <ShmitCore/Types/Time/Instant.hpp>

namespace shmit
{
namespace time
{

class Clock
{
public:
    virtual Instant<Clock> Now() const noexcept = 0;
}

} // namespace time
} // namespace shmit