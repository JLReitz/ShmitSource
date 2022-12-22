#pragma once

#include <ShmitCore/Math/StaticArithmetic.hpp>
#include <ShmitCore/Types/StdTypes.hpp>

#include <numeric>
#include <ratio>
#include <type_traits>

namespace shmit
{
namespace math
{

struct Ratio
{
public:
    constexpr Ratio() = default;

    constexpr Ratio(Ratio const& rhs);
    constexpr Ratio(Ratio&& rhs);

    static constexpr void  Align(Ratio& rhs, Ratio& lhs) noexcept;
    static constexpr Ratio Invert(Ratio ratio) noexcept;

    constexpr operator int() const noexcept;

    constexpr bool operator==(Ratio rhs) const noexcept;
    constexpr bool operator!=(Ratio rhs) const noexcept;

    constexpr bool operator>(intmax_t rhs) const noexcept;
    constexpr bool operator>(Ratio rhs) const noexcept;
    constexpr bool operator>=(intmax_t rhs) const noexcept;
    constexpr bool operator>=(Ratio rhs) const noexcept;
    constexpr bool operator<(intmax_t rhs) const noexcept;
    constexpr bool operator<(Ratio rhs) const noexcept;
    constexpr bool operator<=(intmax_t rhs) const noexcept;
    constexpr bool operator<=(Ratio rhs) const noexcept;

    constexpr Ratio operator+() const noexcept;
    constexpr Ratio operator-() const noexcept;

    constexpr Ratio operator++() noexcept;
    constexpr Ratio operator++(int) noexcept;
    constexpr Ratio operator--() noexcept;
    constexpr Ratio operator--(int) noexcept;

    constexpr Ratio  operator+(Ratio rhs) const noexcept;
    constexpr Ratio& operator+=(intmax_t rhs) noexcept;
    constexpr Ratio& operator+=(Ratio rhs) noexcept;

    constexpr Ratio  operator-(Ratio rhs) const noexcept;
    constexpr Ratio& operator-=(intmax_t rhs) noexcept;
    constexpr Ratio& operator-=(Ratio rhs) noexcept;

    constexpr Ratio  operator*(Ratio rhs) const noexcept;
    constexpr Ratio& operator*=(intmax_t rhs) noexcept;
    constexpr Ratio& operator*=(Ratio rhs) noexcept;

    constexpr Ratio  operator/(Ratio rhs) const noexcept;
    constexpr Ratio& operator/=(intmax_t rhs) noexcept;
    constexpr Ratio& operator/=(Ratio rhs) noexcept;

    constexpr Ratio& operator=(Ratio const& rhs);
    constexpr Ratio& operator=(Ratio&& rhs);

    friend constexpr intmax_t operator+(intmax_t lhs, Ratio rhs);
    friend constexpr Ratio    operator+(Ratio lhs, intmax_t rhs);

    friend constexpr intmax_t operator-(intmax_t lhs, Ratio rhs);
    friend constexpr Ratio    operator-(Ratio lhs, intmax_t rhs);

    friend constexpr intmax_t operator*(intmax_t lhs, Ratio rhs);
    friend constexpr Ratio    operator*(Ratio lhs, intmax_t rhs);

    friend constexpr intmax_t operator/(intmax_t lhs, Ratio rhs);
    friend constexpr Ratio    operator/(Ratio lhs, intmax_t rhs);

    template<intmax_t Numerator, intmax_t Denominator>
    friend constexpr Ratio create_ratio();

    intmax_t num {1};
    intmax_t den {1};

private:
    constexpr Ratio(intmax_t n) noexcept;

    constexpr void SimplifySign() noexcept;
};

template<intmax_t Numerator, intmax_t Denominator>
static constexpr Ratio create_ratio();

template<intmax_t Numerator, intmax_t Denominator>
class StaticRatio : public std::ratio<Numerator, Denominator>
{
public:
    using type = typename std::ratio<Numerator, Denominator>::type;

    static constexpr Ratio Value() noexcept;
};

template<typename T>
struct IsStaticRatio : public std::false_type
{
};

template<intmax_t Numerator, intmax_t Denominator>
struct IsStaticRatio<StaticRatio<Numerator, Denominator>> : public std::true_type
{
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  StaticRatio arithmetic          ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
struct Add<StaticRatio<Numerator1, Denominator1>, StaticRatio<Numerator2, Denominator2>>
{
private:
    using GcdRatio1 = typename StaticRatio<Numerator1, Denominator1>::type;
    using GcdRatio2 = typename StaticRatio<Numerator2, Denominator2>::type;

public:
    using Result = StaticRatio<(GcdRatio1::num * GcdRatio2::den) + (GcdRatio2::num * GcdRatio1::den),
                               GcdRatio1::den * GcdRatio2::den>;
};

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
struct Add<StaticRatio<Numerator, Denominator>, std::integral_constant<intmax_t, Value>>
{
    static_assert(Value >= -__INTMAX_MAX__, "'Value' out of range");

private:
    using OperandRatio = StaticRatio<Numerator, Denominator>;
    using ValueRatio   = StaticRatio<Value, 1>;

public:
    using Result = typename Add<OperandRatio, ValueRatio>::Result;
};

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
struct Subtract<StaticRatio<Numerator1, Denominator1>, StaticRatio<Numerator2, Denominator2>>
{
private:
    using GcdRatio1 = typename StaticRatio<Numerator1, Denominator1>::type;
    using GcdRatio2 = typename StaticRatio<Numerator2, Denominator2>::type;

public:
    using Result = StaticRatio<(GcdRatio1::num * GcdRatio2::den) - (GcdRatio2::num * GcdRatio1::den),
                               GcdRatio1::den * GcdRatio2::den>;
};

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
struct Subtract<StaticRatio<Numerator, Denominator>, std::integral_constant<intmax_t, Value>>
{
    static_assert(Value >= -__INTMAX_MAX__, "'Value' out of range");

private:
    using OperandRatio = StaticRatio<Numerator, Denominator>;
    using ValueRatio   = StaticRatio<Value, 1>;

public:
    using Result = typename Subtract<OperandRatio, ValueRatio>::Result;
};

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
struct Multiply<StaticRatio<Numerator1, Denominator1>, StaticRatio<Numerator2, Denominator2>>
{
private:
    using GcdRatio1 = typename StaticRatio<Numerator1, Denominator1>::type;
    using GcdRatio2 = typename StaticRatio<Numerator2, Denominator2>::type;

public:
    using Result = StaticRatio<GcdRatio1::num * GcdRatio2::num, GcdRatio1::den * GcdRatio2::den>;
};

template<intmax_t Numerator, intmax_t Denominator, intmax_t Scalar>
struct Multiply<StaticRatio<Numerator, Denominator>, std::integral_constant<intmax_t, Scalar>>
{
    static_assert(Scalar >= -__INTMAX_MAX__, "'Scalar' out of range");

private:
    using OperandRatio = StaticRatio<Numerator, Denominator>;
    using ScalarRatio  = StaticRatio<Scalar, 1>;

public:
    using Result = typename Multiply<OperandRatio, ScalarRatio>::Result;
};

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
struct Divide<StaticRatio<Numerator1, Denominator1>, StaticRatio<Numerator2, Denominator2>>
{
private:
    using GcdRatio1 = typename StaticRatio<Numerator1, Denominator1>::type;
    using GcdRatio2 = typename StaticRatio<Numerator2, Denominator2>::type;

public:
    using Result = StaticRatio<GcdRatio1::num * GcdRatio2::den, GcdRatio1::den * GcdRatio2::num>;
};

template<intmax_t Numerator, intmax_t Denominator, intmax_t Divisor>
struct Divide<StaticRatio<Numerator, Denominator>, std::integral_constant<intmax_t, Divisor>>
{
    static_assert(Divisor >= -__INTMAX_MAX__, "'Divisor' out of range");

private:
    using OperandRatio = StaticRatio<Numerator, Denominator>;
    using DivisorRatio = StaticRatio<Divisor, 1>;

public:
    using Result = typename Divide<OperandRatio, DivisorRatio>::Result;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Ratio constructor definitions           ////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

constexpr Ratio::Ratio(Ratio const& rhs) : num {rhs.num}, den {rhs.den}
{
}

constexpr Ratio::Ratio(Ratio&& rhs) : num {rhs.num}, den {rhs.den}
{
}

//  Private     ========================================================================================================

constexpr Ratio::Ratio(intmax_t n) noexcept : num {n}, den {1}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Ratio method definitions in alphabetical order      ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

constexpr void Ratio::Align(Ratio& rhs, Ratio& lhs) noexcept // Static method
{
    // Scale each by the other's denominator if they are not equivalent
    // Else, leave them alone
    if (rhs.den != lhs.den)
    {
        intmax_t rhs_den = rhs.den; // Cache RHS denominator pre-alignment

        rhs.num *= lhs.den;
        rhs.den *= lhs.den;

        lhs.num *= rhs_den;
        lhs.den *= rhs_den;
    }
}

constexpr Ratio Ratio::Invert(Ratio ratio) noexcept // Static method
{
    // Swap numerator and denominator
    intmax_t pre_swap_num = ratio.num;

    ratio.num = ratio.den;
    ratio.den = pre_swap_num;

    // Simplify sign representation since the numerator value is now a bottom
    ratio.SimplifySign();
    return ratio;
}

constexpr Ratio::operator int() const noexcept
{
    // They asked for it, we shall provide
    return num / den;
}

constexpr bool Ratio::operator>(intmax_t rhs) const noexcept
{
    intmax_t whole_part = num / den;

    return (whole_part > rhs);
}

constexpr bool Ratio::operator>(Ratio rhs) const noexcept
{
    intmax_t right_cross = num * rhs.den;
    intmax_t left_cross  = den * rhs.num;

    return right_cross > left_cross;
}

constexpr bool Ratio::operator>=(intmax_t rhs) const noexcept
{
    intmax_t whole_part = num / den;

    return (whole_part >= rhs);
}

constexpr bool Ratio::operator>=(Ratio rhs) const noexcept
{
    intmax_t right_cross = num * rhs.den;
    intmax_t left_cross  = den * rhs.num;

    return right_cross >= left_cross;
}

constexpr bool Ratio::operator<(intmax_t rhs) const noexcept
{
    return !(*this >= rhs);
}

constexpr bool Ratio::operator<(Ratio rhs) const noexcept
{
    return !(*this >= rhs);
}

constexpr bool Ratio::operator<=(intmax_t rhs) const noexcept
{
    return !(*this > rhs);
}

constexpr bool Ratio::operator<=(Ratio rhs) const noexcept
{
    return !(*this > rhs);
}

constexpr Ratio Ratio::operator+() const noexcept
{
    return *this;
}

constexpr Ratio Ratio::operator-() const noexcept
{
    Ratio tmp = *this;
    tmp.num   = -num;

    return tmp;
}

constexpr Ratio Ratio::operator++() noexcept
{
    Ratio tmp = *this;
    num += den;

    return tmp;
}

constexpr Ratio Ratio::operator++(int) noexcept
{
    num += den;

    return *this;
}

constexpr Ratio Ratio::operator--() noexcept
{
    Ratio tmp = *this;
    num -= den;

    return tmp;
}

constexpr Ratio Ratio::operator--(int) noexcept
{
    num -= den;

    return *this;
}

constexpr Ratio Ratio::operator+(Ratio rhs) const noexcept
{
    Ratio tmp = *this;
    tmp += rhs;
    return tmp;
}

constexpr Ratio& Ratio::operator+=(intmax_t rhs) noexcept
{
    num += (rhs * den);

    return *this;
}

constexpr Ratio& Ratio::operator+=(Ratio rhs) noexcept
{
    Align(*this, rhs);

    num += rhs.num;

    return *this;
}

constexpr Ratio Ratio::operator-(Ratio rhs) const noexcept
{
    Ratio tmp = *this;
    tmp -= rhs;
    return tmp;
}

constexpr Ratio& Ratio::operator-=(intmax_t rhs) noexcept
{
    num -= (rhs * den);

    return *this;
}

constexpr Ratio& Ratio::operator-=(Ratio rhs) noexcept
{
    Align(*this, rhs);

    num -= rhs.num;

    return *this;
}

constexpr Ratio Ratio::operator*(Ratio rhs) const noexcept
{
    Ratio tmp = *this;
    tmp *= rhs;
    return tmp;
}

constexpr Ratio& Ratio::operator*=(intmax_t rhs) noexcept
{
    num *= rhs;

    return *this;
}

constexpr Ratio& Ratio::operator*=(Ratio rhs) noexcept
{
    num *= rhs.num;
    den *= rhs.den;

    return *this;
}

constexpr Ratio Ratio::operator/(Ratio rhs) const noexcept
{
    Ratio tmp = *this;
    tmp /= rhs;
    return tmp;
}

constexpr Ratio& Ratio::operator/=(intmax_t rhs) noexcept
{
    den *= rhs;

    return *this;
}

constexpr Ratio& Ratio::operator/=(Ratio rhs) noexcept
{
    Ratio rhs_inverted = Invert(rhs);

    *this *= rhs_inverted;

    return *this;
}

constexpr Ratio& Ratio::operator=(Ratio const& rhs)
{
    num = rhs.num;
    den = rhs.den;
    return *this;
}

constexpr Ratio& Ratio::operator=(Ratio&& rhs)
{
    num = rhs.num;
    den = rhs.den;
    return *this;
}

//  Private     ========================================================================================================

constexpr void Ratio::SimplifySign() noexcept
{
    if ((den < 0) && (num != 0))
    {
        // Set denominator to absolute value
        // Invert numerator to balance denominator's sign change
        den = -(den);
        num = -(num);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  StaticRatio method definitions in alphabetical order        ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

template<intmax_t Numerator, intmax_t Denominator>
constexpr Ratio StaticRatio<Numerator, Denominator>::Value() noexcept // Static method
{
    return create_ratio<Numerator, Denominator>();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  External operator overload definitions          ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr intmax_t operator+(intmax_t lhs, Ratio rhs)
{
    Ratio result = Ratio(lhs) + rhs;
    return result;
}

constexpr Ratio operator+(Ratio lhs, intmax_t rhs)
{
    Ratio result = lhs + Ratio(rhs);
    return result;
}

constexpr intmax_t operator-(intmax_t lhs, Ratio rhs)
{
    Ratio result = Ratio(lhs) - rhs;
    return result;
}

constexpr Ratio operator-(Ratio lhs, intmax_t rhs)
{
    Ratio result = lhs - Ratio(rhs);
    return result;
}

constexpr intmax_t operator*(intmax_t lhs, Ratio rhs)
{
    Ratio result = Ratio(lhs) * rhs;
    return result;
}

constexpr Ratio operator*(Ratio lhs, intmax_t rhs)
{
    Ratio result = lhs * Ratio(rhs);
    return result;
}

constexpr intmax_t operator/(intmax_t lhs, Ratio rhs)
{
    Ratio result = Ratio(lhs) / rhs;
    return result;
}

constexpr Ratio operator/(Ratio lhs, intmax_t rhs)
{
    Ratio result = lhs / Ratio(rhs);
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Global method definitions in alphabetical order         ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<intmax_t Numerator, intmax_t Denominator>
constexpr Ratio create_ratio() // Static method
{
    // Throw compiler error if Denominator is invalid or either parameter value is out of range
    static_assert(Denominator != 0, "'Denominator' may not be 0");
    static_assert(Numerator >= -__INTMAX_MAX__, "'Numerator' out of range");
    static_assert(Denominator >= -__INTMAX_MAX__, "'Denominator' out of range");

    Ratio tmp;
    tmp.num = Numerator;
    tmp.den = Denominator;

    // Simplify sign representation of the Ratio
    tmp.SimplifySign();

    return tmp;
}

} // namespace math
} // namespace shmit