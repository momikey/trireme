#include <iostream>
#include <array>

#include "convert.h"

template<typename T, std::size_t N>
void print_array(const std::array<T,N>& arr)
{
    for (auto&& v : arr)
    {
        std::cout << v << ',';
    }
    std::cout << '\n';
}

int main(int, char**) {
    auto testvalues = {0, 1, 2, 4, 8, -5, -12, -13};
    for (auto&& v : testvalues)
    {
        std::cout << lowest_trit(v) << '\n';
    }

    auto powtest = {0u, 1u, 2u, 6u, 9u, 12u, 18u};
    for (auto&& v : powtest)
    {
        std::cout << pow3(v) << '\n';
    }

    constexpr std::array<int, 9> arr = {1, 1, 0, -1, 1, 0, 0, 1, -1};
    std::cout << to_binary(arr) << '\n';
}
