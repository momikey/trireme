#include <iostream>
#include <array>

#include "convert.hpp"
#include "ternary_math.hpp"

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
    const std::array<int, 6> testvalues = { 3, -9, 5, 19, -32, 89};
    for (auto i : testvalues)
    {
        print_array(to_trits<int, 6>(i));
    }
}
