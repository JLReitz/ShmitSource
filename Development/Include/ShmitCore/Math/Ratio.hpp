#pragma once

#include "Algorithms/StdAlgorithms.hpp"
#include "StaticArithmetic.hpp"

#include <ShmitCore/Types/StdTypes.hpp>

#include <numeric>
#include <ratio>

namespace shmit
{
namespace math
{

/**!
 * @brief Public construction routine for Ratio. Ensures safe rational representation.
 *
 * @tparam Numerator
 * @tparam Denominator
 * @return Ratio
 *
 * @note Denominator may not be 0
 */
template<intmax_t Numerator, intmax_t Denominator>
static constexpr class Ratio create_ratio() noexcept;

/**!
 * @brief Provides runtime support for rational integer arithmetic
 *
 */
struct Ratio
{
public:
    constexpr Ratio() = default;

    constexpr Ratio(Ratio const& rhs);
    constexpr Ratio(Ratio&& rhs);

    /**!
     * @brief Simplifies the ratio to its greatest common denominator
     *
     * @param[in] ratio
     * @return constexpr Ratio
     */
    static constexpr Ratio Simplify(Ratio const& ratio) noexcept;

    /**!
     * @brief Cross-multiplies two Ratios so that their denominators match while maintaining their individual
     * proportions. The results are left unsimplified.
     *
     * @param[in] rhs
     * @param[in] lhs
     */
    static constexpr void Align(Ratio& rhs, Ratio& lhs) noexcept;

    /**!
     * @brief Inverts a Ratio
     *
     * @param[in] ratio Inversion subject
     * @return Inverted ratio
     */
    static constexpr Ratio Invert(Ratio const& ratio) noexcept;

    constexpr          operator intmax_t() const noexcept;
    explicit constexpr operator double() const noexcept;

    constexpr bool operator==(Ratio const& rhs) const noexcept;
    constexpr bool operator!=(Ratio const& rhs) const noexcept;

    constexpr bool operator>(intmax_t rhs) const noexcept;
    constexpr bool operator>(Ratio const& rhs) const noexcept;
    constexpr bool operator>=(intmax_t rhs) const noexcept;
    constexpr bool operator>=(Ratio const& rhs) const noexcept;
    constexpr bool operator<(intmax_t rhs) const noexcept;
    constexpr bool operator<(Ratio const& rhs) const noexcept;
    constexpr bool operator<=(intmax_t rhs) const noexcept;
    constexpr bool operator<=(Ratio const& rhs) const noexcept;

    constexpr Ratio operator+() const noexcept;
    constexpr Ratio operator-() const noexcept;

    constexpr Ratio operator++() noexcept;
    constexpr Ratio operator++(int) noexcept;
    constexpr Ratio operator--() noexcept;
    constexpr Ratio operator--(int) noexcept;

    constexpr Ratio operator+(Ratio const& rhs) const noexcept;

    template<typename T>
    constexpr Ratio& operator+=(T const& rhs) noexcept;

    constexpr Ratio operator-(Ratio const& rhs) const noexcept;

    template<typename T>
    constexpr Ratio& operator-=(T const& rhs) noexcept;

    constexpr Ratio operator*(Ratio const& rhs) const noexcept;

    template<typename T>
    constexpr Ratio& operator*=(T const& rhs) noexcept;

    constexpr Ratio operator/(Ratio const& rhs) const noexcept;

    template<typename T>
    constexpr Ratio& operator/=(T const& rhs) noexcept;

    constexpr Ratio& operator=(Ratio const& rhs) noexcept;
    constexpr Ratio& operator=(Ratio&& rhs) noexcept;

    friend constexpr intmax_t operator+(intmax_t lhs, Ratio const& rhs) noexcept;
    friend constexpr double   operator+(double lhs, Ratio const& rhs) noexcept;
    friend constexpr Ratio    operator+(Ratio const& lhs, intmax_t rhs) noexcept;
    friend constexpr Ratio    operator+(Ratio const& lhs, double rhs) noexcept;

    friend constexpr intmax_t operator-(intmax_t lhs, Ratio const& rhs) noexcept;
    friend constexpr double   operator-(double lhs, Ratio const& rhs) noexcept;
    friend constexpr Ratio    operator-(Ratio const& lhs, intmax_t rhs) noexcept;
    friend constexpr Ratio    operator-(Ratio const& lhs, double rhs) noexcept;

    friend constexpr intmax_t operator*(intmax_t lhs, Ratio const& rhs) noexcept;
    friend constexpr double   operator*(double lhs, Ratio const& rhs) noexcept;
    friend constexpr Ratio    operator*(Ratio const& lhs, intmax_t rhs) noexcept;
    friend constexpr Ratio    operator*(Ratio const& lhs, double rhs) noexcept;

    friend constexpr intmax_t operator/(intmax_t lhs, Ratio const& rhs) noexcept;
    friend constexpr double   operator/(double lhs, Ratio const& rhs) noexcept;
    friend constexpr Ratio    operator/(Ratio const& lhs, intmax_t rhs) noexcept;
    friend constexpr Ratio    operator/(Ratio const& lhs, double rhs) noexcept;

    template<intmax_t Numerator, intmax_t Denominator>
    friend constexpr Ratio create_ratio() noexcept;

    intmax_t numerator {1};
    intmax_t denominator {1};

private:
    /**
     * @brief Constructs a Ratio to represent a whole-number value
     *
     * @param[in] n Whole-number
     */
    constexpr Ratio(intmax_t n) noexcept;
    constexpr Ratio(uintmax_t n) noexcept;

    /**!
     * @brief Simplifies the signs of numerator and denominator
     *
     */
    constexpr void SimplifySign() noexcept;
};

/**!
 * @brief Extends std::ratio to provide additional support for compile-time rational integer arithmetic.
 *
 * @tparam Numerator
 * @tparam Denominator
 *
 * @note Denominator may not be 0
 */
template<intmax_t Numerator, intmax_t Denominator>
struct StaticRatio : public std::ratio<Numerator, Denominator>
{
    using type =
        typename std::ratio<Numerator, Denominator>::type; /*! Back-reference to equivalent std::ratio specialization */

    static constexpr Ratio value = create_ratio<Numerator, Denominator>();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Metafunction definitions            ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**!
 * @brief Default (failed) check for if a type is of std::ratio or StaticRatio
 *
 * @tparam T Any type
 */
template<class T>
struct is_ratio : public std::false_type
{
};

/**
 * @brief Successful check for if a type is of std::ratio
 *
 * @tparam std::ratio<Numerator,Denominator>
 */
template<intmax_t Numerator, intmax_t Denominator>
struct is_ratio<std::ratio<Numerator, Denominator>> : public std::true_type
{
};

/**
 * @brief Successful check for if a type is of StaticRatio
 *
 * @tparam shmit::math::StaticRatio<Numerator,Denominator>
 */
template<intmax_t Numerator, intmax_t Denominator>
struct is_ratio<StaticRatio<Numerator, Denominator>> : public std::true_type
{
};

/**
 * @brief Converts any compile-time rational arithmetic type to a StaticRatio specialization type
 *
 * @tparam Ratio Meets the "Compile-Time Ratio" named requirements
 */
template<class Ratio>
struct to_static_ratio
{
    static_assert(is_ratio<Ratio>::value, "Ratio must be std::ratio or shmit::math::StaticRatio specialization");

    using type = StaticRatio<Ratio::num, Ratio::den>;
};

/**!
 * @brief Accesses the returned type from is_ratio
 *
 * @tparam T Any type
 */
template<class T>
using is_ratio_t = typename is_ratio<T>::type;

/**!
 * @brief Accesses the returned value from is_ratio
 *
 * @tparam T Any type
 */
template<class T>
inline constexpr bool is_ratio_v = is_ratio<T>::value;

/**!
 * @brief Accesses the returned type from to_static_ratio
 *
 * @tparam T Any type
 */
template<class T>
using to_static_ratio_t = typename to_static_ratio<T>::type;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  StaticRatio arithmetic metafunction definitions ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**!
 * @brief Compile-time addition of two StaticRatio specializations
 *
 * @tparam shmit::math::StaticRatio<Numerator1,Denominator1> Augend
 * @tparam shmit::math::StaticRatio<Numerator2,Denominator2> Addend
 * @return shmit::math::StaticRatio
 * @retval shmit::math::Ratio
 */
template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
struct add<StaticRatio<Numerator1, Denominator1>, StaticRatio<Numerator2, Denominator2>>
{
private:
    using Augend    = typename StaticRatio<Numerator1, Denominator1>::type;
    using Addend    = typename StaticRatio<Numerator2, Denominator2>::type;
    using StlResult = std::ratio_add<Augend, Addend>;

public:
    using type = typename to_static_ratio<StlResult>::type; /*! StaticRatio specialization of the sum */

    static constexpr Ratio value = type::value; /*! Runtime value representaion*/
};

/**!
 * @brief Compile-time addition of a StaticRatio specialization and integer constant
 *
 * @tparam shmit::math::StaticRatio<Numerator,Denominator> Augend
 * @tparam std::integral_constant<intmax_t,Value> Addend
 * @return shmit::math::StaticRatio
 * @retval shmit::math::Ratio
 */
template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
struct add<StaticRatio<Numerator, Denominator>, std::integral_constant<intmax_t, Value>>
{
    static_assert(Value >= -__INTMAX_MAX__, "Value out of range");

private:
    using Augend = StaticRatio<Numerator, Denominator>;
    using Addend = StaticRatio<Value, 1>;

public:
    using type = typename add<Augend, Addend>::type; /*! StaticRatio specialization of the sum */

    static constexpr Ratio value = type::value; /*! Runtime value */
};

/**!
 * @brief Compile-time addition of an integer constant and StaticRatio specialization
 *
 * @tparam std::integral_constant<intmax_t,Value> Augend
 * @tparam shmit::math::StaticRatio<Numerator,Denominator> Addend
 * @return std::integral_constant<intmax_t,...>
 * @retval intmax_t
 */
template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
struct add<std::integral_constant<intmax_t, Value>, StaticRatio<Numerator, Denominator>>
{
    static_assert(Value >= -__INTMAX_MAX__, "Value out of range");

private:
    static constexpr intmax_t kSum = Value + (Numerator / Denominator);

public:
    using type =
        typename std::integral_constant<intmax_t, kSum>::type; /*! std::integral_constant specialization of the sum */

    static constexpr intmax_t value = type::value; /*! Runtime value */
};

/**!
 * @brief Compile-time subtraction of one StaticRatio specialization from another
 *
 * @tparam shmit::math::StaticRatio<Numerator1,Denominator1> Minuend
 * @tparam shmit::math::StaticRatio<Numerator2,Denominator2> Subtrahend
 * @return shmit::math::StaticRatio
 * @retval shmit::math::Ratio
 */
template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
struct subtract<StaticRatio<Numerator1, Denominator1>, StaticRatio<Numerator2, Denominator2>>
{
private:
    using Minuend    = typename StaticRatio<Numerator1, Denominator1>::type;
    using Subtrahend = typename StaticRatio<Numerator2, Denominator2>::type;
    using StlResult  = std::ratio_subtract<Minuend, Subtrahend>;

public:
    using type = typename to_static_ratio<StlResult>::type; /*! StaticRatio specialization of the difference */

    static constexpr Ratio value = type::value; /*! Runtime value representaion*/
};

/**!
 * @brief Compile-time subtraction of an integer constant from a StaticRatio specialization
 *
 * @tparam shmit::math::StaticRatio<Numerator,Denominator> Minuend
 * @tparam std::integral_constant<intmax_t,Value> Subtrahend
 * @return shmit::math::StaticRatio
 * @retval shmit::math::Ratio
 */
template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
struct subtract<StaticRatio<Numerator, Denominator>, std::integral_constant<intmax_t, Value>>
{
    static_assert(Value >= -__INTMAX_MAX__, "Value out of range");

private:
    using Minuend    = StaticRatio<Numerator, Denominator>;
    using Subtrahend = StaticRatio<Value, 1>;

public:
    using type = typename subtract<Minuend, Subtrahend>::type; /*! StaticRatio specialization of the difference */

    static constexpr Ratio value = type::value; /*! Runtime value */
};

/**!
 * @brief Compile-time subtraction of a StaticRatio specialization from an integer constant
 *
 * @tparam std::integral_constant<intmax_t,Value> Minuend
 * @tparam shmit::math::StaticRatio<Numerator,Denominator> Subtrahend
 * @return std::integral_constant<intmax_t,...>
 * @retval intmax_t
 */
template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
struct subtract<std::integral_constant<intmax_t, Value>, StaticRatio<Numerator, Denominator>>
{
    static_assert(Value >= -__INTMAX_MAX__, "Value out of range");

private:
    static constexpr intmax_t kDifference = Value - (Numerator / Denominator);

public:
    using type = typename std::integral_constant<intmax_t, kDifference>::type; /*! std::integral_constant specialization
                                                                                  of the difference */

    static constexpr intmax_t value = type::value; /*! Runtime value */
};

/**!
 * @brief Compile-time multiplication of two StaticRatio specializations
 *
 * @tparam shmit::math::StaticRatio<Numerator1,Denominator1> Multiplicand
 * @tparam shmit::math::StaticRatio<Numerator2,Denominator2> Multiplier
 * @return shmit::math::StaticRatio
 * @retval shmit::math::Ratio
 */
template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
struct multiply<StaticRatio<Numerator1, Denominator1>, StaticRatio<Numerator2, Denominator2>>
{
private:
    using Multiplicand = typename StaticRatio<Numerator1, Denominator1>::type;
    using Multiplier   = typename StaticRatio<Numerator2, Denominator2>::type;
    using StlResult    = std::ratio_multiply<Multiplicand, Multiplier>;

public:
    using type = typename to_static_ratio<StlResult>::type; /*! StaticRatio specialization of the product */

    static constexpr Ratio value = type::value; /*! Runtime value representaion*/
};

/**!
 * @brief Compile-time multiplication of a StaticRatio specialization and integer constant
 *
 * @tparam shmit::math::StaticRatio<Numerator,Denominator> Multiplicand
 * @tparam std::integral_constant<intmax_t,Value> Multiplier
 * @return shmit::math::StaticRatio
 * @retval shmit::math::Ratio
 */
template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
struct multiply<StaticRatio<Numerator, Denominator>, std::integral_constant<intmax_t, Value>>
{
    static_assert(Value >= -__INTMAX_MAX__, "Value out of range");

private:
    using Multiplicand = StaticRatio<Numerator, Denominator>;
    using Multiplier   = StaticRatio<Value, 1>;

public:
    using type = typename multiply<Multiplicand, Multiplier>::type; /*! StaticRatio specialization of the product */

    static constexpr Ratio value = type::value; /*! Runtime value */
};

/**!
 * @brief Compile-time multiplication of an integer constant and StaticRatio specialization
 *
 * @tparam std::integral_constant<intmax_t,Value> Multiplicand
 * @tparam shmit::math::StaticRatio<Numerator,Denominator> Multiplier
 * @return std::integral_constant<intmax_t,...>
 * @retval intmax_t
 */
template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
struct multiply<std::integral_constant<intmax_t, Value>, StaticRatio<Numerator, Denominator>>
{
    static_assert(Value >= -__INTMAX_MAX__, "Value out of range");

private:
    static constexpr intmax_t kProduct = Value * (Numerator / Denominator);

public:
    using type =
        typename std::integral_constant<intmax_t, kProduct>::type; /*! std::integral_constant specialization of the product */

    static constexpr intmax_t value = type::value; /*! Runtime value */
};

/**!
 * @brief Compile-time division of two StaticRatio specializations
 *
 * @tparam shmit::math::StaticRatio<Numerator1,Denominator1> Dividend
 * @tparam shmit::math::StaticRatio<Numerator2,Denominator2> Divisor
 * @return shmit::math::StaticRatio
 * @retval shmit::math::Ratio
 */
template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
struct divide<StaticRatio<Numerator1, Denominator1>, StaticRatio<Numerator2, Denominator2>>
{
private:
    using Dividend  = typename StaticRatio<Numerator1, Denominator1>::type;
    using Divisor   = typename StaticRatio<Numerator2, Denominator2>::type;
    using StlResult = std::ratio_divide<Dividend, Divisor>;

public:
    using type = typename to_static_ratio<StlResult>::type; /*! StaticRatio specialization of the quotient */

    static constexpr Ratio value = type::value; /*! Runtime value representaion*/
};

/**!
 * @brief Compile-time division of a StaticRatio specialization by an integer constant
 *
 * @tparam shmit::math::StaticRatio<Numerator,Denominator> Dividend
 * @tparam std::integral_constant<intmax_t,Value> Divisor
 * @return shmit::math::StaticRatio
 * @retval shmit::math::Ratio
 */
template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
struct divide<StaticRatio<Numerator, Denominator>, std::integral_constant<intmax_t, Value>>
{
    static_assert(Value >= -__INTMAX_MAX__, "Value out of range");

private:
    using Dividend = StaticRatio<Numerator, Denominator>;
    using Divisor  = StaticRatio<Value, 1>;

public:
    using type = typename divide<Dividend, Divisor>::type; /*! StaticRatio specialization of the quotient */

    static constexpr Ratio value = type::value; /*! Runtime value */
};

/**!
 * @brief Compile-time division of an integer constant by a StaticRatio specialization
 *
 * @tparam std::integral_constant<intmax_t,Value> Dividend
 * @tparam shmit::math::StaticRatio<Numerator,Denominator> Divisor
 * @return std::integral_constant<intmax_t,...>
 * @retval intmax_t
 */
template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
struct divide<std::integral_constant<intmax_t, Value>, StaticRatio<Numerator, Denominator>>
{
    static_assert(Value >= -__INTMAX_MAX__, "Value out of range");
    static_assert((Numerator / Denominator) != 0, "(int)(Numerator / Denominator) cannot not be 0");

private:
    static constexpr intmax_t kQuotient = Value / (Numerator / Denominator);

public:
    using type = typename std::integral_constant<intmax_t, kQuotient>::type; /*! std::integral_constant specialization of the quotient */

    static constexpr intmax_t value = type::value; /*! Runtime value */
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Ratio constructor definitions           ////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

constexpr Ratio::Ratio(Ratio const& rhs) : numerator {rhs.numerator}, denominator {rhs.denominator}
{
}

constexpr Ratio::Ratio(Ratio&& rhs) : numerator {rhs.numerator}, denominator {rhs.denominator}
{
}

//  Private     ========================================================================================================

constexpr Ratio::Ratio(intmax_t n) noexcept : numerator {n}, denominator {1}
{
}

constexpr Ratio::Ratio(uintmax_t n) noexcept : numerator {static_cast<intmax_t>(n)}, denominator {1}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Ratio method definitions in alphabetical order      ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

constexpr Ratio Ratio::Simplify(Ratio const& ratio) noexcept // Static method
{
    shmit::math::Ratio simplified = ratio;

    // Determine the greatest common divisor between numerator and denominator
    intmax_t gcd = std::gcd(simplified.numerator, simplified.denominator);

    // Divide both numerator and denominator by the GCD
    simplified.numerator /= gcd;
    simplified.denominator /= gcd;

    return simplified;
}

constexpr void Ratio::Align(Ratio& rhs, Ratio& lhs) noexcept // Static method
{
    // Scale each by the other's denominator if they are not equivalent
    // Else, leave them alone
    if (rhs.denominator != lhs.denominator)
    {
        intmax_t rhs_den = rhs.denominator; // Cache RHS denominator pre-alignment

        rhs.numerator *= lhs.denominator;
        rhs.denominator *= lhs.denominator;

        lhs.numerator *= rhs_den;
        lhs.denominator *= rhs_den;
    }
}

constexpr Ratio Ratio::Invert(Ratio const& ratio) noexcept // Static method
{
    Ratio inverted;
    inverted.numerator   = ratio.denominator;
    inverted.denominator = ratio.numerator;

    // Simplify sign representation since the numerator value is now a bottom
    inverted.SimplifySign();
    return inverted;
}

constexpr Ratio::operator intmax_t() const noexcept
{
    // They asked for it, we shall provide
    return numerator / denominator;
}

constexpr Ratio::operator double() const noexcept
{
    // Do the same as above but better, because floating point
    return static_cast<double>(numerator) / denominator;
}

constexpr bool Ratio::operator==(Ratio const& rhs) const noexcept
{
    return (numerator == rhs.numerator) && (denominator == rhs.denominator);
}

constexpr bool Ratio::operator!=(Ratio const& rhs) const noexcept
{
    return !(*this == rhs);
}

constexpr bool Ratio::operator>(intmax_t rhs) const noexcept
{
    intmax_t whole_part = numerator / denominator;

    return (whole_part > rhs);
}

constexpr bool Ratio::operator>(Ratio const& rhs) const noexcept
{
    intmax_t right_cross = numerator * rhs.denominator;
    intmax_t left_cross  = denominator * rhs.numerator;

    return right_cross > left_cross;
}

constexpr bool Ratio::operator>=(intmax_t rhs) const noexcept
{
    intmax_t whole_part = numerator / denominator;

    return (whole_part >= rhs);
}

constexpr bool Ratio::operator>=(Ratio const& rhs) const noexcept
{
    intmax_t right_cross = numerator * rhs.denominator;
    intmax_t left_cross  = denominator * rhs.numerator;

    return right_cross >= left_cross;
}

constexpr bool Ratio::operator<(intmax_t rhs) const noexcept
{
    return !(*this >= rhs);
}

constexpr bool Ratio::operator<(Ratio const& rhs) const noexcept
{
    return !(*this >= rhs);
}

constexpr bool Ratio::operator<=(intmax_t rhs) const noexcept
{
    return !(*this > rhs);
}

constexpr bool Ratio::operator<=(Ratio const& rhs) const noexcept
{
    return !(*this > rhs);
}

constexpr Ratio Ratio::operator+() const noexcept
{
    return *this;
}

constexpr Ratio Ratio::operator-() const noexcept
{
    Ratio tmp     = *this;
    tmp.numerator = -numerator;

    return tmp;
}

constexpr Ratio Ratio::operator++() noexcept
{
    Ratio tmp = *this;
    numerator += denominator;

    return tmp;
}

constexpr Ratio Ratio::operator++(int) noexcept
{
    numerator += denominator;

    return *this;
}

constexpr Ratio Ratio::operator--() noexcept
{
    Ratio tmp = *this;
    numerator -= denominator;

    return tmp;
}

constexpr Ratio Ratio::operator--(int) noexcept
{
    numerator -= denominator;

    return *this;
}

template<typename T>
constexpr Ratio& Ratio::operator+=(T const& rhs) noexcept
{
    static_assert(std::is_arithmetic_v<T>, "T must be fundamental arithmetic type");

    numerator += (rhs * denominator);

    return *this;
}

template<>
constexpr Ratio& Ratio::operator+=(Ratio const& rhs) noexcept
{
    Ratio rhs_unconst = rhs; // Need to copy rhs to non-const variable for alignment

    Align(*this, rhs_unconst);
    numerator += rhs_unconst.numerator;

    return *this;
}

constexpr Ratio Ratio::operator+(Ratio const& rhs) const noexcept
{
    Ratio tmp = *this;
    tmp += rhs;
    return tmp;
}

template<typename T>
constexpr Ratio& Ratio::operator-=(T const& rhs) noexcept
{
    static_assert(std::is_arithmetic_v<T>, "T must be fundamental arithmetic type");

    numerator -= (rhs * denominator);

    return *this;
}

template<>
constexpr Ratio& Ratio::operator-=(Ratio const& rhs) noexcept
{
    Ratio rhs_unconst = rhs; // Need to copy rhs to non-const variable for alignment

    Align(*this, rhs_unconst);
    numerator -= rhs_unconst.numerator;

    return *this;
}

constexpr Ratio Ratio::operator-(Ratio const& rhs) const noexcept
{
    Ratio tmp = *this;
    tmp -= rhs;
    return tmp;
}

template<typename T>
constexpr Ratio& Ratio::operator*=(T const& rhs) noexcept
{
    static_assert(std::is_arithmetic_v<T>, "T must be fundamental arithmetic type");

    numerator *= rhs;

    return *this;
}

template<>
constexpr Ratio& Ratio::operator*=(Ratio const& rhs) noexcept
{
    numerator *= rhs.numerator;
    denominator *= rhs.denominator;

    return *this;
}

constexpr Ratio Ratio::operator*(Ratio const& rhs) const noexcept
{
    Ratio tmp = *this;
    tmp *= rhs;
    return tmp;
}

template<typename T>
constexpr Ratio& Ratio::operator/=(T const& rhs) noexcept
{
    static_assert(std::is_arithmetic_v<T>, "T must be fundamental arithmetic type");

    denominator *= rhs;

    return *this;
}

template<>
constexpr Ratio& Ratio::operator/=(Ratio const& rhs) noexcept
{
    Ratio rhs_inverted = Invert(rhs);

    *this *= rhs_inverted;

    return *this;
}

constexpr Ratio Ratio::operator/(Ratio const& rhs) const noexcept
{
    Ratio tmp = *this;
    tmp /= rhs;
    return tmp;
}

constexpr Ratio& Ratio::operator=(Ratio const& rhs) noexcept
{
    numerator   = rhs.numerator;
    denominator = rhs.denominator;
    return *this;
}

constexpr Ratio& Ratio::operator=(Ratio&& rhs) noexcept
{
    numerator   = rhs.numerator;
    denominator = rhs.denominator;
    return *this;
}

//  Private     ========================================================================================================

constexpr void Ratio::SimplifySign() noexcept
{
    if ((denominator < 0) && (numerator != 0))
    {
        // Set denominator to absolute value
        // Invert numerator to balance denominator's sign change
        denominator = -(denominator);
        numerator   = -(numerator);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  External operator overload definitions          ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr intmax_t operator+(intmax_t lhs, Ratio const& rhs) noexcept
{
    intmax_t ratio_cast = static_cast<intmax_t>(rhs);
    intmax_t result     = lhs + ratio_cast;
    return result;
}

constexpr double operator+(double lhs, Ratio const& rhs) noexcept
{
    double ratio_cast = static_cast<double>(rhs);
    double result     = lhs + ratio_cast;
    return result;
}

constexpr Ratio operator+(Ratio const& lhs, intmax_t rhs) noexcept
{
    Ratio result = lhs + Ratio(rhs);
    return result;
}

constexpr Ratio operator+(Ratio const& lhs, double rhs) noexcept
{
    Ratio result = lhs + Ratio(static_cast<intmax_t>(rhs));
    return result;
}

constexpr intmax_t operator-(intmax_t lhs, Ratio const& rhs) noexcept
{
    intmax_t ratio_cast = static_cast<intmax_t>(rhs);
    intmax_t result     = lhs - ratio_cast;
    return result;
}

constexpr double operator-(double lhs, Ratio const& rhs) noexcept
{
    double ratio_cast = static_cast<double>(rhs);
    double result     = lhs - ratio_cast;
    return result;
}

constexpr Ratio operator-(Ratio const& lhs, intmax_t rhs) noexcept
{
    Ratio result = lhs - Ratio(rhs);
    return result;
}

constexpr Ratio operator-(Ratio const& lhs, double rhs) noexcept
{
    Ratio result = lhs - Ratio(static_cast<intmax_t>(rhs));
    ;
    return result;
}

constexpr intmax_t operator*(intmax_t lhs, Ratio const& rhs) noexcept
{
    intmax_t ratio_cast = static_cast<intmax_t>(rhs);
    intmax_t result     = lhs * ratio_cast;
    return result;
}

constexpr double operator*(double lhs, Ratio const& rhs) noexcept
{
    double ratio_cast = static_cast<double>(rhs);
    double result     = lhs * ratio_cast;
    return result;
}

constexpr Ratio operator*(Ratio const& lhs, intmax_t rhs) noexcept
{
    Ratio result = lhs * Ratio(rhs);
    return result;
}

constexpr Ratio operator*(Ratio const& lhs, double rhs) noexcept
{
    Ratio result = lhs * Ratio(static_cast<intmax_t>(rhs));
    return result;
}

constexpr intmax_t operator/(intmax_t lhs, Ratio const& rhs) noexcept
{
    intmax_t ratio_cast = static_cast<intmax_t>(rhs);
    intmax_t result     = lhs / ratio_cast;
    return result;
}

constexpr double operator/(double lhs, Ratio const& rhs) noexcept
{
    double ratio_cast = static_cast<double>(rhs);
    double result     = lhs / ratio_cast;
    return result;
}

constexpr Ratio operator/(Ratio const& lhs, intmax_t rhs) noexcept
{
    Ratio result = lhs / Ratio(rhs);
    return result;
}

constexpr Ratio operator/(Ratio const& lhs, double rhs) noexcept
{
    Ratio result = lhs / Ratio(static_cast<intmax_t>(rhs));
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Namespace function definitions in alphabetical order        ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<intmax_t Numerator, intmax_t Denominator>
inline constexpr Ratio create_ratio() noexcept
{
    // Throw compiler error if Denominator is invalid or either parameter value is out of range
    static_assert(Denominator != 0, "'Denominator' may not be 0");
    static_assert(Numerator >= -__INTMAX_MAX__, "'Numerator' out of range");
    static_assert(Denominator >= -__INTMAX_MAX__, "'Denominator' out of range");

    Ratio tmp;
    tmp.numerator   = Numerator;
    tmp.denominator = Denominator;

    // Simplify sign representation of the Ratio
    tmp.SimplifySign();

    return tmp;
}

} // namespace math
} // namespace shmit
