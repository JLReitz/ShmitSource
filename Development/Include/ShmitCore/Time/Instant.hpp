#pragma once

#include <ShmitCore/Types/StdTypes.hpp>

namespace shmit
{
namespace time
{

template<class Clock>
class Instant
{
public:
    using Duration = shmit::uint64_t;

private:
    Duration m_ticks {0};
};

} // namespace time
} // namespace shmit