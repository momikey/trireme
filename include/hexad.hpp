#ifndef TRIREME_HEXAD_HPP
#define TRIREME_HEXAD_HPP

#include <cstddef>
#include <utility>
#include <array>

#include "ternary_math.hpp"
#include "convert.hpp"

namespace ternary {
    template<typename T>
    using value_with_carry = std::pair<T, int>;

    struct Hexad
    {
        using value_type = int;

        static const auto width = 6ul;
        static const auto range = pow3(width);
        static const auto max_value = range / 2;
        static const auto min_value = -max_value;

        using trit_container_type = std::array<value_type, width>;
        
        constexpr Hexad(value_type v_): value(low_trits(v_, width)) {}

        constexpr value_type get() const { return value; }
        constexpr trit_container_type trits() const { return to_trits<value_type, width>(value); }

    private:
        value_type value;
    };

    // Arithmetic functions for hexads
    // In general, these return a *new* hexad, rather than modifying
    // either of their operands. Optimizations can cut out most of the
    // copying, so it shouldn't be a problem.
    // Also, I'm not using overloaded operators for this, because some
    // of the operations don't map well to the C++ operator set.
    // In particular, things like rotating (rather than shifting) or
    // using a carry digit don't really mesh. Thus, in the interest of
    // regularity, I'm using named free functions.

    /**
     * @brief Add two hexads, returning the result.
     * 
     * @param lhs 
     * @param rhs 
     * @return Hexad 
     */
    inline Hexad add(const Hexad lhs, const Hexad rhs) { return { lhs.get() + rhs.get() }; }

    /**
     * @brief Subtract the rhs hexad from the lhs, returning the result.
     * 
     * @param lhs 
     * @param rhs 
     * @return Hexad 
     */
    inline Hexad subtract(const Hexad lhs, const Hexad rhs) { return { lhs.get() - rhs.get() }; }

    /**
     * @brief Add two hexads, returning the result with a carry.
     * 
     * @param lhs 
     * @param rhs 
     * @return value_with_carry<Hexad> 
     */
    value_with_carry<Hexad> add_with_carry(const Hexad lhs, const Hexad rhs);

    /**
     * @brief Subtract the rhs hexad from the lhs, returning the result
     * and a carry.
     * 
     * @param lhs 
     * @param rhs 
     * @return value_with_carry<Hexad> 
     */
    value_with_carry<Hexad> subtract_with_carry(const Hexad lhs, const Hexad rhs);

    std::pair<Hexad, Hexad> multiply(const Hexad lhs, const Hexad rhs);

    std::pair<Hexad, Hexad> divide(const Hexad lhs, const Hexad rhs);
}

#endif /* TRIREME_HECAD_HPP */