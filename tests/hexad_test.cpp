#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <array>
#include <utility>

#include "hexad.hpp"

using ternary::Hexad;
namespace bdata = boost::unit_test::data;

struct HexadFixture
{
    HexadFixture() = default;
    ~HexadFixture() = default;

    Hexad smallPositive { 10 };
    Hexad smallNegative { -4 };

    Hexad largePositive { 255 };
    Hexad largeNegative { -321 };

    // Identifiable pattern: +0-+0-
    Hexad tritPattern { 224 };
};

// Note that these are low to high
constexpr std::array<int, 6> tritsInPattern { -1, 0, 1, -1, 0, 1 };

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

    BOOST_AUTO_TEST_SUITE(hexad_operations_arithmetic)

    BOOST_AUTO_TEST_CASE(operation_add)
    {
        BOOST_TEST(add(smallPositive, largePositive).get() == smallPositive.get() + largePositive.get());
        BOOST_TEST(add(smallNegative, largeNegative).get() == smallNegative.get() + largeNegative.get());
        BOOST_TEST(add(smallPositive, smallNegative).get() == smallPositive.get() + smallNegative.get());
        BOOST_TEST(add(smallNegative, largePositive).get() == smallNegative.get() + largePositive.get());
    }
    
    BOOST_AUTO_TEST_CASE(operation_subtract)
    {
        BOOST_TEST(subtract(smallPositive, largePositive).get() == smallPositive.get() - largePositive.get());
        BOOST_TEST(subtract(smallNegative, largeNegative).get() == smallNegative.get() - largeNegative.get());
        BOOST_TEST(subtract(smallPositive, smallNegative).get() == smallPositive.get() - smallNegative.get());
        BOOST_TEST(subtract(smallNegative, largePositive).get() == smallNegative.get() - largePositive.get());
    }
    
    BOOST_AUTO_TEST_CASE(operation_add_with_carry)
    {
        auto r { Hexad::range };
        BOOST_TEST(add_with_carry(smallPositive, smallNegative).first.get() == smallPositive.get() + smallNegative.get());
        BOOST_TEST(add_with_carry(largePositive, largePositive).second == 1);
        BOOST_TEST(add_with_carry(largeNegative, largeNegative).first.get() - r == largeNegative.get() + largeNegative.get());
    }
    
    BOOST_AUTO_TEST_CASE(operation_subtract_with_carry)
    {
        BOOST_TEST(subtract_with_carry(smallPositive, smallNegative).first.get() == smallPositive.get() - smallNegative.get());
        BOOST_TEST(subtract_with_carry(largePositive, largeNegative).second == 1);
    }

    BOOST_AUTO_TEST_CASE(operation_multiply)
    {
        BOOST_TEST(multiply(smallPositive, smallNegative).first.get() == smallPositive.get() * smallNegative.get());

        auto mp { multiply(largePositive, smallPositive) };
        auto r { Hexad::range };
        BOOST_TEST(mp.second.get() * r + mp.first.get() == largePositive.get() * smallPositive.get());
    }

    BOOST_AUTO_TEST_CASE(operation_divide_single_precision)
    {
        auto d1 { divide(largePositive, smallPositive) };
        BOOST_TEST(d1.first.get() == largePositive.get() / smallPositive.get());
        BOOST_TEST(d1.second.get() == largePositive.get() % smallPositive.get());
        
        auto d2 { divide(largePositive, smallNegative) };
        BOOST_TEST(d2.first.get() == largePositive.get() / smallNegative.get());
        BOOST_TEST(d2.second.get() == largePositive.get() % smallNegative.get());
        
        auto d3 { divide(largeNegative, smallPositive) };
        BOOST_TEST(d3.first.get() == largeNegative.get() / smallPositive.get());
        BOOST_TEST(d3.second.get() == largeNegative.get() % smallPositive.get());
        
        auto d4 { divide(largeNegative, smallNegative) };
        BOOST_TEST(d4.first.get() == largeNegative.get() / smallNegative.get());
        BOOST_TEST(d4.second.get() == largeNegative.get() % smallNegative.get());
    }

    BOOST_AUTO_TEST_CASE(operation_divide_double_precision)
    {
        std::pair<Hexad, Hexad> hh { {0}, {1} };
        auto hhvalue { hh.second.get() * Hexad::range + hh.first.get() };

        BOOST_TEST(divide(hh, smallPositive).first.get() == hhvalue / smallPositive.get());
        BOOST_TEST(divide(hh, smallNegative).first.get() == hhvalue / smallNegative.get());
    }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(hexad_operations_logical)

    BOOST_AUTO_TEST_CASE(operation_left_shift_single)
    {
        auto r { Hexad::range };
        BOOST_TEST(left_shift(smallPositive).get() == smallPositive.get() * 3);
        BOOST_TEST(left_shift(largePositive).get() == largePositive.get() * 3 - r);
        BOOST_TEST(left_shift(largeNegative).get() == largeNegative.get() * 3 + r);
    }

    BOOST_AUTO_TEST_CASE(operation_right_shift_single)
    {
        BOOST_TEST(right_shift(smallPositive).get() == smallPositive.get() / 3);
        BOOST_TEST(right_shift(largePositive).get() == largePositive.get() / 3);
        BOOST_TEST(right_shift(largeNegative).get() == largeNegative.get() / 3);
    }
    

    BOOST_AUTO_TEST_CASE(operation_left_shift_multiple)
    {
        for (auto i = 1u; i <= Hexad::width; ++i)
        {
            BOOST_TEST(left_shift(tritPattern, i).second == tritsInPattern[Hexad::width - i]);
        }
    }

    BOOST_AUTO_TEST_CASE(operation_right_shift_multiple)
    {
        for (auto i = 1u; i <= Hexad::width; ++i)
        {
            BOOST_TEST(right_shift(tritPattern, i).second == tritsInPattern[i - 1]);
        }
    }

    BOOST_AUTO_TEST_CASE(operation_left_rotate)
    {
        for (auto i = 1u; i <= Hexad::width; ++i)
        {
            BOOST_TEST(rotate_left(tritPattern, i).trits()[0] == tritsInPattern[Hexad::width - i]);
        }
    }

    BOOST_AUTO_TEST_CASE(operation_right_rotate)
    {
        for (auto i = 1u; i <= Hexad::width; ++i)
        {
            BOOST_TEST(rotate_right(tritPattern, i).trits()[0] == tritsInPattern[i % Hexad::width]);
        }
    }

    BOOST_AUTO_TEST_CASE(operation_left_rotate_carry)
    {
        for (auto i = 1u; i <= Hexad::width; ++i)
        {
            BOOST_TEST(rotate_left_carry(tritPattern, 0, i).second == tritsInPattern[Hexad::width - i]);
            
        }
    }

    BOOST_AUTO_TEST_CASE(operation_right_rotate_carry)
    {
        for (auto i = 1u; i <= Hexad::width; ++i)
        {
            BOOST_TEST(rotate_right_carry(tritPattern, 0, i).second == tritsInPattern[i - 1]);
            
        }
    }

    BOOST_AUTO_TEST_CASE(operation_simple_invert)
    {
        BOOST_TEST(invert(smallPositive).get() == -smallPositive.get());
        BOOST_TEST(invert(largeNegative).get() == -largeNegative.get());
    }
    
    BOOST_AUTO_TEST_CASE(operation_positive_invert)
    {
        auto inv1 { positive_invert(smallPositive) };
        auto inv2 { positive_invert(largeNegative) };

        for (auto i = 0u; i < Hexad::width; ++i)
        {
            if (smallPositive.trits()[i] == 1)
            {
                BOOST_TEST(inv1.trits()[i] == -1);
            }
            else
            {
                BOOST_TEST(inv1.trits()[i] == 1);
            }

            if (largeNegative.trits()[i] == 1)
            {
                BOOST_TEST(inv2.trits()[i] == -1);
            }
            else
            {
                BOOST_TEST(inv2.trits()[i] == 1);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(operation_negative_invert)
    {
        auto inv1 { negative_invert(smallPositive) };
        auto inv2 { negative_invert(largeNegative) };

        for (auto i = 0u; i < Hexad::width; ++i)
        {
            if (smallPositive.trits()[i] == -1)
            {
                BOOST_TEST(inv1.trits()[i] == -1);
            }
            else
            {
                BOOST_TEST(inv1.trits()[i] == -1);
            }

            if (largeNegative.trits()[i] == -1)
            {
                BOOST_TEST(inv2.trits()[i] == 1);
            }
            else
            {
                BOOST_TEST(inv2.trits()[i] == -1);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(operation_forward_diode)
    {
        auto d1 { forward_diode(smallPositive) };
        auto d2 { forward_diode(largeNegative) };

        for (auto i = 0u; i < Hexad::width; ++i)
        {
            if (smallPositive.trits()[i] == 1)
            {
                BOOST_TEST(d1.trits()[i] == 1);
            }
            else
            {
                BOOST_TEST(d1.trits()[i] == 0);
            }

            if (largeNegative.trits()[i] == 1)
            {
                BOOST_TEST(d2.trits()[i] == 1);
            }
            else
            {
                BOOST_TEST(d2.trits()[i] == 0);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(operation_reverse_diode)
    {
        auto d1 { reverse_diode(smallPositive) };
        auto d2 { reverse_diode(largeNegative) };

        for (auto i = 0u; i < Hexad::width; ++i)
        {
            if (smallPositive.trits()[i] == -1)
            {
                BOOST_TEST(d1.trits()[i] == -1);
            }
            else
            {
                BOOST_TEST(d1.trits()[i] == 0);
            }

            if (largeNegative.trits()[i] == -1)
            {
                BOOST_TEST(d2.trits()[i] == -1);
            }
            else
            {
                BOOST_TEST(d2.trits()[i] == 0);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(operation_trit_maximum)
    {
        auto result1 { trit_maximum(smallPositive, smallNegative) };
        auto result2 { trit_maximum(largePositive, largeNegative) };

        for (auto i = 0u; i < Hexad::width; ++i)
        {
            BOOST_TEST(result1.trits()[i] ==
                std::max(smallPositive.trits()[i], smallNegative.trits()[i])
            );

            BOOST_TEST(result2.trits()[i] ==
                std::max(largePositive.trits()[i], largeNegative.trits()[i])
            );
        }
    }
    
    BOOST_AUTO_TEST_CASE(operation_trit_minimum)
    {
        auto result1 { trit_minimum(smallPositive, smallNegative) };
        auto result2 { trit_minimum(largePositive, largeNegative) };

        for (auto i = 0u; i < Hexad::width; ++i)
        {
            BOOST_TEST(result1.trits()[i] ==
                std::min(smallPositive.trits()[i], smallNegative.trits()[i])
            );

            BOOST_TEST(result2.trits()[i] ==
                std::min(largePositive.trits()[i], largeNegative.trits()[i])
            );
        }
    }

    BOOST_AUTO_TEST_CASE(operation_logical_equality)
    {
        auto result1 { logical_equality(smallPositive, smallNegative) };
        auto result2 { logical_equality(largePositive, largeNegative) };

        for (auto i = 0u; i < Hexad::width; ++i)
        {
            BOOST_TEST(result1.trits()[i] ==
                (smallPositive.trits()[i] == smallNegative.trits()[i] ? 1 : -1)
            );

            BOOST_TEST(result2.trits()[i] ==
                (largePositive.trits()[i] == largeNegative.trits()[i] ? 1 : -1)
            );
        }
    }

    BOOST_AUTO_TEST_CASE(operation_logical_multiply)
    {
        auto result1 { logical_multiply(smallPositive, smallNegative) };
        auto result2 { logical_multiply(largePositive, largeNegative) };

        for (auto i = 0u; i < Hexad::width; ++i)
        {
            BOOST_TEST(result1.trits()[i] ==
                (smallPositive.trits()[i] * smallNegative.trits()[i])
            );

            BOOST_TEST(result2.trits()[i] ==
                (largePositive.trits()[i] * largeNegative.trits()[i])
            );
        }
    }

    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()