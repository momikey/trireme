#ifndef TRIREME_CONVERT_HPP
#define TRIREME_CONVERT_HPP

#include <cstdlib>
#include <array>
#include <utility>
#include <string>

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

/**
 * @brief Converts a number to an array of balanced trits.
 * 
 * @tparam Int Any integer type
 * @tparam Size The size of the resulting array
 * @param value The decimal value intended for conversion
 * @return constexpr std::array<Int, Size> An array of trits, each in the set {1,0,-1}
 */
template<typename Int, std::size_t Size>
constexpr std::array<Int, Size> to_trits(Int value) noexcept
{
    return detail::to_trits_impl(value, std::make_index_sequence<Size>{});
}

template<typename Int = int>
std::string triad_to_string(Int value) noexcept
{
    return detail::triad_to_string_impl(value);
}

#endif /* TRIREME_CONVERT_HPP */
