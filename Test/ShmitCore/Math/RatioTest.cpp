#include <ShmitCore/Math/Ratio.hpp>

#include <cmath>
#include <numeric>

#include <gtest/gtest.h>

#define CHECK_INTEGER_ADDITION(check_function) \
    EXPECT_TRUE((check_function<-1, 1, 1>())); /* -1/1 + 1 = 0/1 */ \
    EXPECT_TRUE((check_function<2, 2, -1>())); /* 2/2 + (-1) = 0/2 */ \
\
    EXPECT_TRUE((check_function<3, 6, 0>())); /* 3/6 + 0 = 3/6 */ \
\
    EXPECT_TRUE((check_function<1, 2, 3>()));    /* 1/2 + 3 = 7/2 */ \
    EXPECT_TRUE((check_function<-4, 5, 6>()));   /* -4/5 + 6 = 26/5 */ \
    EXPECT_TRUE((check_function<7, -8, 9>()));   /* 7/-8 + 9 = 65/8 */ \
    EXPECT_TRUE((check_function<-10, -1, 2>())); /* -10/-1 + 2 = 12/1 */ \
    EXPECT_TRUE((check_function<3, 4, -5>()));   /* 3/4 + -5 = -17/4 */ \
    EXPECT_TRUE((check_function<-6, 7, -8>()));  /* -6/7 + -8 = -62/7 */ \
    EXPECT_TRUE((check_function<9, -10, -1>())); /* 9/-10 + -1 = -19/10 */ \
    EXPECT_TRUE((check_function<-2, -3, -4>())); /* -2/-3 + -4 = -10/3 */ \
    EXPECT_TRUE((check_function<5, 6, 7>()));    /* 5/6 + 7 = 47/6 */ \
    EXPECT_TRUE((check_function<-8, 9, 10>()));  /* -8/9 + 10 = 82/9 */ \
    EXPECT_TRUE((check_function<1, -2, 3>()));   /* 1/-2 + 3 = 5/2 */ \
    EXPECT_TRUE((check_function<-4, -5, 6>()));  /* -4/-5 + 6 = 34/5 */ \
    EXPECT_TRUE((check_function<7, 8, -9>()));   /* 7/8 + -9 = -65/8 */ \
    EXPECT_TRUE((check_function<-10, 1, -2>())); /* -10/1 + -2 = -12/1 */ \
    EXPECT_TRUE((check_function<3, -4, -5>()));  /* 3/-4 + -5 = -23/4 */ \
    EXPECT_TRUE((check_function<-6, -7, -8>())); /* -6/-7 + -8 = -50/7 */

#define CHECK_RATIO_ADDITION(check_function) \
    EXPECT_TRUE((check_function<1, 2, 3, 4>()));     /* 1/2 + 3/4 = 10/8 */ \
    EXPECT_TRUE((check_function<-5, 6, 7, 8>()));    /* -5/6 + 7/8 = 2/48 */ \
    EXPECT_TRUE((check_function<9, -1, 2, 3>()));    /* 9/-1 + 2/3 = -25/3 */ \
    EXPECT_TRUE((check_function<-4, -5, 6, 7>()));   /* -4/-5 + 6/7 = 58/35 */ \
    EXPECT_TRUE((check_function<8, 9, -1, 2>()));    /* 8/9 + -1/2 = 7/18 */ \
    EXPECT_TRUE((check_function<-3, 4, -5, 6>()));   /* -3/4 + -5/6 = -38/24 */ \
    EXPECT_TRUE((check_function<7, -8, -9, 1>()));   /* 7/-8 + -9/1 = -79/8 */ \
    EXPECT_TRUE((check_function<-2, -3, -4, 5>()));  /* -2/-3 + -4/5 = -2/15 */ \
    EXPECT_TRUE((check_function<6, 7, 8, -9>()));    /* 6/7 + 8/-9 = -2/63 */ \
    EXPECT_TRUE((check_function<-1, 2, 3, -4>()));   /* -1/2 + 3/-4 = -10/8 */ \
    EXPECT_TRUE((check_function<5, -6, 7, -8>()));   /* 5/-6 + 7/-8 = -82/48 */ \
    EXPECT_TRUE((check_function<-9, -1, 2, -3>()));  /* -9/-1 + 2/-3 = 25/3 */ \
    EXPECT_TRUE((check_function<4, 5, -6, -7>()));   /* 4/5 + -6/-7 = 58/35 */ \
    EXPECT_TRUE((check_function<-8, 9, -1, -2>()));  /* -8/9 + -1/-2 = -7/18 */ \
    EXPECT_TRUE((check_function<3, -4, -5, -6>()));  /* 3/-4 + -5/-6 = 2/24 */ \
    EXPECT_TRUE((check_function<-7, -8, -9, -1>())); /* -7/-8 + -9/-1 = 79/8 */

#define CHECK_INTEGER_SUBTRACTION(check_function) \
    EXPECT_TRUE((check_function<1, 1, 1>()));   /* 1/1 - 1 = 0/1 */ \
    EXPECT_TRUE((check_function<-2, 2, -1>())); /* -2/2 - (-1) = 0/2 */ \
\
    EXPECT_TRUE((check_function<3, 6, 0>())); /* 3/6 - 0 = 3/6 */ \
\
    EXPECT_TRUE((check_function<1, 2, 3>()));    /* 1/2 - 3 = -5/2 */ \
    EXPECT_TRUE((check_function<-4, 5, 6>()));   /* -4/5 - 6 = -34/5 */ \
    EXPECT_TRUE((check_function<7, -8, 9>()));   /* 7/-8 - 9 = -79/8 */ \
    EXPECT_TRUE((check_function<-10, -1, 2>())); /* -10/-1 - 2 = 8/1 */ \
    EXPECT_TRUE((check_function<3, 4, -5>()));   /* 3/4 - -5 = 23/4 */ \
    EXPECT_TRUE((check_function<-6, 7, -8>()));  /* -6/7 - -8 = 50/7 */ \
    EXPECT_TRUE((check_function<9, -10, -1>())); /* 9/-10 - -1 = 1/10 */ \
    EXPECT_TRUE((check_function<-2, -3, -4>())); /* -2/-3 - -4 = 14/3 */ \
    EXPECT_TRUE((check_function<5, 6, 7>()));    /* 5/6 - 7 = -37/6 */ \
    EXPECT_TRUE((check_function<-8, 9, 10>()));  /* -8/9 - 10 = -98/9 */ \
    EXPECT_TRUE((check_function<1, -2, 3>()));   /* 1/-2 - 3 = -7/2 */ \
    EXPECT_TRUE((check_function<-4, -5, 6>()));  /* -4/-5 - 6 = -26/5 */ \
    EXPECT_TRUE((check_function<7, 8, -9>()));   /* 7/8 - -9 = 79/8 */ \
    EXPECT_TRUE((check_function<-10, 1, -2>())); /* -10/1 - -2 = -8/1 */ \
    EXPECT_TRUE((check_function<3, -4, -5>()));  /* 3/-4 - -5 = 17/4 */ \
    EXPECT_TRUE((check_function<-6, -7, -8>())); /* -6/-7 - -8 = 62/7 */

#define CHECK_RATIO_SUBTRACTION(check_function) \
    EXPECT_TRUE((check_function<1, 2, 3, 4>()));     /* 1/2 - 3/4 = -2/8 */ \
    EXPECT_TRUE((check_function<-5, 6, 7, 8>()));    /* -5/6 - 7/8 = -82/48 */ \
    EXPECT_TRUE((check_function<9, -1, 2, 3>()));    /* 9/-1 - 2/3 = -29/3 */ \
    EXPECT_TRUE((check_function<-4, -5, 6, 7>()));   /* -4/-5 - 6/7 = -2/35 */ \
    EXPECT_TRUE((check_function<8, 9, -1, 2>()));    /* 8/9 - -1/2 = 25/18 */ \
    EXPECT_TRUE((check_function<-3, 4, -5, 6>()));   /* -3/4 - -5/6 = 2/24 */ \
    EXPECT_TRUE((check_function<7, -8, -9, 1>()));   /* 7/-8 - -9/1 = 65/8 */ \
    EXPECT_TRUE((check_function<-2, -3, -4, 5>()));  /* -2/-3 - -4/5 = 22/15 */ \
    EXPECT_TRUE((check_function<6, 7, 8, -9>()));    /* 6/7 - 8/-9 = 110/63 */ \
    EXPECT_TRUE((check_function<-1, 2, 3, -4>()));   /* -1/2 - 3/-4 = 2/8 */ \
    EXPECT_TRUE((check_function<5, -6, 7, -8>()));   /* 5/-6 - 7/-8 = 2/48 */ \
    EXPECT_TRUE((check_function<-9, -1, 2, -3>()));  /* -9/-1 - 2/-3 = 29/3 */ \
    EXPECT_TRUE((check_function<4, 5, -6, -7>()));   /* 4/5 - -6/-7 = -2/35 */ \
    EXPECT_TRUE((check_function<-8, 9, -1, -2>()));  /* -8/9 - -1/-2 = -25/18 */ \
    EXPECT_TRUE((check_function<3, -4, -5, -6>()));  /* 3/-4 - -5/-6 = -38/24 */ \
    EXPECT_TRUE((check_function<-7, -8, -9, -1>())); /* -7/-8 - -9/-1 = -65/8 */

#define CHECK_INTEGER_MULTIPLICATION(check_function) \
    EXPECT_TRUE((check_function<0, 1, 5>())); /* 0/1 * 5 = 0/1 */ \
    EXPECT_TRUE((check_function<2, 8, 0>())); /* 2/8 * 0 = 0/8 */ \
\
    EXPECT_TRUE((check_function<1, 1, 1>())); /* 1/1 * 1 = 1/1 */ \
\
    EXPECT_TRUE((check_function<1, 2, 3>()));    /* 1/2 * 3 = 3/2 */ \
    EXPECT_TRUE((check_function<-4, 5, 6>()));   /* -4/5 * 6 = -24/5 */ \
    EXPECT_TRUE((check_function<7, -8, 9>()));   /* 7/-8 * 9 = -63/8 */ \
    EXPECT_TRUE((check_function<-10, -1, 2>())); /* -10/-1 * 2 = 20/1 */ \
    EXPECT_TRUE((check_function<3, 4, -5>()));   /* 3/4 * -5 = -15/4 */ \
    EXPECT_TRUE((check_function<-6, 7, -8>()));  /* -6/7 * -8 = 48/7 */ \
    EXPECT_TRUE((check_function<9, -10, -1>())); /* 9/-10 * -1 = 9/10 */ \
    EXPECT_TRUE((check_function<-2, -3, -4>())); /* -2/-3 * -4 = -8/3 */ \
    EXPECT_TRUE((check_function<5, 6, 7>()));    /* 5/6 * 7 = 35/6 */ \
    EXPECT_TRUE((check_function<-8, 9, 10>()));  /* -8/9 * 10 = -80/9 */ \
    EXPECT_TRUE((check_function<1, -2, 3>()));   /* 1/-2 * 3 = -3/2 */ \
    EXPECT_TRUE((check_function<-4, -5, 6>()));  /* -4/-5 * 6 = 24/5 */ \
    EXPECT_TRUE((check_function<7, 8, -9>()));   /* 7/8 * -9 = -63/8 */ \
    EXPECT_TRUE((check_function<-10, 1, -2>())); /* -10/1 * -2 = 20/1 */ \
    EXPECT_TRUE((check_function<3, -4, -5>()));  /* 3/-4 * -5 = 15/4 */ \
    EXPECT_TRUE((check_function<-6, -7, -8>())); /* -6/-7 * -8 = -48/7 */

#define CHECK_RATIO_MULTIPLICATION(check_function) \
    EXPECT_TRUE((check_function<1, 2, 3, 4>()));     /* 1/2 * 3/4 = 3/8 */ \
    EXPECT_TRUE((check_function<-5, 6, 7, 8>()));    /* -5/6 * 7/8 = -35/48 */ \
    EXPECT_TRUE((check_function<9, -1, 2, 3>()));    /* 9/-1 * 2/3 = -18/3 */ \
    EXPECT_TRUE((check_function<-4, -5, 6, 7>()));   /* -4/-5 * 6/7 = 24/35 */ \
    EXPECT_TRUE((check_function<8, 9, -1, 2>()));    /* 8/9 * -1/2 = -8/18 */ \
    EXPECT_TRUE((check_function<-3, 4, -5, 6>()));   /* -3/4 * -5/6 = 15/24 */ \
    EXPECT_TRUE((check_function<7, -8, -9, 1>()));   /* 7/-8 * -9/1 = 63/8 */ \
    EXPECT_TRUE((check_function<-2, -3, -4, 5>()));  /* -2/-3 * -4/5 = -8/15 */ \
    EXPECT_TRUE((check_function<6, 7, 8, -9>()));    /* 6/7 * 8/-9 = -48/63 */ \
    EXPECT_TRUE((check_function<-1, 2, 3, -4>()));   /* -1/2 * 3/-4 = 3/8 */ \
    EXPECT_TRUE((check_function<5, -6, 7, -8>()));   /* 5/-6 * 7/-8 = 35/48 */ \
    EXPECT_TRUE((check_function<-9, -1, 2, -3>()));  /* -9/-1 * 2/-3 = -18/3 */ \
    EXPECT_TRUE((check_function<4, 5, -6, -7>()));   /* 4/5 * -6/-7 = 24/35 */ \
    EXPECT_TRUE((check_function<-8, 9, -1, -2>()));  /* -8/9 * -1/-2 = -8/18 */ \
    EXPECT_TRUE((check_function<3, -4, -5, -6>()));  /* 3/-4 * -5/-6 = -15/24 */ \
    EXPECT_TRUE((check_function<-7, -8, -9, -1>())); /* -7/-8 * -9/-1 = 63/8 */

#define CHECK_INTEGER_DIVISION(check_function) \
    EXPECT_TRUE((check_function<0, 1, 2>())); /* (0/1) / 2 = 0/2 */ \
\
    EXPECT_TRUE((check_function<1, 1, 1>())); /* (1/1) / 1 = 1/1 */ \
\
    EXPECT_TRUE((check_function<1, 2, 3>()));    /* (1/2) / 3 = 1/6 */ \
    EXPECT_TRUE((check_function<-4, 5, 6>()));   /* (-4/5) / 6 = -4/30 */ \
    EXPECT_TRUE((check_function<7, -8, 9>()));   /* (7/-8) / 9 = -7/72 */ \
    EXPECT_TRUE((check_function<-10, -1, 2>())); /* (-10/-1) / 2 = 10/2 */ \
    EXPECT_TRUE((check_function<3, 4, -5>()));   /* (3/4) / -5 = -3/20 */ \
    EXPECT_TRUE((check_function<-6, 7, -8>()));  /* (-6/7) / -8 = 6/56 */ \
    EXPECT_TRUE((check_function<9, -10, -1>())); /* (9/-10) / -1 = 9/10 */ \
    EXPECT_TRUE((check_function<-2, -3, -4>())); /* (-2/-3) / -4 = -2/12 */ \
    EXPECT_TRUE((check_function<5, 6, 7>()));    /* (5/6) / 7 = 5/42 */ \
    EXPECT_TRUE((check_function<-8, 9, 10>()));  /* (-8/9) / 10 = -8/90 */ \
    EXPECT_TRUE((check_function<1, -2, 3>()));   /* (1/-2) / 3 = -1/6 */ \
    EXPECT_TRUE((check_function<-4, -5, 6>()));  /* (-4/-5) / 6 = 4/30 */ \
    EXPECT_TRUE((check_function<7, 8, -9>()));   /* (7/8) / -9 = -7/72 */ \
    EXPECT_TRUE((check_function<-10, 1, -2>())); /* (-10/1) / -2 = 10/2 */ \
    EXPECT_TRUE((check_function<3, -4, -5>()));  /* (3/-4) / -5 = 3/20 */ \
    EXPECT_TRUE((check_function<-6, -7, -8>())); /* (-6/-7) / -8 = -6/56 */

#define CHECK_RATIO_DIVISION(check_function) \
    EXPECT_TRUE((check_function<1, 2, 3, 4>()));     /* (1/2) / 3/4 = 4/6 */ \
    EXPECT_TRUE((check_function<-5, 6, 7, 8>()));    /* (-5/6) / 7/8 = -40/42 */ \
    EXPECT_TRUE((check_function<9, -1, 2, 3>()));    /* (9/-1) / 2/3 = -27/2 */ \
    EXPECT_TRUE((check_function<-4, -5, 6, 7>()));   /* (-4/-5) / 6/7 = 28/30 */ \
    EXPECT_TRUE((check_function<8, 9, -1, 2>()));    /* (8/9) / -1/2 = -16/9 */ \
    EXPECT_TRUE((check_function<-3, 4, -5, 6>()));   /* (-3/4) / -5/6 = 18/20 */ \
    EXPECT_TRUE((check_function<7, -8, -9, 1>()));   /* (7/-8) / -9/1 = 7/72 */ \
    EXPECT_TRUE((check_function<-2, -3, -4, 5>()));  /* (-2/-3) / -4/5 = -10/12 */ \
    EXPECT_TRUE((check_function<6, 7, 8, -9>()));    /* (6/7) / 8/-9 = -54/56 */ \
    EXPECT_TRUE((check_function<-1, 2, 3, -4>()));   /* (-1/2) / 3/-4 = 4/6 */ \
    EXPECT_TRUE((check_function<5, -6, 7, -8>()));   /* (5/-6) / 7/-8 = 40/42 */ \
    EXPECT_TRUE((check_function<-9, -1, 2, -3>()));  /* (-9/-1) / 2/-3 = -27/2 */ \
    EXPECT_TRUE((check_function<4, 5, -6, -7>()));   /* (4/5) / -6/-7 = 28/30 */ \
    EXPECT_TRUE((check_function<-8, 9, -1, -2>()));  /* (-8/9) / -1/-2 = -16/9 */ \
    EXPECT_TRUE((check_function<3, -4, -5, -6>()));  /* (3/-4) / -5/-6 = -18/20 */ \
    EXPECT_TRUE((check_function<-7, -8, -9, -1>())); /* (-7/-8) / -9/-1 = 7/72 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Ratio test-checker functions                ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
::testing::AssertionResult AssertRuntimeIntegerAddition()
{
    using TestRatio    = shmit::math::StaticRatio<Numerator, Denominator>;
    using IntegerRatio = typename shmit::math::StaticRatio<Value, 1>::type;
    using CheckRatio   = std::ratio_add<typename TestRatio::type, IntegerRatio>; // Check result against STL

    shmit::math::Ratio test_ratio   = TestRatio::value;
    shmit::math::Ratio result_ratio = test_ratio + Value;

    result_ratio = shmit::math::Ratio::Simplify(result_ratio);

    if ((result_ratio.numerator != CheckRatio::num) || (result_ratio.denominator != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator << "/" << Denominator << " + " << Value << " = " << result_ratio.numerator << "/"
               << result_ratio.denominator << " Does not equal expected result: " << CheckRatio::num << "/"
               << CheckRatio::den;
    }

    constexpr intmax_t check_result_int = Value + (Numerator / Denominator);
    intmax_t           result_int       = Value + test_ratio;
    if (result_int != check_result_int)
    {
        return ::testing::AssertionFailure() << Value << " + " << Numerator << "/" << Denominator << " = " << result_int
                                             << " Does not equal expected result: " << check_result_int;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
::testing::AssertionResult AssertRuntimeRatioAddition()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;
    using CheckRatio = std::ratio_add<typename TestRatio1::type, typename TestRatio2::type>; // Check result against STL

    shmit::math::Ratio test_ratio_1 = TestRatio1::value;
    shmit::math::Ratio test_ratio_2 = TestRatio2::value;
    shmit::math::Ratio result_ratio = test_ratio_1 + test_ratio_2;

    result_ratio = shmit::math::Ratio::Simplify(result_ratio);

    if ((result_ratio.numerator != CheckRatio::num) || (result_ratio.denominator != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " + " << Numerator2 << "/" << Denominator2 << " = "
               << result_ratio.numerator << "/" << result_ratio.denominator
               << " Does not equal expected result: " << CheckRatio::num << "/" << CheckRatio::den;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
::testing::AssertionResult AssertRuntimeIntegerSubtraction()
{
    using TestRatio    = shmit::math::StaticRatio<Numerator, Denominator>;
    using IntegerRatio = typename shmit::math::StaticRatio<Value, 1>::type;
    using CheckRatio   = std::ratio_subtract<typename TestRatio::type, IntegerRatio>; // Check result against STL

    shmit::math::Ratio test_ratio   = TestRatio::value;
    shmit::math::Ratio result_ratio = test_ratio - Value;

    result_ratio = shmit::math::Ratio::Simplify(result_ratio);

    if ((result_ratio.numerator != CheckRatio::num) || (result_ratio.denominator != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator << "/" << Denominator << " - " << Value << " = " << result_ratio.numerator << "/"
               << result_ratio.denominator << " Does not equal expected result: " << CheckRatio::num << "/"
               << CheckRatio::den;
    }

    constexpr intmax_t check_result_int = Value - (Numerator / Denominator);
    intmax_t           result_int       = Value - test_ratio;
    if (result_int != check_result_int)
    {
        return ::testing::AssertionFailure() << Value << " - " << Numerator << "/" << Denominator << " = " << result_int
                                             << " Does not equal expected result: " << check_result_int;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
::testing::AssertionResult AssertRuntimeRatioSubtraction()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;
    using CheckRatio = std::ratio_subtract<typename TestRatio1::type, typename TestRatio2::type>; // Check result against STL

    shmit::math::Ratio test_ratio_1 = TestRatio1::value;
    shmit::math::Ratio test_ratio_2 = TestRatio2::value;
    shmit::math::Ratio result_ratio = test_ratio_1 - test_ratio_2;

    result_ratio = shmit::math::Ratio::Simplify(result_ratio);

    if ((result_ratio.numerator != CheckRatio::num) || (result_ratio.denominator != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " - " << Numerator2 << "/" << Denominator2 << " = "
               << result_ratio.numerator << "/" << result_ratio.denominator
               << " Does not equal expected result: " << CheckRatio::num << "/" << CheckRatio::den;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
::testing::AssertionResult AssertRuntimeIntegerMultiplication()
{
    using TestRatio    = shmit::math::StaticRatio<Numerator, Denominator>;
    using IntegerRatio = typename shmit::math::StaticRatio<Value, 1>::type;
    using CheckRatio   = std::ratio_multiply<typename TestRatio::type, IntegerRatio>; // Check result against STL

    shmit::math::Ratio test_ratio   = TestRatio::value;
    shmit::math::Ratio result_ratio = test_ratio * Value;

    result_ratio = shmit::math::Ratio::Simplify(result_ratio);

    if ((result_ratio.numerator != CheckRatio::num) || (result_ratio.denominator != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator << "/" << Denominator << " * " << Value << " = " << result_ratio.numerator << "/"
               << result_ratio.denominator << " Does not equal expected result: " << CheckRatio::num << "/"
               << CheckRatio::den;
    }

    constexpr intmax_t check_result_int = Value * (Numerator / Denominator);
    intmax_t           result_int       = Value * test_ratio;
    if (result_int != check_result_int)
    {
        return ::testing::AssertionFailure() << Value << " * " << Numerator << "/" << Denominator << " = " << result_int
                                             << " Does not equal expected result: " << check_result_int;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
::testing::AssertionResult AssertRuntimeRatioMultiplication()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;
    using CheckRatio = std::ratio_multiply<typename TestRatio1::type, typename TestRatio2::type>; // Check result against STL

    shmit::math::Ratio test_ratio_1 = TestRatio1::value;
    shmit::math::Ratio test_ratio_2 = TestRatio2::value;
    shmit::math::Ratio result_ratio = test_ratio_1 * test_ratio_2;

    result_ratio = shmit::math::Ratio::Simplify(result_ratio);

    if ((result_ratio.numerator != CheckRatio::num) || (result_ratio.denominator != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " * " << Numerator2 << "/" << Denominator2 << " = "
               << result_ratio.numerator << "/" << result_ratio.denominator
               << " Does not equal expected result: " << CheckRatio::num << "/" << CheckRatio::den;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
::testing::AssertionResult AssertRuntimeIntegerDivision()
{
    using TestRatio    = shmit::math::StaticRatio<Numerator, Denominator>;
    using IntegerRatio = typename shmit::math::StaticRatio<Value, 1>::type;
    using CheckRatio   = std::ratio_divide<typename TestRatio::type, IntegerRatio>; // Check result against STL

    shmit::math::Ratio test_ratio   = TestRatio::value;
    shmit::math::Ratio result_ratio = test_ratio / Value;

    result_ratio = shmit::math::Ratio::Simplify(result_ratio);

    if ((result_ratio.numerator != CheckRatio::num) || (result_ratio.denominator != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator << "/" << Denominator << " / " << Value << " = " << result_ratio.numerator << "/"
               << result_ratio.denominator << " Does not equal expected result: " << CheckRatio::num << "/"
               << CheckRatio::den;
    }

    // Special guard as we cannot divide by 0
    constexpr intmax_t check_quotient = Numerator / Denominator;
    if constexpr (check_quotient != 0)
    {
        constexpr intmax_t check_result_int = Value / check_quotient;
        intmax_t           result_int       = Value / test_ratio;
        if (result_int != check_result_int)
        {
            return ::testing::AssertionFailure()
                   << Value << " / (" << Numerator << "/" << Denominator << ") = " << result_int
                   << " Does not equal expected result: " << check_result_int;
        }
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
::testing::AssertionResult AssertRuntimeRatioDivision()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;
    using CheckRatio = std::ratio_divide<typename TestRatio1::type, typename TestRatio2::type>; // Check result against STL

    shmit::math::Ratio test_ratio_1 = TestRatio1::value;
    shmit::math::Ratio test_ratio_2 = TestRatio2::value;
    shmit::math::Ratio result_ratio = test_ratio_1 / test_ratio_2;

    result_ratio = shmit::math::Ratio::Simplify(result_ratio);

    if ((result_ratio.numerator != CheckRatio::num) || (result_ratio.denominator != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " / " << Numerator2 << "/" << Denominator2 << " = "
               << result_ratio.numerator << "/" << result_ratio.denominator
               << " Does not equal expected result: " << CheckRatio::num << "/" << CheckRatio::den;
    }

    return ::testing::AssertionSuccess();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  StaticRatio test-checker functions          ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<intmax_t Numerator, intmax_t Denominator>
void AssertStaticSimpleEquivalence()
{
    using TestRatio  = typename shmit::math::StaticRatio<Numerator, Denominator>::type;
    using CheckRatio = typename std::ratio<Numerator, Denominator>::type;

    EXPECT_EQ(TestRatio::num, CheckRatio::num);
    EXPECT_EQ(TestRatio::den, CheckRatio::den);
}

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
::testing::AssertionResult AssertStaticIntegerAddition()
{
    using TestRatio    = shmit::math::StaticRatio<Numerator, Denominator>;
    using IntegerRatio = typename shmit::math::StaticRatio<Value, 1>::type;
    using CheckRatio   = std::ratio_add<typename TestRatio::type, IntegerRatio>; // Check result against STL

    using ResultRatio = typename shmit::math::add<TestRatio, std::integral_constant<intmax_t, Value>>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator << "/" << Denominator << " + " << Value << " = " << ResultRatio::num << "/"
               << ResultRatio::den << " Does not equal expected result: " << CheckRatio::num << "/" << CheckRatio::den;
    }

    constexpr intmax_t check_result_int = Value + (Numerator / Denominator);
    constexpr intmax_t result_int       = shmit::math::add<std::integral_constant<intmax_t, Value>, TestRatio>::value;

    if (result_int != check_result_int)
    {
        return ::testing::AssertionFailure() << Value << " + " << Numerator << "/" << Denominator << " = " << result_int
                                             << " Does not equal expected result: " << check_result_int;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
::testing::AssertionResult AssertStaticRatioAddition()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;
    using CheckRatio = std::ratio_add<typename TestRatio1::type, typename TestRatio2::type>; // Check result against STL

    using ResultRatio = typename shmit::math::add<TestRatio1, TestRatio2>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " + " << Numerator2 << "/" << Denominator2 << " = "
               << ResultRatio::num << "/" << ResultRatio::den << " Does not equal expected result: " << CheckRatio::num
               << "/" << CheckRatio::den;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
::testing::AssertionResult AssertStaticIntegerSubtraction()
{
    using TestRatio    = shmit::math::StaticRatio<Numerator, Denominator>;
    using IntegerRatio = typename shmit::math::StaticRatio<Value, 1>::type;
    using CheckRatio   = std::ratio_subtract<typename TestRatio::type, IntegerRatio>; // Check result against STL

    using ResultRatio = typename shmit::math::subtract<TestRatio, std::integral_constant<intmax_t, Value>>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator << "/" << Denominator << " - " << Value << " = " << ResultRatio::num << "/"
               << ResultRatio::den << " Does not equal expected result: " << CheckRatio::num << "/" << CheckRatio::den;
    }

    constexpr intmax_t check_result_int = Value - (Numerator / Denominator);
    constexpr intmax_t result_int = shmit::math::subtract<std::integral_constant<intmax_t, Value>, TestRatio>::value;

    if (result_int != check_result_int)
    {
        return ::testing::AssertionFailure() << Value << " - " << Numerator << "/" << Denominator << " = " << result_int
                                             << " Does not equal expected result: " << check_result_int;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
::testing::AssertionResult AssertStaticRatioSubtraction()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;
    using CheckRatio = std::ratio_subtract<typename TestRatio1::type, typename TestRatio2::type>; // Check result against STL

    using ResultRatio = typename shmit::math::subtract<TestRatio1, TestRatio2>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " + " << Numerator2 << "/" << Denominator2 << " = "
               << ResultRatio::num << "/" << ResultRatio::den << " Does not equal expected result: " << CheckRatio::num
               << "/" << CheckRatio::den;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
::testing::AssertionResult AssertStaticIntegerMultiplication()
{
    using TestRatio    = shmit::math::StaticRatio<Numerator, Denominator>;
    using IntegerRatio = typename shmit::math::StaticRatio<Value, 1>::type;
    using CheckRatio   = std::ratio_multiply<typename TestRatio::type, IntegerRatio>; // Check result against STL

    using ResultRatio = typename shmit::math::multiply<TestRatio, std::integral_constant<intmax_t, Value>>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator << "/" << Denominator << " * " << Value << " = " << ResultRatio::num << "/"
               << ResultRatio::den << " Does not equal expected result: " << CheckRatio::num << "/" << CheckRatio::den;
    }

    constexpr intmax_t check_result_int = Value * (Numerator / Denominator);
    constexpr intmax_t result_int = shmit::math::multiply<std::integral_constant<intmax_t, Value>, TestRatio>::value;

    if (result_int != check_result_int)
    {
        return ::testing::AssertionFailure() << Value << " + " << Numerator << "/" << Denominator << " = " << result_int
                                             << " Does not equal expected result: " << check_result_int;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
::testing::AssertionResult AssertStaticRatioMultiplication()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;
    using CheckRatio = std::ratio_multiply<typename TestRatio1::type, typename TestRatio2::type>; // Check result against STL

    using ResultRatio = typename shmit::math::multiply<TestRatio1, TestRatio2>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " + " << Numerator2 << "/" << Denominator2 << " = "
               << ResultRatio::num << "/" << ResultRatio::den << " Does not equal expected result: " << CheckRatio::num
               << "/" << CheckRatio::den;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value>
::testing::AssertionResult AssertStaticIntegerDivision()
{
    using TestRatio    = shmit::math::StaticRatio<Numerator, Denominator>;
    using IntegerRatio = typename shmit::math::StaticRatio<Value, 1>::type;
    using CheckRatio   = std::ratio_divide<typename TestRatio::type, IntegerRatio>; // Check result against STL

    using ResultRatio = typename shmit::math::divide<TestRatio, std::integral_constant<intmax_t, Value>>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << "(" << Numerator << "/" << Denominator << ") / " << Value << " = " << ResultRatio::num << "/"
               << ResultRatio::den << " Does not equal expected result: " << CheckRatio::num << "/" << CheckRatio::den;
    }

    constexpr intmax_t check_quotient = (Numerator / Denominator);
    if constexpr (check_quotient != 0)
    {
        constexpr intmax_t check_result_int = Value / check_quotient;
        constexpr intmax_t result_int = shmit::math::divide<std::integral_constant<intmax_t, Value>, TestRatio>::value;

        if (result_int != check_result_int)
        {
            return ::testing::AssertionFailure()
                   << Value << " / (" << Numerator << "/" << Denominator << ") = " << result_int
                   << " Does not equal expected result: " << check_result_int;
        }
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2>
::testing::AssertionResult AssertStaticRatioDivision()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;
    using CheckRatio = std::ratio_divide<typename TestRatio1::type, typename TestRatio2::type>; // Check result against STL

    using ResultRatio = typename shmit::math::divide<TestRatio1, TestRatio2>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " + " << Numerator2 << "/" << Denominator2 << " = "
               << ResultRatio::num << "/" << ResultRatio::den << " Does not equal expected result: " << CheckRatio::num
               << "/" << CheckRatio::den;
    }

    return ::testing::AssertionSuccess();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Ratio tests                 ////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(Runtime, Infinity_Calculation)
{
    // Force infinity value calculation by dividing 1 by 0
    shmit::math::Ratio test_ratio = shmit::math::create_ratio<0, 1>();
    shmit::math::Ratio inverted   = shmit::math::Ratio::Invert(test_ratio);

    double ratio_cast = static_cast<double>(inverted);
    ASSERT_TRUE(std::isinf(ratio_cast));
}

TEST(Runtime, Not_A_Number_Calculation)
{
    // Force NaN calculation by dividing 0 by 0
    shmit::math::Ratio test_ratio  = shmit::math::create_ratio<0, 1>();
    shmit::math::Ratio invalidated = test_ratio / intmax_t {0};

    double ratio_cast = static_cast<double>(invalidated);
    ASSERT_TRUE(std::isnan(ratio_cast));
}

TEST(Runtime, Integer_Addition)
{
    CHECK_INTEGER_ADDITION(AssertRuntimeIntegerAddition);
}

TEST(Runtime, Ratio_Addition)
{
    CHECK_RATIO_ADDITION(AssertRuntimeRatioAddition);
}

TEST(Runtime, Integer_Subtraction)
{
    CHECK_INTEGER_SUBTRACTION(AssertRuntimeIntegerSubtraction);
}

TEST(Runtime, Ratio_Subtraction)
{
    CHECK_RATIO_SUBTRACTION(AssertRuntimeRatioSubtraction);
}

TEST(Runtime, Integer_Multiplication)
{
    CHECK_INTEGER_MULTIPLICATION(AssertRuntimeIntegerMultiplication);
}

TEST(Runtime, Ratio_Multiplication)
{
    CHECK_RATIO_MULTIPLICATION(AssertRuntimeRatioMultiplication);
}

TEST(Runtime, Integer_Division)
{
    CHECK_INTEGER_DIVISION(AssertRuntimeIntegerDivision);
}

TEST(Runtime, Ratio_Division)
{
    CHECK_RATIO_DIVISION(AssertRuntimeRatioDivision);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  StaticRatio tests           ////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(Static, Simplify)
{
    AssertStaticSimpleEquivalence<1, 1>();
    AssertStaticSimpleEquivalence<2, 4>();
    AssertStaticSimpleEquivalence<3, 9>();
    AssertStaticSimpleEquivalence<-4, 16>();
    AssertStaticSimpleEquivalence<5, -25>();
    AssertStaticSimpleEquivalence<-6, -36>();
}

TEST(Static, Integer_Addition)
{
    CHECK_INTEGER_ADDITION(AssertStaticIntegerAddition);
}

TEST(Static, Ratio_Addition)
{
    CHECK_RATIO_ADDITION(AssertStaticRatioAddition);
}

TEST(Static, Integer_Subtraction)
{
    CHECK_INTEGER_SUBTRACTION(AssertStaticIntegerSubtraction);
}

TEST(Static, Ratio_Subtraction)
{
    CHECK_RATIO_SUBTRACTION(AssertStaticRatioSubtraction);
}

TEST(Static, Integer_Multiplication)
{
    CHECK_INTEGER_MULTIPLICATION(AssertStaticIntegerMultiplication);
}

TEST(Static, Ratio_Multiplication)
{
    CHECK_RATIO_MULTIPLICATION(AssertStaticRatioMultiplication);
}

TEST(Static, Integer_Division)
{
    CHECK_INTEGER_DIVISION(AssertStaticIntegerDivision);
}

TEST(Static, Ratio_Division)
{
    CHECK_RATIO_DIVISION(AssertStaticRatioDivision);
}
