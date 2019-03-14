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

}

#endif /* TRIREME_CONVERT_IMPL_HPP */