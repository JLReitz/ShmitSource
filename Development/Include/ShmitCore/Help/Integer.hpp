#pragma once

#include <ShmitCore/StdTypes.hpp>

namespace shmit
{

/**!
 * @brief Protected conversion from signed to unsigned value
 *
 * @tparam Signed Signed arithmetic fundamental
 * @param n Signed value
 * @retval Signed value if it is >= 0
 * @retval 0 if signed value is < 0
 */
template<typename Signed>
inline constexpr std::make_unsigned_t<Signed> to_unsigned(Signed n)
{
    using Unsigned = std::make_unsigned_t<Signed>;

    Unsigned ret = 0;
    if (n > 0)
        ret = static_cast<Unsigned>(n);
    return ret;
}

} // namespace shmit