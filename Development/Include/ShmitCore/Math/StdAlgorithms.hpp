#include <ShmitCore/StdTypes.hpp>

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

    if constexpr (std::is_signed_v<T>)
    {
        T result = value;

        if (value < 0)
            result *= -1;

        return static_cast<R>(result);
    }
    else
        return static_cast<R>(value);
}

} // namespace math
} // namespace shmit