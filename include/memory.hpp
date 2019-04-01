#ifndef TRIREME_MEMORY_HPP
#define TRIREME_MEMORY_HPP

#include <utility>
#include <array>

#include "word.hpp"
#include "hexad.hpp"
#include "ternary_math.hpp"

namespace ternary
{
    template<std::size_t Address_Width>
    struct Memory
    {
        static constexpr auto range = pow3(Address_Width);

        Memory() = default;

        Hexad get(int address) const noexcept { return memory_[with_offset(address)]; }
        void set(int address, const Hexad& value) noexcept { memory_[with_offset(address)] = value; }
        void set(int address, int value) noexcept { memory_[with_offset(address)] = { value }; }

        private:
        // We use this for a number of calculations, including
        // all memory accesses. (Is there a better way to do it?)
        static constexpr auto offset = range / 2;

        /**
         * @brief Get the lowest trits of an address, so that it fits
         * in the available address space.
         * 
         * @param address The given address
         * @return int The address, but witihin the used address space
         */
        int address_mod(int address) const noexcept { return low_trits(address, Address_Width); }

        int with_offset(int address) const noexcept { return address_mod(address) + offset; }

        std::array<Hexad, range> memory_;
    };

    using BasicMemory = Memory<12>;
}


#endif /* TRIREME_MEMORY_HPP */