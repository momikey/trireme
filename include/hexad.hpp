#ifndef TRIREME_HEXAD_HPP
#define TRIREME_HEXAD_HPP

#include <cstddef>
#include <utility>
#include <array>
#include <string>

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
        
        constexpr Hexad(): value(0) {}
        constexpr Hexad(value_type v_): value(low_trits(v_, width)) {}

        constexpr value_type get() const { return value; }
        constexpr trit_container_type trits() const { return to_trits<value_type, width>(value); }

        std::string trit_string() const noexcept;
        std::string value_string() const noexcept;

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

    /**
     * @brief Multiply two hexads, returning a double-precision result.
     * 
     * @param lhs 
     * @param rhs 
     * @return std::pair<Hexad, Hexad> The low and high hexads of the result
     */
    std::pair<Hexad, Hexad> multiply(const Hexad lhs, const Hexad rhs);

    /**
     * @brief Divide the lhs hexad by the rhs, returning the quotient and remainder.
     * 
     * @param lhs 
     * @param rhs 
     * @return std::pair<Hexad, Hexad> The quotient and remainder
     */
    std::pair<Hexad, Hexad> divide(const Hexad lhs, const Hexad rhs);

    /**
     * @brief Divide a 2-hexad number by the rhs, returning
     * the quotient and remainder.
     * 
     * @param lhs A pair of hexads {low, high} (i.e., in little-endian order)
     * @param rhs 
     * @return std::pair<Hexad, Hexad> The quotient and remainder
     */
    std::pair<Hexad, Hexad> divide(const std::pair<Hexad, Hexad> lhs, const Hexad rhs);

    // Digital operations
    // These are operations that work directly on trits (the analogue of
    // binary AND, OR, etc.), and they normally return new hexads containing
    // the result of the operation.


    /**
     * @brief Shift a hexad left a single place..
     * 
     * @param operand 
     * @return Hexad 
     */
    inline Hexad left_shift(const Hexad operand) { return { operand.get() * 3 }; }

    /**
     * @brief Shift a hexad left a given number of places.
     * 
     * @param operand The operand
     * @param places The number of places to shift
     * @return std::pair<Hexad, int> The result of the shift and the last trit shifted out
     */
    std::pair<Hexad, int> left_shift(const Hexad operand, std::size_t places);

    /**
     * @brief Shift a hexad right a single place
     * 
     * @param operand 
     * @return Hexad 
     */
    inline Hexad right_shift(const Hexad operand) { return { operand.get() / 3}; }

    /**
     * @brief Shift a hexad right a given number of places.
     * 
     * @param operand The operand
     * @param places The number of places to shift
     * @return std::pair<Hexad, int> The result of the shift and the last trit shifted out
     */
    std::pair<Hexad, int> right_shift(const Hexad operand, std::size_t places);

    /**
     * @brief Rotate a hexad left a given number of places.
     * 
     * @param operand 
     * @param places 
     * @return Hexad 
     */
    Hexad rotate_left(const Hexad operand, std::size_t places);

    /**
     * @brief Rotate a hexad right a given number of places.
     * 
     * @param operand 
     * @param places 
     * @return Hexad 
     */
    Hexad rotate_right(const Hexad operand, std::size_t places);

    /**
     * @brief Rotate a hexad and supplementary carry trit a given number of places to the left.
     * 
     * @param operand 
     * @param carry 
     * @param places 
     * @return std::pair<Hexad, int> The rotated hexad and carry trit
     */
    std::pair<Hexad, int> rotate_left_carry(const Hexad operand, int carry, std::size_t places);

    /**
     * @brief Rotate a hexad and supplementary carry trit a given number of places to the right.
     * 
     * @param operand 
     * @param carry 
     * @param places 
     * @return std::pair<Hexad, int> 
     */
    std::pair<Hexad, int> rotate_right_carry(const Hexad operand, int carry, std::size_t places);

    /**
     * @brief Invert each trit in a hexad. This is equivalent to negation.
     * 
     * @param operand 
     * @return Hexad 
     */
    inline Hexad invert(const Hexad operand) { return {-operand.get() }; }

    /**
     * @brief Invert each trit in a hexad, additionally changing 0 to +1.
     * 
     * @param operand 
     * @return Hexad 
     */
    Hexad positive_invert(const Hexad operand);

    /**
     * @brief Invert each trit in a hexad, additionally changing 9 to -1.
     * 
     * @param operand 
     * @return Hexad 
     */
    Hexad negative_invert(const Hexad operand);

    /**
     * @brief Return the tritwise minumum of two hexads. This is equivalent to binary AND.
     * 
     * @param lhs 
     * @param rhs 
     * @return Hexad 
     */
    Hexad trit_minimum(const Hexad lhs, const Hexad rhs);

    /**
     * @brief Return the tritwise maximum of two hexads. This is equivalent to binary OR.
     * 
     * @param lhs 
     * @param rhs 
     * @return Hexad 
     */
    Hexad trit_maximum(const Hexad lhs, const Hexad rhs);

    /**
     * @brief Return a tritwise equality function of two hexads.
     * Each trit in the result is +1 if the corresponding trits
     * in the operands are equal, -1 if they are not. This is
     * similar to binary XOR, but the logic is inverted.
     * 
     * @param lhs 
     * @param rhs 
     * @return Hexad 
     */
    Hexad logical_equality(const Hexad lhs, const Hexad rhs);

    /**
     * @brief Return the tritwise multiplication of two hexads.
     * Each trit in the result is the product of the corresponding
     * trits in the operands. This is similar to binary AND, but
     * in a different way than the "maximum" function.
     * 
     * @param lhs 
     * @param rhs 
     * @return Hexad 
     */
    Hexad logical_multiply(const Hexad lhs, const Hexad rhs);

    /**
     * @brief Return the given hexad with all negative trits changed to 0.
     * 
     * @param operand 
     * @return Hexad 
     */
    Hexad forward_diode(const Hexad operand);

    /**
     * @brief Return the given hexad with all positive trits changed to 0.
     * 
     * @param operand 
     * @return Hexad 
     */
    Hexad reverse_diode(const Hexad operand);
}

#endif /* TRIREME_HECAD_HPP */