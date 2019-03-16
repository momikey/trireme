#include <iostream>
#include <array>

#include "convert.hpp"
#include "ternary_math.hpp"
#include "hexad.hpp"

using namespace ternary;

template<typename T, std::size_t N>
void print_array(const std::array<T,N>& arr)
{
    for (auto&& v : arr)
    {
        std::cout << v << ',';
    }
}

std::ostream& operator<<(std::ostream& os, std::pair<Hexad, Hexad> pair)
{
    return os << pair.first.get() << ',' << pair.second.get();
}

int main(int, char**) {
    const std::array<int, 6> testvalues = { 3, -9, 5, 19, -32, 89};
    for (auto i : testvalues)
    {
        print_array(to_trits<int, 6>(i));
        std::cout << (low_trits(i, 2)) << '\n';
    }

    Hexad h = { 42 };
    Hexad i = { 333 };
    std::cout << add(h, i).get() << '\n';
    std::cout << add_with_carry(h, i) << '\n';
    std::cout << subtract(i, h).get() << '\n';
    std::cout << subtract_with_carry(h, i) << '\n';
    std::cout << multiply(h, i) << '\n';
    std::cout << divide(i, h) << '\n';

    std::cout << divide({ {h}, {i} }, i) << '\n';

    std::cout << left_shift(h, 2) << '\n';
    std::cout << right_shift(i, 3) << '\n';
}
