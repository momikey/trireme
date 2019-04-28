#include <string>
#include <tuple>
#include <algorithm>

#include "word.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    std::string Word::value_string() const noexcept
    {
        std::string result { "%" };
        result += high_.value_string();
        result += middle_.value_string();
        result += low_.value_string();
        
        return result;
    }

    std::string Word::trit_string() const noexcept
    {
        auto result { high_.trit_string() };
        result += " ";
        result += middle_.trit_string();
        result += " ";
        result += low_.trit_string();

        return result;
    }

    std::string Word::raw_trit_string() const noexcept
    {
        auto result { high_.trit_string() };
        result += middle_.trit_string();
        result += low_.trit_string();

        return result;
    }

    addition_result add(const Word& lhs, Word rhs) noexcept
    {
        // Note: This is the only case where we *don't* use only
        // const refs for Word arguments. We're effectively using
        // the `rhs` as a temporary accumulator variable, so it
        // can't be const. But we also don't want to change the
        // original, so it has to be passed by value.
        Hexad low, middle, high;
        int lowc, midc, highc;

        std::tie(low, lowc) = add_with_carry(lhs.low(), rhs.low());
        rhs.set_middle(rhs.middle().get() + lowc);
        std::tie(middle, midc) = add_with_carry(lhs.middle(), rhs.middle());
        rhs.set_high(rhs.high().get() + midc);
        std::tie(high, highc) = add_with_carry(lhs.high(), rhs.high());

        return { {high, middle, low}, highc };
    }

    addition_result add(const Word& lhs, Word::value_type rhs) noexcept
    {
        return add(lhs, Word{ rhs });
    }

    addition_result sub(const Word& lhs, const Word& rhs) noexcept
    {
        return add(lhs, sti(rhs));
    }

    addition_result sub(const Word& lhs, Word::value_type rhs) noexcept
    {
        return add(lhs, Word{ -rhs });
    }

    multiplication_result mul(const Word& lhs, const Word& rhs) noexcept
    {
        return mul(lhs, rhs.value());
    }

    multiplication_result mul(const Word& lhs, Word::value_type rhs) noexcept
    {
        long long result = lhs.value() * static_cast<long long>(rhs);
        return {
            { static_cast<Word::value_type>(low_trits(result, Word::word_size)) },
            { static_cast<Word::value_type>(shift_right(result, Word::word_size)) }
        };
    }

    division_result div(const Word& lhs, const Word& rhs) noexcept
    {
        return div(lhs, rhs.value());
    }

    division_result div(const Word& lhs, Word::value_type rhs) noexcept
    {
        auto d { lhs.value() / rhs };
        auto r { lhs.value() % rhs };

        return { {d}, {r} };
    }

    division_result div(division_dividend lhs, const Word& rhs) noexcept
    {
        return div(lhs, rhs.value());
    }

    division_result div(division_dividend lhs, Word::value_type rhs) noexcept
    {
        long long dividend = lhs.first.value() * pow3<long long>(Word::word_size) + lhs.second.value();

        // Note: double-precision division can overflow
        // TODO We should check for this
        auto d { dividend / rhs };
        auto r { dividend % rhs };

        return {
            {static_cast<Word::value_type>(d)},
            {static_cast<Word::value_type>(r)}
        };
    }

    shift_result shl(const Word& operand, const Word& places) noexcept
    {
        return shl(operand, places.value());
    }

    shift_result shl(const Word& operand, Word::value_type places) noexcept
    {
        if (places > Word::word_size + 1)
        {
            return { {0}, 0 };
        }

        Word::trit_container_with_carry trits;
        auto ht { operand.high().trits() };
        auto mt { operand.middle().trits() };
        auto lt { operand.low().trits() };

        std::copy(lt.begin(), lt.end(), trits.begin());
        std::copy(mt.begin(), mt.end(), trits.begin()+Word::middle_power);
        std::copy(ht.begin(), ht.end(), trits.begin()+Word::high_power);
        trits[Word::word_size] = 0;

        // Shift isn't in STL until C++20, but we can fake it with rotate
        std::rotate(trits.rbegin(), trits.rbegin()+places, trits.rend());
        std::fill_n(trits.begin(), places, 0);

        auto carry { trits[Word::word_size] };

        trits[Word::word_size] = 0;

        return { { to_decimal(trits) }, carry };
    }

    shift_result shr(const Word& operand, const Word& places) noexcept
    {
        return shr(operand, places.value());
    }

    shift_result shr(const Word& operand, Word::value_type places) noexcept
    {
        if (places > Word::word_size + 1)
        {
            return { {0}, 0 };
        }
        else if (places == 0)
        {
            return { operand, 0 };
        }

        Word::trit_container_with_carry trits;
        auto ht { operand.high().trits() };
        auto mt { operand.middle().trits() };
        auto lt { operand.low().trits() };

        std::copy(lt.begin(), lt.end(), trits.begin());
        std::copy(mt.begin(), mt.end(), trits.begin()+Word::middle_power);
        std::copy(ht.begin(), ht.end(), trits.begin()+Word::high_power);
        trits[Word::word_size] = 0;

        // Shift isn't in STL until C++20, but we can fake it with rotate
        std::rotate(trits.begin(), trits.begin()+places, trits.end());
        std::fill_n(trits.rbegin()+1, places, 0); // +1 because carry

        auto carry { trits[Word::word_size] };

        trits[Word::word_size] = 0;

        return { { to_decimal(trits) }, carry };
    }

    shift_result rol(const Word& operand, const Word& places) noexcept
    {
        return rol(operand, places.value());
    }

    shift_result rol(const Word& operand, Word::value_type places) noexcept
    {
        if (places > Word::word_size + 1)
        {
            return { {0}, 0 };
        }
        else if (places == 0)
        {
            return { operand, 0 };
        }

        Word::trit_container trits;
        auto ht { operand.high().trits() };
        auto mt { operand.middle().trits() };
        auto lt { operand.low().trits() };

        std::copy(lt.begin(), lt.end(), trits.begin());
        std::copy(mt.begin(), mt.end(), trits.begin()+Word::middle_power);
        std::copy(ht.begin(), ht.end(), trits.begin()+Word::high_power);

        std::rotate(trits.rbegin(), trits.rbegin()+places, trits.rend());

        trits[Word::word_size] = 0;

        return { { to_decimal(trits) }, 0 };
    }

    shift_result ror(const Word& operand, const Word& places) noexcept
    {
        return ror(operand, places.value());
    }

    shift_result ror(const Word& operand, Word::value_type places) noexcept
    {
        if (places > Word::word_size + 1)
        {
            return { {0}, 0 };
        }
        else if (places == 0)
        {
            return { operand, 0 };
        }

        Word::trit_container trits;
        auto ht { operand.high().trits() };
        auto mt { operand.middle().trits() };
        auto lt { operand.low().trits() };

        std::copy(lt.begin(), lt.end(), trits.begin());
        std::copy(mt.begin(), mt.end(), trits.begin()+Word::middle_power);
        std::copy(ht.begin(), ht.end(), trits.begin()+Word::high_power);

        std::rotate(trits.begin(), trits.begin()+places, trits.end());

        trits[Word::word_size] = 0;

        return { { to_decimal(trits) }, 0 };
    }

    shift_result rcl(const Word& operand, int carry, const Word& places) noexcept
    {
        return rcl(operand, carry, places.value());
    }

    shift_result rcl(const Word& operand, int carry, Word::value_type places) noexcept
    {
        if (places > Word::word_size + 1)
        {
            return { {0}, 0 };
        }
        else if (places == 0)
        {
            return { operand, carry };
        }

        Word::trit_container_with_carry trits;
        auto ht { operand.high().trits() };
        auto mt { operand.middle().trits() };
        auto lt { operand.low().trits() };

        std::copy(lt.begin(), lt.end(), trits.begin());
        std::copy(mt.begin(), mt.end(), trits.begin()+Word::middle_power);
        std::copy(ht.begin(), ht.end(), trits.begin()+Word::high_power);
        trits[Word::word_size] = 0;

        std::rotate(trits.rbegin(), trits.rbegin()+places, trits.rend());

        carry = trits[Word::word_size];

        trits[Word::word_size] = 0;

        return { { to_decimal(trits) }, carry };
    }

    shift_result rcr(const Word& operand, int carry, const Word& places) noexcept
    {
        return rcr(operand, carry, places.value());
    }

    shift_result rcr(const Word& operand, int carry, Word::value_type places) noexcept
    {
        if (places > Word::word_size + 1)
        {
            return { {0}, 0 };
        }
        else if (places == 0)
        {
            return { operand, carry };
        }

        Word::trit_container_with_carry trits;
        auto ht { operand.high().trits() };
        auto mt { operand.middle().trits() };
        auto lt { operand.low().trits() };

        std::copy(lt.begin(), lt.end(), trits.begin());
        std::copy(mt.begin(), mt.end(), trits.begin()+Word::middle_power);
        std::copy(ht.begin(), ht.end(), trits.begin()+Word::high_power);
        trits[Word::word_size] = 0;

        std::rotate(trits.begin(), trits.begin()+places, trits.end());

        carry = trits[Word::word_size];

        trits[Word::word_size] = 0;

        return { { to_decimal(trits) }, carry };
    }

    Word sti(const Word& operand) noexcept
    {
        return {
            invert(operand.high()),
            invert(operand.middle()),
            invert(operand.low())
        };
    }

    Word pti(const Word& operand) noexcept
    {
        return {
            positive_invert(operand.high()),
            positive_invert(operand.middle()),
            positive_invert(operand.low())
        };
    }

    Word nti(const Word& operand) noexcept
    {
        return {
            negative_invert(operand.high()),
            negative_invert(operand.middle()),
            negative_invert(operand.low())
        };
    }

    Word min(const Word& lhs, const Word& rhs) noexcept
    {
        return {
            trit_minimum(lhs.high(), rhs.high()),
            trit_minimum(lhs.middle(), rhs.middle()),
            trit_maximum(lhs.low(), rhs.low())
        };
    }

    Word max(const Word& lhs, const Word& rhs) noexcept
    {
        return {
            trit_maximum(lhs.high(), rhs.high()),
            trit_maximum(lhs.middle(), rhs.middle()),
            trit_maximum(lhs.low(), rhs.low())
        };
    }
    
    Word teq(const Word& lhs, const Word& rhs) noexcept
    {
        return {
            logical_equality(lhs.high(), rhs.high()),
            logical_equality(lhs.middle(), rhs.middle()),
            logical_equality(lhs.low(), rhs.low())
        };
    }

    Word tem(const Word& lhs, const Word& rhs) noexcept
    {
        return {
            logical_multiply(lhs.high(), rhs.high()),
            logical_multiply(lhs.middle(), rhs.middle()),
            logical_multiply(lhs.low(), rhs.low())
        };
    }

    Word fdr(const Word& operand) noexcept
    {
        return {
            forward_diode(operand.high()),
            forward_diode(operand.middle()),
            forward_diode(operand.low())
        };
    }

    Word rdr(const Word& operand) noexcept
    {
        return {
            reverse_diode(operand.high()),
            reverse_diode(operand.middle()),
            reverse_diode(operand.low())
        };
    }

    Word bin(const Word& operand) noexcept
    {
        Word::trit_container result;

        for (auto i = 0; i < Word::word_size; ++i)
        {
            result[i] = (operand.value() >> i) & 1;
        }

        return { to_decimal(result) };
    }

    Word tri(const Word& operand) noexcept
    {
        auto sign { operand.high().trits()[Hexad::width - 1] == -1 ? -1 : 1};

        int16_t result { 0 };

        Word::trit_container trits;
        auto ht { operand.high().trits() };
        auto mt { operand.middle().trits() };
        auto lt { operand.low().trits() };

        std::copy(lt.begin(), lt.end(), trits.begin());
        std::copy(mt.begin(), mt.end(), trits.begin()+Word::middle_power);
        std::copy(ht.begin(), ht.end(), trits.begin()+Word::high_power);

        for (auto i = 0; i < 16; ++i)
        {
            if (trits[i] > -1)
            {
                result += trits[i] << i;
            }
        }

        return { result * sign };
    }
}
