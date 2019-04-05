#ifndef TRIREME_CHARACTER_TO_VALUE_HPP
#define TRIREME_CHARACTER_TO_VALUE_HPP

#include <array>
#include <algorithm>
#include <string>
#include <numeric>

#include "convert_impl.hpp"

namespace detail {
    template<typename Char>
    int character_to_value(const Char c) noexcept
    {
        auto pos { std::find(positive.cbegin(), positive.cend(), c) };
        if (pos != positive.cend())
        {
            return std::distance(positive.cbegin(), pos) + 1;
        }
        else
        {
            auto neg { std::find(negative.cbegin(), negative.cend(), c) };
            return (neg != negative.cend()) ? (std::distance(neg, negative.cbegin()) - 1) : 0;
        }
    }

    template<typename Int = int>
    Int string_to_value(const std::string& st) noexcept
    {
        if (st.length() > powers_of_27.size())
        {
            // Use only the first 6 characters if the string is longer
            return std::inner_product(
                powers_of_27.cbegin(),
                powers_of_27.cend(),
                st.crbegin(),
                0,
                std::plus<>(),
                [](int i, char c) { return character_to_value(c) * i; }
            );
        }
        else
        {
            return std::inner_product(
                st.crbegin(),
                st.crend(),
                powers_of_27.begin(),
                0,
                std::plus<>(),
                [](char c, int i) { return character_to_value(c) * i; }
            );
        }
    }
}

#endif /* TRIREME_CHARACTER_TO_VALUE_HPP */