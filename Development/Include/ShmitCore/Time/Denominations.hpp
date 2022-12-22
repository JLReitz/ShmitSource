#pragma once

#include <ShmitCore/Math/Ratio.hpp>

namespace shmit
{
namespace time
{

template<typename Representation>
struct Second
{
    using Rep   = Representation;
    using Ratio = StaticRatio<1, 1>;

    using Base = Second;
};

template<typename Representation>
struct Nanosecond
{
    using Rep   = Representation;
    using Ratio = StaticRatio<1, 1'000'000'000>;

    using Base = Second;
};

template<typename Representation>
struct Microsecond
{
    using Rep   = Representation;
    using Ratio = StaticRatio<1, 1'000'000>;

    using Base = Second;
};

template<typename Representation>
struct Millisecond
{
    using Rep   = Representation;
    using Ratio = StaticRatio<1, 1'000>;

    using Base = Second;
};

template<typename Representation>
struct Minute
{
    using Rep   = Representation;
    using Ratio = StaticRatio<60, 1>;

    using Base = Second;
};

template<typename Representation>
struct Hour
{
    using Rep   = Representation;
    using Ratio = StaticRatio<3'600, 1>;

    using Base = Second;
};

template<typename Representation>
struct Day
{
    using Rep   = Representation;
    using Ratio = StaticRatio<86'400, 1>;

    using Base = Second;
};

} // namespace time
} // namespace shmit