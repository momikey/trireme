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

    std::pair<Hexad, Hexad> divide(const std::pair<Hexad, Hexad> lhs, const Hexad rhs)
    {
        const auto full = lhs.first.get() + lhs.second.get() * Hexad::range;
        const auto div_ = full / rhs.get();
        const auto mod_ = full % rhs.get();

        return { {div_}, {mod_} };
    }

    std::pair<Hexad, int> left_shift(const Hexad operand, std::size_t places)
    {
        if (places >= Hexad::width)
        {
            return { {0}, (places == Hexad::width) ? lowest_trit(operand.get()) : 0 };
        }
        else
        {
            const auto result = operand.get() * pow3(places);
            return { {result}, nth_trit(operand.get(), Hexad::width - places) };
        }
    }

    std::pair<Hexad, int> right_shift(const Hexad operand, std::size_t places)
    {
        if (places == 0)
        {
            return { operand, places };
        }
        else if (places >= Hexad::width)
        {
            return { {0}, (places == Hexad::width) ? nth_trit(operand.get(), Hexad::width - 1) : 0};
        }
        else
        {
            const auto result = shift_right(operand.get(), places);
            return { {result}, nth_trit(operand.get(), places - 1) };
        }
    }
}