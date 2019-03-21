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
    return (exponent == 0) ? 1 : 3 * pow3<Int>(exponent - 1);
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
template<typename Int = int>
constexpr Int shift_right(Int value, std::size_t places) noexcept
{
    if (places == 0)
    {
        // Shifting right by 0 is a no-op
        return value;
    }
    else
    {
        const auto shifted = value / pow3<Int>(places);
        const auto halfpow = pow3<Int>(places) / 2;

        const auto adjust = abs_c(value - shifted*pow3<Int>(places)) > halfpow
            ? sign_c(value)
            : 0;

        return shifted + adjust;
    }    
}

/**
 * @brief Returns the Nth trit of a given number.
 * 
 * @tparam Int Any signed integral type
 * @param value The given value
 * @param place The desired trit
 * @return constexpr Int A value {1,0,-1} at the nth place of the
 * balanced ternary represntation of _value_
 */
template<typename Int = int>
constexpr Int nth_trit(Int value, std::size_t place) noexcept
{
    return lowest_trit(shift_right(value, place));
}

/**
 * @brief Clamp a given value to fit in a certain number of trits.
 * 
 * @tparam Int Any signed integral type
 * @tparam Width The maximum number of trits in the result
 * @param value The given value
 * @return constexpr Int The value, unless it is outside the range
 * (+/- (3**width-1) / 2), in which case the appropriate maximum is
 * returned.
 */
template<typename Int = int, std::size_t Width>
constexpr Int clamp(Int value) noexcept
{
    const auto max = pow3<Int>(Width) / 2;

    if (value > max)
    {
        return max;
    }
    else if (value < -max)
    {
        return -max;
    }
    else
    {
        return value;
    }
}

/**
 * @brief Get the lowest N trits of a number.
 * 
 * @tparam Int Any signed integral type
 * @param value The given value
 * @tparam width The desired number of trits in the result
 * @return constexpr Int An integer congruent to _value_ mod 3**Width,
 * within the range (+/- (3**Width-1) / 2)
 */
template<typename Int = int>
constexpr Int low_trits(Int value, std::size_t width) noexcept
{
    const auto power = pow3<Int>(width);
    const auto halfpower = power / 2;

    if (abs_c(value) > halfpower)
    {
        const auto m = value % power;

        if (abs_c(m) > halfpower)
        {
            return m - power*sign_c(value);
        }
        else
        {
            return m;
        }
    }
    else
    {
        return value;
    }
}

#endif /* TRIREME_TERNARY_MATH_HPP */