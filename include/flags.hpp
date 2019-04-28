#ifndef TRIREME_FLAGS_HPP
#define TRIREME_FLAGS_HPP

#include <string>
#include <numeric>
#include <algorithm>

#include "word.hpp"
#include "ternary_math.hpp"
#include "convert.hpp"

namespace ternary
{
    // Flags are as follows:
    // 0: Carry (C)
    // 1: Sign (S)
    // 2: Direction (D)
    // 3: reserved
    // 4: reserved
    // 5: Absolute (A)
    // 6: Binary (B)
    // 7: Trap (T)
    // 8: Interrupt (I)
    // 9: Protection (P)
    enum class flags
    {
        carry = 0,
        sign,
        direction,
        // 3: reserved
        // 4: reserved
        absolute = 5,
        binary,
        trap,
        interrupt,
        protection            
    };

    struct FlagRegister
    {
        FlagRegister() = default;

        int get_flag(flags f) const { return get_trit(static_cast<int>(f)); }
        void set_flag(flags f, int value) { set_trit(static_cast<int>(f), value); }

        Word get() const { return w_; }

        std::string to_string() const;

        private:
        Word w_ {};

        int get_trit(int t) const { return nth_trit(w_.value(), t); }
        void set_trit(int t, int value)
            { w_ = { w_.value() - get_trit(t)*pow3(t) + value*pow3(t)}; }        
    };
}

#endif /* TRIREME_FLAGS_HPP */