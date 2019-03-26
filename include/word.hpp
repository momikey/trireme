#ifndef TRIREME_WORD_HPP
#define TRIREME_WORD_HPP

#include <string>
#include <array>
#include <utility>

#include "hexad.hpp"

namespace ternary
{
    struct Word
    {
        static const auto word_size = 18;
        static const auto high_power = 12;
        static const auto middle_power = 6;

        using value_type = Hexad::value_type;
        using trit_container = std::array<value_type, word_size>;
        using trit_container_with_carry = std::array<value_type, word_size+1>;

        Word(Word::value_type v):
            high_(shift_right(v, high_power)),
            middle_(low_trits(shift_right(v, middle_power), middle_power)),
            low_(low_trits(v, middle_power))
        {}

        Word(value_type hi, value_type mid, value_type lo):
            high_(hi), middle_(mid), low_(lo) {}

        Word(Hexad hi, Hexad mid, Hexad lo):
            high_(hi), middle_(mid), low_(lo) {}
        
        value_type value() const noexcept
            { return high_.get() * pow3(high_power) + middle_.get() * pow3(middle_power) + low_.get(); }

        Hexad low() const noexcept { return low_; }
        Hexad middle() const noexcept { return middle_; }
        Hexad high() const noexcept { return high_; }

        void set_low(Hexad l) noexcept { low_ = l; }
        void set_low(value_type l) noexcept { low_ = l; }
        void set_middle(Hexad m) noexcept { middle_ = m; }
        void set_middle(value_type m) noexcept { middle_ = m; }
        void set_high(Hexad h) noexcept { high_ = h; }
        void set_high(value_type h) noexcept { high_ = h; }

        std::string trit_string() const noexcept;
        std::string value_string() const noexcept;

    private:
        Hexad high_;
        Hexad middle_;
        Hexad low_;
    };

    // Operations on Words

    // Convenience types

    // Addition/subtraction results have only a single trit for carries
    using addition_result = std::pair<Word, int>;

    // Multiplication gives a double-precision result
    using multiplication_result = std::pair<Word, Word>;

    // Divisinn requires two types: a double-precision dividend
    // and a quotient/remainder pair. Note that the "result" type
    // is functionally the same, but we redefine it for clarity.
    using division_dividend = multiplication_result;
    using division_result = std::pair<Word, Word>;

    // Shift and rotate operations can produce a carry, the same as addition
    using shift_result = std::pair<Word, int>;

    // Arithmetic
    addition_result add(Word lhs, Word rhs);
    addition_result add(Word lhs, Word::value_type rhs);
    addition_result sub(Word lhs, Word rhs);
    addition_result sub(Word lhs, Word::value_type rhs);

    multiplication_result mul(Word lhs, Word rhs);
    multiplication_result mul(Word lhs, Word::value_type rhs);

    division_result div(Word lhs, Word rhs);
    division_result div(Word lhs, Word::value_type rhs);
    division_result div(division_dividend lhs, Word rhs);
    division_result div(division_dividend lhs, Word::value_type rhs);

    // Logical
    shift_result shl(Word operand, Word places);
    shift_result shl(Word operand, Word::value_type places);
    shift_result shr(Word operand, Word places);
    shift_result shr(Word operand, Word::value_type places);

    shift_result rol(Word operand, Word places);
    shift_result rol(Word operand, Word::value_type places);
    shift_result ror(Word operand, Word places);
    shift_result ror(Word operand, Word::value_type places);

    shift_result rcl(Word operand, int carry, Word places);
    shift_result rcl(Word operand, int carry, Word::value_type places);
    shift_result rcr(Word operand, int carry, Word places);
    shift_result rcr(Word operand, int carry, Word::value_type places);

    Word sti(Word operand);
    Word pti(Word operand);
    Word nti(Word operand);

    Word min(Word lhs, Word rhs);
    Word max(Word lhs, Word rhs);
    Word teq(Word lhs, Word rhs);
    Word tem(Word lhs, Word rhs);

    Word fdr(Word operand);
    Word rdr(Word operand);

    // Conversion
    Word bin(Word operand);
    Word tri(Word operand);
}

#endif /* TRIREME_WORD_HPP */