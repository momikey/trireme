#ifndef TRIREME_FLAGS_HPP
#define TRIREME_FLAGS_HPP

#include "word.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    struct Flags
    {
        Flags() = default;

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
        int get_C() const { return get_trit(0); }
        int get_S() const { return get_trit(1); }
        int get_D() const { return get_trit(2); }
        int get_A() const { return get_trit(5); }
        int get_B() const { return get_trit(6); }
        int get_T() const { return get_trit(7); }
        int get_I() const { return get_trit(8); }
        int get_P() const { return get_trit(9); }

        void set_C(int value) { set_trit(0, value); }
        void set_S(int value) { set_trit(1, value); }
        void set_D(int value) { set_trit(2, value); }
        void set_A(int value) { set_trit(5, value); }
        void set_B(int value) { set_trit(6, value); }
        void set_T(int value) { set_trit(7, value); }
        void set_I(int value) { set_trit(8, value); }
        void set_P(int value) { set_trit(9, value); }

        Word get() const { return w_; }

        private:
        Word w_ {};

        int get_trit(int t) const { return nth_trit(w_.value(), t); }
        void set_trit(int t, int value)
            { w_ = { w_.value() - get_trit(t)*pow3(t) + value*pow3(t)}; }
    };
}

#endif /* TRIREME_FLAGS_HPP */