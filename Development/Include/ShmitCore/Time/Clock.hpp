#pragma once

#include <type_traits>

namespace shmit
{
namespace time
{

template<class T>
struct is_clock
{
private:
    using HasIntegralTimeRep   = std::integral_constant<bool, std::is_integral_v<typename T::TimeRep>>;
    using HasIntegralFrequency = std::integral_constant<bool, std::is_integral_v<decltype(T::kFrequencyHz)>>;

public:
    using type = std::integral_constant<bool, (HasIntegralTimeRep::value && HasIntegralFrequency::value)>;
    static constexpr bool value = type::value;
};

template<class T>
using is_clock_t = typename is_clock<T>::type;

template<class T>
inline constexpr bool is_clock_v = is_clock<T>::value;

} // namespace time
} // namespace shmit
