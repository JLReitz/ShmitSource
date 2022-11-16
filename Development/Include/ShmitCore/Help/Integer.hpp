#pragma once

#include <ShmitCore/Types/StdTypes.hpp>

namespace shmit
{

/**!
 * @brief Protected conversion from signed to unsigned int.
 *
 * @param n Signed value
 * @retval n if n > 0
 * @retval 0 otherwise
 */
uint64_t to_unsigned(int64_t n);

} // namespace shmit