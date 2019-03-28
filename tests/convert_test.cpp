#include <boost/test/unit_test.hpp>

#include <array>

#include "convert.hpp"

struct ConvertFixture
{
    ConvertFixture() = default;
    ~ConvertFixture() = default;

    int positiveValue {42};
    int zeroValue {0};
    int negativeValue {-13};

    // Value is 101 decimal, or 0++ 0+-
    std::array<int, 6> positiveTritArray { -1, 1, -1, 1, 1, 0};

    // Value is -444 decimal or 00- ++- --0
    std::array<int, 9> negativeTritArray { 0, -1, -1, -1, 1, 1, -1, 0, 0};
};

BOOST_FIXTURE_TEST_SUITE(convert, ConvertFixture)

BOOST_AUTO_TEST_CASE(convert_to_decimal)
{
    BOOST_TEST(to_decimal(positiveTritArray) == 101);
    BOOST_TEST(to_decimal(negativeTritArray) == -444);
}

BOOST_AUTO_TEST_CASE(convert_to_trits)
{
    auto ptrits { to_trits<int, 6>(positiveValue) };
    auto ztrits { to_trits<int, 6>(zeroValue) };
    auto ntrits { to_trits<int, 6>(negativeValue) };

    BOOST_TEST(ptrits == (std::array<int, 6> { 0, -1, -1, -1, 1, 0 }));
    BOOST_TEST(ztrits == (std::array<int, 6> { 0, 0, 0, 0, 0, 0}));
    BOOST_TEST(ntrits == (std::array<int, 6> { -1, -1, -1, 0, 0, 0}));
}

BOOST_AUTO_TEST_CASE(convert_to_triad_string)
{
    BOOST_TEST(triad_to_string(negativeValue) == "z");
}

BOOST_AUTO_TEST_SUITE_END()