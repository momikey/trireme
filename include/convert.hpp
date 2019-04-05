#ifndef TRIREME_CONVERT_HPP
#define TRIREME_CONVERT_HPP

#include <cstdlib>
#include <array>
#include <utility>
#include <string>

#include "ternary_math.hpp"
#include "detail/convert_impl.hpp"
#include "detail/character_to_value.hpp"

/**
 * @brief Converts an array of balanced trits to their decimal value.
 * 
 * @tparam Int Any integer type
 * @tparam Size The size of the array
 * @param arr The array of trits (all must be in the set {1,0,-1})
 * @return constexpr Int The decimal value of the balanced ternary digits
 */
template<typename Int, std::size_t Size>
inline constexpr Int to_decimal(const std::array<Int, Size>& arr) noexcept
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
inline constexpr std::array<Int, Size> to_trits(Int value) noexcept
{
    return detail::to_trits_impl(value, std::make_index_sequence<Size>{});
}

/**
 * @brief Converts a number representing a set of 3 trits
 * into a single alphanumeric character:
 * * 0 -> '0'
 * * +1 to +13 -> 'A' to 'M'
 * * -1 to -13 -> 'n' to 'z'
 * 
 * @tparam int Any integer type
 * @param value The value to convert
 * @return std::string A 1-character string following the scheme above
 */
template<typename Int = int>
inline std::string triad_to_string(Int value) noexcept
{
    return detail::triad_to_string_impl(value);
}

template<typename Int = int>
inline Int string_to_value(const std::string& st) noexcept
{
    return detail::string_to_value_impl(st);
}

#endif /* TRIREME_CONVERT_HPP */
