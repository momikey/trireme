#ifndef TRIREME_WORD_HPP
#define TRIREME_WORD_HPP

#include <string>

#include "hexad.hpp"

namespace ternary
{
    struct Word
    {
        using value_type = Hexad::value_type;

        static const auto high_power = 12;
        static const auto middle_power = 6;

        Word(Word::value_type v):
            low(low_trits(v, middle_power)),
            middle(low_trits(shift_right(v, middle_power), middle_power)),
            high(shift_right(v, high_power))
        {}

        Word(value_type hi, value_type mid, value_type lo):
            high(hi), middle(mid), low(lo) {}

        Word(Hexad hi, Hexad mid, Hexad lo):
            high(hi), middle(mid), low(lo) {}
        
        value_type value() const noexcept
            { return high.get() * pow3(high_power) + middle.get() * pow3(middle_power) + low.get(); }

        std::string trit_string() const noexcept;
        std::string value_string() const noexcept;

    private:
        Hexad high;
        Hexad middle;
        Hexad low;
    };
}

#endif /* TRIREME_WORD_HPP */