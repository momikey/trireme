#ifndef TRIREME_MEMORY_HPP
#define TRIREME_MEMORY_HPP

#include <utility>
#include <array>
#include <type_traits>
#include <cassert>

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

        Hexad get(const int address) const noexcept { return memory_[with_offset(address)]; }
        void set(const int address, const Hexad& value) noexcept { memory_[with_offset(address)] = value; }
        void set(const int address, const int value) noexcept { memory_[with_offset(address)] = { value }; }
        void clear() noexcept { memory_.fill(0); }

        Word get_word(const int address) const
            { return get_word_impl(address, std::integral_constant<std::size_t, Address_Width>()); }
        
        void set_word(const int address, const Word& data)
            { return set_word_impl(address, data, std::integral_constant<std::size_t, Address_Width>()); }

        private:
        // We use this for a number of calculations, including
        // all memory accesses. (Is there a better way to do it?)
        static constexpr auto offset = range / 2;

        std::array<Hexad, range> memory_;

        /**
         * @brief Get the lowest trits of an address, so that it fits
         * in the available address space.
         * 
         * @param address The given address
         * @return int The address, but witihin the used address space
         */
        int address_mod(int address) const noexcept { return low_trits(address, Address_Width); }

        int with_offset(int address) const noexcept { return address_mod(address) + offset; }

        // Note SFINAE stuff to handle different architectures
        // (even though we haven't implemented more than 1 yet)
        // This has to be in the class definition, because it
        // otherwise gets really annoying with syntax.
        template<std::size_t A = Address_Width,
            typename std::enable_if_t<A == 12, int> = 0
        >
        Word get_word_impl(const int address, std::integral_constant<std::size_t, A>) const
        {
            Word base { address };
            Word result {};

            base.set_high(0);
            result.set_low(get(base.value()));

            base = add(base, 1).first;
            base.set_high(0);
            result.set_middle(get(base.value()));

            base = add(base, 1).first;
            base.set_high(0);
            result.set_high(get(base.value()));

            return result;
        }

        template<std::size_t A = Address_Width,
            typename std::enable_if_t<A != 12, int> = 0
        >
        Word get_word_impl(const int address, std::integral_constant<std::size_t, A>) const
        {
            // Not implemented yet
            assert(0);
            return { };
        }

        template<std::size_t A = Address_Width,
            typename std::enable_if_t<A == 12, int> = 0
        >
        void set_word_impl(const int address, const Word& data, std::integral_constant<std::size_t, A>)
        {
            Word base { address };

            base.set_high(0);
            set(base.value(), data.low());

            base = add(base, 1).first;
            base.set_high(0);
            set(base.value(), data.middle());

            base = add(base, 1).first;
            base.set_high(0);
            set(base.value(), data.high());
        }

        template<std::size_t A = Address_Width,
            typename std::enable_if_t<A != 12, int> = 0
        >
        void set_word_impl(const int address, const Word& data, std::integral_constant<std::size_t, A>)
        {
            // Not implemented yet
            assert(0);
        }
    };

    using BasicMemory = Memory<12ul>;
}


#endif /* TRIREME_MEMORY_HPP */