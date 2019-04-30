#include "opcode.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    Opcode::Opcode(const Word& w):
        value(w),
        o(shift_right(w.high().get(), 3)),
        m(low_trits(w.high().get(), 3)),
        t(shift_right(w.middle().get(), 3)),
        x(low_trits(w.middle().get(), 3)),
        y(shift_right(w.low().get(), 3)),
        z(low_trits(w.low().get(), 3))
    {}

    int Opcode::low6() const
    {
        return y * pow3(3) + z;
    }

    int Opcode::low9() const
    {
        return x * pow3(6) + low6();
    }

    int Opcode::low12() const
    {
        return t * pow3(9) + low9();
    }
}