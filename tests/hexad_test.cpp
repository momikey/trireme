#include <boost/test/unit_test.hpp>

#include <array>

#include "hexad.hpp"

using ternary::Hexad;

struct HexadFixture
{
    HexadFixture() = default;
    ~HexadFixture() = default;

    Hexad smallPositive { 10 };
    Hexad smallNegative { -4 };

    Hexad largePositive { 255 };
    Hexad largeNegative { -321 };
};

BOOST_FIXTURE_TEST_SUITE(hexad, HexadFixture)

    BOOST_AUTO_TEST_SUITE(hexad_basic_functions)

    BOOST_AUTO_TEST_CASE(default_constructor)
    {
        Hexad h;
        BOOST_TEST(h.get() == 0);
        BOOST_TEST(h.value_string() == "00");
    }

    BOOST_AUTO_TEST_CASE(constructor_with_value)
    {
        Hexad h { 123 };
        BOOST_TEST(h.get() == 123);
    }

    BOOST_AUTO_TEST_CASE(trit_string)
    {
        auto a { smallPositive.trit_string() };
        BOOST_TEST(a.length() == 6);
        BOOST_TEST(a == "000+0+");
    }

    BOOST_AUTO_TEST_CASE(trit_array)
    {
        auto a { smallNegative.trits() };
        BOOST_TEST(a.size() == 6);
        BOOST_TEST(a[0] == -1);
        BOOST_TEST(a[1] == -1);
        BOOST_TEST(a[2] == 0);
    }
    
    BOOST_AUTO_TEST_CASE(value_string)
    {
        BOOST_TEST(smallPositive.value_string() == "0J");
        BOOST_TEST(smallNegative.value_string() == "0q");
        BOOST_TEST(largePositive.value_string() == "IL");
        BOOST_TEST(largeNegative.value_string() == "yC");
    }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(hexad_operations)

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()