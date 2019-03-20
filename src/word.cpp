#include <string>
#include <tuple>

#include "word.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    std::string Word::value_string() const noexcept
    {
        std::string result { "%" };
        result += high_.value_string();
        result += middle_.value_string();
        result += low_.value_string();
        
        return result;
    }

    std::string Word::trit_string() const noexcept
    {
        auto result { high_.trit_string() };
        result += " ";
        result += middle_.trit_string();
        result += " ";
        result += low_.trit_string();

        return result;
    }

    addition_result add(Word lhs, Word rhs)
    {
        Hexad low, middle, high;
        int lowc, midc, highc;

        std::tie(low, lowc) = add_with_carry(lhs.low(), rhs.low());
        rhs.set_middle(rhs.middle().get() + lowc);
        std::tie(middle, midc) = add_with_carry(lhs.middle(), rhs.middle());
        rhs.set_high(rhs.high().get() + midc);
        std::tie(high, highc) = add_with_carry(lhs.high(), rhs.high());

        return { {high, middle, low}, highc };
    }

    addition_result add(Word lhs, Word::value_type rhs)
    {
        return add(lhs, { rhs });
    }

    addition_result sub(Word lhs, Word rhs)
    {
        return add(lhs, sti(rhs));
    }

    addition_result sub(Word lhs, Word::value_type rhs)
    {
        return add(lhs, { -rhs });
    }

    multiplication_result mul(Word lhs, Word rhs);
    multiplication_result mul(Word lhs, Word::value_type rhs);

    division_result div(division_dividend lhs, Word rhs);
    division_result div(division_dividend lhs, Word::value_type rhs);

    shift_result shl(Word operand, Word places);
    shift_result shl(Word operand, Word::value_type places);
    shift_result shr(Word operand, Word places);
    shift_result shr(Word operand, Word::value_type places);

    shift_result rol(Word operand, Word places);
    shift_result rol(Word operand, Word::value_type places);
    shift_result ror(Word operand, Word places);
    shift_result ror(Word operand, Word::value_type places);

    shift_result rcl(Word operand, Word places);
    shift_result rcl(Word operand, Word::value_type places);
    shift_result rcr(Word operand, Word places);
    shift_result rcr(Word operand, Word::value_type places);

    Word sti(Word operand)
    {
        return {
            invert(operand.high()),
            invert(operand.middle()),
            invert(operand.low())
        };
    }

    Word pti(Word operand)
    {
        return {
            positive_invert(operand.high()),
            positive_invert(operand.middle()),
            positive_invert(operand.low())
        };
    }

    Word nti(Word operand)
    {
        return {
            negative_invert(operand.high()),
            negative_invert(operand.middle()),
            negative_invert(operand.low())
        };
    }

    Word min(Word lhs, Word rhs)
    {
        return {
            trit_minimum(lhs.high(), rhs.high()),
            trit_minimum(lhs.middle(), rhs.middle()),
            trit_maximum(lhs.low(), rhs.low())
        };
    }

    Word max(Word lhs, Word rhs)
    {
        return {
            trit_maximum(lhs.high(), rhs.high()),
            trit_maximum(lhs.middle(), rhs.middle()),
            trit_maximum(lhs.low(), rhs.low())
        };
    }
    
    Word teq(Word lhs, Word rhs)
    {
        return {
            logical_equality(lhs.high(), rhs.high()),
            logical_equality(lhs.middle(), rhs.middle()),
            logical_equality(lhs.low(), rhs.low())
        };
    }

    Word fdr(Word operand)
    {
        return {
            forward_diode(operand.high()),
            forward_diode(operand.middle()),
            forward_diode(operand.low())
        };
    }

    Word rdr(Word operand)
    {
        return {
            reverse_diode(operand.high()),
            reverse_diode(operand.middle()),
            reverse_diode(operand.low())
        };
    }

    Word bin(Word operand);
    Word tri(Word operand);
}
