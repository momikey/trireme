#ifndef TRIREME_REGISTERS_HPP
#define TRIREME_REGISTERS_HPP

#include <array>
#include <algorithm>

#include "hexad.hpp"
#include "word.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    struct Registers
    {
        // The basic architecture has 27 registers,
        // or an effective 3-trit address space.
        static constexpr auto address_width = 3;
        static constexpr auto register_count = pow3(address_width);

        // In the basic architecture, register -13 is fixed.
        // It always reads as 0, and writes are discarded.
        static constexpr auto zero_register = -13;

        Registers() = default;

        Word get(int r) const noexcept;
        void set(int r, const Word& w) noexcept;
        void clear_all() noexcept;

        int clamp_address(int r) const noexcept { return clamp<int, address_width>(r); }

        private:
        std::array<Word, register_count/2 + 1> general;
        std::array<Word, register_count/2> special;
    };
}

#endif /* TRIREME_REGISTERS_HPP */