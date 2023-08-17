#pragma once

#include <type_traits>

namespace shmit
{
namespace time
{

/**!
 * @brief Checks a type for compliance with the named requirements for Clock
 *
 * @tparam T Any type
 */
template<class T>
struct is_clock
{
private:
    // Check for encapsulated integral typedef 'TimeRep'
    using HasIntegralTimeRep = std::integral_constant<bool, std::is_integral_v<typename T::TimeRep>>;

    // Check for encapsulated integral constant 'kFrequencyHz'
    using HasIntegralFrequency = std::integral_constant<bool, std::is_integral_v<decltype(T::kFrequencyHz)>>;

public:
    using type = std::integral_constant<bool, (HasIntegralTimeRep::value && HasIntegralFrequency::value)>;
    static constexpr bool value = type::value;
};

/**!
 * @brief Accesses the returned type from is_clock
 *
 * @tparam T Any type
 */
template<class T>
using is_clock_t = typename is_clock<T>::type;

/**!
 * @brief Accesses the returned value from is_clock
 *
 * @tparam T Any type
 */
template<class T>
inline constexpr bool is_clock_v = is_clock<T>::value;

} // namespace time
} // namespace shmit
