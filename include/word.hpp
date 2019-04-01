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

        Word() = default;

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
        Hexad high_ {};
        Hexad middle_ {};
        Hexad low_ {};
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
    addition_result add(const Word& lhs, Word rhs) noexcept;
    addition_result add(const Word& lhs, Word::value_type rhs) noexcept;
    addition_result sub(const Word& lhs, const Word& rhs) noexcept;
    addition_result sub(const Word& lhs, Word::value_type rhs) noexcept;

    multiplication_result mul(const Word& lhs, const Word& rhs) noexcept;
    multiplication_result mul(const Word& lhs, Word::value_type rhs) noexcept;

    division_result div(const Word& lhs, const Word& rhs) noexcept;
    division_result div(const Word& lhs, Word::value_type rhs) noexcept;
    division_result div(division_dividend lhs, const Word& rhs) noexcept;
    division_result div(division_dividend lhs, Word::value_type rhs) noexcept;

    // Logical
    shift_result shl(const Word& operand, const Word& places) noexcept;
    shift_result shl(const Word& operand, Word::value_type places) noexcept;
    shift_result shr(const Word& operand, const Word& places) noexcept;
    shift_result shr(const Word& operand, Word::value_type places) noexcept;

    shift_result rol(const Word& operand, const Word& places) noexcept;
    shift_result rol(const Word& operand, Word::value_type places) noexcept;
    shift_result ror(const Word& operand, const Word& places) noexcept;
    shift_result ror(const Word& operand, Word::value_type places) noexcept;

    shift_result rcl(const Word& operand, int carry, const Word& places) noexcept;
    shift_result rcl(const Word& operand, int carry, Word::value_type places) noexcept;
    shift_result rcr(const Word& operand, int carry, const Word& places) noexcept;
    shift_result rcr(const Word& operand, int carry, Word::value_type places) noexcept;

    Word sti(const Word& operand) noexcept;
    Word pti(const Word& operand) noexcept;
    Word nti(const Word& operand) noexcept;

    Word min(const Word& lhs, const Word& rhs) noexcept;
    Word max(const Word& lhs, const Word& rhs) noexcept;
    Word teq(const Word& lhs, const Word& rhs) noexcept;
    Word tem(const Word& lhs, const Word& rhs) noexcept;

    Word fdr(const Word& operand) noexcept;
    Word rdr(const Word& operand) noexcept;

    // Conversion
    Word bin(const Word& operand) noexcept;
    Word tri(const Word& operand) noexcept;
}

#endif /* TRIREME_WORD_HPP */