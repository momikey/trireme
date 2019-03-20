#include <iostream>
#include <array>

#include "convert.hpp"
#include "ternary_math.hpp"
#include "hexad.hpp"
#include "word.hpp"

using namespace ternary;

template<typename T, std::size_t N>
void print_array(const std::array<T,N>& arr)
{
    for (auto&& v : arr)
    {
        std::cout << v << ',';
    }
}

std::ostream& operator<<(std::ostream& os, Hexad h)
{
    return os << "H" << h.get();
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

    std::pair<Hexad, int> hc = { h, 0 };
    for (int n = 0; n < 6; ++n)
    {
        auto rot { rotate_left_carry(hc.first, hc.second, n) };
        std::cout << rot << '\t';
        print_array(rot.first.trits());
        std::cout << '\n';
    }

    print_array(i.trits());
    std::cout << '\t' << positive_invert(i) << '\t';
    print_array(positive_invert(i).trits());
    std::cout << '\n';

    std::cout << h.trit_string() << '\t' << i.trit_string() << '\n';
    std::cout << h.value_string() << '\t' << i.value_string() << '\n';

    Hexad w1 { 1 };
    Hexad w2 { -1 };
    Hexad w3 { 2 };
    Word w { w1, w2, w3 };

    std::cout << w1.trit_string() << '\t' << w2.trit_string() << '\t' << w3.trit_string() << '\n';
    std::cout << w1.value_string() << '\t' << w2.value_string() << '\t' << w3.value_string() << '\n';
    std::cout << w.value() << '\t' << w.trit_string() << '\n' << w.value_string() << '\n';

    Word ww1 { 12345 };
    Word ww2 { -1234 };
    auto sum = ternary::add(ww1, ww2);

    std::cout << sum.first.value_string() << '\t' << sum.second << '\n';
}
