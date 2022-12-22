#pragma once

#include <ratio>

namespace shmit
{
namespace help
{

template <class Ratio>
struct inverse_ratio
{
    static constexpr intmax_t num = typename Ratio::num;
    static constexpr intmax_t den
}

}
}