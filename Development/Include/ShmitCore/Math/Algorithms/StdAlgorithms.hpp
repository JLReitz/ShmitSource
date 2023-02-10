#include <ShmitCore/Types/StdTypes.hpp>

#include <type_traits>

namespace shmit
{
namespace math
{

template<typename T, typename R = T>
static constexpr R abs(T value)
{
    static_assert(std::is_arithmetic_v<T>, "T must be fundamental arithmetic type");
    static_assert(std::is_arithmetic_v<R>, "T must be fundamental arithmetic type");

    T result = value;

    if (value < 0)
        result *= -1;

    return static_cast<R>(result);
}

} // namespace math
} // namespace shmit