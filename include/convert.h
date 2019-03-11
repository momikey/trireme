#ifndef TRIREME_CONVERT_H
#define TRIREME_CONVERT_H

#include <cstdlib>
#include <array>
#include <utility>

/**
 * @brief Constexpr absolute value function.
 * 
 * @param value Any integer
 * @return constexpr int The absolute value of the given integer
 */
constexpr int abs_c(int value)
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
 * @brief Returns the lowest trit for an integer.
 * 
 * @param value Any signed integer
 * @return constexpr int A value {1, 0, -1} representing the lowest balanced trit
 */
constexpr int lowest_trit(int value)
{
    auto temp = value % 3;

    if (abs_c(temp) != 2) {
        return temp;
    }
    else
    {
        return (value > 0) ? -1 : 1;
    }
}

constexpr int pow3(unsigned int exponent)
{
    return (exponent == 0) ? 1 : 3 * pow3(exponent - 1);
}

template<typename Int, std::size_t Size>
constexpr Int to_binary_impl(const std::array<Int, Size>& arr, std::size_t level)
{
    return (level == 0)
        ? arr[level]
        : arr[level] * pow3(level) + to_binary_impl(arr, level-1);
}

template<typename Int, std::size_t Size>
constexpr Int to_binary(const std::array<Int, Size>& arr)
{
    return to_binary_impl(arr, Size-1);
}

#endif /* TRIREME_CONVERT_H */
