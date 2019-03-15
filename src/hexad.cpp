#include "hexad.hpp"
#include "ternary_math.hpp"

namespace ternary {
    value_with_carry<Hexad> add_with_carry(const Hexad lhs, const Hexad rhs)
    {
        const auto result = lhs.get() + rhs.get();
        const auto carry = shift_right(result, Hexad::width);

        return { {result}, carry };
    }

    value_with_carry<Hexad> subtract_with_carry(const Hexad lhs, const Hexad rhs)
    {
        const auto result = lhs.get() - rhs.get();
        const auto carry = shift_right(result, Hexad::width);

        return { {result}, carry };
    }

    std::pair<Hexad, Hexad> multiply(const Hexad lhs, const Hexad rhs)
    {
        const auto result = lhs.get() * rhs.get();

        return {
            { low_trits(result, Hexad::width) },
            { shift_right(result, Hexad::width) }
        };
    }

    std::pair<Hexad, Hexad> divide(const Hexad lhs, const Hexad rhs)
    {
        const auto div_ = lhs.get() / rhs.get();
        const auto mod_ = lhs.get() % rhs.get();

        return { {div_}, {mod_} };
    }
}