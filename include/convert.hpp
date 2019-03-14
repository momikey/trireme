#ifndef TRIREME_CONVERT_HPP
#define TRIREME_CONVERT_HPP

#include <cstdlib>
#include <array>
#include <utility>

#include "ternary_math.hpp"
#include "detail/convert_impl.hpp"

/**
 * @brief Converts an array of balanced trits to their decimal value.
 * 
 * @tparam Int Any integer type
 * @tparam Size The size of the array
 * @param arr The array of trits (all must be in the set {1,0,-1})
 * @return constexpr Int The decimal value of the balanced ternary digits
 */
template<typename Int, std::size_t Size>
constexpr Int to_decimal(const std::array<Int, Size>& arr) noexcept
{
    return detail::to_decimal_impl(arr, Size-1);
}


template<typename Int, std::size_t Size>
constexpr std::array<Int, Size> to_trits(Int value) noexcept
{
    return detail::to_trits_impl(value, std::make_index_sequence<Size>{});
}

#endif /* TRIREME_CONVERT_HPP */
