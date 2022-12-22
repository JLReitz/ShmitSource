#include <ShmitCore/Math/Ratio.hpp>

#include <gtest/gtest.h>

#define CHECK_INTEGER_ADDITION(check_function) \
    EXPECT_TRUE((check_function<-1, 1, 1, 0, 1>())); /* -1/1 + 1 = 0/1 */ \
    EXPECT_TRUE((check_function<2, 2, -1, 0, 2>())); /* 2/2 + (-1) = 0/2 */ \
\
    EXPECT_TRUE((check_function<3, 6, 0, 3, 6>())); /* 3/6 + 0 = 3/6 */ \
\
    EXPECT_TRUE((check_function<1, 2, 3, 7, 2>()));       /* 1/2 + 3 = 7/2 */ \
    EXPECT_TRUE((check_function<-4, 5, 6, 26, 5>()));     /* -4/5 + 6 = 26/5 */ \
    EXPECT_TRUE((check_function<7, -8, 9, 65, 8>()));     /* 7/-8 + 9 = 65/8 */ \
    EXPECT_TRUE((check_function<-10, -1, 2, 12, 1>()));   /* -10/-1 + 2 = 12/1 */ \
    EXPECT_TRUE((check_function<3, 4, -5, -17, 4>()));    /* 3/4 + -5 = -17/4 */ \
    EXPECT_TRUE((check_function<-6, 7, -8, -62, 7>()));   /* -6/7 + -8 = -62/7 */ \
    EXPECT_TRUE((check_function<9, -10, -1, -19, 10>())); /* 9/-10 + -1 = -19/10 */ \
    EXPECT_TRUE((check_function<-2, -3, -4, -10, 3>()));  /* -2/-3 + -4 = -10/3 */ \
    EXPECT_TRUE((check_function<5, 6, 7, 47, 6>()));      /* 5/6 + 7 = 47/6 */ \
    EXPECT_TRUE((check_function<-8, 9, 10, 82, 9>()));    /* -8/9 + 10 = 82/9 */ \
    EXPECT_TRUE((check_function<1, -2, 3, 5, 2>()));      /* 1/-2 + 3 = 5/2 */ \
    EXPECT_TRUE((check_function<-4, -5, 6, 34, 5>()));    /* -4/-5 + 6 = 34/5 */ \
    EXPECT_TRUE((check_function<7, 8, -9, -65, 8>()));    /* 7/8 + -9 = -65/8 */ \
    EXPECT_TRUE((check_function<-10, 1, -2, -12, 1>()));  /* -10/1 + -2 = -12/1 */ \
    EXPECT_TRUE((check_function<3, -4, -5, -23, 4>()));   /* 3/-4 + -5 = -23/4 */ \
    EXPECT_TRUE((check_function<-6, -7, -8, -50, 7>()));  /* -6/-7 + -8 = -50/7 */

#define CHECK_RATIO_ADDITION(check_function) \
    EXPECT_TRUE((check_function<1, 2, 3, 4, 10, 8>()));     /* 1/2 + 3/4 = 10/8 */ \
    EXPECT_TRUE((check_function<-5, 6, 7, 8, 2, 48>()));    /* -5/6 + 7/8 = 2/48 */ \
    EXPECT_TRUE((check_function<9, -1, 2, 3, -25, 3>()));   /* 9/-1 + 2/3 = -25/3 */ \
    EXPECT_TRUE((check_function<-4, -5, 6, 7, 58, 35>()));  /* -4/-5 + 6/7 = 58/35 */ \
    EXPECT_TRUE((check_function<8, 9, -1, 2, 7, 18>()));    /* 8/9 + -1/2 = 7/18 */ \
    EXPECT_TRUE((check_function<-3, 4, -5, 6, -38, 24>())); /* -3/4 + -5/6 = -38/24 */ \
    EXPECT_TRUE((check_function<7, -8, -9, 1, -79, 8>()));  /* 7/-8 + -9/1 = -79/8 */ \
    EXPECT_TRUE((check_function<-2, -3, -4, 5, -2, 15>())); /* -2/-3 + -4/5 = -2/15 */ \
    EXPECT_TRUE((check_function<6, 7, 8, -9, -2, 63>()));   /* 6/7 + 8/-9 = -2/63 */ \
    EXPECT_TRUE((check_function<-1, 2, 3, -4, -10, 8>()));  /* -1/2 + 3/-4 = -10/8 */ \
    EXPECT_TRUE((check_function<5, -6, 7, -8, -82, 48>())); /* 5/-6 + 7/-8 = -82/48 */ \
    EXPECT_TRUE((check_function<-9, -1, 2, -3, 25, 3>()));  /* -9/-1 + 2/-3 = 25/3 */ \
    EXPECT_TRUE((check_function<4, 5, -6, -7, 58, 35>()));  /* 4/5 + -6/-7 = 58/35 */ \
    EXPECT_TRUE((check_function<-8, 9, -1, -2, -7, 18>())); /* -8/9 + -1/-2 = -7/18 */ \
    EXPECT_TRUE((check_function<3, -4, -5, -6, 2, 24>()));  /* 3/-4 + -5/-6 = 2/24 */ \
    EXPECT_TRUE((check_function<-7, -8, -9, -1, 79, 8>())); /* -7/-8 + -9/-1 = 79/8 */

#define CHECK_INTEGER_SUBTRACTION(check_function) \
    EXPECT_TRUE((check_function<1, 1, 1, 0, 1>()));   /* 1/1 - 1 = 0/1 */ \
    EXPECT_TRUE((check_function<-2, 2, -1, 0, 2>())); /* -2/2 - (-1) = 0/2 */ \
\
    EXPECT_TRUE((check_function<3, 6, 0, 3, 6>())); /* 3/6 - 0 = 3/6 */ \
\
    EXPECT_TRUE((check_function<1, 2, 3, -5, 2>()));    /* 1/2 - 3 = -5/2 */ \
    EXPECT_TRUE((check_function<-4, 5, 6, -34, 5>()));  /* -4/5 - 6 = -34/5 */ \
    EXPECT_TRUE((check_function<7, -8, 9, -79, 8>()));  /* 7/-8 - 9 = -79/8 */ \
    EXPECT_TRUE((check_function<-10, -1, 2, 8, 1>()));  /* -10/-1 - 2 = 8/1 */ \
    EXPECT_TRUE((check_function<3, 4, -5, 23, 4>()));   /* 3/4 - -5 = 23/4 */ \
    EXPECT_TRUE((check_function<-6, 7, -8, 50, 7>()));  /* -6/7 - -8 = 50/7 */ \
    EXPECT_TRUE((check_function<9, -10, -1, 1, 10>())); /* 9/-10 - -1 = 1/10 */ \
    EXPECT_TRUE((check_function<-2, -3, -4, 14, 3>())); /* -2/-3 - -4 = 14/3 */ \
    EXPECT_TRUE((check_function<5, 6, 7, -37, 6>()));   /* 5/6 - 7 = -37/6 */ \
    EXPECT_TRUE((check_function<-8, 9, 10, -98, 9>())); /* -8/9 - 10 = -98/9 */ \
    EXPECT_TRUE((check_function<1, -2, 3, -7, 2>()));   /* 1/-2 - 3 = -7/2 */ \
    EXPECT_TRUE((check_function<-4, -5, 6, -26, 5>())); /* -4/-5 - 6 = -26/5 */ \
    EXPECT_TRUE((check_function<7, 8, -9, 79, 8>()));   /* 7/8 - -9 = 79/8 */ \
    EXPECT_TRUE((check_function<-10, 1, -2, -8, 1>())); /* -10/1 - -2 = -8/1 */ \
    EXPECT_TRUE((check_function<3, -4, -5, 17, 4>()));  /* 3/-4 - -5 = 17/4 */ \
    EXPECT_TRUE((check_function<-6, -7, -8, 62, 7>())); /* -6/-7 - -8 = 62/7 */

#define CHECK_RATIO_SUBTRACTION(check_function) \
    EXPECT_TRUE((check_function<1, 2, 3, 4, -2, 8>()));      /* 1/2 - 3/4 = -2/8 */ \
    EXPECT_TRUE((check_function<-5, 6, 7, 8, -82, 48>()));   /* -5/6 - 7/8 = -82/48 */ \
    EXPECT_TRUE((check_function<9, -1, 2, 3, -29, 3>()));    /* 9/-1 - 2/3 = -29/3 */ \
    EXPECT_TRUE((check_function<-4, -5, 6, 7, -2, 35>()));   /* -4/-5 - 6/7 = -2/35 */ \
    EXPECT_TRUE((check_function<8, 9, -1, 2, 25, 18>()));    /* 8/9 - -1/2 = 25/18 */ \
    EXPECT_TRUE((check_function<-3, 4, -5, 6, 2, 24>()));    /* -3/4 - -5/6 = 2/24 */ \
    EXPECT_TRUE((check_function<7, -8, -9, 1, 65, 8>()));    /* 7/-8 - -9/1 = 65/8 */ \
    EXPECT_TRUE((check_function<-2, -3, -4, 5, 22, 15>()));  /* -2/-3 - -4/5 = 22/15 */ \
    EXPECT_TRUE((check_function<6, 7, 8, -9, 110, 63>()));   /* 6/7 - 8/-9 = 110/63 */ \
    EXPECT_TRUE((check_function<-1, 2, 3, -4, 2, 8>()));     /* -1/2 - 3/-4 = 2/8 */ \
    EXPECT_TRUE((check_function<5, -6, 7, -8, 2, 48>()));    /* 5/-6 - 7/-8 = 2/48 */ \
    EXPECT_TRUE((check_function<-9, -1, 2, -3, 29, 3>()));   /* -9/-1 - 2/-3 = 29/3 */ \
    EXPECT_TRUE((check_function<4, 5, -6, -7, -2, 35>()));   /* 4/5 - -6/-7 = -2/35 */ \
    EXPECT_TRUE((check_function<-8, 9, -1, -2, -25, 18>())); /* -8/9 - -1/-2 = -25/18 */ \
    EXPECT_TRUE((check_function<3, -4, -5, -6, -38, 24>())); /* 3/-4 - -5/-6 = -38/24 */ \
    EXPECT_TRUE((check_function<-7, -8, -9, -1, -65, 8>())); /* -7/-8 - -9/-1 = -65/8 */

#define CHECK_INTEGER_MULTIPLICATION(check_function) \
    EXPECT_TRUE((check_function<0, 1, 5, 0, 1>())); /* 0/1 * 5 = 0/1 */ \
    EXPECT_TRUE((check_function<2, 8, 0, 0, 8>())); /* 2/8 * 0 = 0/8 */ \
\
    EXPECT_TRUE((check_function<1, 1, 1, 1, 1>())); /* 1/1 * 1 = 1/1 */ \
\
    EXPECT_TRUE((check_function<1, 2, 3, 3, 2>()));      /* 1/2 * 3 = 3/2 */ \
    EXPECT_TRUE((check_function<-4, 5, 6, -24, 5>()));   /* -4/5 * 6 = -24/5 */ \
    EXPECT_TRUE((check_function<7, -8, 9, -63, 8>()));   /* 7/-8 * 9 = -63/8 */ \
    EXPECT_TRUE((check_function<-10, -1, 2, 20, 1>()));  /* -10/-1 * 2 = 20/1 */ \
    EXPECT_TRUE((check_function<3, 4, -5, -15, 4>()));   /* 3/4 * -5 = -15/4 */ \
    EXPECT_TRUE((check_function<-6, 7, -8, 48, 7>()));   /* -6/7 * -8 = 48/7 */ \
    EXPECT_TRUE((check_function<9, -10, -1, 9, 10>()));  /* 9/-10 * -1 = 9/10 */ \
    EXPECT_TRUE((check_function<-2, -3, -4, -8, 3>()));  /* -2/-3 * -4 = -8/3 */ \
    EXPECT_TRUE((check_function<5, 6, 7, 35, 6>()));     /* 5/6 * 7 = 35/6 */ \
    EXPECT_TRUE((check_function<-8, 9, 10, -80, 9>()));  /* -8/9 * 10 = -80/9 */ \
    EXPECT_TRUE((check_function<1, -2, 3, -3, 2>()));    /* 1/-2 * 3 = -3/2 */ \
    EXPECT_TRUE((check_function<-4, -5, 6, 24, 5>()));   /* -4/-5 * 6 = 24/5 */ \
    EXPECT_TRUE((check_function<7, 8, -9, -63, 8>()));   /* 7/8 * -9 = -63/8 */ \
    EXPECT_TRUE((check_function<-10, 1, -2, 20, 1>()));  /* -10/1 * -2 = 20/1 */ \
    EXPECT_TRUE((check_function<3, -4, -5, 15, 4>()));   /* 3/-4 * -5 = 15/4 */ \
    EXPECT_TRUE((check_function<-6, -7, -8, -48, 7>())); /* -6/-7 * -8 = -48/7 */

#define CHECK_RATIO_MULTIPLICATION(check_function) \
    EXPECT_TRUE((check_function<1, 2, 3, 4, 3, 8>()));       /* 1/2 * 3/4 = 3/8 */ \
    EXPECT_TRUE((check_function<-5, 6, 7, 8, -35, 48>()));   /* -5/6 * 7/8 = -35/48 */ \
    EXPECT_TRUE((check_function<9, -1, 2, 3, -18, 3>()));    /* 9/-1 * 2/3 = -18/3 */ \
    EXPECT_TRUE((check_function<-4, -5, 6, 7, 24, 35>()));   /* -4/-5 * 6/7 = 24/35 */ \
    EXPECT_TRUE((check_function<8, 9, -1, 2, -8, 18>()));    /* 8/9 * -1/2 = -8/18 */ \
    EXPECT_TRUE((check_function<-3, 4, -5, 6, 15, 24>()));   /* -3/4 * -5/6 = 15/24 */ \
    EXPECT_TRUE((check_function<7, -8, -9, 1, 63, 8>()));    /* 7/-8 * -9/1 = 63/8 */ \
    EXPECT_TRUE((check_function<-2, -3, -4, 5, -8, 15>()));  /* -2/-3 * -4/5 = -8/15 */ \
    EXPECT_TRUE((check_function<6, 7, 8, -9, -48, 63>()));   /* 6/7 * 8/-9 = -48/63 */ \
    EXPECT_TRUE((check_function<-1, 2, 3, -4, 3, 8>()));     /* -1/2 * 3/-4 = 3/8 */ \
    EXPECT_TRUE((check_function<5, -6, 7, -8, 35, 48>()));   /* 5/-6 * 7/-8 = 35/48 */ \
    EXPECT_TRUE((check_function<-9, -1, 2, -3, -18, 3>()));  /* -9/-1 * 2/-3 = -18/3 */ \
    EXPECT_TRUE((check_function<4, 5, -6, -7, 24, 35>()));   /* 4/5 * -6/-7 = 24/35 */ \
    EXPECT_TRUE((check_function<-8, 9, -1, -2, -8, 18>()));  /* -8/9 * -1/-2 = -8/18 */ \
    EXPECT_TRUE((check_function<3, -4, -5, -6, -15, 24>())); /* 3/-4 * -5/-6 = -15/24 */ \
    EXPECT_TRUE((check_function<-7, -8, -9, -1, 63, 8>()));  /* -7/-8 * -9/-1 = 63/8 */

#define CHECK_INTEGER_DIVISION(check_function) \
    EXPECT_TRUE((check_function<0, 1, 2, 0, 2>())); /* (0/1) / 2 = 0/2 */ \
\
    EXPECT_TRUE((check_function<1, 1, 1, 1, 1>())); /* (1/1) / 1 = 1/1 */ \
\
    EXPECT_TRUE((check_function<1, 2, 3, 1, 6>()));      /* (1/2) / 3 = 1/6 */ \
    EXPECT_TRUE((check_function<-4, 5, 6, -4, 30>()));   /* (-4/5) / 6 = -4/30 */ \
    EXPECT_TRUE((check_function<7, -8, 9, -7, 72>()));   /* (7/-8) / 9 = -7/72 */ \
    EXPECT_TRUE((check_function<-10, -1, 2, 10, 2>()));  /* (-10/-1) / 2 = 10/2 */ \
    EXPECT_TRUE((check_function<3, 4, -5, -3, 20>()));   /* (3/4) / -5 = -3/20 */ \
    EXPECT_TRUE((check_function<-6, 7, -8, 6, 56>()));   /* (-6/7) / -8 = 6/56 */ \
    EXPECT_TRUE((check_function<9, -10, -1, 9, 10>()));  /* (9/-10) / -1 = 9/10 */ \
    EXPECT_TRUE((check_function<-2, -3, -4, -2, 12>())); /* (-2/-3) / -4 = -2/12 */ \
    EXPECT_TRUE((check_function<5, 6, 7, 5, 42>()));     /* (5/6) / 7 = 5/42 */ \
    EXPECT_TRUE((check_function<-8, 9, 10, -8, 90>()));  /* (-8/9) / 10 = -8/90 */ \
    EXPECT_TRUE((check_function<1, -2, 3, -1, 6>()));    /* (1/-2) / 3 = -1/6 */ \
    EXPECT_TRUE((check_function<-4, -5, 6, 4, 30>()));   /* (-4/-5) / 6 = 4/30 */ \
    EXPECT_TRUE((check_function<7, 8, -9, -7, 72>()));   /* (7/8) / -9 = -7/72 */ \
    EXPECT_TRUE((check_function<-10, 1, -2, 10, 2>()));  /* (-10/1) / -2 = 10/2 */ \
    EXPECT_TRUE((check_function<3, -4, -5, 3, 20>()));   /* (3/-4) / -5 = 3/20 */ \
    EXPECT_TRUE((check_function<-6, -7, -8, -6, 56>())); /* (-6/-7) / -8 = -6/56 */

#define CHECK_RATIO_DIVISION(check_function) \
    EXPECT_TRUE((check_function<1, 2, 3, 4, 4, 6>()));       /* (1/2) / 3/4 = 4/6 */ \
    EXPECT_TRUE((check_function<-5, 6, 7, 8, -40, 42>()));   /* (-5/6) / 7/8 = -40/42 */ \
    EXPECT_TRUE((check_function<9, -1, 2, 3, -27, 2>()));    /* (9/-1) / 2/3 = -27/2 */ \
    EXPECT_TRUE((check_function<-4, -5, 6, 7, 28, 30>()));   /* (-4/-5) / 6/7 = 28/30 */ \
    EXPECT_TRUE((check_function<8, 9, -1, 2, -16, 9>()));    /* (8/9) / -1/2 = -16/9 */ \
    EXPECT_TRUE((check_function<-3, 4, -5, 6, 18, 20>()));   /* (-3/4) / -5/6 = 18/20 */ \
    EXPECT_TRUE((check_function<7, -8, -9, 1, 7, 72>()));    /* (7/-8) / -9/1 = 7/72 */ \
    EXPECT_TRUE((check_function<-2, -3, -4, 5, -10, 12>())); /* (-2/-3) / -4/5 = -10/12 */ \
    EXPECT_TRUE((check_function<6, 7, 8, -9, -54, 56>()));   /* (6/7) / 8/-9 = -54/56 */ \
    EXPECT_TRUE((check_function<-1, 2, 3, -4, 4, 6>()));     /* (-1/2) / 3/-4 = 4/6 */ \
    EXPECT_TRUE((check_function<5, -6, 7, -8, 40, 42>()));   /* (5/-6) / 7/-8 = 40/42 */ \
    EXPECT_TRUE((check_function<-9, -1, 2, -3, -27, 2>()));  /* (-9/-1) / 2/-3 = -27/2 */ \
    EXPECT_TRUE((check_function<4, 5, -6, -7, 28, 30>()));   /* (4/5) / -6/-7 = 28/30 */ \
    EXPECT_TRUE((check_function<-8, 9, -1, -2, -16, 9>()));  /* (-8/9) / -1/-2 = -16/9 */ \
    EXPECT_TRUE((check_function<3, -4, -5, -6, -18, 20>())); /* (3/-4) / -5/-6 = -18/20 */ \
    EXPECT_TRUE((check_function<-7, -8, -9, -1, 7, 72>()));  /* (-7/-8) / -9/-1 = 7/72 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Ratio test-checker check_functions          ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value, intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertRuntimeIntegerAddition()
{
    using TestRatio = shmit::math::StaticRatio<Numerator, Denominator>;

    shmit::math::Ratio test_ratio   = TestRatio::Value();
    shmit::math::Ratio result_ratio = test_ratio + Value;

    if ((result_ratio.num != NumeratorCheck) || (result_ratio.den != DenominatorCheck))
    {
        return ::testing::AssertionFailure()
               << Numerator << "/" << Denominator << " + " << Value << " = " << result_ratio.num << "/"
               << result_ratio.den << " Does not equal expected result: " << NumeratorCheck << "/" << DenominatorCheck;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2,
         intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertRuntimeRatioAddition()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;

    shmit::math::Ratio test_ratio_1 = TestRatio1::Value();
    shmit::math::Ratio test_ratio_2 = TestRatio2::Value();
    shmit::math::Ratio result_ratio = test_ratio_1 + test_ratio_2;

    if ((result_ratio.num != NumeratorCheck) || (result_ratio.den != DenominatorCheck))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " + " << Numerator2 << "/" << Denominator2 << " = "
               << result_ratio.num << "/" << result_ratio.den << " Does not equal expected result: " << NumeratorCheck
               << "/" << DenominatorCheck;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value, intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertRuntimeIntegerSubtraction()
{
    using TestRatio = shmit::math::StaticRatio<Numerator, Denominator>;

    shmit::math::Ratio test_ratio   = TestRatio::Value();
    shmit::math::Ratio result_ratio = test_ratio - Value;

    if ((result_ratio.num != NumeratorCheck) || (result_ratio.den != DenominatorCheck))
    {
        return ::testing::AssertionFailure()
               << Numerator << "/" << Denominator << " - " << Value << " = " << result_ratio.num << "/"
               << result_ratio.den << " Does not equal expected result: " << NumeratorCheck << "/" << DenominatorCheck;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2,
         intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertRuntimeRatioSubtraction()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;

    shmit::math::Ratio test_ratio_1 = TestRatio1::Value();
    shmit::math::Ratio test_ratio_2 = TestRatio2::Value();
    shmit::math::Ratio result_ratio = test_ratio_1 - test_ratio_2;

    if ((result_ratio.num != NumeratorCheck) || (result_ratio.den != DenominatorCheck))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " - " << Numerator2 << "/" << Denominator2 << " = "
               << result_ratio.num << "/" << result_ratio.den << " Does not equal expected result: " << NumeratorCheck
               << "/" << DenominatorCheck;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value, intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertRuntimeIntegerMultiplication()
{
    using TestRatio = shmit::math::StaticRatio<Numerator, Denominator>;

    shmit::math::Ratio test_ratio   = TestRatio::Value();
    shmit::math::Ratio result_ratio = test_ratio * Value;

    if ((result_ratio.num != NumeratorCheck) || (result_ratio.den != DenominatorCheck))
    {
        return ::testing::AssertionFailure()
               << Numerator << "/" << Denominator << " * " << Value << " = " << result_ratio.num << "/"
               << result_ratio.den << " Does not equal expected result: " << NumeratorCheck << "/" << DenominatorCheck;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2,
         intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertRuntimeRatioMultiplication()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;

    shmit::math::Ratio test_ratio_1 = TestRatio1::Value();
    shmit::math::Ratio test_ratio_2 = TestRatio2::Value();
    shmit::math::Ratio result_ratio = test_ratio_1 * test_ratio_2;

    if ((result_ratio.num != NumeratorCheck) || (result_ratio.den != DenominatorCheck))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " * " << Numerator2 << "/" << Denominator2 << " = "
               << result_ratio.num << "/" << result_ratio.den << " Does not equal expected result: " << NumeratorCheck
               << "/" << DenominatorCheck;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value, intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertRuntimeIntegerDivision()
{
    using TestRatio = shmit::math::StaticRatio<Numerator, Denominator>;

    shmit::math::Ratio test_ratio   = TestRatio::Value();
    shmit::math::Ratio result_ratio = test_ratio / Value;

    if ((result_ratio.num != NumeratorCheck) || (result_ratio.den != DenominatorCheck))
    {
        return ::testing::AssertionFailure()
               << Numerator << "/" << Denominator << " / " << Value << " = " << result_ratio.num << "/"
               << result_ratio.den << " Does not equal expected result: " << NumeratorCheck << "/" << DenominatorCheck;
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2,
         intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertRuntimeRatioDivision()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;

    shmit::math::Ratio test_ratio_1 = TestRatio1::Value();
    shmit::math::Ratio test_ratio_2 = TestRatio2::Value();
    shmit::math::Ratio result_ratio = test_ratio_1 / test_ratio_2;

    if ((result_ratio.num != NumeratorCheck) || (result_ratio.den != DenominatorCheck))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " / " << Numerator2 << "/" << Denominator2 << " = "
               << result_ratio.num << "/" << result_ratio.den << " Does not equal expected result: " << NumeratorCheck
               << "/" << DenominatorCheck;
    }

    return ::testing::AssertionSuccess();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  StaticRatio test-checker check_functions    ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<intmax_t Numerator, intmax_t Denominator>
void AssertStaticSimpleEquivalence()
{
    using TestRatio  = typename shmit::math::StaticRatio<Numerator, Denominator>::type;
    using CheckRatio = typename std::ratio<Numerator, Denominator>::type;

    EXPECT_EQ(TestRatio::num, CheckRatio::num);
    EXPECT_EQ(TestRatio::den, CheckRatio::den);
}

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value, intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertStaticIntegerAddition()
{
    using TestRatio = shmit::math::StaticRatio<Numerator, Denominator>;

    using ResultRatio = typename shmit::math::Add<TestRatio, std::integral_constant<intmax_t, Value>>::Result::type;
    using CheckRatio  = typename std::ratio<NumeratorCheck, DenominatorCheck>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator << "/" << Denominator << " + " << Value << " = " << ResultRatio::num << "/"
               << ResultRatio::den << " Does not equal expected result: " << NumeratorCheck << "/" << DenominatorCheck
               << " (" << CheckRatio::num << "/" << CheckRatio::den << ")";
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2,
         intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertStaticRatioAddition()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;

    using ResultRatio = typename shmit::math::Add<TestRatio1, TestRatio2>::Result::type;
    using CheckRatio  = typename std::ratio<NumeratorCheck, DenominatorCheck>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " + " << Numerator2 << "/" << Denominator2 << " = "
               << ResultRatio::num << "/" << ResultRatio::den << " Does not equal expected result: " << NumeratorCheck
               << "/" << DenominatorCheck << " (" << CheckRatio::num << "/" << CheckRatio::den << ")";
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value, intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertStaticIntegerSubtraction()
{
    using TestRatio = shmit::math::StaticRatio<Numerator, Denominator>;

    using ResultRatio =
        typename shmit::math::Subtract<TestRatio, std::integral_constant<intmax_t, Value>>::Result::type;
    using CheckRatio = typename std::ratio<NumeratorCheck, DenominatorCheck>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator << "/" << Denominator << " - " << Value << " = " << ResultRatio::num << "/"
               << ResultRatio::den << " Does not equal expected result: " << NumeratorCheck << "/" << DenominatorCheck
               << " (" << CheckRatio::num << "/" << CheckRatio::den << ")";
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2,
         intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertStaticRatioSubtraction()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;

    using ResultRatio = typename shmit::math::Subtract<TestRatio1, TestRatio2>::Result::type;
    using CheckRatio  = typename std::ratio<NumeratorCheck, DenominatorCheck>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " + " << Numerator2 << "/" << Denominator2 << " = "
               << ResultRatio::num << "/" << ResultRatio::den << " Does not equal expected result: " << NumeratorCheck
               << "/" << DenominatorCheck << " (" << CheckRatio::num << "/" << CheckRatio::den << ")";
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value, intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertStaticIntegerMultiplication()
{
    using TestRatio = shmit::math::StaticRatio<Numerator, Denominator>;

    using ResultRatio =
        typename shmit::math::Multiply<TestRatio, std::integral_constant<intmax_t, Value>>::Result::type;
    using CheckRatio = typename std::ratio<NumeratorCheck, DenominatorCheck>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator << "/" << Denominator << " * " << Value << " = " << ResultRatio::num << "/"
               << ResultRatio::den << " Does not equal expected result: " << NumeratorCheck << "/" << DenominatorCheck
               << " (" << CheckRatio::num << "/" << CheckRatio::den << ")";
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2,
         intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertStaticRatioMultiplication()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;

    using ResultRatio = typename shmit::math::Multiply<TestRatio1, TestRatio2>::Result::type;
    using CheckRatio  = typename std::ratio<NumeratorCheck, DenominatorCheck>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " + " << Numerator2 << "/" << Denominator2 << " = "
               << ResultRatio::num << "/" << ResultRatio::den << " Does not equal expected result: " << NumeratorCheck
               << "/" << DenominatorCheck << " (" << CheckRatio::num << "/" << CheckRatio::den << ")";
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator, intmax_t Denominator, intmax_t Value, intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertStaticIntegerDivision()
{
    using TestRatio = shmit::math::StaticRatio<Numerator, Denominator>;

    using ResultRatio = typename shmit::math::Divide<TestRatio, std::integral_constant<intmax_t, Value>>::Result::type;
    using CheckRatio  = typename std::ratio<NumeratorCheck, DenominatorCheck>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << "(" << Numerator << "/" << Denominator << ") / " << Value << " = " << ResultRatio::num << "/"
               << ResultRatio::den << " Does not equal expected result: " << NumeratorCheck << "/" << DenominatorCheck
               << " (" << CheckRatio::num << "/" << CheckRatio::den << ")";
    }

    return ::testing::AssertionSuccess();
}

template<intmax_t Numerator1, intmax_t Denominator1, intmax_t Numerator2, intmax_t Denominator2,
         intmax_t NumeratorCheck, intmax_t DenominatorCheck>
::testing::AssertionResult AssertStaticRatioDivision()
{
    using TestRatio1 = shmit::math::StaticRatio<Numerator1, Denominator1>;
    using TestRatio2 = shmit::math::StaticRatio<Numerator2, Denominator2>;

    using ResultRatio = typename shmit::math::Divide<TestRatio1, TestRatio2>::Result::type;
    using CheckRatio  = typename std::ratio<NumeratorCheck, DenominatorCheck>::type;

    if ((ResultRatio::num != CheckRatio::num) || (ResultRatio::den != CheckRatio::den))
    {
        return ::testing::AssertionFailure()
               << Numerator1 << "/" << Denominator1 << " + " << Numerator2 << "/" << Denominator2 << " = "
               << ResultRatio::num << "/" << ResultRatio::den << " Does not equal expected result: " << NumeratorCheck
               << "/" << DenominatorCheck << " (" << CheckRatio::num << "/" << CheckRatio::den << ")";
    }

    return ::testing::AssertionSuccess();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Ratio tests                 ////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
