#include "opcode.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    Opcode::Opcode(const Word& w):
        opcode_(w),
        o_(shift_right(w.high().get(), 3)),
        m_(low_trits(w.high().get(), 3)),
        t_(shift_right(w.middle().get(), 3)),
        x_(low_trits(w.middle().get(), 3)),
        y_(shift_right(w.low().get(), 3)),
        z_(low_trits(w.low().get(), 3))
    {}

    int Opcode::low6() const
    {
        return y_ * pow3(3) + z_;
    }

    int Opcode::low9() const
    {
        return x_ * pow3(6) + low6();
    }

    int Opcode::low12() const
    {
        return t_ * pow3(9) + low9();
    }
}