#ifndef TRIREME_REGISTERS_HPP
#define TRIREME_REGISTERS_HPP

#include <array>

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

        Registers() = default;

        Word get(int r) const noexcept;
        void set(int r, const Word& w) noexcept;

        int clamp_address(int r) const noexcept { return clamp<int, address_width>(r); }

        private:
        std::array<Word, register_count/2 + 1> general;
        std::array<Word, register_count/2> special;
    };
}

#endif /* TRIREME_REGISTERS_HPP */