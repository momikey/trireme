#include <boost/test/unit_test.hpp>

#include "ternary_math.hpp"

BOOST_AUTO_TEST_SUITE(ternary_math)

BOOST_AUTO_TEST_CASE(abs_test)
{
    BOOST_TEST(abs_c(1) == 1);
    BOOST_TEST(abs_c(-1) == 1);
    BOOST_TEST(abs_c(0) == 0);
}

BOOST_AUTO_TEST_CASE(sign_test)
{
    BOOST_TEST(sign_c(42) == 1);
    BOOST_TEST(sign_c(-123) == -1);
    BOOST_TEST(sign_c(0) == 0);
}

BOOST_AUTO_TEST_CASE(power_of_3_test)
{
    BOOST_TEST(pow3(0) == 1);
    BOOST_TEST(pow3(1) == 3);
    BOOST_TEST(pow3(4) == 81);
}

BOOST_AUTO_TEST_CASE(lowest_trit_test)
{
    BOOST_TEST(lowest_trit(0) == 0);
    BOOST_TEST(lowest_trit(-1) == lowest_trit(2));
    BOOST_TEST(lowest_trit(31) == 1);
}

BOOST_AUTO_TEST_CASE(shift_right_test)
{
    auto tc1 { 42 };
    BOOST_TEST(shift_right(tc1, 0) == 42);
    BOOST_TEST(shift_right(tc1, 1) == 14);
    BOOST_TEST(shift_right(tc1, 2) == 5);
    BOOST_TEST(shift_right(tc1, 3) == 2);
    BOOST_TEST(shift_right(tc1, 4) == 1);
    BOOST_TEST(shift_right(tc1, 5) == 0);
}

BOOST_AUTO_TEST_CASE(nth_trit_test)
{
    auto tc2 { 52 };    // +-0-+
    BOOST_TEST(nth_trit(tc2, 0) == 1);
    BOOST_TEST(nth_trit(tc2, 1) == -1);
    BOOST_TEST(nth_trit(tc2, 2) == 0);
    BOOST_TEST(nth_trit(tc2, 3) == -1);
    BOOST_TEST(nth_trit(tc2, 4) == 1);
    BOOST_TEST(nth_trit(tc2, 5) == 0);
}

BOOST_AUTO_TEST_CASE(clamp_test)
{
    auto n1 { clamp<int,6>(0) };
    auto n2 { clamp<int,6>(123) };
    auto n3 { clamp<int,6>(-1234) };
    BOOST_TEST(n1 == 0);
    BOOST_TEST(n2 == 123);
    BOOST_TEST(n3 == -(pow3(6)/2));
}

BOOST_AUTO_TEST_CASE(low_trits_test)
{
    auto tc3 { 52 };    // +-0-+
    BOOST_TEST(low_trits(tc3, 0) == 0);
    BOOST_TEST(low_trits(tc3, 1) == 1);
    BOOST_TEST(low_trits(tc3, 2) == -2);
    BOOST_TEST(low_trits(tc3, 3) == -2);
    BOOST_TEST(low_trits(tc3, 4) == -29);
    BOOST_TEST(low_trits(tc3, 5) == 52);
}

BOOST_AUTO_TEST_SUITE_END()