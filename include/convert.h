#ifndef TRIREME_CONVERT_H
#define TRIREME_CONVERT_H

#include <cstdlib>
#include <array>

/**
 * @brief Returns the lowest trit for an integer.
 * 
 * @param value Any signed integer
 * @return constexpr int A value {1, 0, -1} representing the lowest balanced trit.
 */
constexpr int lowest_trit(int value)
{
    auto temp = value % 3;

    if (std::abs(temp) != 2) {
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

#endif /* TRIREME_CONVERT_H */
