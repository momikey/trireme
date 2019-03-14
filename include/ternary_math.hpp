#ifndef TRIREME_TERNARY_MATH_HPP
#define TRIREME_TERNARY_MATH_HPP

/**
 * @brief Constexpr absolute value function.
 * 
 * @param value Any integer
 * @return constexpr int The absolute value of the given integer
 */
template<typename Int = int>
constexpr Int abs_c(Int value)
{
    if (value >= 0)
    {
        return value;
    }
    else
    {
        return -value;
    }
}

/**
 * @brief Returns the sign of a value as a constant expression.
 * 
 * @tparam T Any numeric type where positive, negative, and zero values are defined
 * @param value Any value of the given type (usually an integer)
 * @return constexpr T +1, -1, or 0 for positive, negative, and zero values, respectively
 */
template<typename Int = int>
constexpr Int sign_c(Int value)
{
    if (value > 0)
    {
        return 1;
    } else if (value < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Compile-time calculation of an integer power of 3.
 * 
 * @param exponent The exponent, which must be >= 0
 * @return constexpr int 3 raised to the given exponent
 */
template<typename Int = int, typename E = unsigned int>
constexpr Int pow3(E exponent) noexcept
{
    return (exponent == 0) ? 1 : 3 * pow3(exponent - 1);
}

/**
 * @brief Returns the lowest trit for an integer.
 * 
 * @param value Any signed integer
 * @return constexpr int A value {1, 0, -1} representing the lowest balanced trit
 */
template<typename Int = int>
constexpr Int lowest_trit(Int value) noexcept
{
    const auto temp = value % 3;

    if (abs_c(temp) != 2) {
        return temp;
    }
    else
    {
        return (value > 0) ? -1 : 1;
    }
}

/**
 * @brief Shift a number to the right using balanced-ternary arithmetic.
 * 
 * @tparam Int Any integral type
 * @param value An integer to shift
 * @param places The number of trits to shift
 * @return constexpr Int The value shifted to the right by the given number of places
 */
template<typename Int>
constexpr Int shift_right(Int value, std::size_t places) noexcept
{
    if (places == 0)
    {
        // Shifting right by 0 is a no-op
        return value;
    }
    else
    {
        const auto shifted = value / pow3(places);
        const auto halfpow = pow3(places) / 2;

        const auto adjust = abs_c(value - shifted*pow3(places)) > halfpow
            ? sign_c(value)
            : 0;

        return shifted + adjust;
    }    
}

template<typename Int>
constexpr Int nth_trit(Int value, std::size_t place) noexcept
{
    return lowest_trit(shift_right(value, place));
}

#endif /* TRIREME_TERNARY_MATH_HPP */