#include <algorithm>

#include "hexad.hpp"
#include "ternary_math.hpp"

namespace ternary {
    value_with_carry<Hexad> add_with_carry(const Hexad lhs, const Hexad rhs)
    {
        const auto result { lhs.get() + rhs.get() };
        const auto carry { shift_right(result, Hexad::width) };

        return { {result}, carry };
    }

    value_with_carry<Hexad> subtract_with_carry(const Hexad lhs, const Hexad rhs)
    {
        const auto result { lhs.get() - rhs.get() };
        const auto carry { shift_right(result, Hexad::width) };

        return { {result}, carry };
    }

    std::pair<Hexad, Hexad> multiply(const Hexad lhs, const Hexad rhs)
    {
        const auto result { lhs.get() * rhs.get() };

        return {
            { low_trits(result, Hexad::width) },
            { shift_right(result, Hexad::width) }
        };
    }

    std::pair<Hexad, Hexad> divide(const Hexad lhs, const Hexad rhs)
    {
        const auto div_ { lhs.get() / rhs.get() };
        const auto mod_ { lhs.get() % rhs.get()};

        return { {div_}, {mod_} };
    }

    std::pair<Hexad, Hexad> divide(const std::pair<Hexad, Hexad> lhs, const Hexad rhs)
    {
        const auto full { lhs.first.get() + lhs.second.get() * Hexad::range };
        const auto div_ { full / rhs.get() };
        const auto mod_ { full % rhs.get() };

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
            const auto result { operand.get() * pow3(places) };
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
            const auto result { shift_right(operand.get(), places) };
            return { {result}, nth_trit(operand.get(), places - 1) };
        }
    }

    Hexad rotate_left(const Hexad operand, std::size_t places)
    {
        if (places % Hexad::width == 0)
        {
            // Simple optimization: rotating n trits by k*n is a no-op
            return operand;
        }
        else
        {
            auto tr { operand.trits() };
            std::rotate(tr.rbegin(), tr.rbegin() + places, tr.rend());
            return { to_decimal(tr) };
        }
        
    }

    Hexad rotate_right(const Hexad operand, std::size_t places)
    {
        if (places % Hexad::width == 0)
        {
            // Simple optimization: rotating n trits by k*n is a no-op
            return operand;
        }
        else
        {
            auto tr { operand.trits() };
            std::rotate(tr.begin(), tr.begin() + places, tr.end());
            return { to_decimal(tr) };
        }
        
    }

    std::pair<Hexad, int> rotate_left_carry(const Hexad operand, int carry, std::size_t places)
    {
        auto tr { operand.trits() };
        auto wc = std::array<Hexad::value_type, Hexad::width+1>{};
        std::copy(tr.begin(), tr.end(), wc.begin());
        wc[Hexad::width] = carry;

        std::rotate(wc.rbegin(), wc.rbegin() + places, wc.rend());
        auto result = Hexad::trit_container_type{};
        std::copy_n(wc.begin(), Hexad::width, result.begin());

        return { { to_decimal(result) }, wc[Hexad::width] };
    }

    std::pair<Hexad, int> rotate_right_carry(const Hexad operand, int carry, std::size_t places)
    {
        auto tr { operand.trits() };
        auto wc = std::array<Hexad::value_type, Hexad::width+1>{};
        std::copy(tr.begin(), tr.end(), wc.begin());
        wc[Hexad::width] = carry;

        std::rotate(wc.begin(), wc.begin() + places, wc.end());
        auto result = Hexad::trit_container_type{};
        std::copy_n(wc.begin(), Hexad::width, result.begin());

        return { { to_decimal(result) }, wc[Hexad::width] };
    }

    Hexad positive_invert(const Hexad operand)
    {
        auto tr { operand.trits() };
        std::transform(tr.begin(), tr.end(), tr.begin(), [](auto e){
            return (e == 1) ? -1 : 1;
        });
        return { to_decimal(tr) };
    }

    Hexad negative_invert(const Hexad operand)
    {
        auto tr { operand.trits() };
        std::transform(tr.begin(), tr.end(), tr.begin(), [](auto e){
            return (e == -1) ? 1 : -1;
        });
        return { to_decimal(tr) };
    }

    Hexad trit_minimum(const Hexad lhs, const Hexad rhs)
    {
        const auto lt { lhs.trits() };
        const auto rt { rhs.trits() };
        auto result = Hexad::trit_container_type{};

        // TODO: Look for a better way to write this
        for (auto i = 0; i < result.size(); ++i)
        {
            result[i] = std::min(lt[i], rt[i]);
        }

        return { to_decimal(result) };
    }

    Hexad trit_maximum(const Hexad lhs, const Hexad rhs)
    {
        const auto lt { lhs.trits() };
        const auto rt { rhs.trits() };
        auto result = Hexad::trit_container_type{};

        // TODO: Look for a better way to write this
        for (auto i = 0; i < result.size(); ++i)
        {
            result[i] = std::max(lt[i], rt[i]);
        }

        return { to_decimal(result) };
    }

    Hexad logical_equality(const Hexad lhs, const Hexad rhs)
    {
        const auto lt { lhs.trits() };
        const auto rt { rhs.trits() };
        auto result = Hexad::trit_container_type{};

        // TODO: Look for a better way to write this
        for (auto i = 0; i < result.size(); ++i)
        {
            result[i] = (lt[i] == rt[i]) ? 1 : -1;
        }

        return { to_decimal(result) };
    }

    Hexad forward_diode(const Hexad operand)
    {
        auto tr { operand.trits() };
        std::transform(tr.begin(), tr.end(), tr.begin(), [](auto e){
            return (e == -1) ? 0 : e;
        });

        return { to_decimal(tr) };
    }

    Hexad reverse_diode(const Hexad operand)
    {
        auto tr { operand.trits() };
        std::transform(tr.begin(), tr.end(), tr.begin(), [](auto e){
            return (e == 1) ? 0 : e;
        });

        return { to_decimal(tr) };
    }

    std::string Hexad::trit_string() const noexcept
    {
        auto tr { trits() };
        auto result = std::string(Hexad::width, ' ' );
        std::transform(tr.begin(), tr.end(), result.rbegin(), [](auto e){
            if (e == 1)
                { return '+'; }
            else if (e == -1)
                { return '-'; }
            else
                { return '0'; }
        });
        return result;
    }

    std::string Hexad::value_string() const noexcept
    {
        auto low { low_trits(value, 3) };
        auto high { shift_right(value, 3) };

        auto result { triad_to_string(high) };
        result += triad_to_string(low);
        return result;
    }
}