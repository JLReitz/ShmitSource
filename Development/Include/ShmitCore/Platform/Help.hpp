#pragma once

namespace shmit
{
namespace platform
{

inline constexpr bool is_bare_metal()
{
#ifdef BARE_METAL_SHMIT
    return true;
#else
    return false;
#endif
}

} // namespace platform
} // namespace shmit