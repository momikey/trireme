#ifndef TRIREME_CONVERT_IMPL_HPP
#define TRIREME_CONVERT_IMPL_HPP

#include <cstdlib>
#include <array>

#include "../ternary_math.hpp"

namespace detail
{
    template<typename Int, std::size_t Size>
    constexpr Int to_decimal_impl(const std::array<Int, Size>& arr, std::size_t level) noexcept
    {
        return (level == 0)
            ? arr[level]
            : arr[level] * pow3(level) + to_decimal_impl(arr, level-1);
    }

    template<typename Int, std::size_t... Is>
    constexpr auto to_trits_impl(Int value, std::index_sequence<Is...>) noexcept
        -> std::array<Int, sizeof...(Is)>
    {
        return {{ nth_trit(value, Is)... }};
    }

    template<typename Int = int>
    std::string triad_to_string_impl(Int value) noexcept
    {
        // Positive triads use capital letters
        const std::array<int, 13> positive {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M'
        };

        // Negative triads use lowercase letters
        const std::array<int, 13> negative {
            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
        };

        if (abs_c(value) > 13)
        {
            // Out of range, so return an empty string
            // We could throw here (have to take off noexcept, obviously),
            // but that seems like it might be a waste. This is more of
            // an internal function, after all.
            return {};
        }
        else
        {
            if (value > 0)
            {
                return std::string(1, positive[(value-1)]);
            }
            else
            {
                return std::string(1, negative[-value-1]);
            }
        }
        
    }
}

#endif /* TRIREME_CONVERT_IMPL_HPP */