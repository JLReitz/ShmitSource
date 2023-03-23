#pragma once

#include "Ratio.hpp"

namespace shmit
{
namespace math
{

/**
 * @brief Concept of a value denomination
 *
 * @tparam Representation Quantifying alue type of the denomination
 * @tparam RatioFromBase Conversion rate from base Denomination. Meets the "Compile-Time Ratio" named requirements.
 */
template<typename Representation, class RatioFromBase>
struct Denomination
{
    static_assert(std::is_arithmetic_v<Representation>, "Representation must be fundamental arithmetic type");

    using Rep      = Representation;
    using FromBase = typename to_static_ratio<RatioFromBase>::type;
    using ToBase   = typename divide<StaticRatio<1, 1>, FromBase>::type;

    static_assert(FromBase::num > 0, "RatioFromBase must be positive");
    static_assert(FromBase::den > 0, "RatioFromBase must be positive");
};

template<class DenominationOut, class DenominationIn>
inline constexpr typename DenominationOut::Rep denomination_cast(typename DenominationIn::Rep const& value)
{
    using ResultRep = typename DenominationOut::Rep;
    using Ratio1    = typename DenominationIn::ToBase;
    using Ratio2    = typename DenominationOut::ToBase;

    // Divide the first ratio by the second to determine how the value must be scaled
    using Scale = typename math::divide<Ratio1, Ratio2>;
    constexpr math::Ratio scale {Scale::value};

    // Cast quantifying value to the output Denomination's representation then perform value transformation
    ResultRep result {static_cast<ResultRep>(value)};
    result *= scale;

    return result;
}

/**!
 * @brief Base denomination of a set. Conversion ratio defaults to 1/1.
 *
 * @tparam Representation Value type of the denomination
 */
template<typename Representation>
using BaseDenomination = Denomination<Representation, std::ratio<1>>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Metafunction definitions            ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**!
 * @brief Default (failed) check for if a type is of Denomination
 *
 * @tparam T Any type
 */
template<class T>
struct is_denomination : public std::false_type
{
};

/**
 * @brief Successful check for if a type is of Denomination
 *
 * @tparam shmit::math::Denomination<Representation,RatioFromBase>
 */
template<typename Representation, class RatioFromBase>
struct is_denomination<Denomination<Representation, RatioFromBase>> : public std::true_type
{
};

/**!
 * @brief Accesses the returned type from is_denomination
 *
 * @tparam T Any type
 */
template<class T>
using is_denomination_t = typename is_denomination<T>::type;

/**!
 * @brief Accesses the returned value from is_denomination
 *
 * @tparam T Any type
 */
template<class T>
inline constexpr bool is_denomination_v = is_denomination<T>::value;

} // namespace math
} // namespace shmit